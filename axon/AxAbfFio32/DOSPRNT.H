//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// HEADER:  DOSPRNT.H
// PURPOSE: Contains prototypes for DOSPRNT.CPP
//
void DOS_Initialize(void);
int DOS_fputs(char *pszStart, FILE *pf);
int cdecl DOS_fprintf(FILE *pf, char *fmt, ...);         /* printf with page pause */
void DOS_SetPauseFlag(BOOL bPause);  
