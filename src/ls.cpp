#include <vector>
#include <time.h>
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
void perm(dirent *direntp, struct stat buf) {//function that gets the permissions
    lstat(direntp->d_name, &buf);//stats current filename


    if(S_ISREG(buf.st_mode)) {//regular file
	cout << '-';
    }
    else if(S_ISDIR(buf.st_mode)) {//directory
	cout << 'd';
    }
    else if(S_ISCHR(buf.st_mode)) {//character device
	cout << 'c';
    }
    else if(S_ISFIFO(buf.st_mode)) {//named pipe
	cout << 'f';
    }
    else if(S_ISLNK(buf.st_mode)) {//linked device
	cout << 'l';
    }
    else if(S_ISSOCK(buf.st_mode)) {//socket
	cout << 's';
    }

    //read, write and execute permissions for user respectively
    (buf.st_mode & S_IRUSR) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWUSR) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXUSR) ? cout << 'x' : cout << '-';

    //read, write and execute permissions for group respectively
    (buf.st_mode & S_IRGRP) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWGRP) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXGRP) ? cout << 'x' : cout << '-';

    //read, write and execute permissions for anyone respectively
    (buf.st_mode & S_IROTH) ? cout << 'r' : cout << '-';
    (buf.st_mode & S_IWOTH) ? cout << 'w' : cout << '-';
    (buf.st_mode & S_IXOTH) ? cout << 'x' : cout << '-';
}

void noflags() {//no flags are given but ls is
    char *dirName = ".";//current directory
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }

    dirent *direntp;//consequent file to be read into
    while ((direntp = readdir(dirp))) {//while there is still a file to read into

	if (errno != 0) {//error checking
	    perror("readdir");
	}

	if (direntp->d_name[0] == '.') {//skip hidden files
	    continue;
	}


        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;
	if (stat(dirName, &buf) == -1) {//error checking and stat
	    perror("stat");
	}
    }


    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }

    cout << endl;

    return;
}

void aflagOnly() {//only -a flag
    char *dirName = ".";//current directory name
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }

    dirent *direntp;//read files into later
    while ((direntp = readdir(dirp))) {//while there are still files to be read

	if (errno != 0) {//error checking
	    perror("readdir");
	}

        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;
	if (stat(dirName, &buf) == -1) {//error checking and stat
	    perror("stat");
	}
    }

    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }

    cout << endl;

    return;
}

void lflagOnly() {//only -l flag
    char *dirName = ".";//current directory
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }

    dirent *direntp;//to read files into
    while ((direntp = readdir(dirp))) {//while there are still files
	if (errno != 0) {
	    perror("readdir");
	}

	if (direntp->d_name[0] == '.') {//skip hidden files
	    continue;
	}

	if (errno != 0) {
	    perror("readdir");
	}

	struct stat buf;
	perm(direntp, buf);//get permissions of this file


	stat(direntp->d_name, &buf);//stat file

	if (errno != 0) {
	    perror("stat");
	}

        cout << ' ';
	cout << buf.st_nlink << ' ';//number of files and directories in filename

	string userid = getpwuid(buf.st_uid)->pw_name;//get user ID
	if (errno != 0) {
	    perror("userid");
	}

	cout << userid << ' ';//prints User ID

	string groupid = getgrgid(buf.st_gid)->gr_name;//get group ID
	if (errno != 0) {
	    perror("group ID");
	}
	cout << groupid << ' ';//print group ID

	cout << buf.st_size << ' ';//print size of file in bytes

	time_t t = buf.st_mtime;//gets time of last modification
	struct tm lt;

	if(localtime_r(&t, &lt) == NULL) {//tranforms time into broken down ascii + error checking
	    perror("localtime");
	}

	char timbuf[80];//char array to hold the characters
	strftime(timbuf, sizeof(timbuf), "%h", &lt);//month of year abbreviated
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);//day of month zero padded
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);//24hour HH:MM
	cout << timbuf << ' ';

        cout << direntp->d_name << endl; //stat here to find attributes of file
	if (stat(dirName, &buf) == -1) {//stat + error checking
	    perror("stat");
	}
    }

    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }


    return;
}

void RflagOnly(const string DIRNAME) {//only R flag with DIRNAME for recursion
    const char *dirName = DIRNAME.c_str();//convert DIRNAME into a c-string
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }


    dirent *direntp;

    cout << DIRNAME << endl;//cout the filename

    vector<char*> NextDir;//vector to store directories

    while ((direntp = readdir(dirp))) {//while there are still files to be read
	if (errno != 0) {//error checking
	    perror("readdir");
	}

	if (direntp->d_name[0] == '.') {//skip hidden files
	    continue;
	}


        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;
	if (stat(dirName, &buf) == -1) {//stat + error checking
	    perror("stat");
	}

	char test[999];//character array used to concat next filename
	strcpy(test, dirName);//copies first filename
	strcat(test, "/");//concats '/' char for directory check
	strcat(test, direntp->d_name);//concats second filename


	stat(test, &buf);//stat new filename

	if (errno != 0) {//error checking
	    perror("stat");
	}

	if (S_ISDIR(buf.st_mode)) {//if it is a valid directory, store
		NextDir.push_back(direntp->d_name);
	}

    }
    cout << endl << endl;

    for (int i = 0; i < NextDir.size(); i++) {//for all stored directories, recursively call function again
	RflagOnly(DIRNAME+"/"+NextDir.at(i));
    }

    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }



    return;
}

void alflag() {//-a flag and -l flag
    char *dirName = ".";//current directory
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }

    dirent *direntp;
    while ((direntp = readdir(dirp))) {//while there are still files to be read
	if (errno != 0) {//error checking
	    perror("readdir");
	}

	struct stat buf;
	perm(direntp, buf);//get permissions


	stat(direntp->d_name, &buf);//stat

	if (errno != 0) {//error checking
	    perror("stat");
	}

        cout << ' ';
	cout << buf.st_nlink << ' ';//number of files and link of file

	string userid = getpwuid(buf.st_uid)->pw_name;//get userID
	if (errno != 0) {//error checking
	    perror("userid");
	}

	cout << userid << ' ';//print user ID

	string groupid = getgrgid(buf.st_gid)->gr_name;//get Group ID
	if (errno != 0) {//error checking
	    perror("group ID");
	}

	cout << groupid << ' ';//print group ID

	cout << buf.st_size << ' ';//print size of file in bytes

	time_t t = buf.st_mtime;//time of last modification
	struct tm lt;

	if (localtime_r(&t, &lt) == NULL) {//error checking + time in ascii
	    perror("localtime");
	}

	char timbuf[80];//character array to get time
	strftime(timbuf, sizeof(timbuf), "%h", &lt);//Month expressed in abbreviated form
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);//day of month padded
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);//24hour HH:MM
	cout << timbuf << ' ';

        cout << direntp->d_name << endl; //stat here to find attributes of file
	if (stat(dirName, &buf) == -1) {//error checking + stat
	    perror("stat");
	}
    }

    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }


    return;
}

void aRflag(const string DIRNAME) {//-a flag and -R flag
    const char *dirName = DIRNAME.c_str();//convert DIRNAME to c-string
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }


    dirent *direntp;

    cout << DIRNAME << endl;

    vector<char*> NextDir;//vector to store directories

    while ((direntp = readdir(dirp))) {//while there are still files to be read
	if (errno != 0) {
	    perror("readdir");
	}

	struct stat buf;
        cout << direntp->d_name << " "; //stat here to find attributes of file
	if (stat(dirName, &buf) == -1) {//stat + error checking
	    perror("stat");
	}

	char test[999];//char array to check next directory
	strcpy(test, dirName);//copy first filename
	strcat(test, "/");//concat '/' char for second filename
	strcat(test, direntp->d_name);//concat second filename

	if (direntp->d_name[0] == '.') {//avoid pushing files that being with .
	    continue;
	}

	stat(test, &buf);//stat new filename
	if (errno != 0) {//error checking
	    perror("stat");
	}

	if (S_ISDIR(buf.st_mode)) {//push new filename if directory
		NextDir.push_back(direntp->d_name);
	}
    }
    cout << endl << endl;


    for (int i = 0; i < NextDir.size(); i++) {//for all stored directories recursively recall function
	RflagOnly(DIRNAME+"/"+NextDir.at(i));
   }
    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }



    return;
}

void lRflag(const string DIRNAME) {//-l flag and -R flag
    const char *dirName = DIRNAME.c_str();//convert DIRNAME to c-string
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }


    dirent *direntp;

    cout << DIRNAME << endl;

    vector<char*> NextDir;//used to store directories

    while ((direntp = readdir(dirp))) {//while there are still files to be read
	if (errno != 0) {//error checking
	    perror("readdir");
	}

	if (direntp->d_name[0] == '.') {//skip hidden files
	    continue;
	}


        cout << direntp->d_name << " "; //stat here to find attributes of file
	struct stat buf;

	perm(direntp, buf);//get permissions


	stat(direntp->d_name, &buf);

	if (errno != 0) {//error checking
	    perror("stat");
	}

        cout << ' ';
	cout << buf.st_nlink << ' ';//number of links and directories of file

	string userid = getpwuid(buf.st_uid)->pw_name;//get User ID
	if (errno != 0) {//error checking
	    perror("userid");
	}

	cout << userid << ' ';//print User ID

	string groupid = getgrgid(buf.st_gid)->gr_name;//get Group ID
	if (errno != 0) {//error checking
	    perror("group ID");
	}
	cout << groupid << ' ';//print group ID

	cout << buf.st_size << ' ';//print size of file in bytes

	time_t t = buf.st_mtime;//time of last modification
	struct tm lt;

	if (localtime_r(&t, &lt) == NULL) {//converts time into ascii
	    perror("localtime");
	}

	char timbuf[80];//char array to contain time
	strftime(timbuf, sizeof(timbuf), "%h", &lt);//abbreviated month
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);//day of month padded
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);//24hour HH:MM
	cout << timbuf << ' ';

	cout << endl;

	if (stat(dirName, &buf) == -1) {//error checking + stat
	    perror("stat");
	}

	char test[999];//char array to store further directories
	strcpy(test, dirName);//copy first filename
	strcat(test, "/");//concat '/' char for second filename
	strcat(test, direntp->d_name);//concat second filename


	stat(test, &buf);
	if (errno != 0) {//error checking
	    perror("stat");
	}


	if (S_ISDIR(buf.st_mode)) {//push back if valid directory
		NextDir.push_back(direntp->d_name);
	}

    }
    cout << endl;

    for (int i = 0; i < NextDir.size(); i++) {//recursively call function for stored directories
	lRflag(DIRNAME+"/"+NextDir.at(i));
    }

    if (closedir(dirp) == -1) {//error checking
	perror("closedir");
    }

    return;
}

void alRflag(const string DIRNAME) {//-a -l -R flags
    const char *dirName = DIRNAME.c_str();//convert to c-string
    DIR *dirp = opendir(dirName);//open directory

    if (dirp == NULL) {//error checking
	perror("opendir");
    }


    dirent *direntp;


    vector<char*> NextDir;//vector to store directories

    while ((direntp = readdir(dirp))) {//while there are still files to be read
	struct stat buf;

	perm(direntp, buf);//get permissions


	stat(direntp->d_name, &buf);

	if (errno != 0) {
	    perror("stat");
	}

        cout << ' ';
	cout << buf.st_nlink << ' ';//# of links and directories of file

	string userid = getpwuid(buf.st_uid)->pw_name;//get user ID
	if (errno != 0) {//error checking
	    perror("userid");
	}

	cout << userid << ' ';//print user ID

	string groupid = getgrgid(buf.st_gid)->gr_name;//get group ID
	if (errno != 0) {//error checking
	    perror("group ID");
	}
	cout << groupid << ' ';//print group ID

	cout << buf.st_size << ' ';//print size of file in bytes

	time_t t = buf.st_mtime;//time of last modification
	struct tm lt;

	if (localtime_r(&t, &lt) == NULL) {//time converted to ascii
	    perror("localtime");
	}

	char timbuf[80];//char array to store the time
	strftime(timbuf, sizeof(timbuf), "%h", &lt);//abbreviated name of month
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%d", &lt);//day of month padded
	cout << timbuf << ' ';

	strftime(timbuf, sizeof(timbuf), "%R", &lt);//24hour HH:MM
	cout << timbuf << ' ';


        cout << direntp->d_name << " " << endl; //stat here to find attributes of file
	if (stat(dirName, &buf) == -1) {
	    perror("stat");
	}

	char test[999];//char array used to concat filenames to check if directory
	strcpy(test, dirName);//first filename
	strcat(test, "/");//concat '/'
	strcat(test, direntp->d_name);//concat second filename

	if (direntp->d_name[0] == '.') {//do not pushback . and .. directories
	    continue;
	}

	stat(test, &buf);
	if (errno != 0) {//error checking
	    perror("stat");
	}

	if (S_ISDIR(buf.st_mode)) {//check if new filename is valid directory
		NextDir.push_back(direntp->d_name);
	}
    }
    cout << endl << endl;


    for (int i = 0; i < NextDir.size(); i++) {//for all stored directories recall recursively
	alRflag(DIRNAME+"/"+NextDir.at(i));
    }

    if (closedir(dirp) == -1) {
	perror("closedir");
    }

    return;
}

int main() {

    string getflags;//gets flag and ls command
    getline(cin,getflags);


    bool aflag = false;//for -a
    bool lflag = false;//for -l
    bool Rflag = false;//for -R

    string DNAME = ".";//used mainly for all -R flags
    if (getflags.find("ls") == std::string::npos) {//if user didn't type "ls"
	exit(0);
    }

    if (getflags.find("-a") != string::npos) {//-a was found
	aflag = true;
    }

    if (getflags.find("-l") != string::npos) {//-l was found
	lflag = true;
    }

    if (getflags.find("-R") != string::npos) {//-R was found
        Rflag = true;
    }

    if (getflags.find("-al") != string::npos || getflags.find("-la") != string::npos) {//-a and -l found
	aflag = true;
	lflag = true;
    }

    if (getflags.find("-aR") != string::npos || getflags.find("-Ra") != string::npos) {//-a and -R found
	aflag = true;
	Rflag = true;
    }

    if (getflags.find("-lR") != string::npos || getflags.find("-Rl") != string::npos) {//-l and -R found
	lflag = true;
	Rflag = true;
    }

    //-a -l and -R found
    if (getflags.find("-alR") != string::npos || getflags.find("-laR") != string::npos
    || getflags.find("-lRa") != string::npos ||getflags.find("-Rla") != string::npos 
    || getflags.find("-Ral") != string::npos || getflags.find("-aRl") != string::npos) {
	aflag = true;
	lflag = true;
	Rflag = true;
    }
	


    if (aflag == false && lflag == false && Rflag == false) {//no flags found
	noflags();
    }

    else if (aflag == true && lflag == false && Rflag == false) {//only -a
	aflagOnly();
    }

    else if (aflag == false && lflag == true && Rflag == false) {//only -l
	lflagOnly();
    }

    else if( aflag == false && lflag == false && Rflag == true) {// only -R
	RflagOnly(DNAME);
    }

    else if (aflag == true && lflag == true && Rflag == false) {// -a and -l
	alflag();
    }

    else if (aflag == true && lflag == false && Rflag == true) {// -a and -R
	aRflag(DNAME);
    }

    else if (aflag == false && lflag == true && Rflag == true) {// -l and -R
	lRflag(DNAME);
    }

    else if (aflag == true && lflag == true && Rflag == true) {// -l -a and -R
	alRflag(DNAME);
    }
	return 0;
}
