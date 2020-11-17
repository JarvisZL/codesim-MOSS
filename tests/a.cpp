#include <iostream>

std::string gethello(){
	return std::string("hello");
}

int main()
{
    std::cout << gethello() << std::endl;
    int MYVAR = 1;
    std::cout << MYVAR << std::endl;
    return 0;
}
