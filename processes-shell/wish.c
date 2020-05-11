#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char pathOne[] = "/bin";
char pathTwo[] = "/usr/bin";
char fslash[] = "/";

void getCommand(char *command, char *path, char *split) {
    // add +1 -> copies '\n', prevents junk data from showing
    strncpy(command, path, strlen(path)+1);
    strncat(command, fslash, strlen(fslash));
    strncat(command, split, strlen(split));
    /*
    for (int i = 0; i < strlen(command); i++) {
        printf("%c %d %d\n", command[i], command[i], i);
    }
    printf("==============");
    */
}

void trimStr(char str[]) {
    for (int i = strlen(str)-1; i >=0; i--) {
        if (str[i] == '\n') {
            str[i] = '\0';
        }
    }
}

void executeLine(FILE *stream) {
    char *line = NULL;
    size_t size = 0;
    getline(&line, &size, stream);
    char *split = NULL;
    while (line != NULL) {
        split = strsep(&line, "&");
        trimStr(split);
        // +1 for '\0' character
        char command[strlen(pathOne)+strlen(fslash)+strlen(split)+1];
        getCommand(command, pathOne, split);

        /*
        int size = strlen(pathOne)+strlen(fslash)+strlen(split);
        printf("%s %d\n", command, size);
        int test = access(command, X_OK);

        char *foo = "/bin/ls";
        int test2 = access(foo, X_OK);
        int cmp = strcmp(foo, command);
        //printf("run: %s %d %d %d %d\n", command, test, test2, cmp, size);

        //printf("%s %ld\n", command, strlen(command));
        //printf("%s %ld\n", foo, strlen(foo));
        */
        char *files[] = { split, NULL };
        execv(command, files);
    }
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
