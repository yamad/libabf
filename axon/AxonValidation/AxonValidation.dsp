# Microsoft Developer Studio Project File - Name="AxonValidation" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AxonValidation - Win32 DebugMD
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AxonValidation.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AxonValidation.mak" CFG="AxonValidation - Win32 DebugMD"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AxonValidation - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AxonValidation - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AxonValidation - Win32 DebugMD" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AxonValidation - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /YX /FD /c
# ADD BASE RSC /l 0xc09
# ADD RSC /l 0xc09
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
OutDir=.\Release
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
PostBuild_Cmds=\AxonDev\Tools\update $(OutDir)\$(TargetName) lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AxonValidation - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /YX /FD /c
# ADD BASE RSC /l 0xc09
# ADD RSC /l 0xc09
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
OutDir=.\Debug
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
PostBuild_Cmds=\AxonDev\Tools\update $(OutDir)\$(TargetName) lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AxonValidation - Win32 DebugMD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AxonValidation___Win32_DebugMD0"
# PROP BASE Intermediate_Dir "AxonValidation___Win32_DebugMD0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugMD"
# PROP Intermediate_Dir "DebugMD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W4 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /YX /FD /c
# ADD CPP /nologo /Zp4 /MDd /W4 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /YX /FD /c
# ADD BASE RSC /l 0xc09
# ADD RSC /l 0xc09
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
OutDir=.\DebugMD
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
PostBuild_Cmds=\AxonDev\Tools\update $(OutDir)\$(TargetName) lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "AxonValidation - Win32 Release"
# Name "AxonValidation - Win32 Debug"
# Name "AxonValidation - Win32 DebugMD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\Common\Axodebug.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\TextFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\Validation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\common\axodebug.h
# End Source File
# Begin Source File

SOURCE=.\AxonValidation.h
# End Source File
# Begin Source File

SOURCE=.\wincpp.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
