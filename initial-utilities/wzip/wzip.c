#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countDigits(int num) {
    int digits = 0;

    do {
        num = num/10;
        digits++;
    } while (num > 0);

    return digits;
}

void compressLine(char *line, char compressed[]) {
    char letter = line[0];
    int count = 1;
    int currIndex = 0;
    for (int i = 1; i < strlen(line); i++) {
        if (line[i] == letter) {
            count++;
        } else {
            int digits = countDigits(count);
            char digitStr[digits+1];
            sprintf(digitStr, "%d", count);
            // printf("%d%c-%d ", digits, letter, count);
            for (int j = 0; j < digits; j++) {
                compressed[currIndex++] = digitStr[j];
            }
            compressed[currIndex++] = letter;
            letter = line[i];
            count = 1;
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
        int maxLen = strlen(line)*2 + 1;
        char compressed[maxLen];
        memset(compressed, '\0', maxLen);
        compressLine(line, compressed);
        printf("%s\n", compressed);
    }
    fclose(fp);
    return 0;
}
