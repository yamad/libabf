#include <stdint.h>
#include <stdlib.h>

#include "File.h"
#include "InMemoryFile.h"

typedef struct InMemoryFileStruct * InMemoryFile;
typedef struct InMemoryFileStruct
{
    size_t data_length;
    fileAddress position;
    int8_t * data;
} InMemoryFileStruct;

File InMemoryFile_Create(int numberBytes) 
{
    InMemoryFile self = malloc(sizeof(InMemoryFileStruct));
    self->data = safe_calloc(numberBytes, sizeof(int8_t));
    if (self->data == NULL) {
        InMemoryFile_Destroy((File)self);
        return NULL;
    }
    
    self->data_length = (size_t)numberBytes;
    self->position = 0;
    return (File)self;
}

void InMemoryFile_Destroy(File super)
{
    InMemoryFile self = (InMemoryFile)super;
    free(self->data);
    free(self);
    self->data = NULL;
    self = NULL;
}


void *safe_calloc(int numberElements, size_t elementSize)
{
    void *mem;

    if (numberElements <= 0 || elementSize <= 0) {
        return NULL;
    }
    mem = calloc((size_t)numberElements, elementSize);
    return mem;
}
