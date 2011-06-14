# Microsoft Developer Studio Generated NMAKE File, Based on AxonValidation.dsp
!IF "$(CFG)" == ""
CFG=AxonValidation - Win32 DebugMD
!MESSAGE No configuration specified. Defaulting to AxonValidation - Win32 DebugMD.
!ENDIF 

!IF "$(CFG)" != "AxonValidation - Win32 Release" && "$(CFG)" != "AxonValidation - Win32 Debug" && "$(CFG)" != "AxonValidation - Win32 DebugMD"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AxonValidation - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\AxonValidation.lib"


CLEAN :
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\FloatFormat.obj"
	-@erase "$(INTDIR)\TextFormat.obj"
	-@erase "$(INTDIR)\Validation.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AxonValidation.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MT /W4 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\AxonValidation.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AxonValidation.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\AxonValidation.lib" 
LIB32_OBJS= \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\FloatFormat.obj" \
	"$(INTDIR)\TextFormat.obj" \
	"$(INTDIR)\Validation.obj"

"$(OUTDIR)\AxonValidation.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

OutDir=.\Release
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\AxonValidation.lib"
   \AxonDev\Tools\update .\Release\AxonValidation lib
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "AxonValidation - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\AxonValidation.lib"


CLEAN :
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\FloatFormat.obj"
	-@erase "$(INTDIR)\TextFormat.obj"
	-@erase "$(INTDIR)\Validation.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AxonValidation.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MTd /W4 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\AxonValidation.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AxonValidation.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\AxonValidation.lib" 
LIB32_OBJS= \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\FloatFormat.obj" \
	"$(INTDIR)\TextFormat.obj" \
	"$(INTDIR)\Validation.obj"

"$(OUTDIR)\AxonValidation.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

OutDir=.\Debug
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\AxonValidation.lib"
   \AxonDev\Tools\update .\Debug\AxonValidation lib
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "AxonValidation - Win32 DebugMD"

OUTDIR=.\DebugMD
INTDIR=.\DebugMD
# Begin Custom Macros
OutDir=.\DebugMD
# End Custom Macros

ALL : "$(OUTDIR)\AxonValidation.lib"


CLEAN :
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\FloatFormat.obj"
	-@erase "$(INTDIR)\TextFormat.obj"
	-@erase "$(INTDIR)\Validation.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AxonValidation.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W4 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\AxonValidation.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AxonValidation.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\AxonValidation.lib" 
LIB32_OBJS= \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\FloatFormat.obj" \
	"$(INTDIR)\TextFormat.obj" \
	"$(INTDIR)\Validation.obj"

"$(OUTDIR)\AxonValidation.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

OutDir=.\DebugMD
TargetName=AxonValidation
SOURCE="$(InputPath)"
PostBuild_Desc=Copying lib to \axon\lib
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\DebugMD
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\AxonValidation.lib"
   \AxonDev\Tools\update .\DebugMD\AxonValidation lib
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
!IF EXISTS("AxonValidation.dep")
!INCLUDE "AxonValidation.dep"
!ELSE 
!MESSAGE Warning: cannot find "AxonValidation.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AxonValidation - Win32 Release" || "$(CFG)" == "AxonValidation - Win32 Debug" || "$(CFG)" == "AxonValidation - Win32 DebugMD"
SOURCE=..\Common\axodebug.cpp

"$(INTDIR)\axodebug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\FloatFormat.cpp

"$(INTDIR)\FloatFormat.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TextFormat.cpp

"$(INTDIR)\TextFormat.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Validation.cpp

"$(INTDIR)\Validation.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

