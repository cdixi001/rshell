#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>

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
	//~~~~~~~~~~~~print username, hostnam, and prompte~~~~~~~~
	char host[32];//add to readme: 32 cuz max is 64 and i dont want long printing
	int pid;
	
	string arg;
	
	pid = gethostname(host, 32);
	if(pid == -1) {				//error
		perror("gethostname");
	} else {
		 cout << getlogin() << "§" << host << ':' << ' ';
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//shit starts here
	//we will begin tonight's program by taking in 
	getline(cin, arg);
	cout << arg << endl;
	
	//add spaces before and after all operators in arg
	addSpaces(arg, ";");
	addSpaces(arg, "&&");
	addSpaces(arg, "||");

	cout << arg << endl;	//testing if addSpaces actually works..
	
	vector<char*> words;
	words = parser(arg, " "); //take out all spaces from arg
	words.push_back(NULL);





	char **command = &words[0]; //stackoverflow.com/questions/5846934/

	vector<char*> onecommand;
	
	size_t i = 0;
	for(i = 0; strcmp(words.at(i), ";") && strcmp(words.at(i), "&&") &&  strcmp(words.at(i), "||"); i++) {
		cout << "theo1";
		onecommand.push_back(words.at(i));
	}
	
	//deleteme printig onecommand for testing
	for(int i = 0; i < onecommand.size(); i++)
		cout << "onecommand" << i << onecommand.at(i) << endl;
	command = &onecommand[0];
			
	//EXECVP STUFF HERE
	//http://github.com/mikeizbicki/ucr-cs100/blob/2015spring/textbook/assignment-help/syscalls/exec.md

	pid = fork();
	if(pid == -1) {
		perror("Error with fork");
		exit(1);
	} else if(pid == 0) {	//child process
		if(-1 == execvp(command[0], command)) {
			perror("Error wit execvp");
		}

		exit(1);
	}
	if(-1 == wait(0)) {
		perror("wait error");
	}
	//parent here
	





	
	
	//TESTING DELETE THIS LATER
	for(int i = 0; i < words.size(); i++) {
		cout << "words" << i << words.at(i) << endl;
	}
}

int main(int argc, char* argv[]) {

	cout << '$' << endl;

	terminal();

	return 0;
}

