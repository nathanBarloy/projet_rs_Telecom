#pragma once

typedef struct Options {
    int i;
    int l;
    char* t;
    char* name;
    char** exec;
} Options;

Options* initOptions();
void freeOptions(Options* options);

char**  parseExecArgs(char* charArgs);   // Parse la chaîne de charactère charArgs en argv pour utilisation dans l'appel de execvp
Options* parser(int argc, char* argv[]);            // Parse les options et renvoit le résultat dans un type Options

int get_char(int fd);           // Lit le charactère pointé par le pointeur courant du descripteur de fichier "fd" et le renvoit s'il n'est pas un EOF
char* readLine(int fd);         // Renvoie une ligne entière par lecture du descripteur "fd"
int searchStringInFile(char* file, char* stringToSearch); // Cherche la chaîne de charactère "stringToSearch" dans le fichier de chemin "file", renvoie 1 si elle est trouvée, 0 sinon

int isImage(char* file);    // Retourne 0 si le fichier de chemin "file" n'est pas une image, 1 sinon

int execCommand(char* file, Options* options);      // Exécute la commande passé dans le paramètre "exec" sur le fichier de chemin "file", renvoie le code d'erreur