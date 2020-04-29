#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *compressLine(char *line) {
    printf("%s",line);
    char letter = letter[0];
    int count = 1;
    for (int i = 1; i < strlen(line); i++) {
        if (line[i] == letter) {
            count++;
        } else {

        }
    }

}

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("wzip: cannot open file\n");
        exit(1);
    }
    char *line = NULL;
    size_t size = sizeof(&line);
    while(getline(&line, &size , fp) != -1) {
        printf("==========\n");
    }
    fclose(fp);
    return 0;
}
