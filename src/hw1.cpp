#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

void printdir( char* dir, const bool &l, const bool &a, const bool &R) {
	if(l) {
		//l stuff here
	} else {
		//non l stuff
		DIR *dirp;
		if(NULL == (dirp = opendir(dir))) {
			perror("error with oepndir(). ");
			exit(1);
		}
		struct dirent *filespecs;
		errno = 0;
		while(NULL != (filespecs = readdir(dirp))) {
			if(filespecs->d_name[0] == '.') {
				if(a) {
					cout << filespecs->d_name << ' ';
				}
			} else cout << filespecs->d_name << ' ';
		}
		if(errno != 0) {
			perror("error with readdir()");
			exit(1);
		}
		cout << endl;
		if(-1 == closedir(dirp)) {
			perror("error with closedir()");
			exit(1);
		}

	}
	//recursion stuff here?
}

void getFlags(const int argc, char** &args, bool &l, bool &a, bool &R, bool &specified) {
	for(size_t i = 1; i < argc; i++) {
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
		} else if(args[i][0] > ' ') {	//32 is ascii for space character
			specified = 1;	//for ls with no file or dir specified
		}
	}
}	


int main(int argc, char** argv) {
	if(argc < 1) {
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
	bool specified = 0;	//tells us if a (file or dir) is specified

	char dot[]  = ".";
	
	getFlags(argc, argv, el, ay, ar, specified);	

	struct stat s;

	if(specified) {
	for(size_t i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			if(-1 == stat(argv[i], &s)) {
				perror("error with stat");
				cout << "could not find file or directory" << endl;
				exit(1);
			}

			if(S_IFDIR & s.st_mode) {	//it's a directory
				//do directory print stuff here
				//see dir_code.cpp file
				printdir(argv[i], el, ay, ar);

			} else {			//it's a file
				//


				cout << argv[i] << endl;
			}
		}
	}
	} else {
		printdir(dot, el, ay, ar);
	}

	return 0;
}
