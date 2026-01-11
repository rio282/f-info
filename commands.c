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
            rewind(file);
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

void prompt_help() {
    printf("finfo takes exactly two or more arguments.\n");
    printf("For help, use --help\n");
}

void cmd_help() {
    printf("Accepted commands:\n");
    for (int i = 0; commands_table[i].name != NULL; i++) {
        printf(" - %s\t: %s\n", commands_table[i].name, commands_table[i].description);
    }
}

void cmd_unknown(const char *command) {
    fprintf(stderr, "Unknown command: %s\n", command);
}

void cmd_linecount(FILE *file, char *_) {
    int lines = 0;
    int c;
    while ((c = fgetc(file)) != EOF) if (c == '\n') lines++;
    printf("Counted %d lines.\n", lines);
}

void cmd_wordcount(FILE *file, char *word) {
    if (!word) return;

    int count = 0;
    char token_buffer[1024];

    // '%s' skips whitespaces
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
    int count = 0;
    for (char token_buffer[1024]; fscanf(file, "%1023s", token_buffer) == 1; count++);
    printf("Found a total %d word(s).\n", count);
}

void cmd_charfrequency(FILE *file, char *_) {
    int frequency_table[128] = {0}; // ASCII

    // collect characters
    int c;
    while ((c = fgetc(file)) != EOF) if (c >= 0 && c < 128) frequency_table[c]++;

    // print
    for (int key = 0; key < 128; key++) {
        if (frequency_table[key] == 0) continue;
        if (isprint(key)) printf("'%c': %d\n", key, frequency_table[key]);
        else if (key == 10) printf("<new lines>: %d\n", frequency_table[key]);
        else printf("0x%02X: %d\n", key, frequency_table[key]);
    }
}

void cmd_metadata(FILE *file, char *_) {
    const long size = get_file_size(file);
    if (size >= 0) printf("Size: %s\n", to_tiny_size(size));
    else printf("Size: unavailable\n");

    // wow... horrendous
#if defined(_WIN32)
    const int fd = _fileno(file);
    HANDLE handle = (HANDLE) _get_osfhandle(fd);
    if (handle == INVALID_HANDLE_VALUE) {
        printf("Permissions: unavailable\n");
        return;
    }

    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(handle, &info)) {
        printf("Permissions: unavailable\n");
        return;
    }

    const int read_only = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
    printf("Readable: yes\n");
    printf("Writable: %s\n", read_only ? "no" : "yes");
    printf("Executable: unknown (Windows)\n");
#else
    int fd = fileno(file);
    struct stat st;

    if (fstat(fd, &st) == 0) {
        printf("Permissions: %o\n", st.st_mode & 0777);
        printf("Readable: %s\n", (st.st_mode & S_IRUSR) ? "yes" : "no");
        printf("Writable: %s\n", (st.st_mode & S_IWUSR) ? "yes" : "no");
        printf("Executable: %s\n", (st.st_mode & S_IXUSR) ? "yes" : "no");
    } else {
        printf("Permissions: unavailable\n");
    }
#endif
}
