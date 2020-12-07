#include "CPPResovler.h"
#include "Winnowing.h"

bool Verflag = false;
int K_s = 7, T_s = 10, W_s;
static std::string cppfile1, cppfile2;
static std::string outstr1, outstr2;

static int ArgsCheck(int _argc, char* _argv[]){
    //argc == 1
    if(_argc == 1){
        std::cout << "codesim: missing arguments." << std::endl;
        std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
        return EXIT_WITHOUT_ARG;
    }

    //argc == 2
    if(_argc == 2){
        std::string str_arg1(_argv[1]);
        //--help || -h
        if(str_arg1 == "-h" || str_arg1 == "--help"){
            std::cout << "usage: codesim [-v|--verbose] code1.cpp code2.cpp" << std::endl;
            return EXIT_HELP;
        }
        // --verbose || -v
        if(str_arg1 == "-v" || str_arg1 == "--verbose"){
            std::cout << "codesim: missing operands after " << "'" << str_arg1 << "'." << std::endl;
            std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
            return EXIT_LACK_OP;
        }

        if(str_arg1.length() >= 5 && str_arg1.substr(str_arg1.length() - 4, 4) == ".cpp"){
            //one operand
            std::cout << "codesim: missing operand after " << "'" << str_arg1 << "'." << std::endl;
            std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
            return EXIT_LACK_OP;
        }
        else{
            //illegal args
            std::cout << "codesim: Illegal arguments " << "'" << str_arg1 << "'." << std::endl;
            std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
            return EXIT_ILLEGAL_ARG;
        }
    }

    //argc == 3
    if(_argc == 3){
        std::string str_arg1(_argv[1]);
        std::string str_arg2(_argv[2]);
        if(str_arg1 == "-v" || str_arg1 == "--verbose"){
            std::cout << "codesim: missing operand after " << "'" << str_arg2 << "'." << std::endl;
            std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
            return EXIT_LACK_OP;
        }
        else if(str_arg1.length() >= 5 && str_arg1.substr(str_arg1.length() - 4, 4) == ".cpp"){
            if(str_arg2.length() >= 5 && str_arg2.substr(str_arg2.length() - 4, 4) == ".cpp"){
                return EXIT_OK;
            }
            else{
                std::cout << "codesim: Illegal arguments " << "'" << str_arg2 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
        }
        else{
            std::cout << "codesim: Illegal arguments " << "'" << str_arg1 << "'." << std::endl;
            std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
            return EXIT_ILLEGAL_ARG;
        }
    }

    //argc == 4
    if(_argc == 4){
        std::string str_arg1(_argv[1]);
        std::string str_arg2(_argv[2]);
        std::string str_arg3(_argv[3]);
        if(str_arg1 != "-v" && str_arg1 != "--verbose"){
            if(!(str_arg1.length() >= 5 && str_arg1.substr(str_arg1.length() - 4, 4) == ".cpp")){
                std::cout << "codesim: Illegal arguments " << "'" << str_arg1 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
            else if(!(str_arg2.length() >= 5 && str_arg2.substr(str_arg2.length() - 4, 4) == ".cpp")){
                std::cout << "codesim: Illegal arguments " << "'" << str_arg2 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
            else{
                std::cout << "codesim: Illegal arguments " << "'" << str_arg3 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
        }
        else{
            if(!(str_arg2.length() >= 5 && str_arg2.substr(str_arg2.length() - 4, 4) == ".cpp")){
                std::cout << "codesim: Illegal arguments " << "'" << str_arg2 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
            else if(!(str_arg3.length() >= 5 && str_arg3.substr(str_arg3.length() - 4, 4) == ".cpp")){
                std::cout << "codesim: Illegal arguments " << "'" << str_arg3 << "'." << std::endl;
                std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
                return EXIT_ILLEGAL_ARG;
            }
            else{
                Verflag = true;
                return EXIT_OK;
            }
        }
    }

    std::cout << "codesim: Too many arguments." << std::endl;
    std::cout << "codesim: Try 'codesim [-h|--help]' for more information." << std::endl;
    return EXIT_ILLEGAL_ARG;
}

//for verbose
static void showfingerprints(const std::string& filename, const std::vector<std::pair<std::size_t, std::size_t>>& fpvec){
    std::cout << "codesim(Verbose): "<< fpvec.size() <<  " fingerprints([hash value, location]) of '" << filename << "': " << std::endl;
    int cnt = 0;
    for(auto& item: fpvec){
        std::cout << "[" << item.first << "," << item.second << "]" << std::endl;
        if(++cnt >= 20)
            break;
    }
    if(fpvec.size() > 20)
        std::cout << "......" << std::endl;
}

static float getsimilarity(const std::vector<std::pair<std::size_t, std::size_t>>& fpvec1, const std::vector<std::pair<std::size_t, std::size_t>>& fpvec2){
    int size1 = fpvec1.size(), size2 = fpvec2.size();
    int matchcnt = 0;
    //verbose
    auto matchoutput = [](int cnt, std::size_t index1, std::size_t index2){
        std::cout << "codesim(Verbose): No." << cnt << " match:";
        std::cout << outstr1.substr(index1, K_s) << "' matches '" << outstr2.substr(index2, K_s) << "'" <<std::endl;
    };
    int verboseoutputcnt = 0;
    for(auto& item1 : fpvec1){
        for(auto& item2 : fpvec2){
            if(item1.first == item2.first){
                //verbose begin
                if(Verflag){
                    if(++verboseoutputcnt < 20)
                        matchoutput(verboseoutputcnt,item1.second, item2.second);
                    if(++verboseoutputcnt == 20)
                        std::cout << "......" << std::endl;
                }
                //verbose end
                matchcnt++;
                break;
            }
        }
    }
    return (float)matchcnt*2/(float)(size1 + size2) * 100;
}

int main(int argc, char* argv[]) {
    W_s = T_s - K_s + 1;
    //参数检查
    int argstatus = ArgsCheck(argc, argv);
    if(argstatus != EXIT_OK)
        return argstatus;
    if(argc == 3){
        cppfile1 = std::string(argv[1]);
        cppfile2 = std::string(argv[2]);
    }
    else if(argc == 4){
        cppfile1 = std::string(argv[2]);
        cppfile2 = std::string(argv[3]);
    } else
        return EXIT_IMP;

    //解析cpp文件得到字符串
    int resolvestatus = resolve(cppfile1, outstr1);
    if(resolvestatus != EXIT_OK){
        std::cerr << "codesim: resolve file:" << cppfile1 << " failed." << std::endl;
        return resolvestatus;
    }
    resolvestatus = resolve(cppfile2, outstr2);
    if(resolvestatus != EXIT_OK){
        std::cerr << "codesim: resolve file:" << cppfile2 << " failed." << std::endl;
        return resolvestatus;
    }

    //verbose
    if(Verflag){
        auto outstrformat = [](const std::string& origin){
            std::string ret(origin);
            return ret.length() > 100 ? ret.substr(0, 100).append("......") : ret;
        };
        std::cout << "codesim(Verbose): file: '" << cppfile1 << "' is converted to '" << outstrformat(outstr1) << "' with length: " << outstr1.length() << std::endl;
        std::cout << "codesim(Verbose): file: '" << cppfile2 << "' is converted to '" << outstrformat(outstr2) << "' with length: " << outstr2.length() << std::endl;
    }

    //WINNOWING
    //first is hash value, second is location
    std::vector<std::pair<std::size_t, std::size_t>> fingerprints1, fingerprints2;
    int winstatus = winnowing(outstr1, fingerprints1, cppfile1);
    if(winstatus != EXIT_OK){
        std::cerr << "codesim: winnowing string:" << outstr1 << " failed." << std::endl;
        return winstatus;
    }
    winstatus = winnowing(outstr2, fingerprints2, cppfile2);
    if(winstatus != EXIT_OK){
        std::cerr << "codesim: winnowing string:" << outstr2 << " failed." << std::endl;
        return winstatus;
    }

    //verbose
    if(Verflag){
        showfingerprints(cppfile1, fingerprints1);
        showfingerprints(cppfile2, fingerprints2);
    }

    float similarity = getsimilarity(fingerprints1, fingerprints2);
    if(Verflag)
        std::cout << "The similarity of '" << cppfile1 << "' and '" << cppfile2 << "' is " << similarity << std::endl;
    else
        std::cout << similarity << std::endl;

    return EXIT_OK;
}
