//Jenny Chong 23733553
#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int removefiles(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    //remove the root directory
    if (remove(fpath))
    	return -1;
    return 0;
}
    
int rmrf(char *path) {
	return nftw(path, removefiles, 64, FTW_DEPTH | FTW_PHYS);
}

void movedir(const char *name) {
	//trash variable is the new path name
    char trash[100];
	strcpy(trash, "./trashbin/");
	strcat(trash, name);
	//cwd variable is curr directory
	//this is what we want to move to ./trashbin
	char cwd[100];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/");
	strcat(cwd, name);

	FILE *o, *n;

    DIR *dir;
    struct dirent *entry;

    //if path is not a directory simple move without recursion
	if (!(dir = opendir(cwd))) {
		o = fopen(cwd, "r");
        n = fopen(trash, "w");
		if (o != NULL && n != NULL) {
        	char c = fgetc(o);
            while (c != EOF) {
            	fputc(c, n);
                c = fgetc(o);
            }
        }
		fclose(o);
        fclose(n);
		return;
	}
	
	//if it is a directory make the directory in ./trashbin
	//if not created already
	struct stat st = {0};
	if (stat(trash, &st) != 0) {	
		mkdir(trash, 0700);
	}
    
	while ((entry = readdir(dir)) != NULL) {
        //this is where we want to move the files to
		strcpy(trash, "./trashbin/");
		strcat(trash, name);
		//the file we want to move into ./trashbin
		getcwd(cwd, sizeof(cwd));
    	strcat(cwd, "/");
		strcat(cwd, name);
		strcat(cwd, "/");
		//if it is a directory descend into it
		if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            movedir(path);
        } 
		else {
			//new path of file
			strcat(trash, "/");
			strcat(trash, entry->d_name);
			//path of file we want to move
			strcat(cwd, entry->d_name);
			//read from cwd and write into trash
        	o = fopen(cwd, "r");
			n = fopen(trash, "w");
			if (o != NULL && n != NULL) {
				char c = fgetc(o);
				while (c != EOF) {
					fputc(c, n);
					c = fgetc(o);
				}
			}
		}
		fclose(o);
		fclose(n);
    }
    closedir(dir);
}

int main(int argc, char **argv) {
	//cwd is the path we want to srm
    char cwd[100];
	//if ./trashbin has not been made, make it
	struct stat st = {0};
    if (stat("./trashbin", &st) != 0) {
    	mkdir("./trashbin", 0700);
    }
	for (int i = 1; i < argc; i++) {
		getcwd(cwd, sizeof(cwd));
    	strcat(cwd, "/");
		strcat(cwd, argv[i]);
		//move the directory/file into ./trashbin
		movedir(argv[i]);
		//remove directory after moving
		rmrf(cwd);
	}
    return 0;
}
