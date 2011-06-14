//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
// MODULE:  PKWARE.CPP
// PURPOSE: Contains the class wrapper for operating the PKWARE compression library.
// AUTHOR:  BHI  Sep 95
//

#include "wincpp.hpp"
#include "pkware.h"
#include "..\common\ArrayPtr.hpp"

// Comment out the following line to statically link to the PKWare DLL.
// (Dont forget to add the .LIB into the project)
#define DYNAMIC_LOAD_DLL   

#ifdef DYNAMIC_LOAD_DLL   

static HINSTANCE s_hDLL = NULL;

#else    // !DYNAMIC_LOAD_DLL   
   // Include the appropriate header file if we are statically linking to the DLL.
   #include /* no dependancy */ "\AxonDev\3rdParty\pkware\win32\implode.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================================
// Definitions for the functions exported by the PKWARE DLLs.
//
typedef UINT (*FN_READ_CALLBACK)(char *buf, UINT *puSize, void *param);
typedef void (*FN_WRITE_CALLBACK)(char *buf, UINT *puSize, void *param);

typedef UINT (*FN_IMPLODE)(FN_READ_CALLBACK fnRead, FN_WRITE_CALLBACK fnWrite,
                           char *work_buf, void *param, UINT *type, UINT *dsize);
typedef UINT (*FN_EXPLODE)(FN_READ_CALLBACK fnRead, FN_WRITE_CALLBACK fnWrite,
                           char *work_buf, void *param);
typedef DWORD (*FN_CRC32)(LPSTR buffer, UINT *psize, LPDWORD old_crc);

#define CMP_BUFFER_SIZE    36312
#define EXP_BUFFER_SIZE    12596

#ifdef __cplusplus
}
#endif

//==============================================================================================
// Static variables for the function pointers loaded from the DLL.
//
static FN_IMPLODE s_fnImplode = NULL;
static FN_EXPLODE s_fnExplode = NULL;
static FN_CRC32   s_fnCRC32   = NULL;

//==============================================================================================
// FUNCTION: UnloadPkWareDLL.
// PURPOSE:  Unload the PKWare DLL from memory and NULL out the function pointers.
//
static void UnloadPkWareDLL()
{
#ifdef DYNAMIC_LOAD_DLL   
   if (s_hDLL)
   {
      FreeLibrary(s_hDLL);
      s_hDLL = NULL;
   }
#endif
   s_fnImplode = NULL;
   s_fnExplode = NULL;
   s_fnCRC32   = NULL;
}


//==============================================================================================
// FUNCTION: LoadPkWareDLL.
// PURPOSE:  Load the PKWare DLL into memory and set up the function pointers.
//
static BOOL LoadPkWareDLL(int *pnError)
{
#ifdef DYNAMIC_LOAD_DLL   
   s_hDLL = LoadLibrary(SZ_PKWARE_DLL);
   if (s_hDLL < (HINSTANCE)HINSTANCE_ERROR)
   {
      if (pnError)
         *pnError = CMP_DLL_NOT_FOUND;
      s_hDLL = NULL;
      return FALSE;
   }
      
   s_fnImplode = (FN_IMPLODE)GetProcAddress(s_hDLL, "implode");
   s_fnExplode = (FN_EXPLODE)GetProcAddress(s_hDLL, "explode");
   s_fnCRC32   = (FN_CRC32)GetProcAddress(s_hDLL, "crc32");
   
   if (!s_fnImplode || !s_fnExplode || !s_fnCRC32)
   {
      if (pnError)
         *pnError = CMP_PROC_NOT_FOUND;
      UnloadPkWareDLL();
      return FALSE;
   }
#else
   s_fnImplode = (FN_IMPLODE)implode;
   s_fnExplode = (FN_EXPLODE)explode;
   s_fnCRC32   = (FN_CRC32)crc32;
#endif
   return TRUE;
}

//*****************************************************************************************
// CLASS:   CFileObject
// PURPOSE: Class for doing data compression from file to file.
//
class CFileObject
{
protected:    // Member data.
   BOOL     m_bDoCRC;      // TRUE = do the Cyclic Redundancy Check.
   DWORD    m_dwCRC;       // The CRC calculated.
   CFileIO &m_File;
   DWORD    m_dwBytesProcessed;

private:    // Unimplemented member functions.
   CFileObject();
   
   // Prevent the use of the default copy constructor and operator=().
   CFileObject(const CFileObject &C);
   const CFileObject &operator=(const CFileObject &F);

   void  DoCRC(LPSTR lpszBuffer, UINT *puSize);
   
public:     // Public member functions.
   CFileObject(CFileIO &F);
   ~CFileObject();

   void  SetCRCFlag(BOOL bDoCRC);
   BOOL  GetCRCFlag();
   DWORD GetCRC();
   
   UINT  ReadData(LPSTR lpszBuffer, UINT *puSize);
   BOOL  WriteData(LPSTR lpszBuffer, UINT *puSize);
   DWORD GetByteCount();
};


//==============================================================================================
// FUNCTION: CFileObject.
// PURPOSE:  Constructor for CFileObject object.
//
CFileObject::CFileObject(CFileIO &F)
   : m_File(F)
{
   MEMBERASSERT();
   m_bDoCRC = FALSE;        // TRUE = do the Cyclic Redundancy Check.
   m_dwCRC  = DWORD(-1);    // The CRC calculated.
   m_dwBytesProcessed = 0;
}


//==============================================================================================
// FUNCTION: ~CFileObject.
// PURPOSE:  Destructor for CFileObject object.
//
CFileObject::~CFileObject()
{
   MEMBERASSERT();
}


//==============================================================================================
// FUNCTION: ReadData
// PURPOSE:  Read the data from the file.
//
UINT CFileObject::ReadData(LPSTR lpszBuffer, UINT *puSize)
{
   MEMBERASSERT();
   UINT uBytes = *puSize;
   DWORD dwDataRead;
   
   // Do the read.
   m_File.Read( lpszBuffer, uBytes, &dwDataRead );
   uBytes = UINT(dwDataRead);
   
   *puSize = uBytes;
   if (m_bDoCRC && (uBytes > 0))
      DoCRC(lpszBuffer, puSize);

   m_dwBytesProcessed += uBytes;      
   return uBytes;
}


//==============================================================================================
// FUNCTION: WriteData
// PURPOSE:  Write the data to the file.
//
BOOL CFileObject::WriteData(LPSTR lpszBuffer, UINT *puSize)
{
   MEMBERASSERT();
   UINT uBytes = *puSize;
   
   if (!m_File.Write( lpszBuffer, uBytes ))
      return FALSE;
   
   *puSize = uBytes;
   if (m_bDoCRC && (uBytes > 0))
      DoCRC(lpszBuffer, puSize);
      
   m_dwBytesProcessed += uBytes;      
   return TRUE;
}

//==============================================================================================
// FUNCTION: GetByteCount
// PURPOSE:  Return the amount of data processed by this object.
//
DWORD CFileObject::GetByteCount()
{
   MEMBERASSERT();
   return m_dwBytesProcessed;
}


//==============================================================================================
// FUNCTION: SetCRCFlag
// PURPOSE:  Sets the state of the CRC calculation flag. TRUE = Calculate CRC.
//
void CFileObject::SetCRCFlag(BOOL bDoCRC)
{
   MEMBERASSERT();
   m_bDoCRC = bDoCRC;
   m_dwCRC = DWORD(-1);
}


//==============================================================================================
// FUNCTION: GetCRCFlag
// PURPOSE:  Returns the state of the CRC calculation flag. TRUE = Calculate CRC.
//
BOOL CFileObject::GetCRCFlag()
{
   MEMBERASSERT();
   return m_bDoCRC;
}


//==============================================================================================
// FUNCTION: DoCRC
// PURPOSE:  Calculates the CRC value.
//
void CFileObject::DoCRC(LPSTR lpszBuffer, UINT *puSize)
{
   m_dwCRC = s_fnCRC32(lpszBuffer, puSize, &m_dwCRC);
}


//==============================================================================================
// FUNCTION: GetCRC
// PURPOSE:  Returns the CRC value calculated.
//
DWORD CFileObject::GetCRC()
{
   MEMBERASSERT();
   if (!m_bDoCRC)
      return 0;
   return ~m_dwCRC;
}


typedef BOOL (CALLBACK *CMPCallback)(void *pvThisPointer, UINT uError);

//*****************************************************************************************
// CLASS:   CZipEngine
// PURPOSE: Class for doing data compression from file to file.
//
class CZipEngine
{
public:     // Public enumerations.
   enum CMP_WINDOW
   {
      Small=1024, Medium=2048, Large=4096
   };

   enum CMP_DATATYPE
   {
      Binary=CMP_BINARY, Ascii=CMP_ASCII
   };

private:    // Member data.
   CMP_WINDOW     m_eWindowSize;    // Size of the compression window.
   CMP_DATATYPE   m_eDataType;      // Type of data compression.
   CFileObject    m_Input;          // Input data object
   CFileObject    m_Output;         // Output data object
                                    
   CMPCallback    m_fnCallback;     // Function pointer for an error callback function.
   void          *m_pvThisPointer;  // Context pointer to be passed when the callback is called.
   
   UINT           m_uLastError;     // Holder for the last error value.

private:    // Unimplemented member functions.
   CZipEngine();
   // Prevent the use of the default copy constructor and operator=().
   CZipEngine(const CZipEngine &);
   const CZipEngine &operator=(const CZipEngine &);

private:    // Internal member functions.
public:
   UINT ReadData(LPSTR lpszBuffer, UINT *puSize);
   void WriteData(LPSTR lpszBuffer, UINT *puSize);

public:     // Public member functions.
   CZipEngine(CFileIO &Input, CFileIO &Output);
   ~CZipEngine();

   void SetWindowSize(CMP_WINDOW eWindowSize);
   void SetDataType(CMP_DATATYPE eDataType);
   
   BOOL Implode(DWORD *pdwByteCount, DWORD *pdwCRC);
   BOOL Explode(DWORD *pdwCRC);

   BOOL SetErrorCallback(CMPCallback fnCallback, void *pvThisPointer);
   BOOL SetLastError(UINT uError);
   UINT GetLastError();
};


//==============================================================================================
// FUNCTION: CZipEngine.
// PURPOSE:  Constructor for CZipEngine object.
//
CZipEngine::CZipEngine(CFileIO &Input, CFileIO &Output)
   : m_Input(Input), m_Output(Output)
{
   MEMBERASSERT();
   m_eWindowSize   = Medium;       // Size of the compression window.
   m_eDataType     = Binary;       // Type of data compression.
   m_fnCallback    = NULL;
   m_pvThisPointer = NULL;
   m_uLastError    = 0;            // Holder for the last error value.
}

//==============================================================================================
// FUNCTION: ~CZipEngine.
// PURPOSE:  Destructor for CZipEngine object.
//
CZipEngine::~CZipEngine()
{
   MEMBERASSERT();
   m_fnCallback    = NULL;
   m_pvThisPointer = NULL;
}

//==============================================================================================
// FUNCTION: ReadData
// PURPOSE:  Read the data from the file.
//
UINT CZipEngine::ReadData(LPSTR lpszBuffer, UINT *puSize)
{
   MEMBERASSERT();

   // Abort if an error occurred.
   if (m_uLastError)
      return 0;
      
   // If the callback function returns false, return zero to stop the process.      
   if (m_fnCallback)
   {
      // Call the callback function. If the callback returns TRUE, continue.
      FNPTRASSERT(m_fnCallback);
      if (!m_fnCallback(m_pvThisPointer, CMP_ABORT))
         return 0;
   }
         
   return m_Input.ReadData(lpszBuffer, puSize);
}


//==============================================================================================
// FUNCTION: WriteData
// PURPOSE:  Write the data to the file.
//
void CZipEngine::WriteData(LPSTR lpszBuffer, UINT *puSize)
{
   MEMBERASSERT();

   // If the write fails, call the callback to allow the user to free up disk space.
   while (!m_Output.WriteData(lpszBuffer, puSize))
   {
      // If no callback function set the size field to zero to stop the process and return.      
      if (!m_fnCallback)
      {
         *puSize = 0;
         m_uLastError = CMP_DISKFULL;
         return;
      }
         
      // Call the callback function. If the callback returns TRUE, go 'round again.
      FNPTRASSERT(m_fnCallback);
      if (!m_fnCallback(m_pvThisPointer, CMP_DISKFULL))
      {
         *puSize = 0;
         return;
      }
   }
}

//==============================================================================================
// FUNCTION: _ReadData
// PURPOSE:  Static function that maps through to the calls.
//
UINT cdecl _ReadData(LPSTR lpszBuffer, UINT *puSize, void *ThisPointer)
{
   // Static function -- no MEMBERASSERT();
   CZipEngine *This = (CZipEngine *)ThisPointer;
   WPTRASSERT(This);
   return This->ReadData(lpszBuffer, puSize);
}

//==============================================================================================
// FUNCTION: _WriteData
// PURPOSE:  Static function that maps through to the object call.
//
void cdecl _WriteData(LPSTR lpszBuffer, UINT *puSize, void *ThisPointer)
{
   // Static function -- no MEMBERASSERT();
   CZipEngine *This = (CZipEngine *)ThisPointer;
   WPTRASSERT(This);
   This->WriteData(lpszBuffer, puSize);
}

//==============================================================================================
// FUNCTION: Implode
// PURPOSE:  Run the compression process.
//
BOOL CZipEngine::Implode(DWORD *pdwByteCount, DWORD *pdwCRC)
{
   MEMBERASSERT();
   m_Input.SetCRCFlag(TRUE);
   m_Output.SetCRCFlag(FALSE);

   CArrayPtr<char> pWorkBuffer(CMP_BUFFER_SIZE);
   if (!pWorkBuffer)
      return SetLastError(CMP_NO_MEMORY);
   
   UINT uDataType   = UINT(m_eDataType);
   UINT uWindowSize = UINT(m_eWindowSize);

   m_uLastError = s_fnImplode(_ReadData, _WriteData, pWorkBuffer, this, &uDataType, &uWindowSize );
   
   if (pdwByteCount)
      *pdwByteCount = m_Output.GetByteCount();
   if (pdwCRC)
      *pdwCRC = m_Input.GetCRC();   
   return (m_uLastError == 0);
}


//==============================================================================================
// FUNCTION: Explode
// PURPOSE:  Run the decompression process.
//
BOOL CZipEngine::Explode(DWORD *pdwCRC)
{
   MEMBERASSERT();
   m_Input.SetCRCFlag(FALSE);
   m_Output.SetCRCFlag(TRUE);
   
   CArrayPtr<char> pWorkBuffer(EXP_BUFFER_SIZE);
   if (!pWorkBuffer)
      return SetLastError(CMP_NO_MEMORY);

   m_uLastError = s_fnExplode(_ReadData, _WriteData, pWorkBuffer, this );

   if (pdwCRC)
      *pdwCRC = m_Output.GetCRC();
   return (m_uLastError == 0);
}


//===============================================================================================
// FUNCTION: SetErrorCallback
// PURPOSE:  Sets a function to be called if a file I/O error occurs while writing to disk.
//
BOOL CZipEngine::SetErrorCallback(CMPCallback fnCallback, void *pvThisPointer)
{
   MEMBERASSERT();
   if (fnCallback)
   {
      FNPTRASSERT(fnCallback);
      m_fnCallback = fnCallback;
   }
   if (pvThisPointer)
   {
      WPTRASSERT((char *)pvThisPointer);
      m_pvThisPointer = pvThisPointer;
   }
   return TRUE;
}


//==============================================================================================
// FUNCTION: SetWindowSize
// PURPOSE:  Set the window size for data compression.
//
void CZipEngine::SetWindowSize(CMP_WINDOW eWindowSize)
{
   MEMBERASSERT();
   m_eWindowSize = eWindowSize;
}

//==============================================================================================
// FUNCTION: SetDataType
// PURPOSE:  Set the data type to be compressed.
//
void CZipEngine::SetDataType(CMP_DATATYPE eDataType)
{
   MEMBERASSERT();
   m_eDataType = eDataType;
}

//==============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  returns the last error value.
//
BOOL CZipEngine::SetLastError(UINT uError)
{
   MEMBERASSERT();
   m_uLastError = uError;
   return FALSE;
}


//==============================================================================================
// FUNCTION: GetLastError
// PURPOSE:  returns the last error value.
//
UINT CZipEngine::GetLastError()
{
   MEMBERASSERT();
   return m_uLastError;
}

//###############################################################################################
//###############################################################################################
//###
//###    External entry points.
//###
//###############################################################################################
//###############################################################################################


//===============================================================================================
// FUNCTION: PKWARE_CompressFile
// PURPOSE:  Compress the data in the input file into the current position of the output file.
//
BOOL PKWARE_CompressFile(CFileIO &FIn, CFileIO &FOut, DWORD *pdwByteCount, DWORD *pdwCRC, int *pnError)
{
   if (!LoadPkWareDLL(pnError))
      return FALSE;
   CZipEngine CompressionEngine(FIn, FOut);
   BOOL bRval = CompressionEngine.Implode(pdwByteCount, pdwCRC);
   UnloadPkWareDLL();
   if (!bRval && pnError)
      *pnError = CompressionEngine.GetLastError();
   return bRval;
}
   
//===============================================================================================
// FUNCTION: PKWARE_ExpandFile
// PURPOSE:  Expand the data at the current position of the input file to the current position
//           of the output file.
//
BOOL PKWARE_ExpandFile(CFileIO &FIn, CFileIO &FOut, DWORD *pdwCRC, int *pnError)
{
   if (!LoadPkWareDLL(pnError))
      return FALSE;
   CZipEngine CompressionEngine(FIn, FOut);
   BOOL bRval = CompressionEngine.Explode(pdwCRC);
   UnloadPkWareDLL();
   if (!bRval && pnError)
      *pnError = CompressionEngine.GetLastError();
   return bRval;
}

   
//#define TESTBED
#ifdef TESTBED

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hInstPrevious, LPSTR lpCmdLine, int nCmdShow)
{
   CFile Input, Output;
   if (!Input.OpenFile("TEST.TXT", TRUE))
   {
      printf("Couldn't open input file.\n");
      return 0;
   }

   if (!Output.OpenFile("TEST.CMP", FALSE))
   {
      printf("Couldn't open output file.\n");
      return 0;
   }

   printf("Imploding...\n");
   DWORD dwCRC1, dwCRC2, dwByteCount;
   if (!PKWARE_CompressFile(Input, Output, &dwByteCount, &dwCRC1))
   {
      printf("Error during implosion.\n");
      return 0;
   }
   
   Input.CloseFile();
   Output.CloseFile();
         
   if (!Input.OpenFile("TEST.CMP", TRUE))
   {
      printf("Couldn't open input file.\n");
      return 0;
   }

   if (!Output.OpenFile("TEST.OUT", FALSE))
   {
      printf("Couldn't open output file.\n");
      return 0;
   }
   
   printf("Exploding...\n");
   if (!PKWARE_ExpandFile(Input, Output, &dwCRC2)
      printf("Error during explosion.\n");
      
   Input.CloseFile();
   Output.CloseFile();
   printf("Completed OK.\nCRC1 = 0x%lX  CRC2 = 0x%lX\n", dwCRC1, dwCRC2);
   return 0;
}

#endif
