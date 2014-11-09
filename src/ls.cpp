#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;

/*
 * This is a BARE BONES example of how to use opendir/readdir/closedir.  Notice
 * that there is no error checking on these functions.  You MUST add error 
 * checking yourself.
 */

void noflags() {
    char *dirName = ".";
    DIR *dirp = opendir(dirName);

        if (dirp == NULL) {
		perror("opendir");
	}

    dirent *direntp;
    while ((direntp = readdir(dirp))) {
	if (direntp->d_name[0] == '.') {
	    continue;
	}


        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;
	if (stat(dirName, &buf) == -1) {
	    perror("stat");
	}
	if (errno != 0) {
		perror("readdir");
	}
    }
    if (closedir(dirp) == -1) {
	perror("closedir");
    }

    cout << endl;

	return;
}

void aflagOnly() {
    char *dirName = ".";
    DIR *dirp = opendir(dirName);

        if (dirp == NULL) {
		perror("opendir");
	}

    dirent *direntp;
    while ((direntp = readdir(dirp))) {
        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;
	if (stat(dirName, &buf) == -1) {
	    perror("stat");
	}
	if (errno != 0) {
		perror("readdir");
	}
    }
    if (closedir(dirp) == -1) {
	perror("closedir");
    }

    cout << endl;

	return;
}

int main() {

    string getflags;
    getline(cin,getflags);


    bool aflag = false;
    bool lflag = false;
    bool rflag = false;


    if (getflags.find("ls") == std::string::npos) {
	exit(0);
    }

    if (getflags.find("-a") != std::string::npos) {
	aflag = true;
    }

    if (getflags.find("-l") != std::string::npos) {
	lflag = true;
    }

	if (getflags.find("-R") != std::string::npos) {
        rflag = true;
    }


    if(aflag == false && lflag == false && rflag == false) {
	noflags();
    }

    else if (aflag == true && lflag == false && rflag == false) {
	aflagOnly();
    }

	return 0;
}
