#pragma once

typedef struct Options {
    int i;
    int l;
    char* t;
    char* name;
    char*** exec;   // Tableau de pointeurs vers des char** différents, un char** par partie séparées par un |
    char* dossier;
} Options;

Options* initOptions();
void freeOptions(Options* options);

typedef struct directory Directory;
typedef struct file File;

struct directory {
	char* name;
	char* path;
	int nbFile;
	int nbDirectory;
	Directory* brother;
	Directory* directoryChild;
	File* fileChild;
};

struct file {
	char* name;
	char* path;
	File* brother;
};

File* initFile();
void freeFile();
Directory* initDirectory();
void freeDirectory();

void createFile(char* name, char* path);
void createFile2(char* path);

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

int isImage(char* file);    // Retourne 0 si le fichier de chemin "file" n'est pas une image, 1 sinon

int execCommandPipe(char* file, Options* options);  // Exécute les commandes passées dans le paramètre "exec" sur le fichier de chemin "file", gère les pipe, renvoie le code d'erreur

int execCommand(char* file, Options* options);      // Exécute la commande passé dans le paramètre "exec" sur le fichier de chemin "file", renvoie le code d'erreur