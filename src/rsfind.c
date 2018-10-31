#include <stdio.h>
#include <stdlib.h>

#include "../headers/functions.h"

int main (int argc, char* argv[]) {

    Options* options = parser(argc, argv);  // Récupération des options

    // ##############
    // PARTIE TESTS
    int resultatTempo = 1;

    if (options->t){    // Pour les tests : si on a renseigné l'option -t
        resultatTempo= searchStringInFile("README.md", options->t);
        printf("Chaîne trouvée dans le fichier? %d \n", resultatTempo);
    }
    if (options->exec){
        execCommand("tests/testsBasiques",options);
    }

    // FIN DE LA PARTIE TESTS
    // ###################### //


    // TODO : Mettre ici les appels des fonctions travaillant sur les options parsées

    freeOptions(options);

    if (resultatTempo == 0){
        exit(1);
    }

    exit(0);
}