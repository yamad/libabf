#include <stdint.h>
#include <stdlib.h>
#include <string.h>


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

int8_t InMemoryFile_getByteAt(File file, filePosition index)
{
    InMemoryFile self = (InMemoryFile) file;
    if (isWithinFile((File)self, index)) {
        return self->data[index];
    }
    return FALSE;
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
    InMemoryFile self = (InMemoryFile) file;
    filePosition new_position = offset + origin;
    if (isWithinFile((File)self, new_position)) {
        self->position = new_position;
    }
    return TRUE;
}

int File_seekFromStart(File file, filePosition offset)
{
    return File_seek(file, offset, 0);
}

int File_seekFromCurrent(File file, filePosition offset)
{
    InMemoryFile self = (InMemoryFile) file;
    return File_seek((File)self, offset, self->position);    
}

int File_seekFromEnd(File file, filePosition offset)
{
    InMemoryFile self = (InMemoryFile) file;
    filePosition lastByte = self->data_length - 1;
    return File_seekFromStart((File)self, lastByte - offset);
}

int File_seekToStart(File file)
{
    return File_seekFromStart(file, 0);
}

int File_seekToEnd(File file)
{
    return File_seekFromEnd(file, 0);
}

int File_writeBlock(File file, const void *ptr, size_t size) 
{
    InMemoryFile self = (InMemoryFile) file;
    if (!isEnoughSpace((File)self, size)) {
        return FALSE;
    }
    memcpy(self->data, ptr, size);
    File_seekFromCurrent((File)self, size);
    return TRUE;
}

int File_writeMultipleBlocks(File file, const void *ptr, size_t size, size_t count) 
{
    size_t totalSize = size * count;
    return File_writeBlock(file, ptr, totalSize);
}

int File_readBlock(File file, void *ptr, size_t size) 
{
    InMemoryFile self = (InMemoryFile) file;
    memcpy(ptr, self->data, size);
    File_seekFromCurrent((File)self, size);
    return TRUE;
}

int File_readMultipleBlocks(File file, void *ptr, size_t size, size_t count)
{
    size_t totalSize = size * count;
    return File_readBlock(file, ptr, totalSize);
}

Boolean isEnoughSpace(File file, size_t size)
{
    InMemoryFile self = (InMemoryFile) file;
    if (isWithinFile((File)self, self->position + size)) {
        return TRUE;
    }
    return FALSE;
}

Boolean isWithinFile(File file, filePosition position) 
{
    InMemoryFile self = (InMemoryFile) file;
    if (0 <= position && self->data_length > position) {
        return TRUE;    
    }
    return FALSE;
}
