//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// HEADER:  VOICETAG.CPP
// PURPOSE: Contains member functions for CVoiceTag class implementation. 
// AUTHOR:  BHI  Nov 1995
//
#include "wincpp.hpp"
#include "voicetag.hpp"
#include "..\Common\pkware.h"
#include "..\Common\ArrayPtr.hpp"

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Initializes the object.
//
CVoiceTag::CVoiceTag(LPCSTR pszFileName, long lDataOffset, ABFVoiceTagInfo *pVTI)
{
   MEMBERASSERT();
   m_pNextItem  = NULL;
   strncpy(m_szFileName, pszFileName, _MAX_PATH-1);
   m_szFileName[_MAX_PATH-1] = '\0';
   m_lDataOffset = lDataOffset;
   m_nLastError  = 0;
   
   m_VTI = *pVTI;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Cleans up the object.
//
CVoiceTag::~CVoiceTag()
{
   MEMBERASSERT();
   ASSERT(m_pNextItem == NULL);
   m_pNextItem = NULL;
}


//===============================================================================================
// FUNCTION: SetNextItem
// PURPOSE:  Set the pointer to the next item in the list, returning the previous value.
//
CVoiceTag *CVoiceTag::SetNextItem(CVoiceTag *pNextItem)
{
   MEMBERASSERT();
   CVoiceTag *rval = m_pNextItem;
   m_pNextItem = pNextItem;
   return rval;
}


//===============================================================================================
// FUNCTION: GetFileName
// PURPOSE:  Get the name of the temp file that this voice tag was saved in.
//
LPCSTR CVoiceTag::GetFileName()
{
   MEMBERASSERT();
   return m_szFileName;
}


//===============================================================================================
// FUNCTION: TransferFile
// PURPOSE:  Transfer the tag into an ABF file with no compression.
//
BOOL CVoiceTag::TransferFile(CFileIO &FIn, CFileIO &FOut)
{
   // Read data in 8k chunks.
   UINT uBlockSize = 8192U;
   CArrayPtr<char> pBuffer(uBlockSize);
   if (!pBuffer)
      return SetLastError(ABF_OUTOFMEMORY);
   
   for (UINT uSize=m_VTI.lUncompressedSize; uSize > 0; uSize -= uBlockSize)
   {
      if (uSize < uBlockSize)
         uBlockSize = uSize;
      if (!FIn.Read( pBuffer, uBlockSize ))
         return SetLastError(ABF_BADTEMPFILE);
      if (!FOut.Write( pBuffer, uBlockSize))
         return SetLastError(ABF_EDISKFULL);
   }
   m_VTI.lCompressedSize = m_VTI.lUncompressedSize;   
   return TRUE;
}

//===============================================================================================
// FUNCTION: PKErrorToABFError
// PURPOSE:  Converts the PKWARE error code to an ABF error code.
//
static int PKErrorToABFError(int nError)
{
   switch (nError)
   {
      case CMP_NO_MEMORY:
         return ABF_OUTOFMEMORY;
      case CMP_DISKFULL:
         return ABF_EDISKFULL;
      case CMP_DLL_NOT_FOUND:
         TRACE1("The DLL '%s' was not found\n", SZ_PKWARE_DLL);
         return ABF_ENOCOMPRESSION;
      case CMP_PROC_NOT_FOUND:
         TRACE1("A required entry point was not found in the DLL '%s'\n", SZ_PKWARE_DLL);
         return ABF_ENOCOMPRESSION;
      default:
         ERRORMSG1("Unexpected PKWARE error code %d\n", nError);
      case CMP_INVALID_DICTSIZE:
      case CMP_INVALID_MODE:
      case CMP_BAD_DATA:
      case CMP_ABORT:
      case CMP_MEMORY_ERROR:
         break;
   }
   return ABF_EREADTAG;
}

//===============================================================================================
// FUNCTION: PKWareCompressFile
// PURPOSE:  Transfer the tag into an ABF file with compression done by PKWARE.
//
BOOL CVoiceTag::PKWareCompressFile(CFileIO &FIn, CFileIO &FOut)
{
#ifdef _WINDOWS
   DWORD dwByteCount, dwCRC;
   int nError = 0;
   if (!PKWARE_CompressFile(FIn, FOut, &dwByteCount, &dwCRC, &nError))
      return SetLastError(PKErrorToABFError(nError));

   m_VTI.lCompressedSize = dwByteCount;
   m_VTI.dwCRC           = dwCRC;
   return TRUE;
#else
   return SetLastError(ABF_ENOCOMPRESSION);
#endif
}

   
//===============================================================================================
// FUNCTION: PKWareExpandFile
// PURPOSE:  Transfer the tag from an ABF file to an uncompressed file.
//
BOOL CVoiceTag::PKWareExpandFile(CFileIO &FIn, CFileIO &FOut)
{
#ifdef _WINDOWS
   DWORD dwCRC;
   int nError = 0;
   if (!PKWARE_ExpandFile(FIn, FOut, &dwCRC, &nError))
      return SetLastError(PKErrorToABFError(nError));
   if (m_VTI.dwCRC != dwCRC)
      return SetLastError(ABF_EBADCRC);
   return TRUE;
#else
   return SetLastError(ABF_ENOCOMPRESSION);
#endif
}

   
//===============================================================================================
// FUNCTION: WriteTag
// PURPOSE:  Writes out the tag to the passed file object.
//
BOOL CVoiceTag::WriteTag( CFileIO &FOut, UINT uTagCount, long lVoiceTagPtr )
{
   MEMBERASSERT();

   // Open the input file and set the file read pointer to the start of the data section.   
   CFileIO FIn;
   if (!FIn.Create(m_szFileName, TRUE))   
      return SetLastError(ABF_BADTEMPFILE);

   // Check that the input file actually contains some data. 
   // The expander does not like zero length compressed files.
   LONGLONG llOffset = 0;
   VERIFY(FIn.Seek( 0, FILE_END, &llOffset ));
   if (UINT(llOffset) == UINT(m_lDataOffset))
      m_VTI.nCompressionType = ABF_COMPRESSION_NONE;

   // Seek to the start of the data portion of the input file.
   VERIFY(FIn.Seek( m_lDataOffset, FILE_BEGIN ));
   
   // Set the output pointer to the end of the file.
   VERIFY(FOut.Seek( 0, FILE_END, &llOffset ));
   m_VTI.lFileOffset = UINT(llOffset);
   
   // Copy the data according to the chosen compression type.
   BOOL bSuccess;
   switch (m_VTI.nCompressionType)
   {
      case ABF_COMPRESSION_PKWARE:
         bSuccess = PKWareCompressFile( FIn, FOut );
         break;
      default:
         ERRORMSG1("Unexpected compression type '%d'.", m_VTI.nCompressionType);
      case ABF_COMPRESSION_NONE:
         bSuccess = TransferFile( FIn, FOut );
         break;
   }
      
   // Close the input file.
   FIn.Close();
   
   // If successful, update the catalog.
   if (bSuccess)
   {
      VERIFY(FOut.Seek( lVoiceTagPtr * ABF_BLOCKSIZE + uTagCount * sizeof(ABFVoiceTagInfo), FILE_BEGIN));
      VERIFY(FOut.Write( &m_VTI, sizeof(m_VTI) ));
   }
   DeleteFile(m_szFileName);
   return bSuccess;
}


//===============================================================================================
// FUNCTION: ReadTag
// PURPOSE:  Reads a tag out of an ABF file into another file.
//
BOOL CVoiceTag::ReadTag( CFileIO &FIn )
{
   MEMBERASSERT();

   // Open the output file and set the file read pointer to the start of the data section.   
   CFileIO FOut;
   if (!FOut.Create(m_szFileName, FALSE))   
      return SetLastError(ABF_BADTEMPFILE);
   VERIFY(FOut.Seek( m_lDataOffset, FILE_BEGIN ));
   
   // Copy the data according to the chosen compression type.
   BOOL bSuccess;
   switch (m_VTI.nCompressionType)
   {
      case ABF_COMPRESSION_PKWARE:
         bSuccess = PKWareExpandFile( FIn, FOut );
         break;
      default:
         ERRORMSG1("Unexpected compression type '%d'.", m_VTI.nCompressionType);
      case ABF_COMPRESSION_NONE:
         bSuccess = TransferFile( FIn, FOut );
         break;
   }
      
   // Close the output file.
   FOut.Close();
   if (!bSuccess)
      DeleteFile(m_szFileName);
      
   return bSuccess;
}


//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error value and always returns FALSE for convenience.
//
BOOL CVoiceTag::SetLastError(int nError)
{
   MEMBERASSERT();
   m_nLastError = nError;
   return FALSE;           // convenience.
}


//===============================================================================================
// FUNCTION: SetErrorCallback
// PURPOSE:  Sets a function to be called if a file I/O error occurs while writing to disk.
//
int CVoiceTag::GetLastError()
{
   MEMBERASSERT();
   return m_nLastError;
}


//************************************************************************************************
// Member functions for CVoiceTagList
//

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CVoiceTagList::CVoiceTagList()
{
   MEMBERASSERT();
   m_pList = NULL;
   m_uItemCount = 0;
   m_nLastError = 0;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CVoiceTagList::~CVoiceTagList()
{
   MEMBERASSERT();
   EmptyList();
   m_pList = NULL;
}
   
//===============================================================================================
// FUNCTION: AddToTail
// PURPOSE:  Adds a new item to the end of the list.
//
void CVoiceTagList::Put(CVoiceTag *pItem)
{
   MEMBERASSERT();
   pItem->SetNextItem(NULL);
   if (m_pList == NULL)
      m_pList = pItem;
   else
   {
      CVoiceTag *p = m_pList;
      while(p->GetNextItem())
         p = p->GetNextItem();
      p->SetNextItem(pItem);
   }
   m_uItemCount++;
}


//===============================================================================================
// FUNCTION: RemoveHead
// PURPOSE:  Unlinks the item at the head of the list and returns it.
//
CVoiceTag *CVoiceTagList::RemoveHead()
{
   MEMBERASSERT();
   CVoiceTag *rval = m_pList;
   if (rval)
   {
      m_pList = rval->SetNextItem(NULL);
      m_uItemCount--;
   }
   return rval;
}


//===============================================================================================
// FUNCTION: EmptyList
// PURPOSE:  Removes and deletes all items from the list.
//
void CVoiceTagList::EmptyList()
{
   MEMBERASSERT();
   while (m_pList)
      delete RemoveHead();
   m_uItemCount = 0;
}


//===============================================================================================
// FUNCTION: GetCount
// PURPOSE:  Retunrs the current count of items in the list.
//
UINT CVoiceTagList::GetCount()
{
   MEMBERASSERT();
   return m_uItemCount;
}


//===============================================================================================
// FUNCTION: Write
// PURPOSE:  Save the voice tags into the ABF file.
//
BOOL CVoiceTagList::Write(CFileIO &F, long lVoiceTagPtr, CABFNotify *pNotify)
{
   MEMBERASSERT();
   WPTRASSERT(pNotify);

   // Show the progress dialog and initialize the return value.
   pNotify->Notify(ABF_NVOICETAGSTART);
   BOOL bRval = FALSE;
   UINT uTagCount = 0;

   // Build the catalogue first...
   ABFVoiceTagInfo VTI;
   memset(&VTI, 0, sizeof(VTI));
   for (UINT i=0; i<GetCount(); i++)      
      if (!F.Write(&VTI, sizeof(VTI)))
         goto Cleanup;
   
   // Write out the tags, updating the catalogue as we go.
   while (m_pList)
   {
      CVoiceTag *pItem = RemoveHead(); 
      BOOL bOK = pItem->WriteTag( F, uTagCount++, lVoiceTagPtr );
      delete pItem;

      if (!bOK)
      {
         EmptyList();
         goto Cleanup;
      }
      pNotify->Notify(ABF_NWRITEVOICETAG);
   }
   bRval = TRUE;

Cleanup:   // Shutdown the progress dialog and return.
   pNotify->Notify(ABF_NVOICETAGEND);
   return bRval;
}


//===============================================================================================
// FUNCTION: SetLastError
// PURPOSE:  Sets the last error value and always returns FALSE for convenience.
//
BOOL CVoiceTagList::SetLastError(int nError)
{
   MEMBERASSERT();
   m_nLastError = nError;
   return FALSE;           // convenience.
}


//===============================================================================================
// FUNCTION: SetErrorCallback
// PURPOSE:  Sets a function to be called if a file I/O error occurs while writing to disk.
//
int CVoiceTagList::GetLastError()
{
   MEMBERASSERT();
   return m_nLastError;
}


