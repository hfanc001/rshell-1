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
	
	bool T = true;
	while (T == true) {
		string cmds;
		getline(cin, cmds);

		int sz = cmds.size();

		char *parse = new char[sz+1];
		strcpy(parse, cmds.c_str());
	
		int comment = 0;
		for (int i = 0; i < cmds.size(); i++)
		{
			if (cmds.at(i) == '#')
			{
			parse[i] = '\0';
			break;
			}
		}
	
		char** cstr = new char*[sz+1];

		int counter = 0;


        	char *pch = strtok(parse, " "); 
        	while (pch != NULL)
        	{
        		cstr[counter] = pch; 
        		counter++; 
        		pch = strtok(NULL, " "); 
       		}
	        cstr[counter] = NULL;

		pid_t pid = fork();
		

		if (pid == -1)
		{
			cout << "child failed" << endl;
			exit(1);
		}

		else if(pid == 0)
		{
			int run = 0;
			while (run < argc) {
				execvp(cstr[run],cstr);
				run++;
			}
			exit(0);
		}
	}
	return 0;
}
