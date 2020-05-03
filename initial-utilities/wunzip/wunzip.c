#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        while (1) {
            int count = 0;
            fread(&count, sizeof(int), 1, fp);

            if (count == 0) {
                break;
            }

            char letter = fgetc(fp);
            for (int i = 0; i < count; i++) {
                printf("%c", letter);
            }
        }
    }
    return 0;
}
