#include <memory.h>
#include "../headers/launch.h"


int examineFile(File* file, Options* options/*, symbolsLibMagic* symbols*/){
// Lance toutes les fonctions de traitement nécessaires (isImage, searchText, exec...) sur le fichier file en fonction des options renseignées dans option. Renvoie 0 si le fichier doit être gardé (s'il passe les tests isImage...), 1 sinon.

    int resultat = 1;
    int errorCode = 0;

    if (options->t){
        resultat = resultat * (1 - searchStringInFile(file->path,options->t));
    }
    if (options->i){
        resultat = resultat * (1 - isImage(file->path,symbols));
    }
    if (options->name){
        resultat = resultat * (strcmp(options->name,file->name) == 0);
    }
    if (options->exec && resultat){
        errorCode = execCommandPipe(file->path,options);
    }

    return 1 - resultat;
}
