#pragma once

int printWrite(int std, char* stringToWrite, ...);   // Equivalent d'un printf, écrit stringToWrite dans la sortie de descripteur spécifié par la valeur de std

void putStringInBuffer(char *string, char **buffer); // Met le charactère pointé par *pString dans *buffer, avance les pointeurs pString et buffer d'un cran
char* convert(int num, int base); // Convertit l'entier num en char* en fonction de la base