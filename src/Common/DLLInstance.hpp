//***********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  DLLInstance.HPP
// PURPOSE: Define the class CDLLInstance an auto pointer for DLL instances.
// AUTHOR:  BHI May 1998
//
#ifndef INC_DLLINSTANCE_HPP
#define INC_DLLINSTANCE_HPP

//***********************************************************************************************
// CLASS:   CDLLInstance
// PURPOSE: A simple smart pointer for DLL instance handles.
//
class CDLLInstance
{
private:
   HINSTANCE   m_hInstance;
   
private:    // Prevent copy constructor and operator=().
   CDLLInstance(const CDLLInstance &);
   const CDLLInstance &operator=(const CDLLInstance &);

public:
   CDLLInstance(LPCSTR pszDLLName);
   ~CDLLInstance();

   // Accessor functions to get at the instance handle.
   operator HINSTANCE () const;
   HINSTANCE Get() const;

   FARPROC   GetProcAddress(LPCSTR pszName);
   HINSTANCE Release();
};
   
//================================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
inline CDLLInstance::CDLLInstance(LPCSTR pszDLLName)
{
   MEMBERASSERT();

   // Turn off the "File not found" error box.
   UINT uSaveErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX);

   m_hInstance = LoadLibrary(pszDLLName);

   // Reset the error mode flag.
   SetErrorMode(uSaveErrorMode);
 
   if (m_hInstance==NULL)
      ERRORMSG1("LoadLibrary(\"%s\") FAILED.\n", pszDLLName);
}

//================================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
inline CDLLInstance::~CDLLInstance()
{
   MEMBERASSERT();
   if (m_hInstance)
      FreeLibrary(m_hInstance);
   m_hInstance = NULL;
}

//================================================================================================
// FUNCTION: Get
// PURPOSE:  Returns the instance handle to the DLL.
//
inline HINSTANCE CDLLInstance::Get() const
{
   MEMBERASSERT();
   return m_hInstance; 
}

//================================================================================================
// FUNCTION: Overloaded cast operator
// PURPOSE:  Returns the instance handle to the DLL.
//
inline CDLLInstance::operator HINSTANCE() const
{
   MEMBERASSERT();
   return Get();
}

//================================================================================================
// FUNCTION: GetProcAddress
// PURPOSE:  Returns a pointer to a DLL entry point.
//
inline FARPROC CDLLInstance::GetProcAddress(LPCSTR pszName)
{
   MEMBERASSERT();
   if (!m_hInstance)
      return NULL;
   return ::GetProcAddress(m_hInstance, pszName);
}

//================================================================================================
// FUNCTION: Release
// PURPOSE:  Releases the held instance handle.
//
inline HINSTANCE CDLLInstance::Release()
{
   MEMBERASSERT();
   HINSTANCE hRval = m_hInstance;
   m_hInstance = NULL;
   return hRval; 
}

#endif          // INC_DLLINSTANCE_HPP
