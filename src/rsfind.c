#include <stdio.h>
#include <stdlib.h>

#include "../headers/functions.h"

int main (int argc, char* argv[]) {

    printf("test2\n");

    int a = 0;
    a++;

    printf("%d\n",a);

    Options* options = parser(argc, argv);

    freeOptions(options);
    return 0;
}