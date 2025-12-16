#ifndef F_INFO_FILE_UTILS_H
#define F_INFO_FILE_UTILS_H

#include <sys/stat.h>

#if defined(_WIN32)
#include <windows.h>
#endif

static int file_exists(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return 0; // file does not exist
    return S_ISREG(path_stat.st_mode);
}


static long get_file_size(FILE *file) {
    const long current = ftell(file);
    if (current < 0) return -1;
    if (fseek(file, 0, SEEK_END) != 0) return -1;

    const long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

#endif // F_INFO_FILE_UTILS_H
