#include "commands.h"
#include "file_utils.h"

#include <stdio.h>
#include <ctype.h>
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

void cmd_linecount(FILE *file, char *_) {
    int lines = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) if (ch == '\n') lines++;
    printf("Counted %d lines.\n", lines);
}

void cmd_wordcount(FILE *file, char *word) {
    if (!word) return;
    rewind(file);

    int count = 0;
    char token_buffer[1024];

    while (fscanf(file, "%1023s", token_buffer) == 1) {
        char word_candidate[1024];
        int cursor = 0;

        for (int i = 0; token_buffer[i]; i++) {
            if (isalnum((unsigned char) token_buffer[i])) {
                word_candidate[cursor++] = token_buffer[i];
            }
        }
        word_candidate[cursor] = '\0';

        // (case-sensitive)
        if (strcmp(word_candidate, word) == 0) count++;
    }

    printf("The word '%s' occurs %d times.\n", word, count);
}

void cmd_globalwordcount(FILE *file, char *_) {
    // TODO...
}
