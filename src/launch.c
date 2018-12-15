#include <memory.h>
#include "../headers/launch.h"



int* examineFile(File* file, Options* options, symbolsLibMagic* symbols){

    static int resErr[2];
    int resultat = 1;
    int errorCode = 0;

    if (options->t){
        resultat = resultat * searchStringInFile(file->path,options->t);
    }
    if (options->i){
        resultat = resultat * isImage(file->path,symbols);
    }
    if (options->name){
        resultat = resultat * (strcmp(options->name,file->name) == 0);
    }
/*    if (options->exec && resultat){
        errorCode = execCommandPipe(file->path,options);
    }*/

    resErr[0] = resultat;
    resErr[1] = errorCode;

    return resErr;
}

