 //***********************************************************************************************
//
//    Copyright (c) 1993-2005 Molecular Devices Corporation.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************

#ifndef INC_PROTOCOLWRITERABF2_H
#define INC_PROTOCOLWRITERABF2_H

#include "SimpleStringCache.hpp"
#include "ProtocolStructs.h"            // Struct definitions for actual file contents
#include "ProtocolWriter.hpp"           // Base class

// Forward declarations.
class CFileDescriptor;
struct ABFFileHeader;

//===============================================================================================
class CProtocolWriterABF2 : public CProtocolWriter
{
private:
   ABFFileHeader        *m_pFH;
   ABF_FileInfo          m_FileInfo;

   CSimpleStringCache    m_Strings;  // The string writing object.
   long                  m_lNextBlock;

private:
   BOOL WriteFileInfo();
   BOOL WriteProtocolInfo();
   BOOL WriteADCInfo();
   BOOL WriteDACInfo();
   BOOL WriteEpochs();
   BOOL WriteStats();
   BOOL WriteUserList();
   BOOL WriteMathInfo();

   BOOL WriteStrings();
   UINT AddString( LPCSTR pszText, UINT uLen );
   BOOL SetLastError( int nError );

public:
   CProtocolWriterABF2( CFileDescriptor *pFI );
   virtual ~CProtocolWriterABF2();
   
   virtual BOOL Write( const ABF_FileInfo *pOldFileInfo, ABFFileHeader *pFH );

   static BOOL CanSave( const ABFFileHeader *pFH );
};

#endif   // INC_PROTOCOLWRITERABF2_H
