#include<stdio.h>
#include<stdarg.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>

#include "../headers/misc.h"

int printWrite(int std, char* stringToWrite, ...){
    // Equivalent d'un printf, écrit stringToWrite dans la sortie de descripteur spécifié par la valeur de std
    // Adapté et corrigé du code proposé sur http://www.firmcodes.com/write-printf-function-c/

    unsigned int n = strlen(stringToWrite);
    char* buffer = malloc(n + 1); // TODO : Ajouter au cas où les arguments sont nombreux
    char* bufferParcours = buffer;
    int i;
    char* s;

//    buffer[n] = '\0'; // Permet de calculer la taille de buffer avec strlen (utile pour la fonction preventOverflow
    // Initialisation des arguments de printWrite
    va_list arg;
    va_start(arg, stringToWrite);

    for (char* stringParcours = stringToWrite ; *stringParcours != '\0' ; stringParcours++){

        while(*stringParcours != '%' && *stringParcours != '\0'){ // Parcours et écriture des charactères sans %
            preventOverflow(&buffer, &bufferParcours, " ", &n);
            *bufferParcours++ = *stringParcours;
            stringParcours++;
        }   // Si on sort de la boucle, c'est soit qu'on a atteint la fin de stringToWrite, soit qu'on a trouvé un % à traiter

        if (*stringParcours != '\0'){
            stringParcours++;

            switch(*stringParcours) {
                case 'c' :
                    i = va_arg(arg, int);    // Récupération de l'argument de type char
                    preventOverflow(&buffer, &bufferParcours, " ", &n);
                    *bufferParcours++ = i;
                    break;

                case 'd' :
                    i = va_arg(arg, int);    // // Récupération de l'argument décimal ou entier
                    if (i < 0) {
                        i = -i;
                        preventOverflow(&buffer, &bufferParcours, " ", &n);
                        *bufferParcours++ = '-';
                    }
                    s = convert(i, 10);
                    preventOverflow(&buffer, &bufferParcours, s, &n);
                    putStringInBuffer(s, &bufferParcours);
                    break;

                case 'o':
                    i = va_arg(arg, int); //Fetch Octal representation
                    s = convert(i, 8);
                    preventOverflow(&buffer, &bufferParcours, s, &n);
                    putStringInBuffer(s, &bufferParcours);
                    break;

                case 's':
                    s = va_arg(arg, char *);        //Fetch string
                    preventOverflow(&buffer, &bufferParcours, s, &n);
                    putStringInBuffer(s, &bufferParcours);
                    break;

                case 'x':
                    i = va_arg(arg, int); //Fetch Hexadecimal representation
                    s = convert(i, 16);
                    preventOverflow(&buffer, &bufferParcours, s, &n);
                    putStringInBuffer(s, &bufferParcours);
                    break;
            }
//            stringParcours++;
        } else{
            break;
            // Ne devrait pas être nécessaire comme le 'for' teste si *stringParcours est \0,
            // mais il semblerait qu'il avance d'un cran en trop et se mette à lire le charactère suivant dans la mémoire,
            // pouvant alors afficher plusieurs chaînes de caractères qui ont été passé en arguments plus loin dans le code
        }
    }
    va_end(arg);

    *bufferParcours = '\0';
    write(std, buffer,strlen(buffer));

    free(buffer);

    return 0;
}

void putStringInBuffer(char* string, char** buffer){
    // Met la chaîne de charactère string dans *buffer, incrémente le buffer d'autant de cran que de charactères de string

    for (unsigned int i = 0 ; i < strlen(string) ; i++){
        **buffer = string[i];
        (*buffer)++;
    }
}

char* convert(int num, int base) {
    // Convertit l'entier num en char* en fonction de la base

    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

int preventOverflow(char** pBuffer, char** pBufferParcours, char* s, unsigned int* bufferSize){
    // Vérifie que la concaténation de s à buffer ne fera pas dépasser son espace mémoire reservé, réallout buffer sinon et gère la nouvelle valeur de bufferParcours

    char* buffer = *pBuffer;
    char* bufferParcours = *pBufferParcours;

    unsigned int nbCharUtilises = (bufferParcours - buffer) / sizeof(char);

    if (*bufferSize <  nbCharUtilises + strlen(s) + 1){ // Si la taille de buffer est inférieure à la somme entre le nombre de caractères déjà utilisés et le nombre de caractères à ajouter :
        int newSize = nbCharUtilises + strlen(s) + 1;
        char* buffer2 = realloc(buffer, newSize );  // Reallocation de buffer
        if (! buffer2){   // Problème de realloc, on s'arrête là
            free(buffer);
            return 1;
        }
        buffer = buffer2;
        buffer[newSize-1] = '\0';
        *bufferSize = newSize;
        bufferParcours = buffer + nbCharUtilises;   // Met à jour le pointeur bufferParcours pour qu'il pointe au même endroit dans la chaîne de caractère buffer

        // On répercute les nouvelles valeurs de buffer et bufferParcours :
        *pBuffer = buffer;
        *pBufferParcours = bufferParcours;
    }
    return 0;
}
