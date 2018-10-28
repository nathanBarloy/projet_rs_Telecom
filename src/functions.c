#include "../headers/functions.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <magic.h>

char *readLine(int fd);

Options* initOptions(){
    // Initialise une instance de type Options

    Options* options = malloc(sizeof(Options));

    // Initialisation aux valeurs par défaut
    options->i = 0;
    options->l = 0;
    options->t = NULL;
    options->name = NULL;
    options->exec = NULL;

    return options;
}

void freeOptions(Options* options){
    // Libère l'espace mémoire pris par l'Options dont le pointeur est passée en argument

    free(options);
}


Options* parser(int argc, char* argv[]){
    // Parse les options et renvoit le résultat dans un type Options

    Options* options = initOptions();
    int c;

    while(1){   // Parsage des options

        static struct option long_options[] =
                {   //<nom>, <has_arg>, <flag>,<val_returned>
                        {"name", required_argument, 0, 'n'},
                        {"exec", required_argument, 0, 'e'},
                        {0, 0, 0, 0}
                };

        int option_index = 0;

        c = getopt_long(argc,argv,"lt:i",long_options, &option_index); // Lecture de la prochaine option


        if (c == -1){   // Cas d'arrêt du parseur : fin des options
            break;
        }
        switch(c){

            case 'l' :  // Option -l
                printf("option -l\n");
                options->l = 1;
                break;

            case 'i' : // Option -i
                printf("option -i\n");
                options->i = 1;
                break;

            case 't' :  // Option -t CHAINE
                printf("option -t avec valeur '%s'\n", optarg);
                options->t = strdup(optarg);
                break;

            case 'n' :  // Option --name CHAINE
                printf("option --name avec valeur '%s'\n", optarg);
                options->name = strdup(optarg);
                break;

            case 'e' :  // Option --exec CMD
                printf("option -exec avec valeur '%s'\n", optarg);
                options->exec = strdup(optarg);
                break;

            case '?' :  // Option lue ne fait pas partie de celles disponibles : erreur
                break;  // getopt_long a déjà affiché un message d'erreur

            default :
                printf("Caractère non reconnu : 0%o ??\n", c);
                break;
        }
    }

    return options;
}

int get_char(int fd) {
    // Lit le charactère pointé par le pointeur courant du descripteur de fichier "fd" et le renvoit s'il n'est pas un EOF
    // Renvoit un EOF si la fin du fichier est atteint (quand la lecture échoue)

    char c;
    if (read(fd, &c, 1) == 1)
        return c;
    return EOF;
}

char* readLine(int fd) {
    // Renvoie une ligne entière par lecture du descripteur "fd"

    size_t maxSize = 10;   // Taille du buffer
    size_t actualSize = 0; // Niveau de remplissage du buffer, on garde une marge de deux espaces de caractères (pour ajouter le '\0' sans avoir réallouer le buffer à la fin de la fonction)
    char* buffer = malloc(maxSize);

    int c;

    if ((c = get_char(fd)) == EOF){ // Si on est à la fin du fichier, on s'arrête là
        return NULL;
    }

    while ( (c = get_char(fd)) != EOF && c != '\n'){    // Parcours des charactères de la ligne

        if (actualSize + 2 > maxSize){  // On va dépasser la capacité du buffer si on ajoute un caractère supplémentaire
            size_t newMaxSize = maxSize * 2;
            char* newBuffer = realloc(buffer, newMaxSize);

            if ( ! newBuffer){  // En cas de problème avec le realloc, on s'arrête là
                free(buffer);
                return NULL;
            }

            buffer = newBuffer; // Le realloc s'est bien passé, on remplace le buffer par un buffer deux fois plus grand
            maxSize = newMaxSize;
        }

        // Ajout du caractère lu dans le buffer et actualisation du niveau de remplissage du buffer
        buffer[actualSize] = c;
        actualSize++;
    }

    if (c == '\n'){
        buffer[actualSize] = c;
    }

    buffer[actualSize] = '\0';

    return buffer;
}

int searchStringInFile(char* file, char* stringToSearch){
    // Cherche la chaîne de charactère "stringToSearch" dans le fichier de chemin "file"
    // Renvoie 1 si la chaîne "stringToSearch" est trouvée dans le fichier "file", 0 sinon

    int fd;
    char* line;

    fd = open(file, O_RDONLY, 0);

    if (fd){
        while (line = readLine(fd)){    // Parcours des lines du fichier
            if (strstr(line, stringToSearch)){  // Si la chaîne recherchée est trouvée dans la line
                return 1;
            }
            free(line);
        }
    }

    close(fd);
    return 0;
}

int isImage(char* file) {
    // Retourne 0 si le fichier de chemin "file" n'est pas une image, 1 si c'est une image et -1 en cas d'erreur
    const char *magic_full;
    magic_t magic_cookie;
    int result;

    magic_cookie = magic_open(MAGIC_MIME_TYPE);

    if (magic_cookie == NULL) {
        printf("Erreur d'initialisation de magic\n");
        return -1;
    }

    if (magic_load(magic_cookie, NULL) != 0) {
        printf("Erreur de chargement de la base de donnée de magic - %s\n", magic_error(magic_cookie));
        magic_close(magic_cookie);
        return -1;
    }

    result = ((strstr(magic_file(magic_cookie, file), "image"))!=NULL);
    magic_close(magic_cookie);

    return result;
}
