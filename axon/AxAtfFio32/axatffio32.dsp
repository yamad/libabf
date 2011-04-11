# Microsoft Developer Studio Project File - Name="axatffio32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axatffio32 - Win32 DebugMD
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axatffio32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axatffio32.mak" CFG="axatffio32 - Win32 DebugMD"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axatffio32 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axatffio32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axatffio32 - Win32 DebugMD" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AxonDev/Comp/AxAtfFio32", KJDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axatffio32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Yu"wincpp.hpp" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBCMTD"
# Begin Special Build Tool
OutDir=.\Release
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "axatffio32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\axatffio32"
# PROP BASE Intermediate_Dir ".\axatffio32"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Yu"wincpp.hpp" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT"
# Begin Special Build Tool
OutDir=.\Debug
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "axatffio32 - Win32 DebugMD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "axatffio32___Win32_DebugMD"
# PROP BASE Intermediate_Dir "axatffio32___Win32_DebugMD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugMD"
# PROP Intermediate_Dir "DebugMD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Yu"wincpp.hpp" /FD /c
# ADD CPP /nologo /Zp4 /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Yu"wincpp.hpp" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT"
# Begin Special Build Tool
OutDir=.\DebugMD
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axatffio32 - Win32 Release"
# Name "axatffio32 - Win32 Debug"
# Name "axatffio32 - Win32 DebugMD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Atfintl.h
# End Source File
# Begin Source File

SOURCE=.\Atfutil.h
# End Source File
# Begin Source File

SOURCE=.\Axatffio32.cpp
# End Source File
# Begin Source File

SOURCE=.\axatffio32.def
# End Source File
# Begin Source File

SOURCE=.\axatffio32.H
# End Source File
# Begin Source File

SOURCE=.\Axatffio32.rc
# End Source File
# Begin Source File

SOURCE=..\Common\Axodebug.cpp
# End Source File
# Begin Source File

SOURCE=..\common\axodebug.h
# End Source File
# Begin Source File

SOURCE=.\fileio.CPP
# End Source File
# Begin Source File

SOURCE=.\Initlize.cpp
# End Source File
# Begin Source File

SOURCE=.\Wincpp.cpp
# ADD CPP /Yc"wincpp.hpp"
# End Source File
# Begin Source File

SOURCE=.\Wincpp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\lib\AxonValidation.lib
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\atferror.str
# End Source File
# Begin Source File

SOURCE=.\AXATFFIO32.ICO
# End Source File
# Begin Source File

SOURCE=.\axatffio32.rc2
# End Source File
# End Group
# End Target
# End Project
