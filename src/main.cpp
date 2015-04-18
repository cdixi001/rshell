#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;


//takes in string and deliminator and tokenizes that string into vector<char*>
vector<char*> parser(string text, const char *delim) {
	char *line = new char[text.length()+1];	//strtok requires char*
	strcpy(line, text.c_str());


	char *token = strtok(line, delim);	//can be any character
	vector<char*> words;


	while(token != NULL) {	
		words.push_back(token);
		token = strtok(NULL, delim);
	}
	
	delete[] token;
	
	return words;
}



void terminal() {
	char host[32];//add to readme: 32 cuz max is 64 and i dont want long printing
	int pid;
	
	string arg;
	
	pid = gethostname(host, 32);
	if(pid == -1) {				//error
		perror("gethostname");
	} else {
		 cout << getlogin() << "§" << host << ':' << ' ';
	}	
	
	getline(cin, arg);
	cout << arg << endl;
	//tryig to get rid of \n \t etc
	//arg.replace(arg.find('\n'), 1, " ");
	vector<char*> words = parser(arg, " ");
	words.push_back(NULL);

	char **command = &words[0]; //stackoverflow.com/questions/5846934/
	
	//EXECVP STUFF HERE
	int status = execvp(command[0], command);
	
	//TESTING DELETE THIS LATER
	for(int i = 0; i < words.size(); i++) {
		cout << i <<  words.at(i) << endl;
	}
}

int main(int argc, char* argv[]) {

	cout << '$' << endl;

	terminal();

	return 0;
}

