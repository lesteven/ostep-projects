#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getFileName(char *file) {
    char *word = NULL;
    int len = strlen(file);
    for (int i = len-1; i >= 0; i--) {
        if (file[i] == '/') {
            word = malloc(sizeof(char) * (len-i));
            strncpy(word, file + sizeof(char) * (i+1), len-i);
            break;
        }
    }
    return word == NULL? file: word;
}

void freeFile(char *name, char *file) {
    if (strcmp(name, file) == 0) {
        free(name);
    }
}

typedef struct Node {
    char *value;
    struct Node *next;
} Node;

void reverse(char *argv[], FILE *read_file, FILE *write_file) {
    char *line = NULL;
    size_t size = 0;

    Node *head = NULL;
    while (getline(&line, &size, read_file) != -1) {
        char *copy = malloc(strlen(line));
        strncpy(copy, line, strlen(line));

        Node *node = malloc(sizeof(*head));
        node->value = copy;

        node->next = head;
        head = node;
    }

    Node *curr_head = head;
    while (curr_head != NULL) {
        //printf("curr_head %s", curr_head->value);
        fputs(curr_head->value, write_file);
        curr_head = curr_head->next;
    }

}

int main(int argc, char *argv[]) {
    if (argc > 3) {
        fprintf(stderr, "%s\n", "usage: reverse <input> <output>");
        exit(1);
    }
    if (argc > 2) {
        char *first = getFileName(argv[1]);
        char *second = getFileName(argv[2]);
        if (strcmp(first, second) == 0) {
            fprintf(stderr, "%s\n", "reverse: input and output file must differ");
            freeFile(first, argv[1]);
            freeFile(second, argv[2]);
            exit(1);
        }
    }

    if (argc == 1) {
        //reverse stdin
        reverse(argv, stdin, stdout);
        return 0;
    } else {
        FILE *read_file = fopen(argv[1], "r");
        if (read_file == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        FILE *write_file = fopen(argv[2], "w");
        reverse(argv, read_file, write_file);
    }

    return 0;
}
