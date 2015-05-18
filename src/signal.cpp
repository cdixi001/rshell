#include<iostream>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>

using namespace std;

int Int=0,Quit=0,Stop=0;



void sigHandler(int x){
    if(x == SIGINT){
        ++Int;
        cout << "C";
    }else if(x == SIGQUIT){
        ++Quit;
        cout << "\\";
    }else if(x == SIGTSTP){
        ++Stop;
        if(Stop == 3){
            cout << "^C:"<<Int<<endl;
            cout << "^\\:" << Quit << endl;
            cout << "^Z:" << Stop << endl;
            exit(0);
        }else{
            cout << "S";
            if(raise(SIGSTOP)!=0){
                perror("raise");
                exit(1);
            }
        }
    }

}

int main(){
    struct sigaction newAction, curAction;
    newAction.sa_handler = sigHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = 0;
    
    if(sigaction(SIGINT, &newAction,NULL)<0){
        perror("sigaction c");
        exit(1);
    }
    if(sigaction(SIGQUIT, &newAction,NULL)<0){
        perror("sigaction \\");
        exit(1);
    }
    if(sigaction(SIGTSTP, &newAction,NULL)<0){
        perror("sigaction z");
        exit(1);
    }   

    while(1){
        cout << 'x' << flush;
        sleep(1);
    }
}
