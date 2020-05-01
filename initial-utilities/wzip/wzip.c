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
        }
    }

}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *filename = argv[i];
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

            compressLine(line, compressed);
        }

        fclose(fp);
    }

    return 0;
}
