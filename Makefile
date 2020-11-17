HXXFILES=$(shell find . -maxdepth 1 -name "*.h" )
CXXFILES=$(shell find . -maxdepth 1 -name "*.cpp" )

parser: ${HXXFILES} ${CXXFILES}
	clang++ -std=c++11 ${HXXFILE} ${CXXFILES} -I /usr/lib/llvm-7/include -o codesim -lclang 

test: parser
	./codesim ./tests/a.cpp ./tests/b.cpp
	

clean:
	rm -rf newfile*
	rm codesim 

.PHONY: clean test