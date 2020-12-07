HXXFILES=$(shell find . -maxdepth 1 -name "*.h" )
CXXFILES=$(shell find . -maxdepth 1 -name "*.cpp" )

parser: ${HXXFILES} ${CXXFILES}
	clang++-7 -std=c++11 ${HXXFILE} ${CXXFILES} -I /usr/lib/llvm-7/include  -o codesim -lclang 

test: parser
	./codesim ./tests/t1.cpp ./tests/t2.cpp

verbosetest: parser
	./codesim -v ./tests/t1.cpp ./tests/t2.cpp

prepare:
	sudo apt-get install clang-7
	sudo apt-get install libclang-7-dev


clean:
	rm -rf newfile*
	rm codesim 
	rm -rf output
	rm -rf .*.swp

.PHONY: clean test vtest prepare
