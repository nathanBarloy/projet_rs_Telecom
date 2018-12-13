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

#endif