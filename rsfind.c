#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {

    printf("test\n");

    int a = 0;
    a++;

    printf("%d\n",a);


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
                break;

            case 'i' : // Option -i
                printf("option -i\n");
                break;

            case 't' :  // Option -t CHAINE
                printf("option -t avec valeur '%s'\n", optarg);
                break;

            case 'n' :  // Option --name CHAINE
                printf("option --name avec valeur '%s'\n", optarg);
                break;

            case 'e' :  // Option --exec CMD
                printf("option -exec avec valeur '%s'\n", optarg);
                break;

            case '?' :  // Option lue ne fait pas partie de celles disponibles : erreur
                break;  // getopt_long a déjà affiché un message d'erreur

            default :
                printf("Caractère non reconnu : 0%o ??\n", c);
                break;
        }

    }

    return 0;
}