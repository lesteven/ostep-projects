#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void searchLines(char *searchTerm, FILE *stream) {
    char *line = NULL;
    size_t size = sizeof(&line);
    while(getline(&line, &size, stream) != -1) {
        char *found = strstr(line, searchTerm);
        if (found != NULL) {
            printf("%s", line);
        }
    }
}

int main(int argc, char *argv[]) {
    // no command line arguments, then print this
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }

    // if search term is empty string, return
    char *searchTerm = argv[1];
    if (strcmp(searchTerm,"") == 0) {
        return 0;
    }

    // if search term given, but no file selected
    if (argc == 2) {
        searchLines(searchTerm, stdin);
    }

    // find search terms if files are provided
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }
        searchLines(searchTerm, fp);
        fclose(fp);
    }
    return 0;
}
