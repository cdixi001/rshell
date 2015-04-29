#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {
	if(argc < 2) {
		cout << "need ore argc" << endl;
		exit(1);
	}

	struct stat s;
	if(-1 == stat(argv[1], &s)) {
		perror("error with stat");
		cout << "could not find file or directory" << endl;
		exit(1);
	}
	if(S_IFDIR & s.st_mode) {	//it's a directory
		//do directory print stuff here
		//see dir_code.cpp file
	} else {			//it's a file
		cout << argv[1] << endl;
	}


	return 0;
}
