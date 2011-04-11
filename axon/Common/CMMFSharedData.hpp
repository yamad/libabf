//***********************************************************************************************
//
//    Copyright (c) 1996 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  CMMFSharedData.hpp
// PURPOSE: Class declaration/implementation for Memory Mapped shared data.
// AUTHOR:  BHI  Aug 1996
//
#ifndef INC_CMMFSharedData_hpp
#define INC_CMMFSharedData_hpp

//###############################################################################################
//###############################################################################################
//###
//###    CLASS:   CMMFData
//###    PURPOSE: This class acts as a pseudo pointer to a simple data type or a 
//###             block of memory that is to be shared between multiple processes.
//###    NOTES:   The shared data will be zeroed when it is first initialized.
//###
//###############################################################################################
//###############################################################################################

template<class T>
class CMMFData
{
private:
   HANDLE m_hFileMap;      // Handle of the memory mapped file.
   T*     m_pObject;       // Pointer to the shared data area.
   BOOL   m_bFirstMapping; // if TRUE after construction, this is the first mapping

private:                   // Unimplemented constructors etc.
   CMMFData();
   CMMFData(const CMMFData &rhs);
   const CMMFData& operator=(const CMMFData& rhs);

public:
   explicit CMMFData(LPCSTR szMapName);   // Constructor -- must take mapping name.
   ~CMMFData();                           // Destructor.

   T* GetPtr() const;      // Return internal pointer.
   T& operator*() const;   // overloaded operator* for accessing internal object
   operator void*() const; // overloaded operator void* for testing successful creation.
   BOOL IsFirstMapping() const;  // Returns TRUE if this instance is the first on this mapping.
};

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
template<class T>
inline CMMFData<T>::CMMFData(LPCSTR szMapName)
{
   MEMBERASSERT();

   // Create an in-memory memory mapped file of the size of the template object.
   m_bFirstMapping = TRUE;
   m_pObject       = NULL;
   m_hFileMap      = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 
                                       0, sizeof(T), szMapName);
   if (!m_hFileMap)
      return;

   // File mapping created successfully, check if it is the first copy.
   m_bFirstMapping = (GetLastError() != ERROR_ALREADY_EXISTS);

   // Map a view of the file into the address space.
   m_pObject = (T*)MapViewOfFile(m_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
   if (!m_pObject)
      return;

   if (m_bFirstMapping)
      memset(m_pObject, 0, sizeof(T));
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
template<class T>
inline CMMFData<T>::~CMMFData()
{
   MEMBERASSERT();
   // Unmap the view.
   if (m_pObject)
   {
      UnmapViewOfFile(m_pObject);
      m_pObject = NULL;
   }
   if (m_hFileMap)
   {
      CloseHandle(m_hFileMap);
      m_hFileMap = NULL;
   }
}

//===============================================================================================
// FUNCTION: GetPtr
// PURPOSE:  Return the internal pointer.
//
template<class T>
inline T* CMMFData<T>::GetPtr() const
{
   MEMBERASSERT();
   return m_pObject;
}

//===============================================================================================
// FUNCTION: operator*()
// PURPOSE:  Overloaded operator.
//
template<class T>
inline T& CMMFData<T>::operator*() const
{
   MEMBERASSERT();
   return *GetPtr();
}

//===============================================================================================
// FUNCTION: operator void*()
// PURPOSE:  Overloaded operator.
//
template<class T>
inline CMMFData<T>::operator void*() const
{
   MEMBERASSERT();
   return (void*)GetPtr();
}

//===============================================================================================
// FUNCTION: IsFirstMapping
// PURPOSE:  Returns true if this object is the first mapping created of the shared data.
//
template<class T>
inline BOOL CMMFData<T>::IsFirstMapping() const
{
   MEMBERASSERT();
   return m_bFirstMapping;
}

//###############################################################################################
//###############################################################################################
//###
//###    CLASS:   CMMFStruct
//###    PURPOSE: This class acts as a pseudo pointer to a simple structure that is 
//###             to be shared between multiple processes.
//###    NOTES:   The shared structure will be zeroed when it is first initialized.
//###
//###############################################################################################
//###############################################################################################

template<class T>
class CMMFStruct : public CMMFData<T>
{
private:                   // Unimplemented constructors etc.
   CMMFStruct();
   CMMFStruct(const CMMFStruct<T> &rhs);
   const CMMFStruct<T>& operator=(const CMMFStruct<T>& rhs);

public:
   explicit CMMFStruct(LPCSTR szMapName);
   ~CMMFStruct();

   T* operator->() const;
};


//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
template<class T>
inline CMMFStruct<T>::CMMFStruct(LPCSTR szMapName)
   : CMMFData<T>(szMapName)
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
template<class T>
inline CMMFStruct<T>::~CMMFStruct()
{
   MEMBERASSERT();
}

//===============================================================================================
// FUNCTION: operator->()
// PURPOSE:  Overloaded operator.
//
template<class T>
inline T* CMMFStruct<T>::operator->() const
{
   MEMBERASSERT();
   return GetPtr();
}

#endif   // INC_CMMFSharedData_hpp

