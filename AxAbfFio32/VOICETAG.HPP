//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// HEADER:  VOICETAG.HPP
// PURPOSE: Contains class definition for the voice tag tracking structure.
// AUTHOR:  BHI  Nov 1995
//

#ifndef INC_VOICETAG_HPP
#define INC_VOICETAG_HPP

#include "abffiles.h"
#include "..\common\FileIO.hpp"
#include "notify.hpp"

//***********************************************************************************************
// CLASS: CVoiceTag
//
class CVoiceTag
{
private:
   char            m_szFileName[_MAX_PATH];
   long            m_lDataOffset;
   ABFVoiceTagInfo m_VTI;
   int             m_nLastError;
   CVoiceTag      *m_pNextItem;

private:
   // Declare but don't implement the default constructor and the copy constructor.
   CVoiceTag();
   CVoiceTag(const CVoiceTag &DP);
   const CVoiceTag &operator=(const CVoiceTag &DP);
           
private:           
   BOOL TransferFile(CFileIO &FOut, CFileIO &FIn);
   BOOL PKWareCompressFile(CFileIO &FOut, CFileIO &FIn);
   BOOL PKWareExpandFile(CFileIO &FOut, CFileIO &FIn);
   
public:
   CVoiceTag(LPCSTR pszFileName, long lDataOffset, ABFVoiceTagInfo *pVTI);
   ~CVoiceTag();      

   CVoiceTag *SetNextItem(CVoiceTag *pNextItem);
   CVoiceTag *GetNextItem();
   LPCSTR GetFileName();
   BOOL WriteTag( CFileIO &F, UINT uTagCount, long lVoiceTagPtr );
   BOOL ReadTag( CFileIO &F );

   BOOL SetLastError(int nError);
   int  GetLastError();
};
   

//===============================================================================================
// FUNCTION: GetNextItem
// PURPOSE:  Returns the pointer to the next item in the list.
//
inline CVoiceTag *CVoiceTag::GetNextItem()
{
   MEMBERASSERT();
   return m_pNextItem;
}


//***********************************************************************************************
// CLASS: CVoiceTagList
//
class CVoiceTagList
{
private:
   CVoiceTag *m_pList;
   UINT       m_uItemCount;
   int        m_nLastError;

private:
   CVoiceTag *RemoveHead();
   void       EmptyList();
   
public:
   CVoiceTagList();
   ~CVoiceTagList();

   void Put(CVoiceTag *pDP);
   UINT GetCount();
   BOOL Write(CFileIO &F, long lVoiceTagPtr, CABFNotify *pNotify);
   
   BOOL SetLastError(int nError);
   int  GetLastError();
};

#endif      // __VOICETAG_HPP__
