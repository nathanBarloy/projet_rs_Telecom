#include "../headers/functions.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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