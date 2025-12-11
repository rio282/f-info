#include "commands.h"
#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void precmd(char *file) {
    if (!file_exists(file)) {
        fprintf(stderr, "File '%s' doesn't exist.\n", file);
        exit(1);
    }
}

void dispatch_cmd(const char *command, char *filename, char *arg) {
    for (int i = 0; commands_table[i].name != NULL; i++) {
        if (strcmp(command, commands_table[i].name) == 0) {
            precmd(filename);

            FILE *file = fopen(filename, "r");
            commands_table[i].fn(file, arg);

            postcmd(file);
            return;
        }
    }
    cmd_unknown(command);
}

void postcmd(FILE *file) {
    fclose(file);
}

void cmd_help() {
    printf("finfo takes exactly two or more arguments.\n");
    printf("For help, use --help\n");
}

void cmd_unknown(const char *command) {
    fprintf(stderr, "Unknown command: %s\n", command);
}

void cmd_linecount(FILE *file, char *arg) {
    int lines = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) if (ch == '\n') lines++;
    printf("Counted %d lines.\n", lines);
}

void cmd_wordcount(FILE *file, char *word) {
    if (!word) return;

    int count = 0;
    char buffer[1024];

    while (fscanf(file, "%1023s", buffer) == 1) {
        if (strcmp(buffer, word) == 0) count++;
    }

    printf("The word '%s' occurs %d times.\n", word, count);
}
