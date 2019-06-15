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
	//trash variable is the old path name
    char trash[100];
	strcpy(trash, "./trashbin/");
	strcat(trash, name);
	//cwd variable is curr directory
	//this is where we want to move the file to
	char cwd[100];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/");
	strcat(cwd, name);
	FILE *o, *n;

    DIR *dir;
    struct dirent *entry;

    //if path is not a directory simple move without recursion
	if (!(dir = opendir(trash))) {
		o = fopen(trash, "r");
        n = fopen(cwd, "w");
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
	
	//if it is a directory make the directory in cwd
	//if not created already
	struct stat st = {0};
	if (stat(cwd, &st) != 0) {	
		mkdir(name, 0700);
	}
    
	while ((entry = readdir(dir)) != NULL) {
        strcpy(trash, "./trashbin/");
		strcat(trash, name);
		//this gets the new path
		//where we want to move our file from trash to
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
			//path of each file from trash
			strcat(trash, "/");
			strcat(trash, entry->d_name);
			//new path of the file
			strcat(cwd, entry->d_name);
			//read from trash and write into cwd
        	o = fopen(trash, "r");
			n = fopen(cwd, "w");
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
	//if no ./trashbin is present then create one
	//exit program after
	struct stat st = {0};
    if (stat("./trashbin", &st) != 0) {
        mkdir("./trashbin", 0700);
		return 0;
    }
	//temp is the path we want to unrm
	char temp[1000];
    for (int i = 1; i < argc; i++) {
		strcpy(temp, "./trashbin/");
		strcat(temp, argv[i]);
		//move the directory/file back into cwd
		movedir(argv[i]);
		//remove directory after moving
		rmrf(temp);
	}
    return 0;
}
