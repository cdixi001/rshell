all: bin/rshell

rshell: bin/rshell

bin/rshell: ./src/test.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/test.cpp -o ./bin/rshell

bin:
	mkdir bin

clean:
	rm -rf bin
