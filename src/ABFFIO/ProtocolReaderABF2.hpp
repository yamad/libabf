 //***********************************************************************************************
//
//    Copyright (c) 2005 Molecular Devices Corporation.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************

#ifndef INC_ABF2PROTOCOLREADER_H
#define INC_ABF2PROTOCOLREADER_H

#pragma once

#include "SimpleStringCache.hpp"
#include "ProtocolStructs.h"            // Struct definitions for actual file contents
#include "ProtocolReader.hpp"           // Base class

//===============================================================================================
class CABF2ProtocolReader : public CProtocolReader
{
private:
   ABF_FileInfo         m_FileInfo;
   CSimpleStringCache   m_Strings;  // The string writing object.


private:
   BOOL ReadFileInfo();
   BOOL ReadProtocolInfo();
   BOOL ReadADCInfo();
   BOOL ReadDACInfo();
   BOOL ReadEpochs();
   BOOL ReadStats();
   BOOL ReadUserList();
   BOOL ReadMathInfo();

   BOOL GetString( UINT uIndex, LPSTR pszText, UINT uBufSize );

public:
   CABF2ProtocolReader( CFileDescriptor *pFI, ABFFileHeader *pFH );
   virtual ~CABF2ProtocolReader();

   static BOOL CanOpen( const void *pFirstBlock, UINT uBytes );
   
   virtual BOOL Read( UINT fFlags );
   virtual const ABF_FileInfo *GetFileInfo() const      { return &m_FileInfo; }
   virtual BOOL ValidateCRC();
};

#endif   // INC_ABF2PROTOCOLREADER_H
