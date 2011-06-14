# Microsoft Developer Studio Generated NMAKE File, Based on axatffio32.dsp
!IF "$(CFG)" == ""
CFG=axatffio32 - Win32 DebugMD
!MESSAGE No configuration specified. Defaulting to axatffio32 - Win32 DebugMD.
!ENDIF 

!IF "$(CFG)" != "axatffio32 - Win32 Release" && "$(CFG)" != "axatffio32 - Win32 Debug" && "$(CFG)" != "axatffio32 - Win32 DebugMD"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axatffio32 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\axatffio32.dll"


CLEAN :
	-@erase "$(INTDIR)\Axatffio32.obj"
	-@erase "$(INTDIR)\axatffio32.pch"
	-@erase "$(INTDIR)\Axatffio32.res"
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\Initlize.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\axatffio32.dll"
	-@erase "$(OUTDIR)\axatffio32.exp"
	-@erase "$(OUTDIR)\axatffio32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MT /W4 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\Axatffio32.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axatffio32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axatffio32.pdb" /machine:I386 /nodefaultlib:"LIBCMTD" /def:".\axatffio32.def" /out:"$(OUTDIR)\axatffio32.dll" /implib:"$(OUTDIR)\axatffio32.lib" 
DEF_FILE= \
	".\axatffio32.def"
LINK32_OBJS= \
	"$(INTDIR)\Axatffio32.obj" \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\fileio.obj" \
	"$(INTDIR)\Initlize.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\Axatffio32.res" \
	"..\..\lib\AxonValidation.lib"

"$(OUTDIR)\axatffio32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\axatffio32.dll"
   \AxonDev\Tools\Update .\Release\axatffio32 dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "axatffio32 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\axatffio32.dll"


CLEAN :
	-@erase "$(INTDIR)\Axatffio32.obj"
	-@erase "$(INTDIR)\axatffio32.pch"
	-@erase "$(INTDIR)\Axatffio32.res"
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\Initlize.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\axatffio32.dll"
	-@erase "$(OUTDIR)\axatffio32.exp"
	-@erase "$(OUTDIR)\axatffio32.ilk"
	-@erase "$(OUTDIR)\axatffio32.lib"
	-@erase "$(OUTDIR)\axatffio32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\Axatffio32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axatffio32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axatffio32.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /def:".\axatffio32.def" /out:"$(OUTDIR)\axatffio32.dll" /implib:"$(OUTDIR)\axatffio32.lib" 
DEF_FILE= \
	".\axatffio32.def"
LINK32_OBJS= \
	"$(INTDIR)\Axatffio32.obj" \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\fileio.obj" \
	"$(INTDIR)\Initlize.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\Axatffio32.res" \
	"..\..\lib\AxonValidation.lib"

"$(OUTDIR)\axatffio32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\axatffio32.dll"
   \AxonDev\Tools\Update .\Debug\axatffio32 dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "axatffio32 - Win32 DebugMD"

OUTDIR=.\DebugMD
INTDIR=.\DebugMD
# Begin Custom Macros
OutDir=.\DebugMD
# End Custom Macros

ALL : "$(OUTDIR)\axatffio32.dll"


CLEAN :
	-@erase "$(INTDIR)\Axatffio32.obj"
	-@erase "$(INTDIR)\axatffio32.pch"
	-@erase "$(INTDIR)\Axatffio32.res"
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\Initlize.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\axatffio32.dll"
	-@erase "$(OUTDIR)\axatffio32.exp"
	-@erase "$(OUTDIR)\axatffio32.ilk"
	-@erase "$(OUTDIR)\axatffio32.lib"
	-@erase "$(OUTDIR)\axatffio32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\Axatffio32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axatffio32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axatffio32.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /def:".\axatffio32.def" /out:"$(OUTDIR)\axatffio32.dll" /implib:"$(OUTDIR)\axatffio32.lib" 
DEF_FILE= \
	".\axatffio32.def"
LINK32_OBJS= \
	"..\..\lib\AxonValidation.lib" \
	"$(INTDIR)\Axatffio32.obj" \
	"$(INTDIR)\Axatffio32.res" \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\fileio.obj" \
	"$(INTDIR)\Initlize.obj" \
	"$(INTDIR)\Wincpp.obj"

"$(OUTDIR)\axatffio32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\DebugMD
TargetName=axatffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\DebugMD
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\axatffio32.dll"
   \AxonDev\Tools\Update .\DebugMD\axatffio32 dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axatffio32.dep")
!INCLUDE "axatffio32.dep"
!ELSE 
!MESSAGE Warning: cannot find "axatffio32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axatffio32 - Win32 Release" || "$(CFG)" == "axatffio32 - Win32 Debug" || "$(CFG)" == "axatffio32 - Win32 DebugMD"
SOURCE=.\Axatffio32.cpp

"$(INTDIR)\Axatffio32.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axatffio32.pch"


SOURCE=.\Axatffio32.rc

"$(INTDIR)\Axatffio32.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\Common\axodebug.cpp

"$(INTDIR)\axodebug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axatffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fileio.CPP

"$(INTDIR)\fileio.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axatffio32.pch"


SOURCE=.\Initlize.cpp

"$(INTDIR)\Initlize.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axatffio32.pch"


SOURCE=.\Wincpp.cpp

!IF  "$(CFG)" == "axatffio32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\axatffio32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axatffio32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\axatffio32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axatffio32 - Win32 DebugMD"

CPP_SWITCHES=/nologo /Zp4 /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "_WINDLL" /D "MAK_AXATFFIO32_DLL" /Fp"$(INTDIR)\axatffio32.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\axatffio32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

