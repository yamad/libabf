# Microsoft Developer Studio Project File - Name="Abflib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ABFLIB - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Abflib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Abflib.mak" CFG="ABFLIB - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Abflib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Abflib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Abflib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\LibRelease"
# PROP Intermediate_Dir ".\LibRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Ox /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_DOS" /YX /c
# ADD CPP /nologo /Zp4 /MT /W4 /Gi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_ABFLIB_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
OutDir=.\LibRelease
TargetName=Abflib
SOURCE="$(InputPath)"
PostBuild_Desc=Copying LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Abflib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\LibDebug"
# PROP Intermediate_Dir ".\LibDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_DOS" /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_ABFLIB_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
OutDir=.\LibDebug
TargetName=Abflib
SOURCE="$(InputPath)"
PostBuild_Desc=Copying LIB
PostBuild_Cmds=\AxonDev\Tools\Update $(OutDir)\$(TargetName) lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Abflib - Win32 Release"
# Name "Abflib - Win32 Debug"
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

SOURCE=..\Common\BufferedArray.cpp
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

SOURCE=.\Voicetag.cpp
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

SOURCE=.\ABFOLDNX.H
# End Source File
# Begin Source File

SOURCE=.\abfutil.h
# End Source File
# Begin Source File

SOURCE=.\axabffio32.h
# End Source File
# Begin Source File

SOURCE=..\common\BufferedArray.hpp
# End Source File
# Begin Source File

SOURCE=.\csynch.hpp
# End Source File
# Begin Source File

SOURCE=.\FILEDESC.HPP
# End Source File
# Begin Source File

SOURCE=..\common\FileIO.hpp
# End Source File
# Begin Source File

SOURCE=.\MSBINCVT.H
# End Source File
# Begin Source File

SOURCE=.\notify.hpp
# End Source File
# Begin Source File

SOURCE=.\Oldheadr.h
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

SOURCE=.\WINCPP.HPP
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ABFERROR.STR
# End Source File
# End Group
# End Target
# End Project
