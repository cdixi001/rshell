#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <vector>

using namespace std;

void printdir(struct stat s, const char* dir, const bool &l, const bool &a, const bool &R) {
	cout << "current directory: " << dir << endl;
	vector<const char*> subdirs;
	DIR *dirp;
	if(NULL == (dirp = opendir(dir))) {
		perror("error with oepndir(). ");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;

	if(l) {
		//l stuff here
	while(NULL != (filespecs = readdir(dirp))) {	
		if(a || (filespecs->d_name[0] != '.')) {
		
		if(-1 == stat(filespecs->d_name, &s)) {
			perror("error with stat");
			cout << "could not find file or directory" << endl;
			exit(1);
		}
 
		if(S_IFDIR & s.st_mode && (strcmp(filespecs->d_name, ".") != 0 && strcmp(filespecs->d_name, "..") != 0)) {
			cout << "d";
			char subdir[strlen(dir) + strlen(filespecs->d_name) + 1];
			strcpy(subdir, dir);
			strcat(subdir, "/");
			strcat(subdir, filespecs->d_name);
			subdirs.push_back(subdir);
			//char* vector stuff here with path appended or whatever
		} else cout << '-';

		cout	<< ((S_IRUSR & s.st_mode) ? "r":"-")
			<< ((S_IWUSR & s.st_mode) ? "w":"-")
			<< ((S_IXUSR & s.st_mode) ? "x":"-")
			<< ((S_IRGRP & s.st_mode) ? "r":"-")
			<< ((S_IWGRP & s.st_mode) ? "w":"-")
			<< ((S_IXGRP & s.st_mode) ? "x":"-")
			<< ((S_IROTH & s.st_mode) ? "r":"-")
			<< ((S_IWOTH & s.st_mode) ? "w":"-")
			<< ((S_IXOTH & s.st_mode) ? "x":"-")
			<< ' '
			<< s.st_nlink << ' '
			<< s.st_uid << ' ' << s.st_gid << ' '
			<< s.st_size << ' ' //some block crap here
			<< filespecs->d_name
			<< endl;
		}
		}



	} else {
		//non l stuff



		while(NULL != (filespecs = readdir(dirp))) {
			if(-1 == stat(filespecs->d_name, &s)) {
				
			cout << "theo" << filespecs->d_name;
				perror("error with stat");
				cout << "not l: could not find file or directory" << endl;
				exit(1);
			}
 			if(S_IFDIR & s.st_mode &&
			(strcmp(filespecs->d_name, ".") != 0 &&
			strcmp(filespecs->d_name, "..") != 0)) {
				//char* vector stuff here with path appended or whatever
				string currentdirectory = dir;
				string dname = filespecs->d_name;
				string sub = currentdirectory + '/' + dname;
				subdirs.push_back(sub.c_str());
			}

			if(a || filespecs->d_name[0] != '.') {
				cout << filespecs->d_name << ' ';
			}
		}



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
	//recursion stuff here?
	if(R) {
		cout << endl;
		for(int i = 0; i < subdirs.size(); i++) {
			struct stat blah;
			if(-1 == stat(subdirs.at(i), &blah)) {
				perror("error with stat");
				cout << "could not find file or directory" << endl;
				exit(1);
			}
			cout << subdirs.at(i) << ':' << endl;
			printdir(blah, subdirs.at(i), l, a, R);
		}
	}
		
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
				printdir(s, argv[i], el, ay, ar);

			} else {			//it's a file
				//


				cout << argv[i] << endl;
			}
		}
	}
	} else {
		cout << "nothing specified" << endl;
		if(-1 == stat(dot, &s)) {
			perror("error with stat");
			cout << "could not find file or directory" << endl;
			exit(1);
		}
		printdir(s, dot, el, ay, ar);
	}

	return 0;
}
