#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void printdir(const int argc, char** &args, const bool &l, const bool &a, const bool &R) {
	if(l) {
		//l stuff here
	} else {
		//non l stuff
	}
	//recursion stuff here?

void getFlags(const int argc, char** &args, bool &l, bool &a, bool &R) {
	for(size_t i = 0; i < argc; i++) {
		if(args[i][0] == '-') {
			for(int j = 0; args[i][j]; j++) {
				switch(args[i][j]) {
					case 'l':	//may not need ''
						l = true;
						break;
					case 'a':
						a = true;
						break;
					case 'R':
						R = true;
						break;
					default:
						break;
				}
			}
		}
	}
}	


int main(int argc, char** argv) {
	if(argc < 2) {
		cout << "need ore argc" << endl;
		exit(1);
	}
	
	//loop through argv, update flags
	//loop through argv, ignoring -'s, do stat to see if file or directory exists
	//if directory, do print direcoty stuff with flag option things
	//if file do file print stuff with flag option things

	bool el = 0;
	bool ay = 0;
	bool ar = 0;
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
		//regardless of flags, cout the  name of the file


		cout << argv[1] << endl;
		if(argv[1][0] == '-') {
			cout << "it's a dash" << endl;
		}
	}


	return 0;
}
