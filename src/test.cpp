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
		if(text.substr(pos, length) != op) {
			text.insert(pos, " ");	//one space before
			text.insert(pos+1+length, " "); //and one space after
			pos = text.find(op, pos+1+length);
		} else pos = text.find(op, pos+2+length);
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

bool setfds(const vector<char*> &onecommand, size_t i, int redirector) {
	int fd;
	if(redirector == 0) {
			if(-1 == close(0)) {
				perror("error with close0");
			}
			fd = open(onecommand.at(i), O_RDONLY);
			if(-1 == fd) {
				perror("error with open0 daassqqqq");
			}
		} else if(redirector == 1) {
			if(-1 == close(1)) {
				perror("error with close1");
			}
			fd = open(onecommand.at(i), O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
			if(-1 == fd) {
				perror("error with open1");
			}
		} else if(redirector == 2) {
			cout << "redirector is 2" << endl;
			if(-1 == close(1)) {
				perror("error with close1");
			}
			cerr << "opening" << endl;
			fd = open(onecommand.at(i), O_WRONLY|O_CREAT|O_APPEND, S_IRWXU);
			if(-1 == fd) {
				perror("error with open1");
			}
		} else if(redirector == 4) {
			if(-1 == close(0)) {
				perror("error with close4");
			}
			
		}
return true;
}

bool execinout(const vector<char*> &onecommand, const vector<char*> newcommand, size_t i, int redirector) {

//~~~~~~~~~~~~~~~~~--------------------------------------------------------
//find next redirector to set future flags
	int nextredirector = -1;
	vector<char*> nextcommand;
	size_t j = i;
	for(j = j; (j < onecommand.size()- 1) && (nextredirector < 0); j++) {
		if(strcmp(onecommand.at(j), "<") == 0) {
			nextredirector = 0;		//input
		} else if(strcmp(onecommand.at(j), ">") == 0) {
			nextredirector = 1;		//single output
		} else if(strcmp(onecommand.at(j), ">>") == 0) {
			nextredirector = 2;		//double output
		} else if(strcmp(onecommand.at(j), "|") == 0) {
			nextredirector = 3;		//pipe
		} else {
			nextcommand.push_back(onecommand.at(j));
		}
	}
//~~~~~~~~~~~~~~~~~-------------------------------------------------------
	
	int forkid = fork();
	if(-1 == forkid) {
		perror("error with fork");
		exit(1);
	} else if(0 == forkid) {
		//the child
		
		setfds(onecommand, i, redirector);
		
//		if(nextredirector > -1) {
//			execinout(onecommand, nextcommand, j, nextredirector);
//		}


		if(-1 == execvp(newcommand.at(0), &newcommand[0])) {
			perror("erro with execvp");
		}
		cout << "before exit execvp" << endl;
		exit(0);
	}
	if(-1 == wait(0)) {
		perror("error with wait");
	}
		
	//parent here i guess
	return true;

}


bool redirtest(const vector<char*> &onecommand, vector<char*> leftside, size_t i, int prevredir) {
	if(prevredir < 3) {
		execinout(onecommand, leftside, i, prevredir);
	}
	

	//getting right side of pipe
	vector<char*> rightside;
	int redirector = -1;
	for(i = i; (i < onecommand.size()- 1) && (redirector < 0); i++) {
		if(strcmp(onecommand.at(i), "<") == 0) {
			redirector = 0;		//input
		} else if(strcmp(onecommand.at(i), ">") == 0) {
			redirector = 1;		//single output
		} else if(strcmp(onecommand.at(i), ">>") == 0) {
			redirector = 2;		//double output
		} else if(strcmp(onecommand.at(i), "|") == 0) {
			redirector = 3;		//pipe
		} else {
			rightside.push_back(onecommand.at(i));
		}
	}
	rightside.push_back(NULL);

	int fd[2];
	if(-1 == pipe(fd)) {
		perror(__FILE__ ": " "pipe");
	}
	
	int forkid = fork();
	if(-1 == forkid) {
		perror("error with fork");
	} else if(0 == forkid) {
		//child
		if(-1 == close(1)) {
			perror("error with close");
		}	//need to error check
		if(-1 == dup(fd[1])) {
			perror("error with dup");
		}	//need to error check
		if(-1 == close(fd[0])) {
			perror("error with close");
		}

		if(-1 == execvp(leftside.at(0), &leftside[0])) {
			perror("erorr with execvp");
		}
		exit(1);
	} else {
	//parent
		if(-1 == wait(0)) {
			perror("error with waitpid");
		}

		if(-1 == close(fd[1])) {
			perror("error with close");
		}

		int backupin;
		if(-1 == (backupin = dup(0))) {
			perror("error with dup");
		}
		if(-1 == close(0)) {
			perror("error with close");
		}
		int blah;
		if(-1 == (blah = dup(fd[0]))) {
			perror("error with dup");
		}
		if(-1 == close(fd[0])) {
			perror("error with close");
		}

		if(rightside.size() > 1) {
			redirtest(onecommand, rightside, i, redirector);
		}

		

		//restore backupin
		if(-1 == dup2(backupin, 0)) {
			perror("error dup2");
		}

	}

	return true;
}




//pipes
bool redir3(const vector<char*> &onecommand, vector<char*> leftside, size_t i) {
	//getting right side of pipe
	vector<char*> rightside;
	int redirector = -1;
	for(i = i; (i < onecommand.size()- 1) && (redirector < 0); i++) {
		if(strcmp(onecommand.at(i), "<") == 0) {
			redirector = 0;		//input
		} else if(strcmp(onecommand.at(i), ">") == 0) {
			redirector = 1;		//single output
		} else if(strcmp(onecommand.at(i), ">>") == 0) {
			redirector = 2;		//double output
		} else if(strcmp(onecommand.at(i), "|") == 0) {
			redirector = 3;		//pipe
		} else {
			rightside.push_back(onecommand.at(i));
		}
	}
	rightside.push_back(NULL);

	int fd[2];
	if(-1 == pipe(fd)) {
		perror(__FILE__ ": " "pipe");
	}
	
	int forkid = fork();
	if(-1 == forkid) {
		perror("error with fork");
	} else if(0 == forkid) {
		//child
		
		if(-1 == close(1)) {
			perror("error with close");
		}	//need to error check
		if(-1 == dup(fd[1])) {
			perror("error with dup");
		}	//need to error check
		if(-1 == close(fd[0])) {
			perror("error with close");
		}

		if(-1 == execvp(leftside.at(0), &leftside[0])) {
			perror("erorr with execvp");
		}
		exit(1);
	} else {
	//parent
				//fork again
		int forkid2 = fork();
		if(-1 == forkid2) {
			perror("error with fork2");
		} else if(0 == forkid2) {
			if(-1 == close(fd[1])) {
				perror("error with close");
			}
			if(-1 == close(0)) {	//need to error check
				perror("error with close");
			}
			if(-1 == dup(fd[0])) {	//need to error check
				perror("error with dup");
			}
			
			//close(1) dup(fd2[1])

			if(-1 == execvp(rightside.at(0), &rightside[0])) {
				perror("eror with execvp");
			}
			exit(1);
		}	

	
	}	//for fork1 else

	if(-1 == close(fd[0])) {
		perror("error with close");
	}
	if(-1 == close(fd[1])) {
		perror("error with close");
	}
	
	//parent again. waiting
	if(-1 == wait(0)) {
		perror("error with waitpid");
	}

	if(-1 == wait(0)) {
		perror("error with wait2");
	}


	return true;
}


bool redirectors(const vector<char*> &onecommand) {
	size_t i = 0;
	//find the first redirector
	//putting everything before it into char* vector along the way
	int redirector = -1;
	vector<char*> newcommand;

//while(i < onecommand.size() - 1) {
	for(i = i; (i < onecommand.size()- 1) && (redirector < 0); i++) {
		if(strcmp(onecommand.at(i), "<") == 0) {
			redirector = 0;		//input
		} else if(strcmp(onecommand.at(i), ">>") == 0) {
			redirector = 2;		//single output
		} else if(strcmp(onecommand.at(i), ">") == 0) {
			redirector = 1;		//double output
		} else if(strcmp(onecommand.at(i), "|") == 0) {
			redirector = 3;		//pipe
		} else {
			newcommand.push_back(onecommand.at(i));
		}
	}


	newcommand.push_back(NULL);
	
	//this is supposed to take care of position of redirector
	//no redirector, redirector is last thing, redirector is first thing
	if(redirector < 0 || i >= onecommand.size() - 1 || i-1 == 0) return false;

	//it's an input
	if(redirector < 3) {
		execinout(onecommand, newcommand, i, redirector);
	} else if(redirector == 3) {
		redirtest(onecommand, newcommand, i, redirector);	
	}


	redirector = -1;
	newcommand.clear();
//}

	return true;
}



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
	//spaces for redirection
	addSpaces(input, "<");
	addSpaces(input, ">>");
	addSpaces(input, ">");
	addSpaces(input, "|");


	
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

	bool hasredirectors = false;

	
	size_t i = 0;
	
	while(i < words.size()) {
		issemi = strcmp(words.at(i), ";") == 0;
		isand = strcmp(words.at(i), "&&") == 0;
		isor = strcmp(words.at(i), "||") == 0;
		ispound = strcmp(words.at(i), "#") == 0;
		isoperator = issemi || isand || isor || ispound;
	
	//once we find in onecommand, don't want to reset it
		if(hasredirectors == false) {
			hasredirectors = (strcmp(words.at(i), "<") == 0) ||
					(strcmp(words.at(i), ">") == 0) ||
					(strcmp(words.at(i), ">>") == 0) ||
					(strcmp(words.at(i), "|") == 0);
		}

		if(!isoperator) {
			onecommand.push_back(words.at(i));
		//	for(int j = 0; j < onecommand.size(); j++) cout << "oc " << j << onecommand.at(j) << endl;
		} else {
			onecommand.push_back(NULL);	//needs to end in null for exec
			if(hasredirectors) {
				//redirectors takes care of chained
				success = redirectors(onecommand);
			} else success = execvpstuff(onecommand);
			hasredirectors = false;	//have to reset the flag
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
