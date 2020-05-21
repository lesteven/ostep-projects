#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

typedef struct Node {
    char *value;
    struct Node *next;
} Node;

Node pathTwo = { "/usr/bin" };
Node pathHead = { "/bin", &pathTwo };
char fslash[] = "/";

void writeError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void getCommand(char *command, char *path, char *split) {
    // add +1 -> copies '\n', prevents junk data from showing
    strncpy(command, path, strlen(path)+1);
    strncat(command, fslash, strlen(fslash));
    strncat(command, split, strlen(split));
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
    *size = 0;

    int start = -1;
    for (int i = 0; i < strlen(command); i++) {
        if (command[i] == ' ' || command[i] == '\t') {
            if (start > -1 && start < i) {
                char *copy = malloc(i-start+1);
                strncpy(copy, command+start, i-start);
                //printf("%d %d %s\n", start, i, copy);

                Node *newNode = malloc(sizeof(*sentinel));
                newNode->value = copy;
                //printf("print word: %s\n", newNode->value);
                sentinel->next = newNode;
                sentinel = newNode;
                *size +=1;
                start = -1;
            }
        } else if (start == -1) {
            start = i;
        }
    }
    if (start != -1) {
        char *copy = malloc(strlen(command)-start+1);
        strncpy(copy, command+start, strlen(command)-start);
        Node *newNode = malloc(sizeof(*sentinel));
        newNode->value = copy;
        //printf("print word: %s\n", newNode->value);
        sentinel->next = newNode;
        *size +=1;
    }
}

void printList(Node *node, int size) {
    while (node != NULL) {
        //printf("printing: %s %p\n", node->value, node);
        printf("printing: %s\n", node->value);
        node = node->next;
    }
    printf("size %d\n", size);

}

bool isBuiltIn(Node *node) {
    if (node == NULL) {
        return false;
    }
    char *builtIn[] = { "exit", "cd", "path" };
    for (int i = 0; i < sizeof(builtIn)/sizeof(char*); i++) {
        if (strcmp(node->value,builtIn[i]) == 0) {
            return true;
        }
    }
    return false;
}

void forkExec(char *command, Node *node, int size) {
    // add all linked list values to files; +1 for NULL
    char *args[size+1];
    for (int i = 0; i < size; i++) {
        args[i] = node->value;
        node = node->next;
    }
    args[size] = NULL;
    pid_t fork_id = fork();
    //printf("starting %d\n", (int) getpid());
    if (fork_id < 0) {
        // error
        writeError();
    } else if (fork_id == 0) {
        // child process created successfully
        execv(command, args);
    }

}

void executeBuiltIn(Node *node, int size) {
    if (strcmp(node->value, "exit") == 0) {
        if (size == 1) {
            exit(0);
        } else {
            writeError();
        }
    } else if (strcmp(node->value, "cd") == 0) {
        if (size == 2) {
            chdir(node->next->value);
        } else {
            writeError();
        }
    } else if (strcmp(node->value, "path") == 0) {
        printf("pathing\n");
    }

}

void executeBin(Node *node, int size) {
    Node *pathCopy = &pathHead;
    // iterate through paths, if successful, execute binaries
    while (pathCopy != NULL) {
        //printf("%s\n", pathCopy->value);

        // +1 for '\0' character
        char command[strlen(pathCopy->value)+strlen(fslash)+strlen(node->value)+1];
        getCommand(command, pathCopy->value, node->value);
        int available = access(command, X_OK);
        if (available == 0) {
            //printf("is available\n");
            forkExec(command, node, size);
            break;
        } else {
            writeError();
            break;
        }
        pathCopy = pathCopy->next;
    }
}

void executeLine(char *line) {
    char *split = NULL;

    while (line != NULL) {
        split = strsep(&line, "&");
        //printf("execute %s\n", split);
        trimStr(split);
        // created linked list for commands and get size
        Node sentinel = { "" };
        int sizeCommands = 0;
        createLinkedList(split, &sentinel, &sizeCommands);
        Node command = *sentinel.next;
        //printList(&sentinel, size);

        // once get command, if built in command
        if (isBuiltIn(&command)) {
            // call own function
            //printf("is built in\n");
            executeBuiltIn(&command, sizeCommands);
        } else {
            // execute from bin
            //printf("not built in\n");
            executeBin(&command, sizeCommands);
        }
    }
    // wait for children to be done and return to parent
    while (wait(NULL) != -1) {
    }
}

int main(int argc, char *argv[]) {
    char *line = NULL;
    size_t size = 0;

    if (argc == 1) {
        // run interactive mode
        while (1) {
            printf("wish> ");
            getline(&line, &size, stdin);
            executeLine(line);
        }

    } else if (argc > 2) {
        writeError();
    } else {
        // batch mode
        FILE *read_file = fopen(argv[1], "r");
        if (read_file == NULL) {
            writeError();
            exit(1);
        } else {
            while (getline(&line, &size, read_file) != -1) {
                executeLine(line);
            }
        }
    }

    return 0;
}
