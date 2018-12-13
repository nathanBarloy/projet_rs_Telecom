#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "../headers/directory.h"

Directory* initDirectory() {
	//initialisation d'un directory
	Directory* directory = malloc(sizeof(Directory));
	
	//initialisation des variables
	directory->name = "";
	directory->path = "";
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
	
    if(chDir!=NULL){
        while (chDir!=NULL) {
            tmp = getBrotherDirectory(chDir);
            freeDirectory(chDir);
            chDir = tmp;
        }
        free(tmp);
    }
    free(chDir);

    File* chFile = dir->fileChild;
    File* tmp2;
    if (chFile!=NULL){
        while (chFile!=NULL) {
            tmp2 = getBrotherFile(chFile);
            freeFile(chFile);
            chFile = tmp2;
        }
        free(tmp2);
    }

    free(chFile);

    if(dir->name){
        free(dir->name);
    }
    if (dir->path && strcmp(dir->path,"")!=0){
        free(dir->path);
    }

    free(dir);
}

Directory* createDirectory(char* name) {
	Directory* dir = initDirectory();
	dir->name = strdup(name);
	return dir;
}

void setBrotherDirectory(Directory* dir, Directory* brother) {
	dir->brother = brother;
}

Directory* getBrotherDirectory(Directory* dir){
	return dir->brother;
}

void addDirectoryChild(Directory* dir, Directory* child) { // insere dans l'ordre lexicographique
	Directory* chDir = dir->directoryChild;
	if (chDir!=NULL) { //si le rep a au moins un fils
		if (strcmp(chDir->name,child->name)>0) {
			setBrotherDirectory(child,chDir);
			dir->directoryChild = child;
		} else {
			Directory* bigBrother = chDir;
			chDir = getBrotherDirectory(chDir);
			while ( chDir!=NULL && strcmp(chDir->name, child->name)<0 ) { //on parcour tant qu'il y a un frere de nom plus petit
				bigBrother = chDir;
				chDir = getBrotherDirectory(chDir);
			}
			setBrotherDirectory(child,chDir); //on insere child entre bigBrotheret chDir
			setBrotherDirectory(bigBrother,child); //on met le frere a child
		}
	} else { // si le rep n'a pas de fils
		dir->directoryChild = child; 
	}
	//child->path = strcat(strcat(dir->path,"/"),child->name);
//	INUTILE : child a été initialisé avec son chemin déjà connu
//	child->path = strdup(dir->path);
//	strcat(child->path,"/");
//	strcat(child->path,child->name);
	dir->nbDirectory ++;
}

void addFileChild(Directory* dir, File* child) { //insere un file dans l'ordre lexicographique
	File *chFile = dir->fileChild;
	File *bigBrother = NULL;
	if (chFile!=NULL) { //si le rep a au moins un fils
		if (strcmp(chFile->name,child->name)>0) {
			setBrotherFile(child,chFile);
			dir->fileChild = child;
		} else {
			bigBrother = chFile;
			chFile = getBrotherFile(chFile);
			while ( chFile!=NULL && strcmp(chFile->name,child->name)<0 ) { //on parcour tant qu'il y a un frere de nom plus petit
				bigBrother = chFile;
				chFile = getBrotherFile(chFile);
			}
			setBrotherFile(child,chFile); //on insere child entre bigBrother et chFile
			setBrotherFile(bigBrother,child); //on met le frere a child
		}
	} else { // si le rep n'a pas de fils
		dir->fileChild = child; 
	}
	//child->path = strcat(strcat(dir->path,"/"),child->name);
//	free(child->path);
	child->path = strdup(dir->path);
	child->path = concatener(child->path,"/");
	child->path = concatener(child->path,child->name);
	dir->nbFile++;
}

void aff(Directory* dir) {
	Directory* chDir = dir->directoryChild;
	while (chDir!=NULL) {
		printf("%s\t",chDir->name);
		chDir = getBrotherDirectory(chDir);
	}
	free(chDir);
	
	File* chFile = dir->fileChild;
	while (chFile!=NULL) {
		printf("%s\t",chFile->name);
		chFile = getBrotherFile(chFile);
	}
	free(chFile);
	printf("\n");
}

/*char *path2name(char *path) {
	char *res=NULL, token = strtok(path,"/");
	while (token != NULL) {
		res = token;
		token = strtok(NULL, "/");
	}
	if (res==NULL) res = token;
	return res;
}*/

char *concatener(char *str1, char *str2) {
	char *res = (char*) malloc( (strlen(str1)+strlen(str2)+1) * sizeof(char) );
	strcpy(res,str1);
	strcat(res,str2);
	free(str1);
	return res;
}