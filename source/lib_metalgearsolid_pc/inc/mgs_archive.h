#ifndef MGS_ARCHIVE_H
#define MGS_ARCHIVE_H

#include <stdint.h>

extern struct DAR_ARCHIVE *openArchive(char *filename);

struct DAR_ARCHIVE {
  uint32_t count;
  uint32_t size;
  char *data;

  struct DAR_ARCHIVE_ENTRY *entries;
};

struct DAR_ARCHIVE_ENTRY {
  char *name;
  uint32_t size;
  char *data;
};

#endif
