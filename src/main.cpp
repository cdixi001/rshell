#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

vector<char*> parser(string text) {
	char *line = new char[text.length()+1];	//strtok requires char*
	strcpy(line, text.c_str());


	char *token = strtok(line, " ");	//can be any character
	vector<char*> words;

	words.push_back(token);

	while(token != NULL) {	
		words.push_back(token);
		token = strtok(NULL, " ");
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
		 cout << getlogin() << '%' << host << ':' << ' ';
	}	
	
	getline(cin, arg);	
	vector<char*> words = parser(arg);

	//TESTING DELETE THIS LATER
	for(int i = 0; i < words.size(); i++) {
		cout << words.at(i) << endl;
	}
}

int main(int argc, char* argv[]) {

	cout << '$' << endl;

	terminal();

	return 0;
}

