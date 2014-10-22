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

                if (cmds.find("exit") != std::string::npos) {
                        exit(0);//exits the program if exit is found
                }

		bool orS = false;// ||
		bool amper = false;// &&

		if (cmds.find("||") != std::string::npos) {//if user inputed ||
			orS = true;
		}
		if (cmds.find("&&") != std::string::npos) {//if user inputed &&
			amper = true;
		}


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

		vector<char*> first;//vctr to hold all commands

        	char* pch = strtok(parse, " ");//parsing with space char
        	while (pch != NULL)//until we hit a null character
        	{
        		cstr[counter] = pch;//parsing into cstr
			first.push_back(pch);//push back cmds
        		counter++;//increment the counter
        		pch = strtok(NULL, " ");//continue parsing
       		}
	        cstr[counter] = NULL;//to avoid a seg fault when running

		unsigned int i = 0;//counter for cmds
		while (i < first.size()) {
			int pid = fork();//child

				
		
	                if(pid == 0)//if child
        	        {
          			if(execvp(first.at(i),&first.at(i)) == -1) {//run the cmds
                       	        	perror("Command failed to run");//err msg
                               		exit(1);
                      		}

				//else if (orS == true) {//efforts to make or work
				//	if (execvp(first.at(i),&first.at(i)) == -1) {
				//	i+=2;
				//	execvp(first.at(i), &first.at(i));
				//	exit(1);
				//	}
				//}
				exit(0);
			}




			else if (pid == -1)//if a child fails
			{
				perror("Child failed");//err message
				exit(1);
			}

			else {
				if (wait(NULL) == -1) {//wait for child
					perror("wait failed");//err message
				}
			}
			i++;//increment counter
		}
	}
	return 0;
}
