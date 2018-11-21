#include<stdlib.h>
#include<stdio.h>
#include "../headers/directory.h"

Directory* initDirectory() {
	//initialisation d'un directory
	Directory* directory = malloc(sizeof(Directory));
	
	//initialisation des variables
	directory->name = NULL;
	directory->path = NULL;
	directory->nbFile = 0;
	directory-> nbDirectory = 0;
	directory-> directoryChild = NULL;
	directory->fileChild = NULL;
	directory->brother = NULL;
	
	return directory;
}

void freeDirectory(Directory* directory) {
	
	/*for (int i=0;i<directory->nbFile;i++) { //On free les files
		freeFile(directory->fileList[i]);
	}
	
	for (int i=0;i<directory->nbFile;i++) { //on free les directory recursivement
		freeDirectory(directory->directoryList[i]);
	}*/
	
	free(directory);
}