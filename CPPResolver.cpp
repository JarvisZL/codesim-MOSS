#include "CPPResovler.h"
#include <clang-c/Index.h>// This is libclang.
#include <sys/stat.h>

// first is name, second is pattern;
std::unordered_map<std::string, std::string> patternmap;
// first is loc <lin,col>, second is name;
std::vector<std::pair<std::pair<unsigned int, unsigned int>, std::string>> identifiervec;

//将CXString变成std::string
static std::string convert(const CXString& str){
    std::string retstr = clang_getCString(str);
    clang_disposeString(str);
    return retstr;
}

//删除掉include,防止影响后面的分析
static std::string removeInclude(const std::string& _filename){
    char nfilename[] = "newfile-XXXXXX";
    int fd;
    if((fd = mkstemp(nfilename)) < 0){
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    std::ifstream fin(_filename);
    if(!fin){
        perror("ifstream");
        unlink(nfilename);
        exit(EXIT_FAILURE);
    }

    std::string tempstr;
    while( getline(fin, tempstr) ){
        if(tempstr.empty()) continue;
        tempstr.erase(0,tempstr.find_first_not_of(' '));
        if(tempstr.length()>= 8 && tempstr.substr(0,8) == "#include") continue;
        tempstr.append("\n");
        write(fd,tempstr.c_str(),tempstr.length());
    }
    fin.close();
    close(fd);
    return std::string(nfilename);
}

static char asciitolower(char in){
    if( in <= 'Z' && in >= 'A'){
        return in - ('Z' - 'z');
    }
    return in;
}

//得到pattern存入patternmap
static int getReplacePattern(const std::string& _filename){
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            _filename.c_str(), nullptr, 0,
            nullptr, 0,
            CXTranslationUnit_None);
    if (unit == nullptr)
    {
        std::cerr << "codesim: unable to parse translation unit in getreplacepattern!" << std::endl;
        return EXIT_FAILURE;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
            cursor,
            [](CXCursor c, CXCursor parent, CXClientData client_data)
            {
                std::string kindstr = convert(clang_getCursorKindSpelling(clang_getCursorKind(c)));
                std::string namestr = convert(clang_getCursorSpelling(c));
                if(!namestr.empty()){
                    if(kindstr == "FunctionDecl"){
                        patternmap.insert(std::make_pair(namestr, "f"));
                    }
                    else if(kindstr == "VarDecl" || kindstr == "FieldDecl" || kindstr == "EnumConstantDecl"){
                        patternmap.insert(std::make_pair(namestr, "v"));
                    }
                    else if(kindstr == "ParmDecl"){
                        patternmap.insert(std::make_pair(namestr, "p"));
                    }
                    else if(kindstr == "ClassDecl"){
                        patternmap.insert(std::make_pair(namestr, "c"));
                    }
                    else if(kindstr == "CXXMethod" || kindstr == "CXXDestructor" || kindstr == "CXXConstructor"){
                        patternmap.insert(std::make_pair(namestr, "f"));
                    }
                    else if(kindstr == "EnumDecl"){
                        patternmap.insert(std::make_pair(namestr, "e"));
                    }
                    else if(kindstr == "StructDecl"){
                        patternmap.insert(std::make_pair(namestr, "s"));
                    }
                }
                return CXChildVisit_Recurse;
            },
            nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return EXIT_OK;
}

//得到identifier存入identifiervec
static int getIdentifierTokens(const std::string& _filename){
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            _filename.c_str(), nullptr, 0,
            nullptr, 0,
            CXTranslationUnit_None);
    if (unit == nullptr)
    {
        std::cerr << "codesim: unable to parse translation unit!" << std::endl;
        return EXIT_FAILURE;
    }

    struct stat statbuff;
    if(stat(_filename.c_str(), &statbuff) < 0){
        std::cerr <<"codesim: get file stat failed." << std::endl;
        return EXIT_FAILURE;
    }
    CXFile cxfile = clang_getFile(unit, _filename.c_str());
    CXSourceLocation loc_start = clang_getLocationForOffset(unit, cxfile, 0);
    CXSourceLocation loc_end = clang_getLocationForOffset(unit, cxfile, statbuff.st_size);
    CXSourceRange range = clang_getRange(loc_start,loc_end);

    //词法单元
    unsigned numTokens = 0;
    CXToken* tokens = NULL;
    clang_tokenize(unit, range, &tokens, &numTokens);
    if(tokens == NULL){
        std::cout <<"codesim: tokens NULL!" << std::endl;
        return EXIT_FAILURE;
    }

    for( unsigned i = 0; i < numTokens; ++i ){
        CXTokenKind kind = clang_getTokenKind(tokens[i]);
        CXString name = clang_getTokenSpelling(unit, tokens[i]);
        CXSourceLocation loc = clang_getTokenLocation(unit, tokens[i]);
        unsigned line, column;
        clang_getSpellingLocation(loc, NULL, &line, &column, NULL);
        if(kind == CXToken_Identifier){
            identifiervec.push_back(std::make_pair(std::make_pair(line, column), convert(name)));
        }
    }

    clang_disposeTokens(unit, tokens, numTokens);
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return EXIT_OK;
}

//替换_outstr中的名称
static int replace(const std::string& _filename, std::string& _outstr){
    patternmap.clear();
    int getstatus = getReplacePattern(_filename);
    if(getstatus != EXIT_OK)
        return getstatus;

    identifiervec.clear();
    getstatus = getIdentifierTokens(_filename);
    if(getstatus != EXIT_OK)
        return getstatus;

    unsigned int flcnt = 0;
    auto vecit = identifiervec.begin();
    std::ifstream fin(_filename);
    if(!fin){
        perror("ifstream");
        unlink(_filename.c_str());
        exit(EXIT_FAILURE);
    }
    std::string eachline;
    while( getline(fin, eachline) ){
        flcnt++;
        unsigned offset = 0;
        //std::cout << "origin: " << eachline << std::endl;
        while(vecit != identifiervec.end() && vecit->first.first == flcnt){
            unsigned int column = vecit->first.second;
            std::string replacestr = patternmap[vecit->second];
           // std::cout << "replace: " << replacestr << std::endl;
            if(!replacestr.empty()) {
                eachline.replace(column - offset - 1,vecit->second.length(),replacestr);
                offset += vecit->second.length() - replacestr.length();
            }
            vecit = vecit + 1;
        }
        _outstr.append(eachline);
    }
    return EXIT_OK;
}

//去除空格
static void trim(std::string& str){
    int index = 0;
    if(!str.empty()){
        while( (index = str.find(' ', index)) != std::string::npos){
            str.erase(index,1);
        }
    }
}

//解析cpp文件，替换名称删除空格并最终变成小写字符串返回
int resolve(const std::string& _filename, std::string& _outstr){
    //removeinclude
    std::string nfile;
    nfile = removeInclude(_filename);
    if(nfile.empty()){
        std::cerr << "codesim: temp filename is empty!" << std::endl;
        unlink(nfile.c_str());
        return EXIT_IMP;
    }
    //加上.cpp后缀，后面分析
    std::string cmd = "mv " + nfile + " " + nfile.append(".cpp");
    int sysstatus = system(cmd.c_str());
    if(sysstatus == -1){
        unlink(nfile.c_str());
        return EXIT_FAILURE;
    }
    else if(!WIFEXITED(sysstatus)){
        unlink(nfile.c_str());
        return EXIT_FAILURE;
    }
    else if(WEXITSTATUS(sysstatus)){
        unlink(nfile.c_str());
        return EXIT_FAILURE;
    }

    //替换
    if(!_outstr.empty())
        _outstr.clear();
    int replacestatus = replace(nfile, _outstr);
    if(replacestatus != EXIT_OK){
        std::cerr << "codesim: replacement failed." << std::endl;
        unlink(nfile.c_str());
        return replacestatus;
    }

    //统一小写
    std::transform(_outstr.begin(), _outstr.end(), _outstr.begin(), asciitolower);
    //去除空格
    trim(_outstr);

    std::cout << _outstr <<std::endl;
    unlink(nfile.c_str());
    return EXIT_OK;
}

