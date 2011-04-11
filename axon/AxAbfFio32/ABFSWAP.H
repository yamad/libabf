//***********************************************************************************************
//
//    Copyright (c) 1993-1995 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// HEADER:  ABFSWAP.H
// PURPOSE: Contains functions to support byte swapping.
//

#define SWAP_ALL  0

void SwapTwoBytes(void *pv, UINT uCount);
void SwapFourBytes(void *pv, UINT uCount);
void ByteSwapABFHeader(ABFFileHeader *pFH);
void ByteSwapTags(ABFTag *pTagArray, UINT uCount);
void ByteSwapScopes(ABFScopeConfig *pCfg, UINT uCount);

