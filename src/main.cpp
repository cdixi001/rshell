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
/*
vector<char*> parser(string text, const char *delim) {
	char *line = new char[arg.length()+1];	//strtok requires char*					//got this from cplusplus.com/reference/string/string/c_str
	vector<char*> words;
	strcpy(line, text.c_str());


	char *token = strtok(line, delim);	//can be any character


	while(token != NULL) {	
		cout << "toekn" << token << endl;
		words.push_back(token);
		token = strtok(NULL, delim);
	}
cout << words.at(0);	
cout << words.at(0);
	delete[] line;		//this goddamn line messig it up
	return words;
}
*/


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
	char delim[] = " ";

	//~~~~~~~~~~~~PARSER~~~~~~~~~~~~~~~~~~
//used to be a bbeautiful function then stupid delete crap messed it upu
	char *line = new char[arg.length()+1];	//strtok requires char*					//got this from cplusplus.com/reference/string/string/c_str
	strcpy(line, arg.c_str());


	char *token = strtok(line, delim);	//can be any character


	while(token != NULL) {	
		words.push_back(token);
		token = strtok(NULL, delim);
	}


	//~~~~~~~~~~~~END PARSER~~~~~~~~~~~~~~~~

	//words = parser(arg, " "); //take out all spaces from arg
	words.push_back('\0');
	

	cout << "theo5" << endl;

	char **command = &words[0]; //stackoverflow.com/questions/5846934/
	cout << "theo6" << endl;	

	vector<char*> onecommand;
	
	unsigned int i = 0;
	//bool issemi = strcmp(words.at(i), ";") == 0;
	//bool isand = strcmp(words.at(i), "&&") == 0;
	//bool isor = strcmp(words.at(i), "||") == 0;
	//bool isoperator = issemi || isand || isor;
	bool prev = 1;
	//bool curr = 1;
	bool test = i < words.size();
	cout << "isoperator" << isoperator << endl;

	while(i < words.size()){
		cout << "theo1" << test;
		onecommand.push_back(words.at(i));
		i++;
	
	//	issemi = strcmp(words.at(i), ";") == 0;
	//	isand = strcmp(words.at(i), "&&") == 0;
	//	isor = strcmp(words.at(i), "||") == 0;
	//	isoperator = issemi || isand || isor;
		cout << "theo3" << endl;
	}
	cout << "theo4" << endl;
	if(1) {
		if(issemi) {
			i++;
		}
		if(isand && prev) {
			i++;
		}
		if(isor && !prev) {
			i++;
		}
	} else {
		
		//deleteme printig onecommand for testing
		for(unsigned int i = 0; i < onecommand.size(); i++)
			cout << "onecommand" << i << onecommand.at(i) << endl;
		command = &onecommand[0];
			
	//EXECVP STUFF HERE
	//http://github.com/mikeizbicki/ucr-cs100/blob/2015spring/textbook/assignment-help/syscalls/exec.md

		pid = fork();
		if(pid == -1) {
			prev = false;
			perror("Error with fork");
			exit(1);
		} else if(pid == 0) {	//child process
			if(-1 == execvp(command[0], command)) {
				prev = false;
				perror("Error wit execvp");
				exit(1);
			}
			prev = true;
			exit(1);
		}
		if(-1 == wait(0)) {
			perror("wait error");
		}
		//parent here


	}
		onecommand.clear(); //onto the next command!
		

	delete[] line;
	
	//TESTING DELETE THIS LATER
//	for(int i = 0; i < words.size(); i++) cout << "words" << i << words.at(i) << endl;
	
}

int main(int argc, char* argv[]) {

	cout << '$' << endl;

	terminal();

	return 0;
}

