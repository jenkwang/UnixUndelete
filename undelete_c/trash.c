//Jenny Chong 23733553
//this program works best compared to srm and unrm
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int removefiles(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	//do not remove the root directory
    if (ftwbuf->level) {
		//unlinks files from directory
		unlink(fpath);
		//removes the empty directory
		rmdir(fpath);
	}
    return 0;
}

int rmrf(char *path)
{
    return nftw(path, removefiles, 64, FTW_DEPTH | FTW_PHYS);
}

int main() {
	char *trash = "./trashbin";
	struct stat st = {0};
	//if directory exists delete contents
	if (stat(trash, &st) == 0) {
		rmrf(trash);
		return 0;
	}
	//make directory if it doesn't exit
	mkdir(trash, 0700);
	return 0;
}
