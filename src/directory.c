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

Directory* createDirectory(char* name, char* path) {
	Directory* dir = initDirectory();
	dir->name = name;
	dir->path = path;
	return dir;
}

void setBrotherDirectory(Directory* dir, Directory* brother) {
	dir->brother = brother;
}

Directory* getBrotherDirectory(Directory* dir){
	return dir->brother;
}

void addDirectoryChild(Directory* dir, Directory* child) {
	Directory* chDir = dir->directoryChild;
	if (chDir!=NULL) { //si le rep a au moins un fils
		while ( getBrotherDirectory(chDir)!=NULL ) { //on parcour tant qu'il y a un frere
			chDir = getBrotherDirectory(chDir);
		}
		setBrotherDirectory(chDir,child); //on met le frere a child
	} else { // si le rep n'a pas de fils
		dir->directoryChild = child; 
	}
}