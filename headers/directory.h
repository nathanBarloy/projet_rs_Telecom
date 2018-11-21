#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include "file.h"

typedef struct directory Directory;

struct directory {
	char* name;
	char* path;
	int nbFile;
	int nbDirectory;
	Directory* brother;
	Directory* directoryChild;
	File* fileChild;
};

Directory* initDirectory();
void freeDirectory();
Directory* createDirectory(char* name, char* path);
void setBrotherDirectory(Directory* dir, Directory* brother);
Directory* getBrotherDirectory(Directory* dir);
void addDirectoryChild(Directory* dir, Directory* child);
void addFileChild(Directory* dir, File* child);
void delDirectoryChild(Directory* dir, Directory* child);
void delFileChild(Directory* dir, File* child);
#endif