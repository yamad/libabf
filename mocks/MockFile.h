#ifndef MOCKFILE_H
#define MOCKFILE_H

#include "File.h"

void MockFile_Create(int maxExpectations);
void MockFile_Destroy(void);

void MockFile_Expect_Read(fileAddress addr);

#endif
