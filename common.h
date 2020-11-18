#ifndef CODESIM_COMMON_H
#define CODESIM_COMMON_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <regex>
#include <cmath>

//MACRO
#define EXIT_OK 0
#define EXIT_IMP -1

//For ArgsCheck
#define EXIT_WITHOUT_ARG -2
#define EXIT_LACK_OP -3
#define EXIT_ILLEGAL_ARG -4
#define EXIT_HELP -5

extern bool Verflag;
extern int K_s, T_s, W_s;

#endif //CODESIM_COMMON_H
