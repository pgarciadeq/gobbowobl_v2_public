//Header defining FileSystem struct
#ifndef FileSystem_H
#define FileSystem_H

#include <stdint.h>

#define FILE_ENTRY_SIZE 32
#define MAX_FILES 16

struct FileEntry {
    char name[16];
    uint32_t size;
    uint32_t offset;
    uint8_t reserved[8];
};

#endif
