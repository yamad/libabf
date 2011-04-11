//**********************************************************************************************
//
//    Copyright (c) 1997 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// HEADER:  UserList.HPP
// PURPOSE: Contains class definition for the CUserList class.
// AUTHOR:  BHI  May 1997
//

#ifndef INC_USERLIST_HPP
#define INC_USERLIST_HPP

#include "\AxonDev\Comp\AxABFFIO32\abfheadr.h"

// Union to use to store the user list entries.
union UserListItem
{
   int   n;
   float f;

   UserListItem();
};

inline UserListItem::UserListItem()
{
   // Set everything to 0.
   memset( this, 0, sizeof(UserListItem) );
}   


class CUserList
{
private:
   BOOL     m_bEnable;        // TRUE if the user list is enabled.
   BOOL     m_bRepeat;        // TRUE if the user list is repeated.
   int      m_nParamToVary;   // The parameter that the user list applies to.
   int      m_nListEpoch;     // If epoch based, the number of the epoch.
   UINT     m_uListEntries;   // The number of entries in the list.
   UINT     m_uListNum;       // The user list number 

   enum { MAX_LIST_ITEMS = ABF_USERLISTLEN/2+1 };
   UserListItem   m_aItems[MAX_LIST_ITEMS];
   UserListItem   m_aDigTrainItems[MAX_LIST_ITEMS];
   
private:
   // Prevent the use of the default operator=() and copy constructor.
   CUserList(const CUserList &);
   const CUserList &operator=(const CUserList &);
   
private:
   void SetFloat(UINT uIndex, LPCSTR pszItem);
   float GetFloat(UINT uIndex) const;
   void SetInteger(UINT uIndex, LPCSTR pszItem);
   int  GetInteger(UINT uIndex) const;
   void SetBinary(UINT uIndex, LPCSTR pszItem);
   void SetItem(UINT uIndex, LPCSTR pszItem);
   UINT Episode2Entry(UINT uEpisode) const;

   // Digital level and Digital Train
   int GetDigitalLevelBitPattern( LPCSTR combined_train_and_level_pattern );
   int GetDigitalTrainBitPattern( LPCSTR combined_train_and_level_pattern );
   int GetDigitalTrainsInt( UINT uIndex ) const;

public:
   CUserList();

   void Initialize(const ABFFileHeader *pFH, UINT uListNum);
   int  GetActiveEpoch() const;

   void UpdateHeader( ABFFileHeader *pFH, UINT uDAC, UINT uEpisode ) const;
   UserListItem GetItem( UINT uEpisode, BOOL *pbIsInteger ) const; 
   UserListItem GetDigitalTrainItem( UINT uEpisode, BOOL *pbIsInteger ) const;
   UINT GetNumEntries() const;
};

//==============================================================================================
// FUNCTION: SetFloat
// PURPOSE:  Sets a float value into the array of unions.
//
inline void CUserList::SetFloat(UINT uIndex, LPCSTR pszItem)
{
   MEMBERASSERT();
   ASSERT(uIndex < MAX_LIST_ITEMS);
   m_aItems[uIndex].f = float(atof(pszItem));
}

//==============================================================================================
// FUNCTION: GetFloat
// PURPOSE:  Gets a float value from the array of unions.
//
inline float CUserList::GetFloat(UINT uIndex) const
{
   MEMBERASSERT();
   ASSERT(uIndex < MAX_LIST_ITEMS);
   return m_aItems[uIndex].f;
}

//==============================================================================================
// FUNCTION: SetInteger
// PURPOSE:  Sets an integer value into the array of unions.
//
inline void CUserList::SetInteger(UINT uIndex, LPCSTR pszItem)
{
   MEMBERASSERT();
   ASSERT(uIndex < MAX_LIST_ITEMS);
   m_aItems[uIndex].n = atoi(pszItem);
}

//==============================================================================================
// FUNCTION: GetInteger
// PURPOSE:  Gets an integer value from the array of unions.
//
inline int CUserList::GetInteger(UINT uIndex) const
{
   MEMBERASSERT();
   ASSERT(uIndex < MAX_LIST_ITEMS);
   return m_aItems[uIndex].n;
}

//==============================================================================================
// FUNCTION: GetDigitalTrainsInt
// PURPOSE:  Returns the digital trains item.
//
inline int CUserList::GetDigitalTrainsInt( UINT uIndex ) const
{
   MEMBERASSERT();
   ASSERT( uIndex < MAX_LIST_ITEMS );
   return m_aDigTrainItems[ uIndex ].n;
}

//==============================================================================================
// FUNCTION: SetBinary
// PURPOSE:  Sets a integer value into the array of unions, interpreting the 
//           string as being in base 2.
//
inline void CUserList::SetBinary(UINT uIndex, LPCSTR pszItem)
{
   MEMBERASSERT();
   ASSERT(uIndex < MAX_LIST_ITEMS);
   m_aItems[ uIndex ].n              = GetDigitalLevelBitPattern( pszItem );
   m_aDigTrainItems[ uIndex ].n      = GetDigitalTrainBitPattern( pszItem );
}

#endif   // INC_COMMAND_HPP
