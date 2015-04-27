#include <iostream>
#include "Timer.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

void readwrite(const int bufsize, char *file1, char *file2){
	int fdnew;
	int fdold;
	int size;
	if(-1 == (fdold = open(file1 , O_RDONLY))){
		perror("There was an error with open(). ");
		exit(1);
	}
	if(-1 == (fdnew = open(file2 , O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR))){
		perror("There was an error with open(). ");
		exit(1);
	}
	char *c = new char[bufsize];
	do{
		if(-1 == (size = read(fdold, c, sizeof(c)))){
			perror("There was an error with read(). ");
			exit(1);
		}
		if(-1 == write(fdnew, c, size)){
			perror("There was an error with write(). ");
				exit(1);
		} 
	
	} while(size > 0);
	if(-1 == close(fdnew)){
		perror("There was an errir with close(). ");
		exit(1);
	}
	if(-1 == close(fdold)){
		perror("There was an error with close(). ");
		exit(1);
	}
	delete [] c;
}

bool exists(char *file){
	ifstream infile(file);
	if(infile.good()){
		return 1;
	}
	return 0;
}

int main(int argc, char **argv){
	if(argc < 3)
		exit(1);

	if(!exists(argv[1])) {
		cerr << "PRAALAM: Source file does not exist!" << endl;
		exit(1);
	}
	if(exists(argv[2])) {
		cerr << "PRAALAM: Destination file already exists." << endl;
		exit(1);
	}
	Timer t;
	double eTime;
	ofstream outfile(argv[2]);
	t.start();
	// copy file char by char
	if(argc > 3){
		if(strcmp(argv[3], "1") == 0){
			cout << "Method 1 is being used." << endl;
			ifstream inFile(argv[1]);
			char c;
			while(inFile.get(c)){
				outfile.put(c);	
			}
			inFile.close();
		}
		// copy file using syscalls
		else{
			int size;
			if((strcmp(argv[3], "2")) == 0){
				cout << "Method 2 is being used." << endl;
				size = 1;
			}
			else{	
				cout << "Method 3 is being used." << endl;
				size = BUFSIZ;
			}
			readwrite(size, argv[1], argv[2]);
		}
	} else {
		int size;
		cout << "Method 3 is being used." << endl;
		size = BUFSIZ;
		readwrite(size, argv[1], argv[2]);
	}
	t.elapsedUserTime(eTime);
	cout << "UserTime: " << eTime << endl;
	t.elapsedWallclockTime(eTime);
	cout << "Wallclock: " << eTime << endl;
	t.elapsedSystemTime(eTime);
	cout << "SystemTime: " << eTime << endl;
	return 0;
}
