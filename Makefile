#rsfind: src/rsfind.c src/functions.o src/misc.o src/directory.o src/file.o
#	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o src/misc.o -o rsfind -ldl
##	gcc -Wall -Werror -Wextra src/rsfind.c src/functions.o -lmagic -o rsfind	# Ancien build : avant l'utilisation de libdl
#
#functions.o : src/functions.c src/misc.o
#	gcc -Wall -Werror -Wextra -c src/functions.c src/misc.o
#
#misc.o : src/misc.c
#	gcc -Wall -Werror -Wextra -c src/misc.c
#
#directory.o : src/directory.c src/file.o
#	gcc -Wall -Werror -Wextra -c src/directory.c src/file.o
#
#file.o : src/file.c
#	gcc -Wall -Werror -Wextra -c src/file.c
#
#
#test: rsfind test.sh
#	bash test.sh

CC = gcc
CFLAGS = -Wall -g
EXEC = src/rsfind

all : $(EXEC)

src/rsfind: src/functions.o src/misc.o src/file.o src/directory.o
	$(CC) -Wall -Werror -Wextra src/rsfind.c src/functions.o src/misc.o src/directory.o src/file.o  -o rsfind -ldl -g

src/rsfind.o : headers/functions.h

src/functions.o : headers/misc.h headers/directory.h

src/directory.o : headers/file.h

src/%.o :  src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)


test: rsfind test.sh
	bash test.sh

clean :
	rm -rf src/*.o

mrproper :
	rm -rf $(EXEC)
