#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mgs_archive.h"

struct DAR_ARCHIVE *openArchive(char *filename) {
  struct DAR_ARCHIVE *archive = malloc(sizeof(struct DAR_ARCHIVE));
  assert(archive != NULL);

  // Open file
  FILE *file = fopen(filename, "rb");
  // assert(!file);

  // Get file length
  fseek(file, 0, SEEK_END);
  archive->size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory
  archive->data = (char *)malloc(archive->size + 1);
  assert(archive->data);

  // Read file contents into buffer
  fread(archive->data, archive->size, 1, file);

  // Get entry count
  archive->count = *(uint32_t *)(archive->data);

  // Allocate memory for entries
  archive->entries = malloc(sizeof(struct DAR_ARCHIVE_ENTRY) * archive->count);

  // Offset
  uint32_t index_offset = 4;

  for (uint32_t entry_index = 0; entry_index < archive->count; entry_index++) {
    // Entry Name
    archive->entries[entry_index].name = (archive->data + index_offset);

    // Adjust offset
    index_offset += strlen(archive->entries[entry_index].name) + 1;

    // Align Offset. File is 4byte aligned.
    if ((index_offset % 4) != 0)
      index_offset = index_offset - (index_offset % 4) + 4;

    // Entry Data Size
    archive->entries[entry_index].size =
        *(uint32_t *)(archive->data + index_offset);

    // Adjust offset
    index_offset += 4;

    // Store data location
    archive->entries[entry_index].data = (archive->data + index_offset);

    // Adjust offset
    index_offset += archive->entries[entry_index].size + 1;

    // Debug
    printf("Name: %s Size: %d Index: %08X\n",
           archive->entries[entry_index].name,
           archive->entries[entry_index].size, index_offset);
  }

  fclose(file);

  printf("Opening Archive Count: %d Filesize: %d\n", archive->count,
         archive->size);

  return archive;
}
