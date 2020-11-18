#include "CPPResovler.h"

static bool Verflag = false;
std::string cppfile1, cppfile2;

const int k = 10, t = 20;
#define w  t - k + 1

int ArgsCheck(int _argc, char* _argv[]){
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

int main(int argc, char* argv[]) {
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
    std::string outstr1, outstr2;
    int resolvestatus = resolve(cppfile1, outstr1);
    if(resolvestatus != EXIT_OK){
        std::cerr << "resolve file:" << cppfile1 << " failed." << std::endl;
    }
//    resolvestatus = resolve(cppfile2, outstr2);
//    if(resolvestatus != EXIT_OK){
//        std::cerr << "resolve file:" << cppfile2 << " failed." << std::endl;
//    }

    //std::cout << outstr1 << std::endl << outstr2;


    return EXIT_OK;
}
