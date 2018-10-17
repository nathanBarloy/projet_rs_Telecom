#pragma once

typedef struct Options {
    int i;
    int l;
    char* t;
    char* name;
    char* exec;
} Options;

Options* initOptions();
void freeOptions(Options* options);

Options* parser(int argc, char* argv[]);