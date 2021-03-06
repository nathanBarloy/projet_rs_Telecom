#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>

#include "../headers/functions.h"
#include "../headers/misc.h"
#include "../headers/launch.h"

int main (int argc, char* argv[]) {

    void* libMagic = NULL;
    symbolsLibMagic* symbols;
    Options* options = parser(argc, argv);  // Récupération des options

    if (options->i){    // Chargement dynamique de la librairie libmagic
//        if (!(libMagic = dlopen("libm",RTLD_NOW))){
//            printWrite(STDERR_FILENO,dlerror());
//            exit(1);
//        }
        //printWrite(STDERR_FILENO,"ECHEC de chargement des symboles sdfsdsdhhhffffffffffffffffffffffffffs \n");
        if (!(libMagic = dlopen("libmagic.so",RTLD_NOW))){
            printWrite(STDERR_FILENO,"ERREUR : %s \n",dlerror());
            exit(1);
        }

        if (!( symbols = loadSymbols(libMagic))){
            exit(1);
        }

    }
	
	
	// ##############
    // PARTIE TESTS
	
	
	DIR *dirp=NULL;
	dirp = opendir("./tests/testEnvironnement/images");
	closedir(dirp);
	
	Directory* test = m_ls(options->dossier,options->dossier,options,symbols);
	
	affLs(test,options);
	
//	printWrite(STDOUT_FILENO, "ExamineFile de comments.txt : %d",examineFile("../comments.txt",options,symbols));

	freeDirectory(test);
	
	
	/*
	RegChar *regchar = parserRegexp("[^0-9][a-zA-Z0-9]*");
	
	int res = identification("0alut",regchar,0);
	
	printf("%d\n",res);
	
	freeRegChar(regchar);
	*/

    /*

    int resultatTempo = 1;

    if (options->t){    // Pour les tests : si on a renseigné l'option -t
        resultatTempo= searchStringInFile("README.md", options->t);
        printWrite(STDOUT_FILENO,"Chaîne trouvée dans le fichier? %d \n", resultatTempo);
    }
    if (options->exec && options->dossier){
        execCommandPipe(options->dossier,options);
    }

    if (options->i){
        printWrite(STDOUT_FILENO, "Fichier img1 est une image ? : %d\n", isImage("tests/testEnvironnement/images/img1", symbols));
        printWrite(STDOUT_FILENO, "Test entier %d\n",-1000);
        printWrite(STDOUT_FILENO, "Fichier notAnImage est une image ? : %d\n", isImage("tests/testEnvironnement/images/notAnImage", symbols));
    }
	
	if (options->i==0 && options->t==0 && options->exec==NULL) {
		m_ls(".",options->a);
	}
*/

    // FIN DE LA PARTIE TESTS
    // ###################### //

    // DEBUT DES APPELS DE FONCTIONS DU MAIN :


    // TODO : Mettre ici les appels des fonctions travaillant sur les options parsées





    // FIN DES EXECUTIONS DE FONCTIONS
    // LIBERATION DES STRUCTURES :

    /*if (libMagic){  // Décharge la librairie libMagic
        freeSymbols(symbols);
        dlclose(libMagic);
    }*/

    freeOptions(options);
    exit(0);
}