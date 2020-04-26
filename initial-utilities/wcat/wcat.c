#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }
        char *line = NULL;
        size_t size = sizeof(&line);
        while(getline(&line, &size , fp) != -1) {
            printf("%s",line);
        }
        fclose(fp);
    }
    return 0;
}
