#include "functions.h"

#ifndef RS2018_LAUNCH_H
#define RS2018_LAUNCH_H


int* examineFile(File* file, Options* options, symbolsLibMagic* symbols);   // Lance toutes les fonctions de traitement nécessaires (isImage, searchText, exec...) sur le fichier de chemin file en fonction des options renseignées dans option. Renvoie 1 si le fichier doit être gardé (s'il passe les tests isImage...), 0 sinon.


#endif //RS2018_LAUNCH_H
