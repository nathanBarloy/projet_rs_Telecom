#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "../headers/directory.h"

Directory* initDirectory() { // n le nombre de dossiers et de fichiers
	//initialisation d'un directory
	Directory* directory = malloc(sizeof(Directory));
	
	//initialisation des variables
	directory->name = "";
	directory->path = "";
	directory->nbFile = 0;
	directory-> nbDirectory = 0;
	directory->ordre = (int*) malloc(1024*sizeof(int));
	directory-> directoryChild = NULL;
	directory->fileChild = NULL;
	directory->brother = NULL;
	
	return directory;
}

void freeDirectory(Directory* dir) {
	// libère le directory dir
    Directory* chDir = dir->directoryChild;
    Directory* tmp;
	
    if(chDir!=NULL){ //on libère les fils Dir
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
    if (chFile!=NULL){ //on libère les fils File
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

    if(dir->ordre) {
    	free(dir->ordre);
    }

    free(dir);
}

Directory* createDirectory(char* name) {
	//crée un directory qui a comme nom name
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

void addDirectoryChild(Directory* dir, Directory* child) { // insere un directory dans l'ordre lexicographique
	Directory* chDir = dir->directoryChild;
	if (chDir!=NULL) { //si le rep a au moins un fils
		while ( chDir->brother!=NULL ) { //on parcour tant qu'il y a un frere de nom plus petit
			chDir = getBrotherDirectory(chDir);
		}
		setBrotherDirectory(chDir,child); //on insere child entre bigBrotheret chDir
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

void addFileChild(Directory* dir, File* child) { //insere un file child dans dir
	File *chFile = dir->fileChild;
	if (chFile!=NULL) { //si le rep a au moins un fils
		while ( chFile->brother!=NULL ) { //on parcour tant qu'il y a un frere
			chFile = getBrotherFile(chFile);
		}
		setBrotherFile(chFile,child); //on insere child entre bigBrother et chFile
	} else { // si le rep n'a pas de fils
		dir->fileChild = child; 
	}
	if (child->path){
		free(child->path);
	}
	child->path = creerPath(dir->path,child->name);
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

