#ifndef INMEMORYFILE_H
#define INMEMORYFILE_H

#include <stdlib.h>

#include "File.h"

void *calloc_safe(int numberElements, size_t elementSize);
void * allocateByteArray(int numberBytes);

File InMemoryFile_Create(int numberBytes);
void InMemoryFile_Destroy(File file);

int8_t InMemoryFile_getByteAt(File file, filePosition index);

Boolean isWithinFile(File, filePosition);
Boolean isEnoughSpace(File, size_t);

#endif
