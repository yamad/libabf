//******************************************************************************
//
//    Copyright (c) 1999 Axon Instruments.
//    All rights reserved.
//
//******************************************************************************
// HEADER:  crc16.h
// PURPOSE: Declares function for calculating a 16 bit CRC code.
// SOURCE:  http://www.brokersys.com/snippets/   file: CRC_16F.C
//
WORD CalculateCRC(WORD wInitCRC, const void *pvData, UINT uBytes);
