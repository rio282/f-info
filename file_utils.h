#ifndef F_INFO_FILE_UTILS_H
#define F_INFO_FILE_UTILS_H

#include <stdio.h>
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


static char *to_tiny_size(const long b) {
    static char ts[32];

    if (b < 1000) {
        snprintf(ts, sizeof(ts), "%ld b", b);
        return ts;
    }

    double bytes = b;
    int divs = 0;
    const char *sizes[6] = {"", "k", "m", "g", "t", "p"};

    while (bytes >= 1000 && divs < 5) {
        bytes /= 1000;
        divs++;
    }

    snprintf(ts, sizeof(ts), "%.2f %sb", bytes, sizes[divs]);
    return ts;
}

#endif // F_INFO_FILE_UTILS_H
