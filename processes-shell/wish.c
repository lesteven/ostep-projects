#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void executeLine(FILE *stream) {
    char *line = NULL;
    size_t size = 0;
    getline(&line, &size, stream);
    printf("wish> command entered: %s", line);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // run interactive mode
        while (1) {
            printf("wish> ");
            executeLine(stdin);
        }

    } else if (argc > 2) {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
    } else {
        // batch mode
        FILE *read_file = fopen(argv[1], "r");
        if (read_file == NULL) {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

    }

    return 0;
}
