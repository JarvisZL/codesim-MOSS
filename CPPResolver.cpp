#include "CPPResovler.h"
#include <clang-c/Index.h>  // This is libclang.

//解析cpp文件，替换变量和函数名删除空格并最终变成字符串返回

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

int resolve(const std::string& _filename, std::string& _outstr){
    //removeinclude
    std::string nfile;
    nfile = removeInclude(_filename);



    //unlink(nfile.c_str());
    return EXIT_OK;
}
