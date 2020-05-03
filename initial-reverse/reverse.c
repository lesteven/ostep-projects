#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getFileName(char *file) {
    char *word = NULL;
    int len = strlen(file);
    for (int i = len-1; i >= 0; i--) {
        if (file[i] == '/') {
            word = malloc(sizeof(char) * (len-i));
            strncpy(word, file + sizeof(char) * (i+1), len-i);
            break;
        }
    }
    return word == NULL? file: word;
}

void freeFile(char *name, char *file) {
    if (strcmp(name, file) == 0) {
        free(name);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 3) {
        fprintf(stderr, "%s\n", "usage: reverse <input> <output>");
        exit(1);
    }
    if (argc > 2) {
        char *first = getFileName(argv[1]);
        char *second = getFileName(argv[2]);
        if (strcmp(first, second) == 0) {
            fprintf(stderr, "%s\n", "reverse: input and output file must differ");
            freeFile(first, argv[1]);
            freeFile(second, argv[2]);
            exit(1);
        }
    }

    if (argc == 1) {
        //reverse stdin
        return 0;
    }
    char *read = argv[1];
    FILE *fp = fopen(read, "r");
    if (fp == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", read);
        exit(1);
    }

    return 0;
}
