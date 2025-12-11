#ifndef F_INFO_COMMANDS_H
#define F_INFO_COMMANDS_H

#include <stdio.h>

// internal
typedef void (*command_fn)(FILE *file, char *arg);

typedef struct {
    const char *name;
    command_fn fn;
} command_entry;

void precmd(char *file);

void dispatch_cmd(const char *command, char *filename, char *arg);

void postcmd(FILE *file);

// user helper commands
void cmd_help();

void cmd_unknown(const char *command);

// commands
void cmd_linecount(FILE *file, char *_);

void cmd_globalwordcount(FILE *file, char *_);

void cmd_wordcount(FILE *file, char *word);

// command tables
static command_entry commands_table[] = {
    {"help", cmd_help},
    {"lc", cmd_linecount},
    {"gwc", cmd_globalwordcount},
    {"wc", cmd_wordcount},
    {NULL, NULL}
};

static char *commands_with_arg[] = {
    "wc"
};

#endif //F_INFO_COMMANDS_H
