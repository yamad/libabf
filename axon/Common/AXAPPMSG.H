//***********************************************************************************************
//
//    Copyright (c) 1994-1997 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// HEADER:  AXAPPMSG.H
// PURPOSE: Axon Application Message System, header file.
// AUTHOR:  JDH Jun 1995
//

#ifndef INC_COMMON_AXAPPMSG_H
#define INC_COMMON_AXAPPMSG_H

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//    Command constants
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define  APP_COMMAND_STR               "AxoAppCommand" 

#define  AM_RESET_STOPWATCH            0x0001     // lParam = 0L
#define  AM_STATBAR_STRING             0x0002     // lParam = LPSTR of the message to place in the status bar.
#define  AM_LOGENTRY                   0x0003     // lParam = AM_LogEntryRec structure pointer
#define  AM_DEBUG                      0x0004     // lParam = char * (debug text)
#define  AM_CONTEXT_HELP               0x0005     // lParam = AM_HelpContextRec structure pointer
#define  AM_IS_HELP_AVALIABLE          0x0006     // lParam = 0L 
#define  AM_RELEASE_SPLASH             0x0007     // lParam = 0L
#define  AM_STOPWATCH_TIME             0x0008     // lParam = (long *) seconds
#define  AM_IS_LOG_AVALIABLE           0x0009     // lParam = 0L
#define  AM_CHECK_DONGLE               0x000A     // lParam = (BOOL *) dongle present.
#define  AM_PUMP_MESSAGE               0x000B     // lParam = flags
#define  AM_USER_MESSAGE               0x0100     // App specific messages start here.

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//    Help message format definitions/structures
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Help Event Messages are structured like this;
//
// hWnd   - Should be the main-frame of the Axon Application
// uMsg   - Should be the ID of the message type registered as APP_COMMAND_STR
// wParam - Must be AM_CONTEXT_HELP
// lParam - A pointer to an AM_HelpContextRec
//

// The version of the event structure
#define  AXOAPP_HELP_STRUCTURE_VERSION 2

// The maximum size of the component name
#define  AXOAPP_MAX_COMPONENT_NAMELEN  64

// The log event structure
typedef struct
{ 
   UINT           m_uVersion;          // Should be set to AXOAPP_HELP_STRUCTURE_VERSION
   char           m_szComponentName[AXOAPP_MAX_COMPONENT_NAMELEN]; 
                                       // The name of the component
   UINT           m_uCommand;          // The command number within the component
} AM_HelpContextRec, *LPAM_HelpContextRec;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//    Log message format definitions/structures
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Log Event Messages are structured like this;
//
// hWnd   - Should be the main-frame of the Axon Application
// uMsg   - Should be the ID of the message type registered as APP_COMMAND_STR
// wParam - Must be AM_LOGENTRY
// lParam - A pointer to an AM_LogEntryRec
//

// Flags for the log event

// Add the time-stamp
#define AXOAPP_LOG_FLAG_DATESTAMP      0x00000001
// Don't flush the log file to the disk, by default the system will
// always flush the file after the addition of an event
#define AXOAPP_LOG_FLAG_DONT_FLUSH     0x00000002
// Mandatatory no time-stamp flag
#define AXOAPP_LOG_FLAG_MAND_NOSTAMP   0x00000004
// Mandatatory time-stamp flag
#define AXOAPP_LOG_FLAG_MAND_STAMP     0x00000010
// Acquisition stopwatch stamp flag
#define AXOAPP_LOG_FLAG_TIMER          0x00000020

// The log event internal priorities

// This event should only be written in verbose mode
#define AXOAPP_LOG_PRIORITY_VERBOSE    0
// This event should be written in verbose and terse mode
#define AXOAPP_LOG_PRIORITY_TERSE      1 

// The log event types that you should send
#define AXOAPP_LOG_COMMENT             AXOAPP_LOG_PRIORITY_TERSE
#define AXOAPP_LOG_RESULTS             AXOAPP_LOG_PRIORITY_VERBOSE
#define AXOAPP_LOG_INFORMATION         AXOAPP_LOG_PRIORITY_VERBOSE


// The version of the event structure
#define AXOAPP_LOG_EVENT_VERSION       1

// The log event structure
typedef struct
{ 
   UINT           m_uVersion;          // Should be set to AXOAPP_LOG_EVENT_VERSION
   LONG           m_lFlags;            // The flags on the message
   UINT           m_uPriority;         // The priority of the message
   LPSTR          m_lpszEntryText;     // The text of the entry 
   LONG           m_lTimeStamp;        // The time for the timestamp (0L if you don't care)
} AM_LogEntryRec, *LPAM_LogEntryRec;

//----------------------------------------------------------------------
// Clampex <=7.01    accepts a structure version <=1 (unsigned 32-bit integer)
// Axoutils          sends a structure version 1
// The structure was packed on 4-byte boundaries
//
// Current           accepts a structure version 0,1 (unsigned 32-bit integer)
// Axoutils          sends a structure version 0
// The structure was packed on 4-byte boundaries
//
// The structure is basically a union of all the different versions.
// C++ won't allow us to provide a default ctor for a union, 
// so we wrap the union in a structure (which can have a ctor)

#pragma pack(push, 4)
struct sLABBOOK_ENTRY
{
   union U
   {
      // An unused char array to set the over-all size of the structure
      char  m_ForceStructureSize[64];
      // The structure as used in Clampex <= 7.01
      struct   sVERSION00
      {
         UINT           m_uReserved;         // Must == 1
         LONG           m_lFlags;            // The flags on the message
         UINT           m_uPriority;         // The priority of the message
         LPSTR          m_lpszEntryText;     // The text of the entry 
         LONG           m_lTimeStamp;        // The time for the timestamp (0L if you don't care)
      }  m_sData00;
      // The current version of the structure
      // From now on, the version is indicated by the "sizeof" fields
      struct   sVERSION01
      {
         UINT           m_uReserved;         // Must == 0
         LONG           m_lFlags;            // The flags on the message
         UINT           m_uPriority;         // The priority of the message
         LPCSTR         m_lpszEntryText;     // The text of the entry 
         LONG           m_lTimeStamp;        // The time for the timestamp (0L if you don't care)
         // NOTE: All fields before this line MUST be backwards-compatible with m_sData00
         size_t         m_nBytesAllocated;   // The physical length of the structure
         size_t         m_nBytesUsed;        // The number of bytes used.
         LPCSTR         m_szFileName;        // Pointer to the file name, or NULL
         LPCSTR         m_szEventType;       // Pointer to the name of the event type, or NULL
         LPCSTR         m_szEventSubType;    // Pointer to the name of the event sub-type (eg fit/parameters vs fit/results), or NULL
         // NOTE: All fields before this line MUST be backwards-compatible with m_sData01
      }  m_sData01;
      struct   sVERSION02
      {
         UINT           m_uReserved;         // Must == 0
         LONG           m_lFlags;            // The flags on the message
         UINT           m_uPriority;         // The priority of the message
         LPCSTR         m_lpszEntryText;     // The text of the entry 
         LONG           m_lTimeStamp;        // The time for the timestamp (0L if you don't care)
         // NOTE: All fields before this line MUST be backwards-compatible with m_sData00
         size_t         m_nBytesAllocated;   // The physical length of the structure
         size_t         m_nBytesUsed;        // The number of bytes used.
         LPCSTR         m_szFileName;        // Pointer to the file name, or NULL
         LPCSTR         m_szEventType;       // Pointer to the name of the event type, or NULL
         LPCSTR         m_szEventSubType;    // Pointer to the name of the event sub-type (eg fit/parameters vs fit/results), or NULL
         // NOTE: All fields before this line MUST be backwards-compatible with m_sData01
         SYSTEMTIME     m_TimeStamp;         // The time for the timestamp (0L if you don't care)
         // NOTE: All fields before this line MUST be backwards-compatible with m_sData02
      }  m_sData02;
   } u;
   sLABBOOK_ENTRY();
};
#pragma pack(pop)

// Default ctor to initialize the structure
inline
sLABBOOK_ENTRY::sLABBOOK_ENTRY()
{
   // Initialize the whole structure to 0
   memset(u.m_ForceStructureSize, 0, sizeof(u.m_ForceStructureSize));

   // Initialize the fields to indicate we are version 02
   u.m_sData02.m_uReserved          = 0;
   u.m_sData02.m_nBytesAllocated    = sizeof(u.m_ForceStructureSize);
   u.m_sData02.m_nBytesUsed         = sizeof(u.m_sData02);

   // Initialize the data fields
   // (These are all NULL)

   // Check we haven't made our data structure too large
   // If this is so, we need to increase the size of m_ForceStructureSize
   C_ASSERT(sizeof(u.m_sData01) <= sizeof(u.m_ForceStructureSize));
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//    Pump Message flags
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// These flags can be combined (using bit-wise OR)

// Wait/don't wait for a message
#define  AXOAPP_PUMP_MESSAGE_WAIT         0x00000001L
#define  AXOAPP_PUMP_MESSAGE_NOWAIT       0x00000000L

// Pump multiple/single messages
#define  AXOAPP_PUMP_MESSAGE_MULTIPLE     0x00000002L
#define  AXOAPP_PUMP_MESSAGE_SINGLE       0x00000000L


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//    Return codes
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Generic return codes
#define  ARC_MSG_SUCCESS               1
#define  ARC_MSG_NO_METHOD             0
#define  ARC_MSG_INVALID               2
#define  ARC_MSG_BAD_PERMS             3
#define  ARC_INTERNAL_ERROR            4
#define  ARC_OLD_VERS                  5

// Debug message return codes
#define  ARC_MSG_DISK_FULL             10

// Help message return codes
#define  ARC_COMPONENT_NOT_FOUND       20
#define  ARC_HELP_NOT_FOUND            21

// Log message return codes
#define  ARC_NO_LOGFILE                30
#define  ARC_LOW_PRIORITY              32

#endif // INC_COMMON_AXAPPMSG_H
