#ifndef FILE_H
#define FILE_H

#include "platform.h"

typedef struct FileStruct * File;

typedef off_t fileAddress;

File File_Create(size_t size);
File File_OpenForRead(const char *path);
File File_OpenForWrite(const char *path);
int File_Close(File *fileptr);

int File_ReadBlock(File *file, void *ptr, size_t size);
int File_ReadMultipleBlocks(File *file, void *ptr, size_t size, size_t count);
int File_WriteBlock(File *file, const void *ptr, size_t size);
int File_WriteMultipleBlocks(File *file, const void *ptr, size_t size, size_t count);

int File_Seek(File *file, fileAddress offset, fileAddress origin);
int File_SeekFromStart(File *file, fileAddress offset);
int File_SeekFromCurrent(File *file, fileAddress offset);
int File_SeekFromEnd(File *file, fileAddress offset);

fileAddress File_CurrentPosition(File *file);
Boolean File_isOpenForRead(File *file);
Boolean File_isOpenForWrite(File *file);

#endif
