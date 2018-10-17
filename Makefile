rsfind: src/rsfind.c src/functions.o
	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o -o rsfind 

functions.o : src/functions.c
	gcc -Wall -Werror -Wextra -c src/functions.c

test: rsfind test.sh
	bash test.sh
