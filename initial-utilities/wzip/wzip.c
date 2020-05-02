#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void compressLine(char *line, char compressed[]) {
    int index = 0;
    while (index < strlen(line)) {
        char letter = line[index];
        int count = 1;
        index++;
        while (letter == line[index]) {
            index++;
            count++;
        }
        if (letter != '\0' && count != 1) {
            fwrite(&count, sizeof(int), 1, stdout);
            printf("%c", letter);
            //printf("%c %d ", letter, count);
        }
    }
    //printf("\n");
}

void createTempFile(int argc, char *argv[], char *newFile) {
    FILE *write = fopen(newFile, "w");
    for (int i = 1; i < argc; i++) {
        char *filename = argv[i];
        FILE *fp = fopen(filename, "r");

        char *line = NULL;
        size_t size = sizeof(&line);

        while(getline(&line, &size , fp) != -1) {
            fputs(line,write);
        }
        fclose(fp);
    }
    fclose(write);

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    /*
    char *name = argv[1];
    char *temp = "_temp";
    char filename[strlen(name) + strlen(temp)];
    strcpy(filename, name);
    strcat(filename, temp);
    */
    char *filename = "_tmp.txt";
    createTempFile(argc, argv, filename);

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("wzip: cannot open file\n");
        exit(1);
    }

    char *line = NULL;
    size_t size = sizeof(&line);

    while(getline(&line, &size , fp) != -1) {
        int maxLen = strlen(line)*2 + 1;
        char compressed[maxLen];
        memset(compressed, '\0', maxLen);

        //printf("%s", line);
        compressLine(line, compressed);
    }

    fclose(fp);
    return 0;
}
