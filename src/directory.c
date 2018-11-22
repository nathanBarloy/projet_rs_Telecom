#include<stdlib.h>
#include<stdio.h>
#include<string.h>

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

void freeDirectory(Directory* dir) {
	
		Directory* chDir = dir->directoryChild;
		Directory* tmp;
		while (chDir!=NULL) {
			tmp = getBrotherDirectory(chDir);
			freeDirectory(chDir);
			chDir = tmp;
		}
		
		File* chFile = dir->fileChild;
		File* tmp2;
		while (chFile!=NULL) {
			tmp2 = getBrotherFile(chFile);
			freeFile(chFile);
			chFile = tmp2;
		}
		
		free(dir);
}

Directory* createDirectory(char* name) {
	Directory* dir = initDirectory();
	dir->name = name;
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
	//child->path = strcat(strcat(dir->path,"/"),child->name);
	child->path = strdup(dir->path);
	strcat(child->path,"/");
	strcat(child->path,child->name);
	dir->nbDirectory ++;
}

void addFileChild(Directory* dir, File* child) {
	File* chFile = dir->fileChild;
	if (chFile!=NULL) { //si le rep a au moins un fils
		while ( getBrotherFile(chFile)!=NULL ) { //on parcour tant qu'il y a un frere
			chFile = getBrotherFile(chFile);
		}
		setBrotherFile(chFile,child); //on met le frere a child
	} else { // si le rep n'a pas de fils
		dir->fileChild = child; 
	}
	//child->path = strcat(strcat(dir->path,"/"),child->name);
	child->path = strdup(dir->path);
	strcat(child->path,"/");
	strcat(child->path,child->name);
	dir->nbFile++;
}

void aff(Directory* dir) {
	Directory* chDir = dir->directoryChild;
	while (chDir!=NULL) {
		printf("%s\t",chDir->name);
		chDir = getBrotherDirectory(chDir);
	}
	
	File* chFile = dir->fileChild;
	while (chFile!=NULL) {
		printf("%s\t",chFile->name);
		chFile = getBrotherFile(chFile);
	}
	printf("\n");
}