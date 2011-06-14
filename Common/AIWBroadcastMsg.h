//***********************************************************************************************
//
//    Copyright (c) 1993-1999 Axon Instruments.
//    All rights reserved.
//
//***********************************************************************************************
// MODULE:  AIWBroadcastMsg.H
// PURPOSE: Notification messages for Imaging Workbench broadcast messages.
// AUTHOR:  RHW Mar 1999
//

#ifndef INC_AIWBROADCASTMSG_H
#define INC_AIWBROADCASTMSG_H


// Notification messages
#define AIW_BROADCAST_MESSAGE_STR     "AIWBroadcastMsg"

#define AIW_STARTACQUISITION           1        // lParam either AIW_LPARAM_VIEW or AIW_LPARAM_RECORD        
#define AIW_ACQUISITIONDONE            2        // lParam either AIW_LPARAM_VIEW or AIW_LPARAM_RECORD        
#define AIW_NEWLASTFILE                3        // lParam == 0, reserved, file found in registry under \Axon\CommonSettings\LastAcquiredFile\AXI

#define AIW_LPARAM_VIEW                0        // lParam for a view acquisition.
#define AIW_LPARAM_RECORD              1        // lParam for a record acquisition.
#define AIW_LPARAM_LIVEVIDEO           2        // lParam for going into live video mode


#endif   // INC_AIWBROADCASTMSG_H
