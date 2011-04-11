# Microsoft Developer Studio Project File - Name="Header" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Header - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HEADER.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HEADER.MAK" CFG="Header - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Header - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Header - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Header - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\HeaderRelease"
# PROP Intermediate_Dir ".\HeaderRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Ox /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_DOS" /YX /c
# ADD CPP /nologo /Zp4 /MT /W4 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Yu"wincpp.hpp" /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1400 /subsystem:console /machine:IX86
# ADD LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winmm.lib winspool.lib version.lib setargv.obj ole32.lib /nologo /stack:0x1400 /subsystem:console /machine:IX86
# Begin Special Build Tool
OutDir=.\HeaderRelease
TargetName=HEADER
SOURCE="$(InputPath)"
PostBuild_Desc=Copying EXE to bin
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\HeaderDebug"
# PROP Intermediate_Dir ".\HeaderDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_DOS" /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Yu"wincpp.hpp" /FD /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1400 /subsystem:console /debug /machine:IX86
# ADD LINK32 ole32.lib oldnames.lib kernel32.lib user32.lib gdi32.lib winmm.lib winspool.lib version.lib setargv.obj /nologo /stack:0x1400 /subsystem:console /debug /machine:IX86
# Begin Special Build Tool
OutDir=.\HeaderDebug
TargetName=HEADER
SOURCE="$(InputPath)"
PostBuild_Desc=Copying EXE to bin
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Header - Win32 Release"
# Name "Header - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\Common\ADCDAC.H
# End Source File
# Begin Source File

SOURCE=..\Common\ArrayPtr.hpp
# End Source File
# Begin Source File

SOURCE=..\Common\Axodebug.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\AXODEFN.H
# End Source File
# Begin Source File

SOURCE=..\Common\COLORS.H
# End Source File
# Begin Source File

SOURCE=.\Cparam.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\crc.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Common\crcmodel.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Common\FILEPATH.CPP
# End Source File
# Begin Source File

SOURCE=.\Header.cpp
# End Source File
# Begin Source File

SOURCE=.\header.rc
# End Source File
# Begin Source File

SOURCE=..\Common\Version.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\WIN32.H
# End Source File
# Begin Source File

SOURCE=.\Wincpp.cpp
# ADD CPP /Yc"wincpp.hpp"
# End Source File
# Begin Source File

SOURCE=..\..\Lib\Abflib.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\Axoutils32.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\AxonValidation.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ABFFILES.H
# End Source File
# Begin Source File

SOURCE=.\abfheadr.h
# End Source File
# Begin Source File

SOURCE=.\ABFINFO.H
# End Source File
# Begin Source File

SOURCE=.\ABFUTIL.H
# End Source File
# Begin Source File

SOURCE=.\AxABFFIO32.h
# End Source File
# Begin Source File

SOURCE=..\common\axodebug.h
# End Source File
# Begin Source File

SOURCE=.\CPARAM.H
# End Source File
# Begin Source File

SOURCE=..\Common\crc.h
# End Source File
# Begin Source File

SOURCE=..\Common\crcmodel.h
# End Source File
# Begin Source File

SOURCE=.\MSBINCVT.H
# End Source File
# Begin Source File

SOURCE=..\common\version.hpp
# End Source File
# Begin Source File

SOURCE=.\WINCPP.HPP
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
