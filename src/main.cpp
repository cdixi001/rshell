#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

//adds space before and after all instances of op in text to text
void addSpaces(string &text, const string &op) {
	size_t length = op.length();
	size_t pos;
	pos = text.find(op, 0);	//some inspiration from stackoverflow.com/questions/4034750


	while(pos != string::npos) {
		//pos+1 to go to thing pos was pointing at, but want to move AFTER that
		text.insert(pos, " ");	//one space before
		text.insert(pos+1+length, " "); //and one space after
		pos = text.find(op, pos+1+length);
	}
}

//takes in string and deliminator and tokenizes that string into vector<char*>
vector<char*> parser(string text, const char *delim) {
	char *line = new char[text.length()+1];	//strtok requires char*
						//got this from cplusplus.com/reference/string/string/c_str
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
	
	//operator stuff
	addSpaces(arg, ";");
	addSpaces(arg, "&&");
	addSpaces(arg, "||");

	cout << arg << endl;	//testing if addSpaces actually works..
	
	vector<char*> words;
	words = parser(arg, " ");
	//words.push_back(NULL); //don't need. vector already null terminanted

	char **command = &words[0]; //stackoverflow.com/questions/5846934/
	
	//EXECVP STUFF HERE
	//int status = execvp(command[0], command);
	
	
	//TESTING DELETE THIS LATER
	for(int i = 0; i < words.size(); i++) {
		cout << i;
		cout <<  words.at(i) << endl;
	}
}

int main(int argc, char* argv[]) {

	cout << '$' << endl;

	terminal();

	return 0;
}

