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
Directory* createDirectory(char* name); // crée un repertoire avec un nom
void setBrotherDirectory(Directory* dir, Directory* brother); // ajoute un frère
Directory* getBrotherDirectory(Directory* dir); //retourne le frere
void addDirectoryChild(Directory* dir, Directory* child); //ajoute un fils repertoire
void addFileChild(Directory* dir, File* child); //ajoute un fils fichier
void delDirectoryChild(Directory* dir, Directory* child); //pas implementé
void delFileChild(Directory* dir, File* child);
void aff(Directory* dir); //affiche le conntenu du repertoire
#endif