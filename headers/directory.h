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

#endif