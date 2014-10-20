all: bin rshell

bin:
	mkdir bin

rshell:
	g++ -Wall -Werror src/hw0.cpp -o ./bin/rshell
