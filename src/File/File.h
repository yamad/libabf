#ifndef FILE_H
#define FILE_H

#include "platform.h"

typedef struct FileStruct * File;

typedef off_t filePosition;

File File_Create(size_t size);
File File_OpenForRead(const char *path);
File File_OpenForWrite(const char *path);
int File_Close(File file);

int File_ReadBlock(File file, void *ptr, size_t size);
int File_ReadMultipleBlocks(File file, void *ptr, size_t size, size_t count);
int File_WriteBlock(File file, const void *ptr, size_t size);
int File_WriteMultipleBlocks(File file, const void *ptr, size_t size, size_t count);

int File_seek(File file, filePosition offset, filePosition origin);
int File_SeekFromStart(File file, filePosition offset);
int File_SeekFromCurrent(File file, filePosition offset);
int File_SeekFromEnd(File file, filePosition offset);

filePosition File_getCurrentPosition(File file);
Boolean File_isOpenForRead(File file);
Boolean File_isOpenForWrite(File file);

#endif
