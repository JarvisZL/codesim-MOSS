#include <iostream>
#include <clang-c/Index.h>  // This is libclang.
using namespace std;

/*ostream& operator<<(ostream& stream, const CXString& str){
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}
*/

static std::string convert(const CXString& str){
    std::string retstr = clang_getCString(str);
    clang_disposeString(str);
    return retstr;
}


int main(){
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
    index,
    "a.cpp", nullptr, 0,
    nullptr, 0,
    CXTranslationUnit_None);
  if (unit == nullptr)
  {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
    cursor,
    [](CXCursor c, CXCursor parent, CXClientData client_data)
    {
      cout << "Cursor '" << convert(clang_getCursorSpelling(c)) << "' of kind '"
        << convert(clang_getCursorKindSpelling(clang_getCursorKind(c))) << "'\n";
      return CXChildVisit_Recurse;
    },
    nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}
