all: bin/rshell bin/cp bin/ls

rshell: bin/rshell

bin/rshell: ./src/test.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/test.cpp -o ./bin/rshell

bin/cp: ./src/cp.cpp | bin
	 g++ -Wall -Werror -ansi -pedantic ./src/cp.cpp -o ./bin/cp

bin/ls: ./src/hw1.cpp | bin
	 g++ -Wall -Werror -ansi -pedantic ./src/hw1.cpp -o ./bin/ls

bin:
	mkdir bin

clean:
	rm -rf bin
