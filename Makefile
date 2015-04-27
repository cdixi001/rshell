all: bin/rshell bin/cp

rshell: bin/rshell

bin/rshell: ./src/test.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/test.cpp -o ./bin/rshell

bin/cp: ./src/cp.cpp | bin
	 g++ -Wall -Werror -ansi -pedantic ./src/cp.cpp -o ./bin/cp

bin:
	mkdir bin

clean:
	rm -rf bin
