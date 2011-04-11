/////////////////////////////////////////////////////////////////////////////
// CComSafeArray class
#ifndef INC_COMSAFEARRAY_HPP
#define INC_COMSAFEARRAY_HPP

#ifndef LPCSAFEARRAY
typedef const SAFEARRAY* LPCSAFEARRAY;
#endif

#ifndef LPCVARIANT
typedef const VARIANT* LPCVARIANT;
#endif

#include "ComDebug.h"

class CComSafeArray : public tagVARIANT
{
private:
   void Initialize()    { memset(this, 0, sizeof(*this)); vt = VT_EMPTY; }

public:  // Constructors & destructor
	CComSafeArray()      { Initialize(); }
	CComSafeArray(const SAFEARRAY& saSrc, VARTYPE vtSrc);
	CComSafeArray(LPCSAFEARRAY pSrc, VARTYPE vtSrc);
	CComSafeArray(const CComSafeArray& saSrc);
	CComSafeArray(const VARIANT& varSrc);
	CComSafeArray(LPCVARIANT pSrc);

	~CComSafeArray()     { Clear(); }

public:  // Operations
	void Clear()         { COM_VERIFY(::VariantClear(this)); }
	void Attach(VARIANT *pSrc);
   void Detach(VARIANT *pDest);

	CComSafeArray& operator=(const CComSafeArray& saSrc);
	CComSafeArray& operator=(const VARIANT& varSrc);
	CComSafeArray& operator=(LPCVARIANT pSrc);

	operator LPVARIANT()          { return this; }
	operator LPCVARIANT() const   { return this; }

	// One dim array helpers
	BOOL  CreateOneDim(VARTYPE vtSrc, DWORD dwElements, const void* pvSrcData = NULL, long nLBound = 0);
	DWORD GetOneDimSize() const;
	BOOL  ResizeOneDim(DWORD dwElements);

	// Multi dim array helpers
	BOOL Create(VARTYPE vtSrc, DWORD dwDims, DWORD* rgElements);

	// SafeArray wrapper classes
	BOOL Create(VARTYPE vtSrc, DWORD dwDims, SAFEARRAYBOUND* rgsabounds);

	void AccessData(void** ppvData)  { COM_VERIFY(::SafeArrayAccessData(parray, ppvData)); }
	void UnaccessData()              { COM_VERIFY(::SafeArrayUnaccessData(parray)); }
	void AllocData()                 { COM_VERIFY(::SafeArrayAllocData(parray)); }

	BOOL AllocDescriptor(DWORD dwDims)
   { return SUCCEEDED(::SafeArrayAllocDescriptor(dwDims, &parray)); }

	void Copy(LPSAFEARRAY* ppsa)
   { COM_VERIFY(::SafeArrayCopy(parray, ppsa)); }

	long GetLBound(DWORD dwDim) const
   {
      long lLBound = 0;
      COM_VERIFY(::SafeArrayGetLBound(parray, dwDim, &lLBound));
      return lLBound;
   }

	long GetUBound(DWORD dwDim) const
   {
      long lUBound = 0;
      COM_VERIFY(::SafeArrayGetUBound(parray, dwDim, &lUBound));
      return lUBound;
   }

	BOOL GetElement(long* rgIndices, void* pvData) const
   { return SUCCEEDED(::SafeArrayGetElement(parray, rgIndices, pvData)); }

   BOOL GetElement(long* rgIndices, VARIANT *pVar) const;

	void PtrOfIndex(long* rgIndices, void** ppvData)
   { COM_VERIFY(::SafeArrayPtrOfIndex(parray, rgIndices, ppvData)); }

	void PutElement(long* rgIndices, void* pvData)
   { COM_VERIFY(::SafeArrayPutElement(parray, rgIndices, pvData)); }

	BOOL Redim(SAFEARRAYBOUND *psaboundNew)
   { return SUCCEEDED(::SafeArrayRedim(parray, psaboundNew)); }

	void Lock()                { COM_VERIFY(::SafeArrayLock(parray)); }
	void Unlock()              { COM_VERIFY(::SafeArrayUnlock(parray)); }

	UINT GetDim() const       { return ::SafeArrayGetDim(parray); }
	UINT GetElemSize() const  { return ::SafeArrayGetElemsize(parray); }

	void Destroy()             { COM_VERIFY(::SafeArrayDestroy(parray)); }
	void DestroyData()         { COM_VERIFY(::SafeArrayDestroyData(parray)); }
	void DestroyDescriptor()   { COM_VERIFY(::SafeArrayDestroyDescriptor(parray)); }

   void DumpElement(long* piIndices) const;
   void Dump(int nDepth) const;
};

#ifndef _DEBUG
inline void CComSafeArray::DumpElement(long* piIndices) const {}
inline void CComSafeArray::Dump(int nDepth) const {}
#endif

#endif   // INC_COMSAFEARRAY_HPP
