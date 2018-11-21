rsfind: src/rsfind.c src/functions.o src/misc.o
	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o -lmagic -o rsfind

functions.o : src/functions.c src/misc.o
	gcc -Wall -Werror -Wextra -c src/functions.c src/misc.o

misc.o : src/misc.c
	gcc -Wall -Werror -Wextra -c src/misc.c

test: rsfind test.sh
	bash test.sh
