#ifndef _FILE_H
#define _FILE_H

typedef struct file File;

struct file {
	char* name;
	char *path;
	File* brother;
};

File* initFile();
void freeFile(File* file);
File* createFile(char* name); //création d'un file a partir de nom
File* getBrotherFile(File* file); //retourne le frère de file
void setBrotherFile(File* file, File* brother); //met le frere de file à brother
char *concatener(char *str1, char *str2); //concatene str1 et str2, et renvoie la chaine correspondante. Free str1, mais pas str2 !
char *creerPath(char* path,char *name); // crée le nouveau path : path +"/" + name

#endif