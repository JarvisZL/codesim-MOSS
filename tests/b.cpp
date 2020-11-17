#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
using namespace std;
int main(){
     	char filename[] = "hello-XXXXXX";
	int fd;
	if((fd = mkstemp(filename)) < 0){
		perror("mkstemp");
		exit(1);
	}
	cout << filename << endl;
	unlink(filename);
	close(fd);
	return 0;
}
