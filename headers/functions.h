#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <magic.h>
#include "directory.h"
#pragma once

typedef struct Options {
    int i;
    int l;
	int a;
	int print;	// Vaut 2 si --print est entré après l'option --exec
	int printDir;
    char* t;
    char* name;
    char*** exec;   // Tableau de pointeurs vers des char** différents, un char** par partie séparées par un |
    char* dossier;
} Options;

typedef struct symbolsLibMagic{
    magic_t (*magic_open)(int);
    char* (*magic_error)(magic_t);
    int (*magic_close)(magic_t);
    int (*magic_load)(magic_t, const char*);
    char* (*magic_file)(magic_t, const char*);
} symbolsLibMagic;

typedef struct RegChar RegChar;

struct RegChar{ //structure permettant de definir des expressions regulieres
	int interrogation;
	int etoile;
	int plus;
	int inverse;
	char *contenu;
	RegChar *suite;
};

Options* initOptions();
void freeOptions(Options* options);

symbolsLibMagic* loadSymbols(void* libMagic);   // Charge les fonctions de libMagic et renvoie un pointeur vers la structure contenant les pointeurs de ces fonctions
void freeSymbols(symbolsLibMagic* symbols);     // Libère la structure des pointeurs de fonctions de libMagic

char* insertString(char* string, char* dest, int index);       // Insert la chaîne de charactères string dans la chaîne de charactères dest à l'indice index
void removeChar(char* str, int index);      // Supprime le charactère situé à l'indice index de la chaîne de charactère str

char*** parseExecGeneral(char* charArgs);   // Parse la chaîne de charactères charArgs en pargv, tableau de pointeurs vers des argv correspondant à des commandes entrecoupées de pipe
char**  parseExecArgs(char* charArgs);   // Parse la chaîne de charactères charArgs en argv pour utilisation dans l'appel de execvp
char*** replaceBracketGeneral(char*** pargv, char* file);       // Remplace les "{}" par file, et renvoie un char*** semblable au pargv. Ne modifie ni pargv passé en paramètre, ni les argv pointées par pargv, ni les chaînes de caractères consécutives pointées par les argv
char** replaceBracketWithFile(char** argv, char* file);        // Remplace les "{}" par file, et renvoie un char** semblable au argv. Ne modifie ni argv passé en paramètre, ni les chaînes de caractères consécutives pointées par argv

Options* parser(int argc, char* argv[]);            // Parse les options et renvoit le résultat dans un type Options

int get_char(int fd);           // Lit le charactère pointé par le pointeur courant du descripteur de fichier "fd" et le renvoit s'il n'est pas un EOF
char* readLine(int fd);         // Renvoie une ligne entière par lecture du descripteur "fd"
int searchStringInFile(char* file, char* stringToSearch); // Cherche la chaîne de charactère "stringToSearch" dans le fichier de chemin "file", renvoie 1 si elle est trouvée, 0 sinon

int isImage(char *file, symbolsLibMagic *symbols);    // Retourne 0 si le fichier de chemin "file" n'est pas une image, 1 sinon

int execCommandPipe(char* file, Options* options);  // FONCTION A LANCER POUR LE EXEC : Exécute les commandes passées dans le paramètre "exec" sur le fichier de chemin "file", gère les pipe, renvoie le code d'erreur

int execCommand(char* file, Options* options);      // Exécute la commande passé dans le paramètre "exec" sur le fichier de chemin "file", renvoie le code d'erreur

void freePargv(char*** pargv); // Libère l'espace mémoire de pArgs

Directory* m_ls(char *path,char *name, Options* options, symbolsLibMagic* symbols);
//void m_ls(char *d,int a);
void affLs(Directory* dir, Options *options);
void normalize(char *path);

RegChar *initRegChar(); //initie un RegChar
void freeRegChar(RegChar *reg); //free un RegChar
RegChar *parserRegexp( char* str); //transforme une chaine de charactère representant une expression reguliere en RegChar
int identification(char *str, RegChar *regchar, int ind); //indique si la chaine str correspond au regchar passé
char *substring(char *str, int i, int n); //retourne la sous chaine de str, a partir de l'indice i et de taille n (moins si on atteint la fin de la chaine)
int strocc(char *str, char c, int i); //retourne l'indice de la premiere occurrence de c dans str a partir de l'indice i, -1 si aucune occurrence
int isIn(char *str,char c, int inv); //indique si le caractere c est dans le regroupement str

#endif
