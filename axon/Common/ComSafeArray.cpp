#include "wincpp.hpp"
#include "ComSafeArray.hpp"

/////////////////////////////////////////////////////////////////////////////
// CComSafeArray class

CComSafeArray::CComSafeArray(const SAFEARRAY& saSrc, VARTYPE vtSrc)
{
	Initialize();
	vt = (VARTYPE)(vtSrc | VT_ARRAY);
	VERIFY(::SafeArrayCopy((LPSAFEARRAY)&saSrc, &parray));
}

CComSafeArray::CComSafeArray(LPCSAFEARRAY pSrc, VARTYPE vtSrc)
{
	Initialize();
	vt = (VARTYPE)(vtSrc | VT_ARRAY);
	VERIFY(::SafeArrayCopy((LPSAFEARRAY)pSrc, &parray));
}

CComSafeArray::CComSafeArray(const CComSafeArray& saSrc)
{
	Initialize();
	*this = saSrc;
}

CComSafeArray::CComSafeArray(const VARIANT& varSrc)
{
	Initialize();
	*this = varSrc;
}

CComSafeArray::CComSafeArray(LPCVARIANT pSrc)
{
	Initialize();
	*this = pSrc;
}

// Operations
void CComSafeArray::Attach(VARIANT *pSrc)
{
	ASSERT(pSrc->vt & VT_ARRAY);

	// Free up previous safe array if necessary
	Clear();

	// give control of data to CComSafeArray
	memcpy(this, pSrc, sizeof(*pSrc));
	pSrc->vt = VT_EMPTY;
}

void CComSafeArray::Detach(VARIANT *pDest)
{
	// Clear out the variant
	VERIFY(SUCCEEDED(::VariantClear(pDest)));

	*pDest = *this;
   vt     = VT_EMPTY;
}

// Assignment operators
CComSafeArray& CComSafeArray::operator=(const CComSafeArray& saSrc)
{
	ASSERT(saSrc.vt & VT_ARRAY);

	VERIFY(::VariantCopy(this, (LPVARIANT)&saSrc));
	return *this;
}

CComSafeArray& CComSafeArray::operator=(const VARIANT& varSrc)
{
	ASSERT(varSrc.vt & VT_ARRAY);

	VERIFY(::VariantCopy(this, (LPVARIANT)&varSrc));
	return *this;
}

CComSafeArray& CComSafeArray::operator=(LPCVARIANT pSrc)
{
	ASSERT(pSrc->vt & VT_ARRAY);

	VERIFY(::VariantCopy(this, (LPVARIANT)pSrc));
	return *this;
}

BOOL CComSafeArray::CreateOneDim(VARTYPE vtSrc, DWORD dwElements, const void* pvSrcData, long nLBound)
{
	ASSERT(dwElements >= 0);

	// Setup the bounds and create the array
   SAFEARRAYBOUND rgsabound = { 0 };
	rgsabound.cElements      = dwElements;
	rgsabound.lLbound        = nLBound;
	if (!Create(vtSrc, 1, &rgsabound))
      return FALSE;

	// Copy over the data if neccessary
	if (pvSrcData != NULL)
	{
		void *pvDestData = NULL;
		AccessData(&pvDestData);

		memcpy(pvDestData, pvSrcData, GetElemSize() * dwElements);
		UnaccessData();
	}
   return TRUE;
}

DWORD CComSafeArray::GetOneDimSize() const
{
	ASSERT(GetDim() == 1);

	long nUBound = GetUBound(1);
	long nLBound = GetLBound(1);

	return nUBound - nLBound + 1;
}

BOOL CComSafeArray::ResizeOneDim(DWORD dwElements)
{
	ASSERT(GetDim() == 1);

   SAFEARRAYBOUND rgsabound = { 0 };
	rgsabound.cElements      = dwElements;
	rgsabound.lLbound        = 0;

	return Redim(&rgsabound);
}

BOOL CComSafeArray::Create(VARTYPE vtSrc, DWORD dwDims, DWORD* rgElements)
{
	ASSERT(rgElements != NULL);

	// Allocate and fill proxy array of bounds (with lower bound of zero)
	CArrayPtr<SAFEARRAYBOUND> rgsaBounds(dwDims);
   if (!rgsaBounds)
      return FALSE;

   rgsaBounds.Zero(dwDims);

	for (DWORD dwIndex = 0; dwIndex < dwDims; dwIndex++)
	{
		// Assume lower bound is 0 and fill in element count
		rgsaBounds[dwIndex].lLbound   = 0;
		rgsaBounds[dwIndex].cElements = rgElements[dwIndex];
	}

   return Create(vtSrc, dwDims, rgsaBounds);
}

BOOL CComSafeArray::Create(VARTYPE vtSrc, DWORD dwDims, SAFEARRAYBOUND* rgsabound)
{
	ASSERT(dwDims > 0);
	ASSERT(rgsabound != NULL);

	// Validate the VARTYPE for SafeArrayCreate call
	ASSERT(!(vtSrc & VT_ARRAY));
	ASSERT(!(vtSrc & VT_BYREF));
	ASSERT(!(vtSrc & VT_VECTOR));
	ASSERT(vtSrc != VT_EMPTY);
	ASSERT(vtSrc != VT_NULL);

	// Free up old safe array if necessary
	Clear();

	parray = ::SafeArrayCreate(vtSrc, dwDims, rgsabound);
	if (parray == NULL)
		return FALSE;

	vt = VARTYPE(vtSrc | VT_ARRAY);
   return TRUE;
}

BOOL CComSafeArray::GetElement(long* rgIndices, VARIANT *pVar) const
{
   ::VariantClear(pVar);
   BYTE *pbData = (BYTE*)_alloca( GetElemSize() );
   if (!GetElement(rgIndices, pbData))
      return FALSE;

   CComVariant Var;
   switch (vt & ~VT_ARRAY)
   {
      case VT_BOOL:
         Var = *(VARIANT_BOOL*)pbData;
	      break;

      case VT_I1:
         Var = *((char*)pbData);
	      break;

      case VT_I2:
         Var = *((short*)pbData);
	      break;

      case VT_I4:
         Var = *((long*)pbData);
	      break;

      case VT_UI1:
         Var = *((BYTE*)pbData);
	      break;

      case VT_UI2:
         Var = *((WORD*)pbData);
	      break;

      case VT_R4:
         Var = *((float*)pbData);
	      break;

      case VT_R8:
         Var = *((double*)pbData);
	      break;

      case VT_CY:
		   Var.vt = VT_CY;
		   Var.cyVal = *((CY*)pbData);
   	   break;

      case VT_DATE:
		   Var.vt = VT_DATE;
		   Var.date = *((DATE*)pbData);
         break;

      case VT_BSTR:
         Var = *((BSTR*)pbData);
	      break;

      case VT_ERROR:
         Var = *((SCODE*)pbData);
	      break;

      case VT_DISPATCH:
      case VT_UNKNOWN:
         Var = *((IUnknown**)pbData);
	      break;

      case VT_VARIANT:
		   Var = *((VARIANT*)pbData);
   	   break;

      default:
	      ASSERT(FALSE);
	      break;
   }
   Var.Detach(pVar);
   return TRUE;
}

#ifdef _DEBUG

static void DumpVariant(LPCVARIANT pSrc)
{
	// No support for VT_BYREF & VT_ARRAY
	if (pSrc->vt & VT_BYREF || pSrc->vt & VT_ARRAY)
		return;

	switch (pSrc->vt)
	{
	   case VT_BOOL:
         TRACE1("VT_BOOL = %s", V_BOOL(pSrc) ? "TRUE" : "FALSE");
         break;
	   case VT_UI1:
		   TRACE1("bVal = %c", pSrc->bVal);
         break;
	   case VT_I2:
		   TRACE1("iVal = %d", pSrc->iVal);
         break;
	   case VT_I4:
		   TRACE1("lVal = %d", pSrc->lVal);
         break;
	   case VT_CY:
		{
			CComVariant var(*pSrc);
			var.ChangeType(VT_BSTR);
         DumpVariant(&var);
         break;
		}

	   case VT_R4:
		   TRACE1("fltVal = %g", pSrc->fltVal);
         break;
	   case VT_R8:
		   TRACE1("dblVal = %g", pSrc->dblVal);
         break;
	   case VT_DATE:
		{
			CComVariant var(*pSrc);
			var.ChangeType(VT_BSTR);
         DumpVariant(&var);
		}

	   case VT_BSTR:
		   TRACE1("bstrVal = %s", pSrc->bstrVal);
         break;
	   case VT_ERROR:
	      TRACE1("E%08X", pSrc->scode);
         break;
	   case VT_DISPATCH:
	   case VT_UNKNOWN:
	      TRACE1("0x%p", pSrc->punkVal);
         break;
	   case VT_EMPTY:
	   case VT_NULL:
		   return;

	   default:
		   ASSERT(FALSE);
		   return;
	}
}

void CComSafeArray::Dump(int nDepth) const
{
   TRACE1("\nCComSafeArray:\n\tvt = %d", vt);
   TRACE("\n\tbounds:");
   
   long nDimensions = GetDim();
   long *piLBounds = (long*)_alloca( nDimensions*sizeof( long ) );
   long *piUBounds = (long*)_alloca( nDimensions*sizeof( long ) );

   // Dump the bounds
   for ( long iDimension = 0; iDimension < nDimensions; iDimension++ )
   {
      piLBounds[iDimension] = GetLBound( iDimension+1 );
      piUBounds[iDimension] = GetUBound( iDimension+1 );
      TRACE2("\n\t(%d, %d)", piLBounds[iDimension], piUBounds[iDimension]);
   }
   
   if ( nDepth > 0 )
   {
      // Dump the contents of the array.
      long *piIndices = (long*)_alloca( nDimensions*sizeof( long ) );
      for ( iDimension = 0; iDimension < nDimensions; iDimension++ )
         piIndices[iDimension] = piLBounds[iDimension];
      
      while ( piIndices[0] <= piUBounds[0] )
      {
         TRACE("\n\t");
         for ( iDimension = 0; iDimension < nDimensions; iDimension++ )
            TRACE1("[%d]", piIndices[iDimension]);
         TRACE(" = ");
         
         // Dump the value of the element
         CComVariant Var;
         GetElement(piIndices, &Var);
         DumpVariant(&Var);
         
         // Increment the rightmost index, with wraparound and carry logic
         iDimension = nDimensions-1;
         BOOL bWrapped = TRUE;
         do
         {
            bWrapped = FALSE;
            piIndices[iDimension]++;
            if ( piIndices[iDimension] > piUBounds[iDimension] )
            {
               if ( iDimension > 0 )
               {
                  // We've overstepped the bounds of this dimension, so wrap
                  // around to the lower bound and make sure to increment the
                  // next dimension to the left.
                  bWrapped = TRUE;
                  piIndices[iDimension] = piLBounds[iDimension];
                  iDimension--;
               }
            }
         } while( bWrapped && (iDimension >= 0) );
      }
   }
}

#endif
