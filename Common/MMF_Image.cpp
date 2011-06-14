//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  MMF_Image.cpp
// PURPOSE: Class to wrap a block of memory stored as a Memory Mapped File.
// AUTHOR:  BHI  May 1998
//
#include "wincpp.hpp"
#include "MMF_Image.hpp"

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CMMF_Image::CMMF_Image()
{
   MEMBERASSERT();
   // The documentation says to use the OS page size, but I have found this to fail on large images.
   // A page size of 64k seems to work better.
   SYSTEM_INFO SysInfo = { 0 };
   GetSystemInfo(&SysInfo);
   m_uPageSize     = SysInfo.dwPageSize;     // The page size used by the operating system.
   m_uPageSize     = 64 * 1024;              // Override to use 64k pages.

   m_uFileSize     = 0;
   m_uMappedOffset = 0;                       // The base offset of the currently mapped region.
   m_uMappedBytes  = 0;                       // The number of bytes spanned by the currently mapped region.

#ifndef MMF_IMAGE_USE_HEAP_MEMORY
   m_hFileMap      = NULL;
   m_pImage        = NULL;                   // Pointer to the currently mapped portion.
#endif
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CMMF_Image::~CMMF_Image()
{
   MEMBERASSERT();
   Close();
}

//===============================================================================================
// FUNCTION: Resize
// PURPOSE:  Resizes the MMF file.
//
BOOL CMMF_Image::Resize(UINT uBytes, BOOL bZero)
{
   MEMBERASSERT();
   Close();

   if (uBytes==0)
      return TRUE;

#ifdef MMF_IMAGE_USE_HEAP_MEMORY
   m_pImage.Alloc(uBytes);                   // Image in memory (for debugging)
#else
   char szFilename[_MAX_PATH];
   if (!AXU_GetTempFileName("MMF", 0, szFilename))
      return FALSE;

   if (!m_File.Create(szFilename, FALSE, FILE_FLAG_DELETE_ON_CLOSE))
   {
      DeleteFile(szFilename);
      return FALSE;
   }

   // Create an in-memory memory mapped file of the size of the template object.
   HANDLE hBackingFile = m_File.GetFileHandle();   // HANDLE(0xFFFFFFFF)
   m_hFileMap = CreateFileMapping(hBackingFile, NULL, PAGE_READWRITE, 0, uBytes, NULL);
   if (m_hFileMap == NULL)
   {
      m_File.Close();
      SHOW_SYSTEM_ERROR(0);
      ERRORMSG("CreateFileMapping() failed.");
      return FALSE;
   }
#endif
   m_uFileSize = uBytes;

   if (bZero)
   {
      void *pImage = GetPtr();
      if (pImage)
         memset(pImage, 0, uBytes);
      ReleasePtr();
   }
   return TRUE;
}

//===============================================================================================
// FUNCTION: Attach
// PURPOSE:  Attaches the MMF_Image to an existing file.
//
BOOL CMMF_Image::Attach(LPCSTR pszFilename)
{
   MEMBERASSERT();
   Close();

#ifndef MMF_IMAGE_USE_HEAP_MEMORY
   ASSERT((GetFileAttributes(pszFilename) & FILE_ATTRIBUTE_READONLY)==0);

   // Open the file with read and write access.
   if (!m_File.CreateEx(pszFilename, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL))
      return FALSE;

   // Create an in-memory memory mapped file of the size of the template object.
   m_uFileSize = (UINT)m_File.GetFileSize();

   // Create an in-memory memory mapped file of the size of the template object.
   HANDLE hBackingFile = m_File.GetFileHandle();   // HANDLE(0xFFFFFFFF)

   m_hFileMap  = CreateFileMapping(hBackingFile, NULL, PAGE_READWRITE, 0, m_uFileSize, NULL);
   if (m_hFileMap == NULL)
   {
      SHOW_SYSTEM_ERROR(0);
      ERRORMSG("CreateFileMapping() failed.");
      Close();
      return FALSE;
   }

#endif
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetPtr
// PURPOSE:  Maps a region and returns a pointer to it.
//
void *CMMF_Image::GetPtr(UINT uOffset, UINT uBytes)
{
   MEMBERASSERT();

   if (uBytes==0)    // If all file is wanted, make sure the offset is zero.
   {
      ASSERT(uOffset==0);
      uBytes = GetSize();
   }

#ifdef MMF_IMAGE_USE_HEAP_MEMORY
   return m_pImage.Get() + uOffset;
#else
   // The memory map file should be open at this stage.
   ASSERT(m_hFileMap != NULL);
   if (m_hFileMap == NULL)
      return NULL;

   UINT uFirstBit = uOffset % m_uPageSize;
   uOffset       -= uFirstBit;
   uBytes        += uFirstBit;

   // Round up to the nearest multiple of pages.
   if (uBytes % m_uPageSize)
   {
      uBytes += m_uPageSize - (uBytes % m_uPageSize);
      if (uOffset + uBytes > m_uFileSize)
         uBytes = m_uFileSize - uOffset;
   }

   // Map in the region if it is not already mapped.
   if ((m_uMappedOffset!=uOffset) || (m_uMappedBytes!=uBytes))
   {
      // Release the currently held region.
      ReleasePtr();

      // Map a view of the file into the address space.
      m_pImage = MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, uOffset, uBytes);
      if (!m_pImage)
      {
         SHOW_SYSTEM_ERROR(0);
         ERRORMSG("MapViewOfFile() failed.");
         return NULL;
      }
      m_uMappedOffset = uOffset;
      m_uMappedBytes  = uBytes;
   }

   return LPBYTE(m_pImage) + uFirstBit;
#endif
}

//===============================================================================================
// FUNCTION: ReleasePtr
// PURPOSE:  Releases a mapped region.
//
void CMMF_Image::ReleasePtr()
{
   MEMBERASSERT();

#ifndef MMF_IMAGE_USE_HEAP_MEMORY
   // Map a view of the file into the address space.
   if (m_pImage)
   {
      UnmapViewOfFile(m_pImage);
      m_pImage = NULL;
   }
#endif
   m_uMappedOffset = 0;                   // The base offset of the currently mapped region.
   m_uMappedBytes  = 0;                   // The number of bytes spanned by the currently mapped region.
}

//===============================================================================================
// FUNCTION: Close
// PURPOSE:  Unmap any mapped portions and close the memory mapped file.
//
void CMMF_Image::Close()
{
   MEMBERASSERT();
   ReleasePtr();

#ifdef MMF_IMAGE_USE_HEAP_MEMORY
   m_pImage.Free();
#else
   if (m_hFileMap)
   {
      CloseHandle(m_hFileMap);
      m_hFileMap = NULL;
   }
   m_File.Close();
#endif
   m_uFileSize = 0;
}

//===============================================================================================
// FUNCTION: GetSize
// PURPOSE:  Return the total size of the MMF file.
//
UINT CMMF_Image::GetSize() const
{
   MEMBERASSERT();
   return m_uFileSize;
}
