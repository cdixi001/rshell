all: bin/rshell bin/cp bin/ls bin/rm bin/mv

rshell: bin/rshell

bin/rshell: ./src/test.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/test.cpp -o ./bin/rshell

bin/cp: ./src/cp.cpp | bin
	 g++ -Wall -Werror -ansi -pedantic ./src/cp.cpp -o ./bin/cp

bin/ls: ./src/hw1.cpp | bin
	 g++ -Wall -Werror -ansi -pedantic ./src/hw1.cpp -o ./bin/ls

bin/rm: ./src/rm.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/rm.cpp -o ./bin/rm

bin/mv: ./src/mv.cpp | bin
	g++ -Wall -Werror -ansi -pedantic ./src/mv.cpp -o ./bin/mv


bin:
	mkdir bin

clean:
	rm -rf bin
