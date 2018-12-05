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
