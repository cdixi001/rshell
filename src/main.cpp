#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string>

using namespace std;

void terminal() {
	char host[32];//add to readme: 32 cuz max is 64 and i dont want long printing
	int pid;
	string arg;
	
	pid = gethostname(host, 32);
	if(pid == -1) {
		perror("gethostname");
	} else {
		 cout << getlogin() << '@' << host << ':';
		pid = fork();
		if(pid == -1) {
			perror("fork");
		} else if(pid == 0){
			if(-1 == execvp("pwd",NULL)) {
				perror("error in execvp");
			}
		} else {
			cout << "dask" << endl;		
		}
	}	

	cin >> arg;

	if(arg == "penis") {
		cout << ' ' << '|' << '\n';
		cout << ' ' << '|' << '\n';
		cout << ' ' << '|' << '\n';
		cout << 'o' << '|' << 'o' << endl;
	} else

	cout << arg << endl;

}

int main(int argc, char* argv[]) {

	cout << '$' << endl;
	terminal();

	return 0;
}

