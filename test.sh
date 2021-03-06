#!/bin/bash

# Script permettant d'exécuter les tests contenus dans les fichiers du répertoire "tests"
# Le nom des fichiers tests est à passer en argument
# PAR DEFAUT : s'il n'y a pas d'arguments, le script applique ces tests sur tous les fichiers du répertoire tests
# Un fichier test est organisé ainsi :
#       - une ligne contient une commande (faisant intervenir les fonctions développées dans ce projet) à exécuter pour tester ses retours
#       - la ligne suivante contient la commande de référence (utilisant les fonctions de bash uniquement) dont les sorties doivent correspondre avec celles de la première ligne
#       - une ligne vide (juste un retour à la ligne), pour séparer deux commandes à tester
#       => Cette alternance est répétée jusqu'au EOF


testsPath="tests/filesToExec"

# S'il n'y a pas d'arguments, le script fait ces tests sur les fichiers trouvés dans le répertoire de tests
if [ $# -ne 1 ]; then
    files=$(ls "$testsPath/")
    else
    files=("$@")
fi


# Parcours des fichiers tests
for var in $files; do

    filePath="$testsPath/$var"     # Détermine le path du fichier à lire
    printf "\n\n ###Exécution des tests du fichier : '$filePath' ###\n"
    i=0

    #Parcours des lignes de commandes du file :
    while IFS='' read -r line || [[ -n "$line" ]]; do
#        printf "\nText read from file: $line"

        case $(($i%3)) in       # En fonction de la ligne :
        0)
            CMD_RS=$(eval $line)
            errorRS=$((! $?))  # Garde en mémoire le fait que la commande ai fonctionnée ou pas
            lineRS=$line;;  #On garde en mémoire la commande du projet RS pour l'afficher en cas de différence de sortie avec la commande de BASH
#            echo "CMD RS = $CMD_RS";;

        1)
            CMD_BASH=$(eval $line)
            errorBASH=$((! $?))    # Garde en mémoire le fait que la commande ai fonctionnée ou pas

#             echo "CMD BASH = $CMD_BASH"

            if [[ "$CMD_RS" = "$CMD_BASH"  &&  $errorRS = $errorBASH ]]; then     # Compare les sorties standards et les codes d'erreur des deux commandes
                printf "\t [\033[0;32mOK\033[0m] RS : '$lineRS' Equiv : '$line'"
            else
                printf "\t [\033[0;31mFAIL\033[0m] RS : '$lineRS' Equiv : '$line'"
            fi
            printf "\n\t\t Code erreur : RS : $errorRS \tBASH : $errorBASH\n";;

        esac

        ((i++))
    done < $filePath

done

# Clean exit with status 0
exit 0