#include "../headers/functions.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <magic.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>
#include <dlfcn.h>
#include "../headers/directory.h"
#include "../headers/file.h"
#include "../headers/misc.h"
#include "../headers/launch.h"


Options* initOptions(){
    // Initialise une instance de type Options

    Options* options = malloc(sizeof(Options));

    // Initialisation aux valeurs par défaut
    options->i = 0;
    options->l = 0;
	options->a = 0;
	options->print = 0;
    options->t = NULL;
    options->name = NULL;
    options->exec = NULL;
    options->dossier = ".";
    options->ename = NULL;
	options->regcharEname = NULL;

    return options;
}

void freeOptions(Options* options){
    // Libère l'espace mémoire pris par l'Options dont le pointeur est passée en argument

    int i =0;
    if (options->exec != NULL){
        while(options->exec[i] != NULL){
            free(options->exec[i]);
            i++;
        }
        free(options->exec);
    }
    if (options->t){
        free(options->t);
    }
    if (options->dossier){
        free(options->dossier);
    }
    if (options->name){
        free(options->name);
    }
    if (options->ename){
        free(options->ename);
    }
	if (options->regcharEname){
        free(options->regcharEname);
    }

    free(options);
}

symbolsLibMagic* loadSymbols(void* libMagic){
    // Charge les fonctions de libMagic et renvoie un pointeur vers la structure contenant les pointeurs de ces fonctions
    symbolsLibMagic* symbols = malloc(sizeof(symbolsLibMagic));

    int error = 1;
    error *= 1- !( symbols->magic_open = (magic_t (*)(int)) dlsym(libMagic,"magic_open"));
    error *= 1- !( symbols->magic_error = (char* (*)(magic_t)) dlsym(libMagic,"magic_error"));
    error *= 1- !( symbols->magic_close = (int (*) (magic_t)) dlsym(libMagic,"magic_close"));
    error *= 1- !( symbols->magic_load= (int (*)(magic_t, const char*)) dlsym(libMagic,"magic_load"));
    error *= 1- !( symbols->magic_file= (char* (*)(magic_t, const char*)) dlsym(libMagic,"magic_file"));

    if (1 - error){ // Teste si un des chargement de symbole a échoué
        printWrite(STDERR_FILENO, "ECHEC de chargement des symboles de magic open\n");
        freeSymbols(symbols);
        return NULL;
    }
    return symbols;
}
void freeSymbols(symbolsLibMagic* symbols){
    // Libère la structure des pointeurs de fonctions de libMagic
    free(symbols);
}

char* insertString(char* string, char* dest, int index){
    // Insert la chaîne de charactères string dans la chaîne de charactères dest à l'indice index

    int n = strlen(dest);
    int m = strlen(string);

    char result[n+m+1];  // On prévoit la place nécessaire pour la chaîne de charactère contenant l'insertion de string dans dest (le +1 sert pour le '\0' final)

    strncpy(result, dest, index); // Copie des charactères de dest jusqu'à l'indice index dans le result
    result[index] = '\0';           // Rend result exploitable par les fonctions travaillant sur des char*

    strcat(result, string);         // Concaténation du result avec string à insérer
    strcat(result, dest + index);   // Concaténation avec le reste de dest

    char* result2 = malloc(sizeof(char)* (n+m+1));
    strncpy(result2,result, n+m+1);
    return result2;
}

void removeChar(char* str, int index){
    // Supprime le charactère situé à l'indice index de la chaîne de charactère str

    memmove(&str[index], &str[index + 1], strlen(str) - index);
}

char*** parseExecGeneral(char* charArgs){
    // Parse la chaîne de charactères charArgs en pargv, tableau de pointeurs vers des argv correspondant à des commandes entrecoupées de pipe

    char*** pargv = malloc(sizeof(char**) * ((int) ((strlen(charArgs) + 1) / 2) + 1) );  // On adapte la taille de argv au pire des cas (des arguments de taille 1 séparé d'un pipe chacun)
    int i = 0; // Indice de parcours de charArgs
    int lastI = 0;  // Indice du début de la commande que l'on est en train de parcourir (actualisée à chaque nouvelle commande séparée par un pipe)
    int j = 0;  // Indice de parcours de pargv


    while(charArgs[i] != '\0'){
        if (charArgs[i] == '|'){    // Il faut parser la chaîne de caractère non encore parsée jusqu'à charArgs[i] en remplacant ce charactère '|' par '\0'
            charArgs[i] = '\0';
            pargv[j] = parseExecArgs(charArgs + lastI);
            lastI = i + 1;  // La prochaine commande à parser, si elle existe, débute à l'index i + 1
            j++;
        }
        i++;
    }
    pargv[j++] = parseExecArgs(charArgs + lastI); // Pour parser la dernière commande (qui ne finie pas par un pipe et n'est donc pas parsée dans le if du while ci-dessus)

    pargv[j] = NULL;

    char*** pargv2 = realloc(pargv, sizeof(char**) * (j+1));  // On réduit la taille de pargv au strict nécessaire : le nombre de commandes séparées par un pipe, plus un
    if (! pargv2){   // Problème de realloc, on s'arrête là
        free(pargv);
        return NULL;
    }

    pargv = pargv2;
    return pargv;
}


char** parseExecArgs(char* charArgs) {
    // Parse la chaîne de charactère charArgs et la renvoie en argv pour utilisation dans l'appel de execvp
    // Renvoie NULL en cas de problème
    //TODO : mettre la taille de argv dans argv[0] ?

    char** argv = malloc(sizeof(char*) * ((int) ((strlen(charArgs) + 1) / 2) + 1) );  // On adapte la taille de argv au pire des cas (des arguments de taille 1 séparé d'un espace chacun)

    int i = 0;  // Index de lecture de charArgs
    int j = 0;  // Index de numéro d'argument de argv

    while(charArgs[i] != '\0'){     // Tant qu'on a pas trouvé la fin de la chaîne de caractères

        while(charArgs[i] == ' ' || charArgs[i] == '\t'  || charArgs[i] == '\n'){   // On boucle sur les caractères "blancs" à ne pas prendre en compte de notre parseur
            charArgs[i++] = '\0';   // Remplace l'espace blanc par un \0 et avance l'index de lecture d'un cran
        }
        if(charArgs[i] != '\0'){
            argv[j++] = charArgs + i;   // Conserve l'adresse du début d'un argument
        }
        while(charArgs[i] != ' ' && charArgs[i] != '\t'  && charArgs[i] != '\n' && charArgs[i] != '\0'){ // Avance jusqu'à ne plus lire de caractères séparateur d'argument
            i++;
        }
    }
    argv[j++] = '\0';   // Marque la fin de la liste d'arguments, NB : j est la taille effectivement utilisée de argv

    char** argv2 = realloc(argv, sizeof(char*) * j);  // On réduit la taille de argv au strict nécessaire : le nombre d'arguments trouvés plus un
    if (! argv2){   // Problème de realloc, on s'arrête là
        free(argv);
        return NULL;
    }

    return argv2;
}

char*** replaceBracketGeneral(char*** pargv, char* file){
    // Remplace les "{}" par file, et renvoie un char*** semblable au pargv. Ne modifie ni pargv passé en paramètre, ni les argv pointées par pargv, ni les chaînes de caractères consécutives pointées par les argv

    int n = 0;
    while (pargv[n] != NULL){   // Détermination de la taille de pargv
        n++;
    }

    char*** pargv2 = malloc(sizeof(char**) * (n +1 ));
    for (int j = 0; j < n +1 ; j++){   // Copie des char** de pargv dans pargv2
        pargv2[j] = pargv[j];
    }

    int i = 0;
    while(pargv2[i] != NULL){
        pargv2[i] = replaceBracketWithFile(pargv2[i], file);
        i++;
    }

    return pargv2;
}

char** replaceBracketWithFile(char** argv, char* file){
    // Remplace les "{}" par file, et renvoie un char** semblable au argv
    // Ne modifie ni argv passé en paramètre, ni les chaînes de caractères consécutives pointées par argv

    int i = 0;
    while(argv[i] != NULL){ // Détermination de la taille de argv
        i++;
    }

    char** argv2 = malloc(sizeof(char*) * (i +1 ));
    for (int j = 0; j < i +1 ; j++){   // Copie des char* de argv dans argv2
        argv2[j] = argv[j];
    }

    int l = 0;  // Indice de parcours de argv2
    while(l < i){  // Parcours de argv2
        if (strlen(argv2[l]) == 2){  // Le char* pointé par argv2[l] doit faire exactement 2 caractères pour être "{}"
            if (argv2[l][0] == '{' && argv2[l][1] == '}'){
                argv2[l] = file;    // Remplacement du char* de argv2 qui pointait vers "{}" par le char* file
            }
        }
        l++;
    }
    return argv2;
}

Options* parser(int argc, char* argv[]){
    // Parse les options et renvoit le résultat dans un type Options

    Options* options = initOptions();
    int c;
    char*** pargv = NULL;

    while(1){   // Parsage des options
        static struct option long_options[] =
                {   //<nom>, <has_arg>, <flag>,<val_returned>
                        {"name", required_argument, 0, 'n'},
                        {"print", no_argument , 0, 'p'},
                        {"exec", required_argument, 0, 'e'},
                        {"ename", required_argument, 0, 'r'},
                        {0, 0, 0, 0}
                };

        int option_index = 0;

        c = getopt_long(argc,argv,"alt:i",long_options, &option_index); // Lecture de la prochaine option


        if (c == -1){   // Cas d'arrêt du parseur : fin des options
            break;
        }
        switch(c){

            case 'l' :  // Option -l
//                printf("option -l\n");
                options->l = 1;
                break;

            case 'i' : // Option -i
//                printf("option -i\n");
                options->i = 1;
                break;
			
			case 'a' : // Option -a
				options->a = 1;
				break;

            case 't' :  // Option -t CHAINE
//                printf("option -t avec valeur '%s'\n", optarg);
                options->t = strdup(optarg);
                break;

                break;
            case 'n' :  // Option --name CHAINE
//                printf("option --name avec valeur '%s'\n", optarg);
                options->name = strdup(optarg);
                break;

            case 'e' :  // Option --exec CMD
//                printf("option -exec avec valeur '%s'\n", optarg);
                pargv = parseExecGeneral(optarg);
                if (! pargv){    // Problème de parsing de la valeur de l'option -exec en char** argv
                    printf("ERREUR : Problème de parsing de la valeur de l'option -exec en char** argv");
                }
                options-> exec = pargv;
                break;

            case 'p' :  // Option --print
//                printf("option -print\n");
                options->print = 1;
                if (options->exec){ // Si l'option --print est entrée après l'exec, on le signifie
                    options->print = 2;
                }
                break;

            case 'r' : // Option --ename
                options->ename = strdup(optarg);
				options->regcharEname = parserRegexp(options->ename);
                break;

            case '?' :  // Option lue ne fait pas partie de celles disponibles : erreur
                break;  // getopt_long a déjà affiché un message d'erreur

            default :
//                printf("Caractère non reconnu : 0%o ??\n", c);
                printWrite(STDERR_FILENO,"Caractère non reconnu : 0%o ??\n", c);
                break;
        }
    }

    // Détermination du -print : il doît être activé par défaut si aucune autre option (--exec ou -l) n'est renseignée
    if (!options->print){
        options->print = (options->exec ==NULL) * (1 - options->l);
    }

    // Si la moindre options de recherche est activée, on n'affichera pas les dossiers
    options->printDir = (options->name ==NULL) * (options->t ==NULL) * (1- options->i);

    if (argv[optind]){  // Cas où le dossier de travail est renseigné en argument (position indifférente dans l'appel de rsfind)
        options->dossier = strdup(argv[optind]);
		normalize(options->dossier);
    }
    else {  // Par défaut le dossier de travail est ".", c'est à dire le dossier d'exécution de rsfind
        options->dossier = strdup(".");
		}
//    else {        //PROBLEME : selon que le programme soit lancé depuis CLion ou le terminal, argv[0] est différent (dossier de travail pour CLion, chaîne de charactère tapée pour lancer rsfind dans le terminal)
//        printf("Dossier en cours : %s \n", argv[0]);
//        options->dossier = strdup(argv[0]); // Cas par défaut : le chemin d'exécution est pris comme dossier de travail
//
//    }

    return options;
}

int get_char(int fd) {
    // Lit le charactère pointé par le pointeur courant du descripteur de fichier "fd" et le renvoit s'il n'est pas un EOF
    // Renvoit un EOF si la fin du fichier est atteint (quand la lecture échoue)

    char c;
    if (read(fd, &c, 1) == 1)
        return c;
    return EOF;
}

char* readLine(int fd) {
    // Renvoie une ligne entière par lecture du descripteur "fd"

    size_t maxSize = 10;   // Taille du buffer
    size_t actualSize = 0; // Niveau de remplissage du buffer, on garde une marge de deux espaces de caractères (pour ajouter le '\0' sans avoir réallouer le buffer à la fin de la fonction)
    char* buffer = malloc(maxSize);

    int c;

    if ((c = get_char(fd)) == EOF){ // Si on est à la fin du fichier, on s'arrête là
        free(buffer);
        return NULL;
    }

    while ( (c = get_char(fd)) != EOF && c != '\n'){    // Parcours des charactères de la ligne

        if (actualSize + 2 > maxSize){  // On va dépasser la capacité du buffer si on ajoute un caractère supplémentaire
            size_t newMaxSize = maxSize * 2;
            char* newBuffer = realloc(buffer, newMaxSize);

            if ( ! newBuffer){  // En cas de problème avec le realloc, on s'arrête là
                free(buffer);
                return NULL;
            }

            buffer = newBuffer; // Le realloc s'est bien passé, on remplace le buffer par un buffer deux fois plus grand
            maxSize = newMaxSize;
        }

        // Ajout du caractère lu dans le buffer et actualisation du niveau de remplissage du buffer
        buffer[actualSize] = c;
        actualSize++;
    }

    if (c == '\n'){
        buffer[actualSize] = c;
    }

    buffer[actualSize] = '\0';

    return buffer;
}

int searchStringInFile(char* file, char* stringToSearch){
    // Cherche la chaîne de charactère "stringToSearch" dans le fichier de chemin "file"
    // Renvoie 1 si la chaîne "stringToSearch" est trouvée dans le fichier "file", 0 sinon

    int fd;
    char* line = NULL;

    fd = open(file, O_RDONLY, 0);

    if (fd){
        while ((line = readLine(fd))){    // Parcours des lines du fichier
            if (strstr(line, stringToSearch)){  // Si la chaîne recherchée est trouvée dans la line
                return 1;
            }
            free(line);
        }
    }

    close(fd);
    return 0;
}

int isImage(char *file, symbolsLibMagic *symbols) {
    // Retourne 0 si le fichier de chemin "file" n'est pas une image, 1 si c'est une image et -1 en cas d'erreur

    // Résolution des fonctions de libMagic :
    magic_t (*magic_open)(int);
    char* (*magic_error)(magic_t);
    int (*magic_close)(magic_t);
    int (*magic_load)(magic_t, const char*);
    char* (*magic_file)(magic_t, const char*);

    magic_open = symbols->magic_open;
    magic_close = symbols->magic_close;
    magic_error = symbols->magic_error;
    magic_file = symbols->magic_file;
    magic_load = symbols->magic_load;

//    const char *magic_full;
    magic_t magic_cookie;
    int result;

    magic_cookie = magic_open(MAGIC_MIME_TYPE);

    if (magic_cookie == NULL) {
//        printf("Erreur d'initialisation de magic\n");
        printWrite(STDERR_FILENO, "Erreur d'initialisation de magic\n");
        return -1;
    }

    if (magic_load(magic_cookie, NULL) != 0) {
//        printf("Erreur de chargement de la base de donnée de magic - %s\n", magic_error(magic_cookie));
        printWrite(STDERR_FILENO,"Erreur de chargement de la base de donnée de magic - %s\n", magic_error(magic_cookie));
        magic_close(magic_cookie);
        return -1;
    }
    result = ((strstr(magic_file(magic_cookie, file), "image"))!=NULL);
    magic_close(magic_cookie);

    return result;
}

int execCommandPipe(char* file, Options* options) {
    // FONCTION A LANCER POUR LE EXEC :
    // Exécute les commandes passées dans le paramètre "exec" sur le fichier de chemin "file", gère les pipe, renvoie le code d'erreur
    char*** pargv = replaceBracketGeneral(options->exec, file);
    int i = 0;

    // Création du pipe
    int p[2];
    pid_t pid;
    int fd_in = 0;

    while (pargv[i] != NULL) {
        pipe(p);
        if ((pid = fork()) == -1) {
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            dup2(fd_in, 0); // Changement de l'entrée standard par la sortie de l'exec précedent
            if (pargv[i+1] != NULL) // Il y a une commande à exécuter après celle ci, donc il faut gérer la redirection des sorties et entrées
                dup2(p[1], 1);
            close(p[0]);
            execvp(pargv[i][0], pargv[i]);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            close(p[1]);
            fd_in = p[0]; // Sauvegarde du numéro de descripteur pour le changement d'entrée standard plus tard
            i++;
        }
    }
    freePargv(pargv);
    return 0;
}

void freePargv(char*** pargv){
    // Libère l'espace mémoire de pArgs

    char** argv = NULL;
    int i = 0;

    while( (argv = pargv[i])){  // Parcours des argv pointés par les cases de pargv
        free(argv);
        i++;
    }

    free(pargv);
}

int execCommand(char* file, Options* options){
    // Exécute la commande passé dans le paramètre "exec" sur le fichier de chemin "file", renvoie le code d'erreur
    // TODO : Gestion des pipes et des codes erreurs de la commande exécuté

    char** argv = NULL;
    char*** pargv = replaceBracketGeneral(options->exec, file);
    if(! fork()){   // Pour le fils
        argv = pargv[0];    //TODO : parcours sur pargv
        execvp(argv[0], argv);
//        printf("Echec du execvp !");
        // Si on arrive à cette ligne, c'est que execvp a échoué
        printWrite(STDERR_FILENO, "Echec du execvp !");
        return 1;
    }
    else{           // Pour le père : attend que son fils se termine
        wait(NULL);
    }

    return 0;
}




Directory* m_ls(char *path,char *name, Options* options, symbolsLibMagic* symbols) {
	//crée un Directory représentant le repertoir de chemin path, en prenant en compte les options
	Directory* directory = createDirectory(name);
	directory->path = strdup(path);
	DIR *dirp=NULL;
	struct dirent *dp=NULL;
    char *newPath = NULL;
	Directory *newDir;
	File* newFile = NULL;
	int i =0;
	
	dirp = opendir(path); //dirp est le repertoir relatif a path

	while ((dp = readdir(dirp)) != NULL) { //dp parcours les fichier dans le repertoir dirp
		if ( strcmp(dp->d_name,".")!= 0 && strcmp(dp->d_name,"..")!= 0) { //on ne prend pas les fichiers . et ..
			if (dp->d_type == DT_DIR) { //si dp est un repertoir
				//Directory* newDir = createDirectory(dp->d_name);
				//printf("%s\n",dp->d_name);
				newPath = creerPath(path,dp->d_name);
				newDir = m_ls(newPath, dp->d_name,options,symbols); //appel récursif
				if (newDir->directoryChild!=NULL || newDir->fileChild!=NULL) {
					addDirectoryChild(directory, newDir);
                    directory->ordre[i++]=1;
                }
                free(newPath);
			}
			if (dp->d_type==DT_REG) { //si dp est un fichier
				newFile = createFile(dp->d_name);
				newFile->path = creerPath(path,dp->d_name);
				if (examineFile(newFile,options,symbols)[0]){
                    addFileChild(directory,newFile);
                    directory->ordre[i++]=0;
                }
			}
		}
		
	}
	//TODO : Supprimer dans cette fonction les dossiers vides (cf les TODO de la fonction affLs)
	free(dp);
	closedir(dirp);
	return directory;
}
/*void m_ls(char *d,int a) { // a représente l'option -a : 1 si activée
	DIR *dirp;
	struct dirent *dp;
	dirp = opendir(d);
	while ((dp = readdir(dirp)) != NULL) {
		if (a==1 || (dp->d_name)[0] != '.') { //a enlever pour -a
//			printf ("%s\t", dp->d_name);
			printWrite (STDOUT_FILENO,"%s\t", dp->d_name);
		}
	}
//	printf("\n");
	printWrite(STDOUT_FILENO,"\n");
	closedir(dirp);
}*/

void affLs(Directory* dir, Options *options) {
	//affiche les éléments de dir dans l'ordre alphabétique
	//les fils File et Directory sont déjà dans l'ordre : il faut les fusionner en les affichant
	Directory* chDir = dir->directoryChild;
	File* chFile = dir->fileChild;
	int i;

	//TODO : Faire l'exec (si options->exec) sur les dossiers du résultat
	//TODO : Pour cela : faire l'exec sur les dossiers parcouru ici, et supprimer avant cette fonction les dossiers vides

	if (options->printDir && strcmp(dir->name,".")) {
        if (options->print == 1){
            printWrite(STDOUT_FILENO,"%s\n",dir->path);
        }
        if (options->exec){
            execCommandPipe(dir->path,options);
        }
        if (options->print == 2) {
            printWrite(STDOUT_FILENO, "%s\n", dir->path);
        }
	}

	/*while (chDir!=NULL || chFile!=NULL) { //tant qu'il y a un fils Dir ou un fils File 
		if (chDir==NULL || (chFile!=NULL && strcmp(chFile->path,chFile->path)>0)) { // afficher File
			if(!options->name || strcmp(options->name,chFile->name)==0) {
				printWrite(STDOUT_FILENO,"%s\n",chFile->path);
			}
			chFile = getBrotherFile(chFile);
		} else { //afficher directory
			affLs(chDir, options);
			chDir = getBrotherDirectory(chDir);
		}
	}*/

	for (i=0;i<(dir->nbFile+dir->nbDirectory);i++) {
		if (dir->ordre[i]) { //si doit afficher dossier
			affLs(chDir, options);
			chDir = getBrotherDirectory(chDir);
		} else if (chFile){ //sinon affiche fichier
			    // Affichage et exécution pour le fichier dans le bon ordre (en fonction de l'ordre de saisie des options
			    if (options->print == 1){
                    printWrite(STDOUT_FILENO,"%s\n",chFile->path);
                }
                if (options->exec){
                    execCommandPipe(chFile->path,options);
                }
                if (options->print == 2){
                    printWrite(STDOUT_FILENO,"%s\n",chFile->path);
                }
			chFile = getBrotherFile(chFile);
		}
	}
}

void normalize(char *path) {
	if (path[strlen(path)-1]=='/') {
		path[strlen(path)-1] = '\0';
	}
}

RegChar *initRegChar() { //initialisation d'un regchar
	RegChar * reg = (RegChar*) malloc(sizeof(RegChar));
	reg->interrogation = 0;
	reg->etoile = 0;
	reg->plus = 0;
	reg->inverse = 0;
	reg->contenu = NULL;
	reg->suite = NULL;
	
	return reg;
}
void freeRegChar(RegChar *reg){ //free une structure regchar
	if (reg->contenu) {
		free(reg->contenu);
	}
	
	if (reg->suite) {
		freeRegChar(reg->suite);
	}
	free(reg);
}

RegChar *parserRegexp( char* str) { //transforme une chaine de charactère representant une expression reguliere en RegChar
	RegChar *res = initRegChar();
	RegChar *regActuel = res;
	RegChar *regPrecedent = NULL;
	int i=0, n=strlen(str),a; //i est vl'indice ou l'on se trouve dans str
	
	while(i<n) { //tant qu'on est pas au bout de la chaine
		
		if (str[i]=='[') { //si on a un crochet ouvrant, on va lire jusqu'au crochet fermant
			i++;
			if (str[i]=='^') { //si on a un inverse
				regActuel->inverse = 1;
				i++;
			}
			a = strocc(str,']',i);
			regActuel->contenu = substring(str,i,a-i);
			i = a+1;
		} else {
			regActuel->contenu = substring(str,i,1);
			i++;
		}
		
		//on regarde le caractere qui suit pour savoir si c'est ? + ou *
		if (str[i]=='?') {
			regActuel->interrogation = 1;
			i++;
		}
		if (str[i]=='*') {
			regActuel->etoile = 1;
			i++;
		}
		if (str[i]=='+') {
			regActuel->plus = 1;
			i++;
		}
		
		if(regPrecedent) {
			regPrecedent->suite = regActuel; //la suite du regchar precedent est le regchar actuel
		}
		regPrecedent = regActuel;
		regActuel = initRegChar();
	}
	freeRegChar(regActuel);
	
	return res;
}

int identification(char *str, RegChar *regchar, int ind) { //indique si la chaine str correspond au regchar passé en se positionnant a l'indice ind
	int i = ind;
	
	// 2 cas d'arret, si regchar est null ou si i>=strlen(str) : on ne retourne 1 que si on a plus de regchar et que i==n ie on a lu toute la string
	if(regchar==NULL) {
		return (i==strlen(str));
	}
	if (i>=strlen(str)) {
		return 0;
	}
	
	// on va regarder les differentes options activees
	if (regchar->interrogation) {
		if (identification(str,regchar->suite,i) ) return 1; //on regarde si ca marche avec ? donne vide
		if ( isIn(regchar->contenu,str[i],regchar->inverse) && identification(str,regchar->suite,i+1) ) return 1; // sinon, on regarde si le caractere actuel correspond, et si la suite est possible
		return 0;
	}
	
	if (regchar->plus) {
		while(i<strlen(str) && isIn(regchar->contenu,str[i],regchar->inverse) ) { // a chaque boucle, on regarde s'il est possible d'avoir la suite, et si non, on essaye de faire entre le caractère actuel dans le plus
			i++;
			if (identification(str,regchar->suite,i) ) return 1;
		}
		return 0;
	}
	
	if (regchar->etoile) { // comme pour le plus, mais avec la possibiite de ne rien donner
		if (identification(str,regchar->suite,i) ) return 1;
		while(i<strlen(str) && isIn(regchar->contenu,str[i],regchar->inverse) ) {
			i++;
			if (identification(str,regchar->suite,i) ) return 1;
		}
		return 0;
	}
	
	if (isIn(regchar->contenu,str[i],regchar->inverse)) { // si aucune option n'est activee
		return identification(str,regchar->suite,i+1);
	}
	
	return 0;
}

char *substring(char *str, int i, int n) { //retourne la sous chaine de str, a partir de l'indice i et de taille n (moins si on atteint la fin de la chaine)
	char *res = (char*) malloc((n+1)*sizeof(char));
	int k=0;
	while(k<n && str[i+k]!='\0') {
		res[k] = str[i+k];
		k++;
	}
	res[k] = '\0';
	
	return res;
}

int strocc(char *str, char c, int i) { //retourne l'indice de la premiere occurrence de c dans str a partir de l'indice i, -1 si aucune occurrence
	int k=i;
	int res = -1;
	while( str[k]!='\0' && str[k]!=c) {
		k++;
	}
	if (str[k]==c) {
		res = k;
	}
	return res;
}

int isIn(char *str,char c, int inv) { //indique si le caractere c est dans le regroupement str, en prenant en compte l'inversion 
	int i;
	for (i=0;i<strlen(str);i++) {
		
		if (str[i]=='.') { //le . accepte tout
			return 1-inv;
		}
		if (c==str[i]) { //le caractere est le bon
			return 1-inv;
		}
		if (str[i]=='-') { //le tiret represente une liste
			if (c>=str[i-1] && c<=str[i+1]) {
				return 1-inv;
			}
		}
	}
	return inv;
}