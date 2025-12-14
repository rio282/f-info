#include <stdio.h>
#include <string.h>

#include "commands.h"

#define COMMAND_PREFIX "--"

int main(const int argc, char *argv[]) {
    if (argc == 1) {
        prompt_help();
        return 1;
    }

    if (strncmp(COMMAND_PREFIX, argv[1], strlen(COMMAND_PREFIX)) == 0) {
        char *command = argv[1] + strlen(COMMAND_PREFIX);
        int files_pointer_offset = 2;

        // first, check if the help command is called
        if (strcmp(command, "help") == 0) {
            cmd_help();
            return 0;
        }

        // check for arguments
        char *additional_argument = NULL;
        for (int i = 0; i < sizeof commands_with_arg / sizeof commands_with_arg[0]; i++) {
            if (strcmp(command, commands_with_arg[i]) == 0) {
                additional_argument = argv[2]; // grab third: <file called> <command> <(possible) arg> <file(s)>
                files_pointer_offset++;
                break;
            }
        }

        // we can start to get the files because we finally know where to start because of files_pointer_offset
        char **files = argv + files_pointer_offset;
        const int file_count = argc - files_pointer_offset;

        // call command on each file
        if (file_count == 0) {
            cmd_unknown(command);
            return 1;
        }

        // call specified command on each file
        for (int i = 0; i < file_count; i++) {
            printf("Called '%s' on file: %s\n", command, files[i]);
            dispatch_cmd(command, files[i], additional_argument);
            if (i != file_count - 1) printf("\n\n");
        }
    } else {
        prompt_help();
        return 1;
    }

    return 0;
}
