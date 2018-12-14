#include<stdlib.h>
#include<stdio.h>
#include<string.h>
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
	if(file->name){
		free(file->name);
	}
	if (file->path){
		free(file->path);
	}
	free(file);
}

File* createFile(char* name) { //creation a partir du nom et du path
	File* file = initFile();
	file->name = strdup(name);
	return file;
}

void setBrotherFile(File* file, File* brother) { 
	file->brother = brother;
}

File* getBrotherFile(File* file) {
	return file->brother;
}

char *concatener(char *str1, char *str2) { //concatene str1 et str2, retourne le résultat et free str1, mais pas str2 !
	char *res = (char*) malloc( (strlen(str1)+strlen(str2)+1) * sizeof(char) );
	strcpy(res,str1);
	strcat(res,str2);
	free(str1);
	return res;
}

char *creerPath(char* path,char *name) { // crée le nouveau path : path +"/" + name
	char* newPath = strdup(path);
	newPath = concatener(newPath,"/");
	newPath = concatener(newPath, name); 
	return newPath;
}