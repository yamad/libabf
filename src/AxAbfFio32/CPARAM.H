/****************************************************************************\
*                                                                            *
*   Written 1993 by AXON Instruments Inc.                                    *
*                                                                            *
*   This file is not protected by copyright. You are free to use, modify     *
*   and copy the code in this file.                                          *
*                                                                            *
\****************************************************************************/

// This is CPARAM.H.

// This file contains procedure prototypes for all procedures that are
// exported by CPARAM.CPP

#define CP_SUCCESS          0
#define CP_EOPENFILE        1
#define CP_EREADFILE        2
#define CP_ESEEKFILE        3
#define CP_EWRITEFILE       4
#define CP_ENOTFOUND        5
#define CP_EINVALIDFILE     6
#define CP_EBADINDEX        8

int CP_SetShowParam(char *pszFileName, char *pszParam, char *pszValue, int CPIndex, int CPItsAChar);
int CP_DumpHeaderOffsets(char *pszFileName);
void CP_ShowError(int ErrorNum);

