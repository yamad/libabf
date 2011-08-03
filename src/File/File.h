#ifndef FILE_H
#define FILE_H

#include "platform.h"

typedef struct FileStruct * File;

typedef off_t filePosition;

File File_Create(size_t size);
File File_OpenForRead(const char *path);
File File_OpenForWrite(const char *path);
int File_Close(File file);

int File_readBlock(File file, void *ptr, size_t size);
int File_readMultipleBlocks(File file, void *ptr, size_t size, size_t count);
int File_writeBlock(File file, const void *ptr, size_t size);
int File_writeMultipleBlocks(File file, const void *ptr, size_t size, size_t count);

int File_seek(File file, filePosition offset, filePosition origin);
int File_seekFromStart(File file, filePosition offset);
int File_seekFromCurrent(File file, filePosition offset);
int File_seekFromEnd(File file, filePosition offset);
int File_seekToStart(File file);
int File_seekToEnd(File file);

filePosition File_getCurrentPosition(File file);
Boolean File_isOpenForRead(File file);
Boolean File_isOpenForWrite(File file);

#endif
