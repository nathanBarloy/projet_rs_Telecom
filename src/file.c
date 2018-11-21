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