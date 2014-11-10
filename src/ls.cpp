#include <queue>
#include <vector>
#include <time.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <iostream>

using namespace std;

/*
 * This is a BARE BONES example of how to use opendir/readdir/closedir.  Notice
 * that there is no error checking on these functions.  You MUST add error 
 * checking yourself.
 */
void perm(dirent *direntp, struct stat buf) {
    lstat(direntp->d_name, &buf);


    if(S_ISREG(buf.st_mode)) {
	cout << '-';
    }
    else if(S_ISDIR(buf.st_mode)) {
	cout << 'd';
    }
    else if(S_ISCHR(buf.st_mode)) {
	cout << 'c';
    }
    else if(S_ISFIFO(buf.st_mode)) {
	cout << 'f';
    }
    else if(S_ISLNK(buf.st_mode)) {
	cout << 'l';
    }
    else if(S_ISSOCK(buf.st_mode)) {
	cout << 's';
    }


    (buf.st_mode & S_IRUSR) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWUSR) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXUSR) ? cout << 'x' : cout << '-';

    (buf.st_mode & S_IRGRP) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWGRP) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXGRP) ? cout << 'x' : cout << '-';

    (buf.st_mode & S_IROTH) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWOTH) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXOTH) ? cout << 'x' : cout << '-';
}

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

void lflagOnly() {
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

	struct stat buf;
	perm(direntp, buf);


	stat(direntp->d_name, &buf);
        cout << ' ';
	cout << buf.st_nlink << ' ';

	string userid = getpwuid(buf.st_uid)->pw_name;
	if (errno != 0) {
	    perror("userid");
	}

	cout << userid << ' ';

	string groupid = getgrgid(buf.st_gid)->gr_name;
	if (errno != 0) {
	    perror("group ID");
	}
	cout << groupid << ' ';

	cout << buf.st_size << ' ';

	time_t t = buf.st_mtime;
	struct tm lt;
	localtime_r(&t, &lt);
	char timbuf[80];
	strftime(timbuf, sizeof(timbuf), "%h", &lt);
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);
	cout << timbuf << ' ';

        cout << direntp->d_name << endl; //stat here to find attributes of file
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


	return;
}

void RflagOnly(const string DIRNAME) {
    const char *dirName = DIRNAME.c_str();
    DIR *dirp = opendir(dirName);

    if (dirp == NULL) {
	perror("opendir");
    }


    dirent *direntp;

	cout << DIRNAME << endl;

    vector<char*> NextDir;

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

	char test[999];
	strcpy(test, dirName);
	strcat(test, "/");
	strcat(test, direntp->d_name);


	stat(test, &buf);
	if (S_ISDIR(buf.st_mode)) {
		NextDir.push_back(direntp->d_name);
	}

    }
    cout << endl << endl;
    for (int i = 0; i < NextDir.size(); i++) {
	RflagOnly(DIRNAME+"/"+NextDir.at(i));
   }
    if (closedir(dirp) == -1) {
	perror("closedir");
    }



	return;
}

void alflag() {
    char *dirName = ".";
    DIR *dirp = opendir(dirName);

        if (dirp == NULL) {
		perror("opendir");
	}

    dirent *direntp;
    while ((direntp = readdir(dirp))) {

	struct stat buf;
	perm(direntp, buf);


	stat(direntp->d_name, &buf);
        cout << ' ';
	cout << buf.st_nlink << ' ';

	string userid = getpwuid(buf.st_uid)->pw_name;
	if (errno != 0) {
	    perror("userid");
	}

	cout << userid << ' ';

	string groupid = getgrgid(buf.st_gid)->gr_name;
	if (errno != 0) {
	    perror("group ID");
	}
	cout << groupid << ' ';

	cout << buf.st_size << ' ';

	time_t t = buf.st_mtime;
	struct tm lt;
	localtime_r(&t, &lt);
	char timbuf[80];
	strftime(timbuf, sizeof(timbuf), "%h", &lt);
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);
	cout << timbuf << ' ';

        cout << direntp->d_name << endl; //stat here to find attributes of file
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


    return;
}
int main() {

    string getflags;
    getline(cin,getflags);


    bool aflag = false;
    bool lflag = false;
    bool Rflag = false;

    string DNAME = ".";
    if (getflags.find("ls") == std::string::npos) {
	exit(0);
    }

    if (getflags.find("-a") != string::npos) {
	aflag = true;
    }

    if (getflags.find("-l") != string::npos) {
	lflag = true;
    }

    if (getflags.find("-R") != string::npos) {
        Rflag = true;
    }

    if (getflags.find("-al") != string::npos || getflags.find("-la") != string::npos) {
	aflag = true;
	lflag = true;
    }


    if(aflag == false && lflag == false && Rflag == false) {
	noflags();
    }

    else if (aflag == true && lflag == false && Rflag == false) {
	aflagOnly();
    }

    else if(aflag == false && lflag == true && Rflag == false) {
	lflagOnly();
    }

    else if(aflag == false && lflag == false && Rflag == true) {
	RflagOnly(DNAME);
    }

    else if(aflag == true && lflag == true && Rflag == false) {
	alflag();
    }


	return 0;
}
