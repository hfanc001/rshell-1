// Name Alex Tan
// Net ID: atan009
// Assignment #0: rshell
// Lab Section: 022
// TA: Taeyoung Kim
// Due date: 10/21/14
// I hereby certify that I have no recieved assistance on this assignment,
// or used code, from ANY outside source other than the instruction team.

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <stdio.h>
using namespace std;
using namespace boost;

int main(int argc, char *argv[]) {
	
	bool T = true;//continuous loop to run entire program
	while (T == true) {//always true
		if (getlogin() != NULL) {//only runs if there is a login
			cout << getlogin() << "@";//display login and @
		}

		char hostname[255] = {0};//max bytes is 255 in hostname
		if (gethostname(hostname, 255) != -1) {//if there is a hostname
			gethostname(hostname, 255);//put hostname -> hostname
			cout << hostname << " ";//cout host name
		}

		cout << "$" << " ";//prompt


		string cmds;//what the user types in
		getline(cin, cmds);//takes what user types

		int sz = cmds.size();//size of what user typed in

		char *parse = new char[sz+1];//c string array using size of cmds
		strcpy(parse, cmds.c_str());//turn cmds into cstring in parse
	
		for (unsigned int i = 0; i < cmds.size(); i++)//loop for comment
		{
			if (cmds.at(i) == '#')//if user inputed a comment
			{
			parse[i] = '\0';//change char in parse to null
			break;
			}//break b/c everything after null char will be ignored
		}
	
		char** cstr = new char*[sz+1];//what parse will be parsed into

		int counter = 0;//used to keep track of the end of cstr

		if (cmds.find("exit") != std::string::npos) {
			exit(0);//exits the program if exit is found
		}

        	char *pch = strtok(parse, " ;");//parsing with space char
        	while (pch != NULL)//until we hit a null character
        	{
        		cstr[counter] = pch;//parsing into cstr
			cout << pch;
        		counter++;//increment the counter
        		pch = strtok(NULL, " ");//continue parsing
       		}
	        cstr[counter] = NULL;//to avoid a seg fault when running

		int pid = fork();//child
		

		if (pid == -1)//if a child fails
		{
			perror("Child failed");
			exit(1);
		}

		else if(pid == 0)//if parent
		{
			wait(0);//wait until child finishes
			int run = 0;//starting number of commands

			while (run < argc) {//first to last command
				if(execvp(cstr[run],cstr) == -1) {//run the cmds
					perror("Command failed to run");
					exit(1);
				}
				run++;
			}

			exit(0);
		}
	}
	return 0;
}
