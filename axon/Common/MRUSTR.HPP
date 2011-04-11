//***********************************************************************************************
//
//    Copyright (c) 1995-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  MRUSTR.HPP
// PURPOSE: Class definition for maintaining a Most-Recently-Used list of strings
// AUTHOR:  BHI  Mar 1995
//

#ifndef INC_MRUSTR_HPP
#define INC_MRUSTR_HPP

class CMRUStringList
{
private:
   enum { MRU_MAXSTRINGS = 20 };
   enum { MRU_MAXSTRLEN  = 128 };
   UINT  m_uStringCount;
   char *m_aszList[MRU_MAXSTRINGS];

private:
   void GetListEntry(UINT uItem, LPSTR pszListEntry);

private:    // Unimplemented copy functions.
   CMRUStringList(const CMRUStringList &M);
   const CMRUStringList &operator=(const CMRUStringList &M);

public:
   CMRUStringList();
   ~CMRUStringList();

   BOOL DeleteItem(LPCSTR pName);
   BOOL AddItem(LPCSTR pName);
   BOOL GetItem(UINT uItem, LPSTR pszRval, UINT uLen) const;
   LPCSTR GetItem(UINT uItem) const;
   void LoadList(LPCSTR pszIniFile, LPCSTR pszKey);
   void SaveList(LPCSTR pszIniFile, LPCSTR pszKey);
};

#endif      // INC_MRUSTR_HPP
