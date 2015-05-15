#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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


//adds space before and after all instances of op in text to text
void addNumbers(string &text, const string &op) {
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

bool redir0(const vector<char*> &onecommand, size_t i) {
		//close stdin
		if(-1 == close(0)) {
			perror("error with close");
		}

		int fd = open(onecommand.at(i), O_RDONLY);	//open to lowest file desc
		if(-1 == fd) {
			perror("error with open");
			exit(1);
		}
}

bool redirectors(const vector<char*> &onecommand) {
	size_t i = 0;
	//find the first redirector
	//putting everything before it into char* vector along the way
	int redirector = -1;
	vector<char*> newcommand;
	for(i = 0; (i < onecommand.size() - 1) && (redirector < 0); i++) {
		if(strcmp(onecommand.at(i), "<") == 0) {
			redirector = 0;		//input
		} else if(strcmp(onecommand.at(i), ">") == 0) {
			redirector = 1;		//single output
		} else if(strcmp(onecommand.at(i), ">>") == 0) {
			redirector = 2;		//double output
		} else if(strcmp(onecommand.at(i), "|") == 0) {
			redirector = 3;		//pipe
		} else {
			newcommand.push_back(onecommand.at(i));
			cout << "theo" << onecommand.at(i) << endl;
		}
	}

	newcommand.push_back(NULL);
	
	if(redirector < 0) return false;

	int backup = dup(redirector);
	if(-1 == backup) {
		perror("error with dup");
	}
	//it's an input
	if(redirector == 0) {
		if(i < onecommand.size()) {		//check if file is in the vector
			redir0(onecommand, i);
		} else {
			perror("improper use of <");
			return false;
		}
	}

	int forkid = fork();
		if(-1 == forkid) {
			perror("error with fork");
			exit(1);
		} else if(0 == forkid) {
			//the child
			if(-1 == execvp(newcommand.at(0), &newcommand[0])) {
				perror("erro with execvp");
			}
			cout << "before exit execvp" << endl;
			exit(0);
		}
		cout << "in parent" << endl;
		if(-1 == wait(0)) {
			perror("error with wait");
		}

		
		//parent here i guess
		if(-1 == dup2(backup, redirector)) {
			perror("error with dup2");
		}
		if(-1 == close(backup)) {
			perror("error with close backup");
		}
		

	
	return true;
}

/*
vector<char*> redirectionstuff(vector<char*> &onecommand) {
	vector<char*> newcommand;
	for(unsigned int i = 0; i < onecommand.size(); i++) {
		if(strcmp(onecommand.at(i), "<") == 0) {
			if(i > 0 && i+1 > onecommand.size()) {
				//let's do input < first
				if(-1 == close(0)) {
					perror("close");
				}
				int fd = open(blah, O_RDONLY);	//opened to 0 since we closed 0
				if(-1 == fd) {
					perror("error with open");
					exit(1);
				}
				//run some execvp shti
				newcommand.push_back(onecommand.at(i-1));
				while(i < )
		
				//close(0);		//closing what i oepened
			} else {
				cout << "Invalid use of redirection" << endl;
				exit(1);
			}
		}
	}
	return newcommand;
}
*/			

bool execvpstuff(vector<char*> &onecommand) {
	bool returnval = true;
	int pipearr[2];

	char** command = &onecommand[0]; //stackoverflow.com/questions/5846934/

		//EXECVP STUFF HERE
	//http://github.com/mikeizbicki/ucr-cs100/blob/2015spring/textbook/assignment-help/syscalls/exec.md
	if(onecommand.size() < 2) {
		returnval = true;
	} else {
		
		if(strcmp(command[0], "exit") == 0) {
			onecommand.clear();
			exit(0);
		} else {
			pipe(pipearr);		//some pipe bs.
			int pid = fork();
			if(pid == -1) {
				perror("Error with fork");
				exit(1);
			} else if(pid == 0) {	//child process
				close(pipearr[0]);
				if(-1 == execvp(command[0], command)) {
					returnval = false;
					write(pipearr[1], &returnval, sizeof(returnval));
					close(pipearr[1]);
					perror("Error wit execvp");
				}
				onecommand.clear();		//clear vector so it can hold next command.
				exit(0);
			}
			if(-1 == wait(0)) {
				perror("error with wait");
			}
			close(pipearr[1]);
			read(pipearr[0], &returnval, sizeof(returnval));
			close(pipearr[0]);	
			//parent here
			onecommand.clear();
			return returnval;
		}
	}
	return returnval;
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
		printf("%c[%dm", 0x1B, 100);
		printf("%c[%dm", 0x1B, 96);
		cout << getlogin() << "Ω" << host << " §";
		printf("%c[%dm", 0x1B, 0);
		cout << ' ';
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//shit starts here
	//we will begin tonight's program by taking in 
	getline(cin, input);
	
	//add spaces before and after all operators in input
	addSpaces(input, ";");
	addSpaces(input, "&&");
	addSpaces(input, "||");
	addSpaces(input, "#");

	
	//vector<char*> words;
	//words = parser(arg, " "); //take out all spaces from arg


//~~~~~~~~~~~~~~~~~~~~~~~~~~PARSER~~~~~~~~~~~~~~~~~~~~~~~~~~
	char *line = new char[input.length()+1];	//strtok requires char*
						//got this from cplusplus.com/reference/string/string/c_str
	strcpy(line, input.c_str());


	char *token = strtok(line, " \t\n");	//can be any character
	vector<char*> words;


	while(token != NULL) {	
		words.push_back(token);
		token = strtok(NULL, " \t\n");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

	char cmenot[] = ";";
	words.push_back(cmenot);


	vector<char*> onecommand;
	onecommand.clear();

	bool success = true;	//status of previous command. used for operators	
	
	bool issemi;
	bool isand;
	bool isor;
	bool ispound;
	bool isoperator;

	
	size_t i = 0;
	
	while(i < words.size()) {
		issemi = strcmp(words.at(i), ";") == 0;
		isand = strcmp(words.at(i), "&&") == 0;
		isor = strcmp(words.at(i), "||") == 0;
		ispound = strcmp(words.at(i), "#") == 0;
		isoperator = issemi || isand || isor || ispound;
		
		if(!isoperator) {
			onecommand.push_back(words.at(i));
		//	for(int j = 0; j < onecommand.size(); j++) cout << "oc " << j << onecommand.at(j) << endl;
		} else {
			onecommand.push_back(NULL);	//needs to end in null for exec
			success = redirectors(onecommand);
			cout << success << endl;
	//		success = execvpstuff(onecommand);
			if(issemi) {
				onecommand.clear();
				//moveon
			} else if(isand && success) {
				//moveon
				onecommand.clear();
			} else if(isor && !success) {
				onecommand.clear();
				//moveon
			} else {
				break;
			}
		}
		i++;
	}
		



	delete[] line;
	
	
	//TESTING DELETE THIS LATER
//	for(int i = 0; i < words.size(); i++) cout << "words" << i << words.at(i) << endl;

}

int main(int argc, char* argv[]) {

	cout << "¤¤¡¡¡¡¡Bienvenidos a la terminál rshell de Chirag!!!!!¤¤" << endl;
	
	while(1)
	terminal();

	return 0;
}
