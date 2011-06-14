//***********************************************************************************************
//
//    Copyright (c) 1993-2004 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  ClampexBroadcastMsg.H
// PURPOSE: Notification messages for Clampex and AxoScope broadcast messages.
// AUTHOR:  PRC  Dec 1998
//          GRB  Feb 2004
//

#ifndef INC_CLAMPEXBROADCASTMSG_H
#define INC_CLAMPEXBROADCASTMSG_H

// Notification messages
#define CLX_BROADCAST_MESSAGE_STR     "ClampexBroadcastMsg"
#define ASP_BROADCAST_MESSAGE_STR     "AxoScopeBroadcastMsg"
#define AXON_BROADCAST_MESSAGE_STR    "Axon_BroadcastMsg"

// wParams for CLX_BROADCAST_MESSAGE_STR message (for backwards compatibility with older version of Axoscope)
#define CLX_STARTACQUISITION            4   // Sent from Clampex when trial actually starts     
#define CLX_ACQUISITIONDONE             5   // Sent from Clampex when trial is done or stopped, lParam is nothing     
#define CLX_NEWLASTFILE                20   // A new ABF data file has been written to disk
                                            // Full path is written to registry key \HKCU\Software\Axon\CommonSettings\LastAcquiredFile\ABF-Clampex

// lParams for CLX_STARTACQUISITION (for backwards compatibility with older version of Axoscope)
#define CLX_LPARAM_VIEW                 0   // Acquisition started in view mode.
#define CLX_LPARAM_RECORD               1   // Acquisition started in record mode.

// wParams for CLX_BROADCAST_MESSAGE_STR message (3rd party)
#define CLX_ACQ_STARTED                 CLX_STARTACQUISITION // Sent from Clampex when trial actually starts.
#define CLX_ACQ_DONE                    CLX_ACQUISITIONDONE  // Sent from Clampex when trial is done or stopped, lParam is nothing

// lParams for CLX_ACQ_STARTED
#define CLX_LPARAM_ACQ_STARTED_VIEW     CLX_LPARAM_VIEW      // Acquiring data with current protocol but not saving to file.
#define CLX_LPARAM_ACQ_STARTED_RECORD   CLX_LPARAM_RECORD    // Acquiring data with current protocol and saving to file. 
#define CLX_LPARAM_ACQ_STARTED_RERECORD 2                    // Acquiring data with current protocol and saving over last file.

///////////////////////////////////////////////////////////////////////////////////
//                         Axon_BroadcastMsg
///////////////////////////////////////////////////////////////////////////////////
//
// A single application can now listen to this message and catch all new file Events
// Registry entries updated in parallel with this  (eg OpusX writes an ABF file)
// HKEY_CURRENT_USER\Software\Axon\CommonSettings\LastAcquiredFile    
//          Who       = "OpusXpress"
//          TYPE      = "ABF"
//          ABF       = "FileName"
//          ABF-OPUSX = "FileName"
//
#define  AXBM_NEW_LASTFILE_ABF         0x0100     // lParam = Who
//                                     0x0101 ... reserve space for 255 File Types
#define  AXBM_NEW_LASTFILE_XXX         0x01FF      

   // LParam Who defintions
   #define  AXBM_CLAMPEX_ID              0x0001    
   #define  AXBM_AXOSCOPE_ID             0x0002    
   #define  AXBM_OPUSXPRESS_ID           0x0003

#endif   // INC_CLAMPEXBROADCASTMSG_H
