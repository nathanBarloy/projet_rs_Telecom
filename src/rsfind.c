#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include "../headers/functions.h"
#include "../headers/misc.h"

int main (int argc, char* argv[]) {

    void* libMagic;
    Options* options = parser(argc, argv);  // Récupération des options

    if (options->i){    // Chargement dynamique de la librairie libmagic
//        if (!(libMagic = dlopen("libm",RTLD_NOW))){
//            printWrite(STDERR_FILENO,dlerror());
//            exit(1);
//        }
        if (!(libMagic = dlopen("./libmagc.so",RTLD_NOW))){
            char* err = dlerror();
            printf("%s \n",err);
            printWrite(STDOUT_FILENO,"ERREUR : %s \n",err);
//            exit(1);
        }
    }


    // ##############
    // PARTIE TESTS
    int resultatTempo = 2;

    if (options->t){    // Pour les tests : si on a renseigné l'option -t
//        resultatTempo= searchStringInFile("README.md", options->t);
        resultatTempo = 3;
        printf("Chaîne trouvée dans le fichier? %d \n", resultatTempo);
        printWrite(STDOUT_FILENO,"%s Ch %s \n", "testtreeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeees longtesttreeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeees long", "un autre stringgggg?");
    }
    if (options->exec && options->dossier){
        execCommandPipe(options->dossier,options);
    }

    if (options->i){
        printWrite(STDOUT_FILENO, "Fichier img1 est une image ? : %d", isImage("images/img1", libMagic));
        printWrite(STDOUT_FILENO, "Fichier notAnImage est une image ? : %d", isImage("images/notAnImage", libMagic));
    }
    // FIN DE LA PARTIE TESTS
    // ###################### //


    // TODO : Mettre ici les appels des fonctions travaillant sur les options parsées

    freeOptions(options);

    if (resultatTempo == 0){
        exit(1);
    }

    if (libMagic){  // Décharge la librairie libMagic
        dlclose(libMagic);
    }

    exit(0);
}