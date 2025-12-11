#ifndef F_INFO_FILE_UTILS_H
#define F_INFO_FILE_UTILS_H

#include <sys/stat.h>

static int file_exists(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return 0; // file does not exist
    return S_ISREG(path_stat.st_mode);
}

#endif // F_INFO_FILE_UTILS_H
