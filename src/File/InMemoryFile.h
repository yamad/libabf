#ifndef INMEMORYFILE_H
#define INMEMORYFILE_H

#include <stdlib.h>

#include "File.h"

void *calloc_safe(int numberElements, size_t elementSize);
void * allocateByteArray(int numberBytes);

File InMemoryFile_Create(int numberBytes);
void InMemoryFile_Destroy(File);

int isWithinFile(File, filePosition);

#endif
