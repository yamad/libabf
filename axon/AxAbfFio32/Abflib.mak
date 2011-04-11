# Microsoft Developer Studio Generated NMAKE File, Based on Abflib.dsp
!IF "$(CFG)" == ""
CFG=ABFLIB - WIN32 DEBUG
!MESSAGE No configuration specified. Defaulting to ABFLIB - WIN32 DEBUG.
!ENDIF 

!IF "$(CFG)" != "Abflib - Win32 Release" && "$(CFG)" != "Abflib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Abflib - Win32 Release"

OUTDIR=.\LibRelease
INTDIR=.\LibRelease
# Begin Custom Macros
OutDir=.\LibRelease
# End Custom Macros

ALL : "$(OUTDIR)\Abflib.lib"


CLEAN :
	-@erase "$(INTDIR)\Abffiles.obj"
	-@erase "$(INTDIR)\Abfheadr.obj"
	-@erase "$(INTDIR)\Abfhwave.obj"
	-@erase "$(INTDIR)\Abfinfo.obj"
	-@erase "$(INTDIR)\Abfutil.obj"
	-@erase "$(INTDIR)\Abfvtags.obj"
	-@erase "$(INTDIR)\BufferedArray.obj"
	-@erase "$(INTDIR)\Csynch.obj"
	-@erase "$(INTDIR)\DACFile.obj"
	-@erase "$(INTDIR)\Filedesc.obj"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\FileReadCache.obj"
	-@erase "$(INTDIR)\Msbincvt.obj"
	-@erase "$(INTDIR)\Oldheadr.obj"
	-@erase "$(INTDIR)\pkware.obj"
	-@erase "$(INTDIR)\PopulateEpoch.obj"
	-@erase "$(INTDIR)\SimpleStringCache.obj"
	-@erase "$(INTDIR)\StringResource.obj"
	-@erase "$(INTDIR)\TextBuffer.obj"
	-@erase "$(INTDIR)\UserList.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Voicetag.obj"
	-@erase "$(OUTDIR)\Abflib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MT /W4 /Gi /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_ABFLIB_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Abflib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Abflib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Abffiles.obj" \
	"$(INTDIR)\Abfheadr.obj" \
	"$(INTDIR)\Abfhwave.obj" \
	"$(INTDIR)\Abfinfo.obj" \
	"$(INTDIR)\Abfutil.obj" \
	"$(INTDIR)\Abfvtags.obj" \
	"$(INTDIR)\BufferedArray.obj" \
	"$(INTDIR)\Csynch.obj" \
	"$(INTDIR)\DACFile.obj" \
	"$(INTDIR)\Filedesc.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\FileReadCache.obj" \
	"$(INTDIR)\Msbincvt.obj" \
	"$(INTDIR)\Oldheadr.obj" \
	"$(INTDIR)\pkware.obj" \
	"$(INTDIR)\PopulateEpoch.obj" \
	"$(INTDIR)\SimpleStringCache.obj" \
	"$(INTDIR)\StringResource.obj" \
	"$(INTDIR)\TextBuffer.obj" \
	"$(INTDIR)\UserList.obj" \
	"$(INTDIR)\Voicetag.obj"

"$(OUTDIR)\Abflib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

OutDir=.\LibRelease
TargetName=Abflib
SOURCE="$(InputPath)"
PostBuild_Desc=Copying LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\LibRelease
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\Abflib.lib"
   \AxonDev\Tools\Update .\LibRelease\Abflib lib
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Abflib - Win32 Debug"

OUTDIR=.\LibDebug
INTDIR=.\LibDebug
# Begin Custom Macros
OutDir=.\LibDebug
# End Custom Macros

ALL : "$(OUTDIR)\Abflib.lib"


CLEAN :
	-@erase "$(INTDIR)\Abffiles.obj"
	-@erase "$(INTDIR)\Abfheadr.obj"
	-@erase "$(INTDIR)\Abfhwave.obj"
	-@erase "$(INTDIR)\Abfinfo.obj"
	-@erase "$(INTDIR)\Abfutil.obj"
	-@erase "$(INTDIR)\Abfvtags.obj"
	-@erase "$(INTDIR)\BufferedArray.obj"
	-@erase "$(INTDIR)\Csynch.obj"
	-@erase "$(INTDIR)\DACFile.obj"
	-@erase "$(INTDIR)\Filedesc.obj"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\FileReadCache.obj"
	-@erase "$(INTDIR)\Msbincvt.obj"
	-@erase "$(INTDIR)\Oldheadr.obj"
	-@erase "$(INTDIR)\pkware.obj"
	-@erase "$(INTDIR)\PopulateEpoch.obj"
	-@erase "$(INTDIR)\SimpleStringCache.obj"
	-@erase "$(INTDIR)\StringResource.obj"
	-@erase "$(INTDIR)\TextBuffer.obj"
	-@erase "$(INTDIR)\UserList.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Voicetag.obj"
	-@erase "$(OUTDIR)\Abflib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_ABFLIB_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Abflib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Abflib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Abffiles.obj" \
	"$(INTDIR)\Abfheadr.obj" \
	"$(INTDIR)\Abfhwave.obj" \
	"$(INTDIR)\Abfinfo.obj" \
	"$(INTDIR)\Abfutil.obj" \
	"$(INTDIR)\Abfvtags.obj" \
	"$(INTDIR)\BufferedArray.obj" \
	"$(INTDIR)\Csynch.obj" \
	"$(INTDIR)\DACFile.obj" \
	"$(INTDIR)\Filedesc.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\FileReadCache.obj" \
	"$(INTDIR)\Msbincvt.obj" \
	"$(INTDIR)\Oldheadr.obj" \
	"$(INTDIR)\pkware.obj" \
	"$(INTDIR)\PopulateEpoch.obj" \
	"$(INTDIR)\SimpleStringCache.obj" \
	"$(INTDIR)\StringResource.obj" \
	"$(INTDIR)\TextBuffer.obj" \
	"$(INTDIR)\UserList.obj" \
	"$(INTDIR)\Voicetag.obj"

"$(OUTDIR)\Abflib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

OutDir=.\LibDebug
TargetName=Abflib
SOURCE="$(InputPath)"
PostBuild_Desc=Copying LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\LibDebug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\Abflib.lib"
   \AxonDev\Tools\Update .\LibDebug\Abflib lib
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Abflib.dep")
!INCLUDE "Abflib.dep"
!ELSE 
!MESSAGE Warning: cannot find "Abflib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Abflib - Win32 Release" || "$(CFG)" == "Abflib - Win32 Debug"
SOURCE=.\Abffiles.cpp

"$(INTDIR)\Abffiles.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Abfheadr.cpp

"$(INTDIR)\Abfheadr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Abfhwave.cpp

"$(INTDIR)\Abfhwave.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Abfinfo.cpp

"$(INTDIR)\Abfinfo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Abfutil.cpp

"$(INTDIR)\Abfutil.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Abfvtags.cpp

"$(INTDIR)\Abfvtags.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\BufferedArray.cpp

"$(INTDIR)\BufferedArray.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Csynch.cpp

"$(INTDIR)\Csynch.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DACFile.CPP

"$(INTDIR)\DACFile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Filedesc.cpp

"$(INTDIR)\Filedesc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\FileIO.CPP

"$(INTDIR)\FileIO.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\FileReadCache.cpp

"$(INTDIR)\FileReadCache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Msbincvt.cpp

"$(INTDIR)\Msbincvt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Oldheadr.cpp

"$(INTDIR)\Oldheadr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\pkware.cpp

"$(INTDIR)\pkware.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PopulateEpoch.cpp

"$(INTDIR)\PopulateEpoch.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SimpleStringCache.cpp

"$(INTDIR)\SimpleStringCache.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StringResource.CPP

"$(INTDIR)\StringResource.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TextBuffer.cpp

"$(INTDIR)\TextBuffer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UserList.cpp

"$(INTDIR)\UserList.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Voicetag.cpp

"$(INTDIR)\Voicetag.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

