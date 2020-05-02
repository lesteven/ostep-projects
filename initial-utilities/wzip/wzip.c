#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int count = 0;
    // 0 is null character in ascii
    char letter = 0;
    char curr_char = 0;

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        while ((curr_char = fgetc(fp)) != EOF) {
            if (letter == 0) {
                letter = curr_char;
                count++;
            } else if (curr_char != letter) {
                fwrite(&count, sizeof(int), 1, stdout);
                printf("%c", letter);
                count = 1;
                letter = curr_char;
            } else {
                count++;
            }
        }
        fclose(fp);
    }

    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        printf("%c", letter);
    }
    return 0;
}
