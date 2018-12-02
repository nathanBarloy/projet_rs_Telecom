rsfind: src/rsfind.c src/functions.o src/misc.o
	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o -o rsfind
#	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o -lmagic -o rsfind	# Ancien build : avant l'utilisation de libdl

functions.o : src/functions.c src/misc.o
	gcc -Wall -Werror -Wextra -c src/functions.c src/misc.o

misc.o : src/misc.c
	gcc -Wall -Werror -Wextra -c src/misc.c

test: rsfind test.sh
	bash test.sh
