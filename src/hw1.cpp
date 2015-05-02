#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <algorithm>
#include <iomanip>

using namespace std;

bool sort_cap(char* a, char* b) {
	return (tolower(static_cast<unsigned char>(a[0])) < tolower(static_cast<unsigned char>(b[0])));
}

void printdir(struct stat s, const char* dir, const bool &l, const bool &a, const bool &R) {
	vector<string> subdirs;
	vector<char*> thingstoprint;
	string toprint;
	string currentdirectory = dir;
	cout << currentdirectory << endl;
	string dname = "";
	string sub = "";
	
//	struct group *grp;
//	struct passwd *usr;
	struct tm *clk;
	string oldtime;
	int totalcount = 0;

	DIR *dirp;
	if(NULL == (dirp = opendir(dir))) {
		perror("error with oepndir(). ");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	
	while(NULL != (filespecs = readdir(dirp))) {
		dname = filespecs->d_name;
		sub = currentdirectory + '/' + dname;
		if(-1 == stat(sub.c_str(), &s)) {		
			cout << "theo" << sub.c_str() << endl;
			perror("error with stat");
			cout << "not l: could not find file or directory" << endl;
			exit(1);
		}
		if(S_IFDIR & s.st_mode &&
			(strcmp(filespecs->d_name, ".") != 0 &&
			strcmp(filespecs->d_name, "..") != 0)) {
				//char* vector stuff here with path appended or whatever
				subdirs.push_back(sub);
		}
	
		if(a || (filespecs->d_name[0] != '.')) {
			if(l) {
				clk = gmtime(&(s.st_mtime));
				if(NULL == clk) {
					perror("error with gmtime");
					exit(1);
				}
				string oldtime = asctime(clk);
				oldtime = oldtime.erase(0, 4);
				oldtime = oldtime.erase(12, 9);

			cout	<< ((S_IFDIR & s.st_mode) ? "d":"-")
				<< ((S_IRUSR & s.st_mode) ? "r":"-")
				<< ((S_IWUSR & s.st_mode) ? "w":"-")
				<< ((S_IXUSR & s.st_mode) ? "x":"-")
				<< ((S_IRGRP & s.st_mode) ? "r":"-")
				<< ((S_IWGRP & s.st_mode) ? "w":"-")
				<< ((S_IXGRP & s.st_mode) ? "x":"-")
				<< ((S_IROTH & s.st_mode) ? "r":"-")
				<< ((S_IWOTH & s.st_mode) ? "w":"-")
				<< ((S_IXOTH & s.st_mode) ? "x":"-")
				<< ' ' << setw(3) << left
				<< s.st_nlink << setw(10) << left
//				<< usr->pw_name << ' ' << grp->gr_name << ' '
				<< s.st_uid << setw(7) << left << s.st_gid << setw(7) << left
				<< s.st_size << setw(15) << left //some block crap here
				<< oldtime << setw(10) << left 
				<< filespecs->d_name << endl;
				
				totalcount += s.st_blocks;
			} else {
				cout << filespecs->d_name << ' ';
			}
		}
		thingstoprint.push_back(filespecs->d_name);
	}
 
	if(errno != 0) {
			perror("error with readdir()");
			exit(1);
		}
	if(-1 == closedir(dirp)) {
		perror("error with closedir()");
		exit(1);
	}
	
/*	
	sort(thingstoprint.begin(), thingstoprint.end());

	for(size_t i = 0; i < thingstoprint.size(); i++) {
 		dname = thingstoprint.at(i);
		sub = currentdirectory + '/' + dname;
		if(-1 == stat(sub.c_str(), &s)) {		
			cout << "theo" << sub.c_str() << endl;
			perror("error with stat");
			cout << "not l: could not find file or directory" << endl;
			exit(1);
		}
		if(S_IFDIR & s.st_mode &&
			(strcmp(thingstoprint.at(i), ".") != 0 &&
			strcmp(thingstoprint.at(i), "..") != 0)) {
				//char* vector stuff here with path appended or whatever
				subdirs.push_back(sub);
		}
	
		if(a || (thingstoprint.at(i)[0] != '.')) {
			if(l) {
				clk = gmtime(&(s.st_mtime));
				if(NULL == clk) {
					perror("error with gmtime");
					exit(1);
				}
				string oldtime = asctime(clk);
				oldtime = oldtime.erase(0, 4);
				oldtime = oldtime.erase(12, 9);

			cout	<< ((S_IFDIR & s.st_mode) ? "d":"-")
				<< ((S_IRUSR & s.st_mode) ? "r":"-")
				<< ((S_IWUSR & s.st_mode) ? "w":"-")
				<< ((S_IXUSR & s.st_mode) ? "x":"-")
				<< ((S_IRGRP & s.st_mode) ? "r":"-")
				<< ((S_IWGRP & s.st_mode) ? "w":"-")
				<< ((S_IXGRP & s.st_mode) ? "x":"-")
				<< ((S_IROTH & s.st_mode) ? "r":"-")
				<< ((S_IWOTH & s.st_mode) ? "w":"-")
				<< ((S_IXOTH & s.st_mode) ? "x":"-")
				<< setw(12) << left
				<< s.st_nlink << setw(10) << left
//				<< usr->pw_name << ' ' << grp->gr_name << ' '
				<< s.st_uid << setw(10) << left << s.st_gid << setw(10) << left
				<< s.st_size << setw(10) << left //some block crap here
				<< oldtime << setw(10) << left 
				<< filespecs->d_name;
				
				totalcount += s.st_blocks;
			} else {
				cout << thingstoprint.at(i) << ' ';
			}
		}
	}
*/


	
	cout << endl;
	if(l) {
		cout << "total = " << totalcount/2 << endl;
	}


	
	//recursion stuff here?
	if(R) {
		sort(subdirs.begin(), subdirs.end());
		cout << endl;
		for(size_t i = 0; i < subdirs.size(); i++) {
			struct stat blah;
			if(-1 == stat(subdirs.at(i).c_str(), &blah)) {
				perror("error with stat");
				cout << "could not find (sub) file or directory " <<
					subdirs.at(i) << endl;
				exit(1);
			}
			cout << subdirs.at(i) << ':' << endl;
			printdir(blah, subdirs.at(i).c_str(), l, a, R);
		}
	}
		
}

void getFlags(const int argc, char** &args, bool &l, bool &a, bool &R, bool &specified) {
	for(int i = 1; i < argc; i++) {
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

//	char dot[]  = ".";
	
	getFlags(argc, argv, el, ay, ar, specified);	
	
	vector<const char*> args;
	string arrg;
	if(specified) {
		for(int i = 1; i < argc; i++) {
			if(argv[i][0] != '-') {
				arrg = argv[i];
				args.push_back(arrg.c_str());
			}
		}
	} else {
		args.push_back(".");
	}

	sort(args.begin(), args.end());

	struct group *grp;
	struct passwd *usr;

	struct stat s;

//	if(specified) {
	for(size_t i = 0; i < args.size(); i++) {
			if(-1 == stat(args.at(i), &s)) {
				perror("error with stat");
				cout << "could not find file or directory" << endl;
				exit(1);
			}

			if(S_IFDIR & s.st_mode) {	//it's a directory
				//do directory print stuff here
				//see dir_code.cpp file
				printdir(s, args.at(i), el, ay, ar);

			} else {			//it's a file
				if(el) {
				usr = getpwuid(s.st_uid);
				grp = getgrgid(s.st_gid);
				if(NULL == usr) perror("Error with getgrpid");
				if(NULL == grp) perror("Error with getgrgid");

			cout	<< ((S_IFDIR & s.st_mode) ? "d":"-")
				<< ((S_IRUSR & s.st_mode) ? "r":"-")
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
				<< usr->pw_name << ' ' << grp->gr_name << ' '
				<< s.st_size << ' ' //some block crap here
				<< ' ';
				}

				cout << args.at(i) << endl;
			}
		}

//	} else {
//		cout << "nothing specified" << endl;
//		if(-1 == stat(dot, &s)) {
//			perror("error with stat");
//			cout << "could not find file or directory" << endl;
//			exit(1);
//		}
//		printdir(s, dot, el, ay, ar);
//	}

	return 0;
}
