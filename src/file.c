#include<stdlib.h>
#include<stdio.h>
#include "../headers/file.h"

File* initFile() {
	//initialisation de d'un File
	File* file = malloc(sizeof(File));
	
	//initilisation des variables de File
	file->name = NULL;
	file->path = NULL;
	file->brother = NULL;
	
	return file;
	}

void freeFile(File* file) {
	free(file);
}

File* createFile(char* name) { //creation a partir du nom et du path
	File* file = initFile();
	file->name = name;
	return file;
}

void setBrotherFile(File* file, File* brother) { 
	file->brother = brother;
}

File* getBrotherFile(File* file) {
	return file->brother;
}
