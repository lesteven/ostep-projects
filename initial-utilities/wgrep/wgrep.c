#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // no command line arguments, then print this
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    char *searchTerm = argv[1];
    if (strcmp(searchTerm,"") == 0) {
        return 0;
    }

    // find search terms if files are provided
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }
        char *line = NULL;
        size_t size = sizeof(&line);
        while(getline(&line, &size , fp) != -1) {
            char *found = strstr(line, searchTerm);
            if (found != NULL) {
                printf("%s", line);
            }
        }
        fclose(fp);
    }
    return 0;
}
