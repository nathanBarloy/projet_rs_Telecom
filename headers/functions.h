#pragma once

typedef struct Options {
    int i;
    int l;
    char* t;
    char* name;
    char* exec;
} Options;

Options* initOptions();
void freeOptions(Options* options);

Options* parser(int argc, char* argv[]);     // Parse les options et renvoit le résultat dans un type Options

int get_char(int fd); // Lit le charactère pointé par le pointeur courant du descripteur de fichier "fd" et le renvoit s'il n'est pas un EOF
char* readLine(int fd);         // Renvoie une ligne entière par lecture du descripteur "fd"
int searchStringInFile(char* file, char* stringToSearch); // Cherche la chaîne de charactère "stringToSearch" dans le fichier de chemin "file", renvoie 1 si elle est trouvée, 0 sinon
