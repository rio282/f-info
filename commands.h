#ifndef F_INFO_COMMANDS_H
#define F_INFO_COMMANDS_H

#include <stdio.h>

// internal
typedef void (*command_fn)(FILE *file, char *arg);

typedef struct {
    const char *name;
    command_fn fn;
    const char *description;
} command_entry;

void precmd(char *file);

void dispatch_cmd(const char *command, char *filename, char *arg);

void postcmd(FILE *file);

// user helper commands
void prompt_help();

void cmd_unknown(const char *command); // TODO: rename

// commands
void cmd_help();

void cmd_linecount(FILE *file, char *_);

void cmd_globalwordcount(FILE *file, char *_);

void cmd_wordcount(FILE *file, char *word);

void cmd_charfrequency(FILE *file, char *_);

void cmd_metadata(FILE *file, char *_);

// command tables
static command_entry commands_table[] = {
    {"help", cmd_help, "Display a list of available commands and their descriptions"},
    {"lc", cmd_linecount, "Count the number of lines in the file"},
    {"gwc", cmd_globalwordcount, "Count the total number of words in a file"},
    {"wc", cmd_wordcount, "Count a specific word in the file. Usage: wc <word> <file(s)>"},
    {"cf", cmd_charfrequency, "Display the frequency of each character in the file"},
    {"md", cmd_metadata, "Displays the metadata of the input file"},
    {NULL, NULL}
};

static char *commands_with_arg[] = {
    "wc"
};

#endif //F_INFO_COMMANDS_H
