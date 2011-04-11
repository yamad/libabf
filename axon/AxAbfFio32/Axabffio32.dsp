# Microsoft Developer Studio Project File - Name="axabffio32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AXABFFIO32 - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Axabffio32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Axabffio32.mak" CFG="AXABFFIO32 - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axabffio32 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axabffio32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

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
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /Yu"wincpp.hpp" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib version.lib ole32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBCMTD"
# Begin Special Build Tool
OutDir=.\Release
TargetName=Axabffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\axabffio32"
# PROP BASE Intermediate_Dir ".\axabffio32"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /FR /Yu"wincpp.hpp" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib version.lib ole32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT"
# Begin Special Build Tool
OutDir=.\Debug
TargetName=Axabffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axabffio32 - Win32 Release"
# Name "axabffio32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Abffiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Abfheadr.cpp
# End Source File
# Begin Source File

SOURCE=.\Abfhwave.cpp
# End Source File
# Begin Source File

SOURCE=.\Abfinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Abfutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Abfvtags.cpp
# End Source File
# Begin Source File

SOURCE=.\Axabffio32.cpp
# End Source File
# Begin Source File

SOURCE=.\axabffio32.def
# End Source File
# Begin Source File

SOURCE=.\axabffio32.rc
# End Source File
# Begin Source File

SOURCE=..\Common\Axodebug.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\BufferedArray.cpp
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

SOURCE=.\Csynch.cpp
# End Source File
# Begin Source File

SOURCE=.\DACFile.CPP
# End Source File
# Begin Source File

SOURCE=.\Filedesc.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\FileIO.CPP
# End Source File
# Begin Source File

SOURCE=..\Common\FILEPATH.CPP
# End Source File
# Begin Source File

SOURCE=..\Common\FileReadCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Msbincvt.cpp
# End Source File
# Begin Source File

SOURCE=.\Oldheadr.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\pkware.cpp
# End Source File
# Begin Source File

SOURCE=.\PopulateEpoch.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleStringCache.cpp
# End Source File
# Begin Source File

SOURCE=.\StringResource.CPP
# End Source File
# Begin Source File

SOURCE=.\TextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\UserList.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\VERSION.CPP
# End Source File
# Begin Source File

SOURCE=.\Voicetag.cpp
# End Source File
# Begin Source File

SOURCE=.\Wincpp.cpp
# ADD CPP /Yc"wincpp.hpp"
# End Source File
# Begin Source File

SOURCE=..\..\lib\Axoutils32.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Abffiles.h
# End Source File
# Begin Source File

SOURCE=.\abfheadr.h
# End Source File
# Begin Source File

SOURCE=.\ABFINFO.H
# End Source File
# Begin Source File

SOURCE=.\ABFOLDNX.H
# End Source File
# Begin Source File

SOURCE=.\abfutil.h
# End Source File
# Begin Source File

SOURCE=.\axabffio32.h
# End Source File
# Begin Source File

SOURCE=..\common\axodebug.h
# End Source File
# Begin Source File

SOURCE=..\common\BufferedArray.hpp
# End Source File
# Begin Source File

SOURCE=..\Common\crc.h
# End Source File
# Begin Source File

SOURCE=..\Common\crcmodel.h
# End Source File
# Begin Source File

SOURCE=.\csynch.hpp
# End Source File
# Begin Source File

SOURCE=.\DACFile.hpp
# End Source File
# Begin Source File

SOURCE=.\FILEDESC.HPP
# End Source File
# Begin Source File

SOURCE=..\common\FileIO.hpp
# End Source File
# Begin Source File

SOURCE=.\Msbincvt.h
# End Source File
# Begin Source File

SOURCE=.\notify.hpp
# End Source File
# Begin Source File

SOURCE=.\Oldheadr.h
# End Source File
# Begin Source File

SOURCE=..\Common\pkware.h
# End Source File
# Begin Source File

SOURCE=.\PopulateEpoch.hpp
# End Source File
# Begin Source File

SOURCE=.\SimpleStringCache.hpp
# End Source File
# Begin Source File

SOURCE=.\StringResource.h
# End Source File
# Begin Source File

SOURCE=.\TextBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\UserList.hpp
# End Source File
# Begin Source File

SOURCE=.\VOICETAG.HPP
# End Source File
# Begin Source File

SOURCE=.\Wincpp.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Abferror.str
# End Source File
# Begin Source File

SOURCE=.\AXABFFIO32.ICO
# End Source File
# Begin Source File

SOURCE=.\axabffio32.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Lib\AxonValidation.lib
# End Source File
# End Target
# End Project
