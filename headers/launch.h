#include "functions.h"

#ifndef RS2018_LAUNCH_H
#define RS2018_LAUNCH_H

int examineFile(char* file, Options* options, symbolsLibMagic* symbols);   // Lance toutes les fonctions de traitement nécessaires (isImage, searchText, exec...) sur le fichier de chemin file en fonction des options renseignées dans option. Renvoie 0 si le fichier doit être gardé (s'il passe les tests isImage...), 1 sinon.

#endif //RS2018_LAUNCH_H
