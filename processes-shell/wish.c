#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct Node {
    char *value;
    struct Node *next;
} Node;

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
            break;
        }
    }
}

void createLinkedList(char *command, Node *sentinel, int *size) {
    char *sep = NULL;
    *size = 0;
    while (command != NULL) {
        sep = strsep(&command, " ");
        if (strlen(sep) == 0) {
            continue;
        }
        //Node next = Node { sep };
        *size += 1;
        Node *newNode = malloc(sizeof(*sentinel));
        newNode->value = sep;
        printf("%s %p\n", newNode->value, &newNode);
        sentinel->next = newNode;
        sentinel = newNode;
    }
}
void printList(Node *node, int size) {
    node = node->next;
    while (node != NULL) {
        printf("%s\n", node->value);
        node = node->next;
    }
    printf("size %d\n", size);

}

void executeLine(FILE *stream) {
    char *line = NULL;
    size_t size = 0;
    getline(&line, &size, stream);
    char *split = NULL;

    while (line != NULL) {
        split = strsep(&line, "&");

        trimStr(split);
        // created linked list for commands and get size
        Node sentinel = { "" };
        int size = 0;
        createLinkedList(split, &sentinel, &size);
        printList(&sentinel, size);


        // once get command, if built in command
        // call own function, else execute from bin

        // iterate through paths, if successful
        // execute binaries
        // +1 for '\0' character
        char command[strlen(pathOne)+strlen(fslash)+strlen(split)+1];
        getCommand(command, pathOne, split);


        // int available = access(command, X_OK);

        char *files[] = { split, NULL };
        pid_t fork_id = fork();
        //printf("starting %d\n", (int) getpid());
        if (fork_id < 0) {
            // error
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        } else if (fork_id == 0) {
            // child process created successfully
            execv(command, files);
        }
    }
    // wait for children to be done and return to parent
    while (wait(NULL) != -1) {
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
