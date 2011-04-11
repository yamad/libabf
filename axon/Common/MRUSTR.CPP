//***********************************************************************************************
//
//    Copyright (c) 1994-1995 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  MRUSTR.CPP
// PURPOSE: Class member functions for maintaining a Most-Recently-Used file list
// AUTHOR:  BHI  Jul 1994
//

#include "wincpp.hpp"
#include "mrustr.hpp"

static char s_szIDTplt[] = "Item%d";

//===============================================================================================
// FUNCTION: Constructor
// PURPOSE:  Object initialization.
//
CMRUStringList::CMRUStringList()
{
   MEMBERASSERT();
   m_uStringCount = 0;
   for (UINT i=0; i<MRU_MAXSTRINGS; i++)
      m_aszList[i] = NULL;
}

//===============================================================================================
// FUNCTION: Destructor
// PURPOSE:  Object cleanup.
//
CMRUStringList::~CMRUStringList()
{
   MEMBERASSERT();
   for (UINT i=0; i<m_uStringCount; i++)
      if (m_aszList[i])
      {
         free(m_aszList[i]);
         m_aszList[i] = NULL;
      }
}

//===============================================================================================
// FUNCTION: AddItem
// PURPOSE:  Add a new item into the top of the list. If it is already there it is moved to the top.
//
BOOL CMRUStringList::AddItem(LPCSTR pszNewStr)
{
   MEMBERASSERT();
   LPSZASSERT(pszNewStr);
   if (!pszNewStr || (strlen(pszNewStr)==0))
      return FALSE;

   // Create a copy of the string.      
   char *psz = strdup(pszNewStr);
   if (!psz)
      return FALSE;
      
   // Search for Item, deleting it if found.
   DeleteItem(psz);

   // If the MRU list is full reduce it by one and shift entries up one
   // so that the new entry is inserted at the top of the list.
   if (m_uStringCount == MRU_MAXSTRINGS)
   {
      free( m_aszList[--m_uStringCount] );
      m_aszList[m_uStringCount] = NULL;
   }
   for (UINT i=m_uStringCount; i!=0; i--)
      m_aszList[i] = m_aszList[i-1];

   // Insert the new Item and increment the count.
   m_aszList[0] = psz;
   ++m_uStringCount;
   return TRUE;
}

//===============================================================================================
// FUNCTION: GetItem
// PURPOSE:  Gets the string stored at a particular index.
//
BOOL CMRUStringList::GetItem(UINT uItem, LPSTR pszRval, UINT uLen) const
{
   MEMBERASSERT();
   if (uItem >= m_uStringCount)
      return FALSE;
      
   AXU_strncpyz(pszRval, m_aszList[uItem], uLen);
   return TRUE;
}


//===============================================================================================
// FUNCTION: GetItem
// PURPOSE:  Gets the string stored at a particular index.
//
LPCSTR CMRUStringList::GetItem(UINT uItem) const
{
   MEMBERASSERT();
   if (uItem >= m_uStringCount)
      return NULL;
      
   return m_aszList[uItem];
}


//===============================================================================================
// FUNCTION: DeleteItem
// PURPOSE:  Deletes an item if it is found in the list.
//
BOOL CMRUStringList::DeleteItem(LPCSTR pszDelStr)
{
   MEMBERASSERT();
   LPSZASSERT(pszDelStr);
   if (strlen(pszDelStr)==0)
      return FALSE;

   // Search for Item, deleting it if found.
   for (UINT i=0; i<m_uStringCount; i++)
      if (stricmp(m_aszList[i], pszDelStr)==0)
         break;
   if (i>=m_uStringCount)
      return FALSE;

   free( m_aszList[i] );
   m_aszList[i] = NULL;
      
   for (; i<m_uStringCount-1; i++)
      m_aszList[i] = m_aszList[i+1];
   m_aszList[i] = NULL;
   --m_uStringCount;
   return TRUE;
}

//===============================================================================================
// FUNCTION: LoadList
// PURPOSE:  Loads the list out of a particular section in an INI file.
//
void CMRUStringList::LoadList(LPCSTR pszIniFile, LPCSTR pszKey)
{
   MEMBERASSERT();
   LPSZASSERT(pszIniFile);
   LPSZASSERT(pszKey);

   char sz[MRU_MAXSTRLEN];
   char szID[16];

   for (int i=MRU_MAXSTRINGS-1; i >= 0; i--)
   {
      sprintf(szID, s_szIDTplt, i);
      if (AXU_GetPrivateProfileString(pszKey, szID, "", sz, sizeof(sz), pszIniFile))
         AddItem(sz);
   }
}


//===============================================================================================
// FUNCTION: SaveList
// PURPOSE:  Saves the list to a particular section in an INI file.
//
void CMRUStringList::SaveList(LPCSTR pszIniFile, LPCSTR pszKey)
{
   MEMBERASSERT();
   LPSZASSERT(pszIniFile);
   LPSZASSERT(pszKey);
   
   char sz[MRU_MAXSTRLEN];
   char szID[16];
   LPSTR psz;
   
   for (UINT i=0; i<MRU_MAXSTRINGS; i++)
   {
      sprintf(szID, s_szIDTplt, i);
      psz = GetItem(i, sz, sizeof(sz)) ? sz : NULL;
      AXU_WritePrivateProfileString(pszKey, szID, psz, pszIniFile);
   }
}
