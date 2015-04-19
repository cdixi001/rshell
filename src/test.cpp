#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>

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


/*
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
*/

bool execvpstuff(vector<char*> &onecommand) {
	char **command = &onecommand[0]; //stackoverflow.com/questions/5846934/
		//EXECVP STUFF HERE
	//http://github.com/mikeizbicki/ucr-cs100/blob/2015spring/textbook/assignment-help/syscalls/exec.md
	if(onecommand.size() < 2) {
		return true;
	}
	
	if(strcmp(command[0], "exit") == 0) {
		cout << "theo exit" << endl;
		exit(0);
	} else {

	int pid = fork();
	if(pid == -1) {
		perror("Error with fork");
		exit(1);
	} else if(pid == 0) {	//child process
		if(-1 == execvp(command[0], command)) {
			perror("Error wit execvp");
		}
		onecommand.clear();		//clear vector so it can hold next command.
		return false;
	}
	if(-1 == wait(0)) {
		return false;
		perror("wait error");
	}
	//parent here
	onecommand.clear();
	return true;
	}
}


void terminal() {
	//~~~~~~~~~~~~print username, hostnam, and prompte~~~~~~~~
	char host[32];//add to readme: 32 cuz max is 64 and i dont want long printing
	int pid;
	
	string input;
	
	pid = gethostname(host, 32);
	if(pid == -1) {				//error
		perror("gethostname");
	} else {
		 cout << getlogin() << "§" << host << ':' << ' ';
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//shit starts here
	//we will begin tonight's program by taking in 
	getline(cin, input);
	cout << input << endl;
	
	//add spaces before and after all operators in input
	addSpaces(input, ";");
	addSpaces(input, "&&");
	addSpaces(input, "||");
	addSpaces(input, "#");

	cout << input << endl;	//testing if addSpaces actually works..
	
	//vector<char*> words;
	//words = parser(arg, " "); //take out all spaces from arg


//~~~~~~~~~~~~~~~~~~~~~~~~~~PARSER~~~~~~~~~~~~~~~~~~~~~~~~~~
	char *line = new char[input.length()+1];	//strtok requires char*
						//got this from cplusplus.com/reference/string/string/c_str
	strcpy(line, input.c_str());


	char *token = strtok(line, " ");	//can be any character
	vector<char*> words;


	while(token != NULL) {	
		words.push_back(token);
		token = strtok(NULL, " ");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

	char semi[] = ";";
	words.push_back(semi);


	for(size_t i = 0; i < words.size(); i++) {
		cout << "words " << i << words.at(i) << endl;
	}



	vector<char*> onecommand;
	onecommand.clear();

	bool success = true;	//status of previous command. used for operators	
	
	bool issemi;
	bool isand;
	bool isor;
	bool ispound;
	bool isoperator;

	bool isexit;
	bool isblank;

	
	
	size_t i = 0;
	
	while(i < words.size()) {
		issemi = strcmp(words.at(i), ";") == 0;
		isand = strcmp(words.at(i), "&&") == 0;
		isor = strcmp(words.at(i), "||") == 0;
		ispound = strcmp(words.at(i), "#") == 0;
		isoperator = issemi || isand || isor || ispound;
		
		cout << "theo"<< i << words.at(i) << endl;

		if(!isoperator) {
			onecommand.push_back(words.at(i));
			for(int j = 0; j < onecommand.size(); j++) cout << "oc " << j << onecommand.at(j) << endl;
		} else {
			onecommand.push_back(NULL);
			success = execvpstuff(onecommand);
			if(issemi) {
				onecommand.clear();
				cout << "issemi" << endl;
				//moveon
			} else if(isand && success) {
				//moveon
				onecommand.clear();
				cout << "isand && success" << endl;
			} else if(isor && !success) {
				onecommand.clear();
				cout << "isor && success" << endl;
				//moveon
			} else {
				break;
			}
		}
		i++;
	}
		

	cout << "outside while" << endl;




	delete[] line;
	
	
	//TESTING DELETE THIS LATER
//	for(int i = 0; i < words.size(); i++) cout << "words" << i << words.at(i) << endl;

}

int main(int argc, char* argv[]) {

	cout << '$' << endl;
	
	while(1)
	terminal();

	return 0;
}

