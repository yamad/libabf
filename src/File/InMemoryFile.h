#ifndef INMEMORYFILE_H
#define INMEMORYFILE_H

#include <stdlib.h>

#include "File.h"

void *safe_calloc(int numberElements, size_t elementSize);

File InMemoryFile_Create(int numberBytes);
void InMemoryFile_Destroy(File);

#endif
