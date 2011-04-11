//**********************************************************************************************
//
//    Copyright (c) 1998 Axon Instruments.
//    All rights reserved.
//
//**********************************************************************************************
// HEADER:  WaitCursor.hpp
// PURPOSE: Simple class to put up a wait cursor.
// AUTHOR:  BHI  Jan 1998
//
#ifndef INC_WAITCURSOR_HPP
#define INC_WAITCURSOR_HPP

//===============================================================================================
// The class definition
//
class CSysCursor
{
private:
   HCURSOR m_hSaveCursor;

public:
   CSysCursor(LPCSTR pszCursor)
   {
      m_hSaveCursor = SetCursor(LoadCursor(NULL, pszCursor));
   }

   ~CSysCursor()
   {
      SetCursor(m_hSaveCursor);
   }
};

#ifndef _MFC_VER
//===============================================================================================
// The class definition
//
class CWaitCursor : public CSysCursor
{
public:
   CWaitCursor() : CSysCursor(IDC_WAIT) {}
};
#endif

//===============================================================================================
// The class definition
//
class CArrowCursor : public CSysCursor
{
public:
   CArrowCursor() : CSysCursor(IDC_ARROW) {}
};

#endif  // INC_WAITCURSOR_HPP
