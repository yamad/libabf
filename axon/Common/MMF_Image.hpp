//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  MMF_Image.hpp
// PURPOSE: Class to wrap a block of memory stored as a Memory Mapped File.
// AUTHOR:  BHI  May 1998
//
#ifndef INC_MMF_IMAGE_HPP
#define INC_MMF_IMAGE_HPP

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Uncomment the following #define to use heap memory instead of a MMF (debugging).
// #define MMF_IMAGE_USE_HEAP_MEMORY

#ifdef MMF_IMAGE_USE_HEAP_MEMORY
   #include "ArrayPtr.hpp"
#else
   #include "FileIO.hpp"
#endif

class CMMF_Image
{
private:
   UINT              m_uFileSize;            // The size of the file mapping region.
   UINT              m_uPageSize;            // The page size used by the operating system.
   UINT              m_uMappedOffset;        // The base offset of the currently mapped region.
   UINT              m_uMappedBytes;         // Bytes spanned by the currently mapped region.

#ifdef MMF_IMAGE_USE_HEAP_MEMORY
   CArrayPtr<BYTE>   m_pImage;               // Image in memory (for debugging)
#else
   CFileIO           m_File;                 // Handle to the backing file.
   HANDLE            m_hFileMap;             // Handle of the memory mapped file.
   void             *m_pImage;               // Pointer to the currently mapped portion.
#endif

private:
   // Unimplemented constructors etc.
   CMMF_Image(const CMMF_Image &rhs);
   const CMMF_Image& operator=(const CMMF_Image& rhs);

public:
   // Constructor / Destructor
   CMMF_Image();
   ~CMMF_Image();

   // Resize the MMF and optionally zero the contents.
   BOOL Resize(UINT uBytes, BOOL bZero);

   // Attaches an MMF to an existing file.
   BOOL Attach(LPCSTR pszFilename);

   // Map a region of the MMF into RAM.
   void *GetPtr(UINT uOffset=0, UINT uBytes=0);

   // Release a mapped region.
   void ReleasePtr();

   // Get the size of the MMF file.
   UINT GetSize() const;

   // Release any mapping and close the MMF.
   void Close();
};


#endif   // INC_CMMF_IMAGE_HPP
