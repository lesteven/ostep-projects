#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // run interactive mode

    } else if (argc > 2) {
        fprintf(stderr,"wish shell only accepts one argument\n");
    } else {
        // batch mode
        FILE *read_file = fopen(argv[1], "r");
        if (read_file == NULL) {
            fprintf(stderr, "wish: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

    }

    return 0;
}
