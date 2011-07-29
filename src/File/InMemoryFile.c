#include <stdint.h>
#include <stdlib.h>

#include "File.h"
#include "InMemoryFile.h"

#include "memory.c"

typedef struct InMemoryFileStruct * InMemoryFile;
typedef struct InMemoryFileStruct
{
    filePosition data_length;
    filePosition position;
    int8_t * data;
} InMemoryFileStruct;

File InMemoryFile_Create(int numberBytes) 
{
    InMemoryFile self = NEW(InMemoryFileStruct);
    if (self == NULL) {
        return NULL;
    }
    self->data = allocateByteArray(numberBytes);
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

void * allocateByteArray(int numberBytes) 
{
    int8_t * ptr_mem = calloc_safe(numberBytes, sizeof(int8_t));
    if (ptr_mem == NULL) {
        return NULL;
    }
    return ptr_mem;
}

void *calloc_safe(int numberElements, size_t elementSize)
{
    void *mem;

    if (numberElements <= 0 || elementSize <= 0) {
        return NULL;
    }
    mem = calloc((size_t)numberElements, elementSize);
    return mem;
}

filePosition File_getCurrentPosition(File file) 
{
    return ((InMemoryFile)file)->position;
}

int File_seek(File file, filePosition offset, filePosition origin)
{
/*    InMemoryFile self = (InMemoryFile) file; */
    if (isWithinFile(file, offset + origin)) {
    }
    return TRUE;
}

int isWithinFile(File file, filePosition position) 
{
    InMemoryFile self = (InMemoryFile) file;
    if (self->data_length > position) {
        return TRUE;    
    }
    return FALSE;
}
