#ifndef _FILE_H
#define _FILE_H

typedef struct file File;

struct file {
	char* name;
	char* path;
	File* brother;
};

File* initFile();
void freeFile();

#endif