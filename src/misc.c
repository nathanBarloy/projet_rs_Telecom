#include<stdio.h>
#include<stdarg.h>
#include <memory.h>
#include <stdlib.h>
#include <zconf.h>
#include "../headers/misc.h"

int printWrite(int std, char* stringToWrite, ...){
    // Equivalent d'un printf, écrit stringToWrite dans la sortie de descripteur spécifié par la valeur de std
    // Adapté et corrigé du code proposé sur http://www.firmcodes.com/write-printf-function-c/

    int n = strlen(stringToWrite);
    char* buffer = malloc(sizeof(char) * n); // TODO : Ajouter au cas où les arguments sont nombreux
    char* bufferParcours = buffer;
    int i;
    char* s;

    // Initialisation des arguments de printWrite
    va_list arg;
    va_start(arg, stringToWrite);

    for (char* stringParcours = stringToWrite ; *stringParcours != '\0' ; stringParcours++){

        while(*stringParcours != '%' && *stringParcours != '\0'){ // Parcours et écriture des charactères sans %
            *buffer++ = *stringParcours;
            stringParcours++;
        }   // Si on sort de la boucle, c'est soit qu'on a atteint la fin de stringToWrite, soit qu'on a trouvé un % à traiter

        stringParcours++;

        switch(*stringToWrite) {
            case 'c' :
                i = va_arg(arg, int);    // Récupération de l'argument de type char
                *buffer++ = i;
                break;

            case 'd' :
                i = va_arg(arg, int);    // // Récupération de l'argument décimal ou entier
                if (i < 0) {
                    i = -i;
                    *buffer++ = '-';
                }
                s = convert(i, 10);
                putStringInBuffer(s, &buffer);
                break;

            case 'o':
                i = va_arg(arg, int); //Fetch Octal representation
                s = convert(i, 8);
                putStringInBuffer(s, &buffer);
                break;

            case 's':
                s = va_arg(arg, char *);        //Fetch string
                putStringInBuffer(s, &buffer);
                break;

            case 'x':
                i = va_arg(arg, int); //Fetch Hexadecimal representation
                s = convert(i, 16);
                putStringInBuffer(s, &buffer);
                break;
        }
    }
    va_end(arg);

    bufferParcours = '\0';
    write(std, buffer,strlen(buffer));

    free(buffer);
}

void putStringInBuffer(char* string, char** buffer){
    // Met la chaîne de charactère string dans *buffer, incrémente le buffer d'autant de cran que de charactères de string

    for (int i = 0 ; i < strlen(string) ; i++){
        **buffer = string[i];
        *buffer++;
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
