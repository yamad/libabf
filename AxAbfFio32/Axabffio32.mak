# Microsoft Developer Studio Generated NMAKE File, Based on Axabffio32.dsp
!IF "$(CFG)" == ""
CFG=AXABFFIO32 - WIN32 DEBUG
!MESSAGE No configuration specified. Defaulting to AXABFFIO32 - WIN32 DEBUG.
!ENDIF 

!IF "$(CFG)" != "axabffio32 - Win32 Release" && "$(CFG)" != "axabffio32 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Axabffio32.dll"

!ELSE 

ALL : "AxoUtils32 - Win32 Release" "$(OUTDIR)\Axabffio32.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"AxoUtils32 - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Abffiles.obj"
	-@erase "$(INTDIR)\Abfheadr.obj"
	-@erase "$(INTDIR)\Abfhwave.obj"
	-@erase "$(INTDIR)\Abfinfo.obj"
	-@erase "$(INTDIR)\Abfutil.obj"
	-@erase "$(INTDIR)\Abfvtags.obj"
	-@erase "$(INTDIR)\Axabffio32.obj"
	-@erase "$(INTDIR)\Axabffio32.pch"
	-@erase "$(INTDIR)\axabffio32.res"
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\BufferedArray.obj"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\crcmodel.obj"
	-@erase "$(INTDIR)\Csynch.obj"
	-@erase "$(INTDIR)\DACFile.obj"
	-@erase "$(INTDIR)\Filedesc.obj"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\filepath.obj"
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
	-@erase "$(INTDIR)\version.obj"
	-@erase "$(INTDIR)\Voicetag.obj"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\Axabffio32.dll"
	-@erase "$(OUTDIR)\Axabffio32.exp"
	-@erase "$(OUTDIR)\Axabffio32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /Fp"$(INTDIR)\Axabffio32.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\axabffio32.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Axabffio32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib version.lib ole32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Axabffio32.pdb" /machine:I386 /nodefaultlib:"LIBCMTD" /def:".\axabffio32.def" /out:"$(OUTDIR)\Axabffio32.dll" /implib:"$(OUTDIR)\Axabffio32.lib" 
DEF_FILE= \
	".\axabffio32.def"
LINK32_OBJS= \
	"$(INTDIR)\Abffiles.obj" \
	"$(INTDIR)\Abfheadr.obj" \
	"$(INTDIR)\Abfhwave.obj" \
	"$(INTDIR)\Abfinfo.obj" \
	"$(INTDIR)\Abfutil.obj" \
	"$(INTDIR)\Abfvtags.obj" \
	"$(INTDIR)\Axabffio32.obj" \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\BufferedArray.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\crcmodel.obj" \
	"$(INTDIR)\Csynch.obj" \
	"$(INTDIR)\DACFile.obj" \
	"$(INTDIR)\Filedesc.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\filepath.obj" \
	"$(INTDIR)\FileReadCache.obj" \
	"$(INTDIR)\Msbincvt.obj" \
	"$(INTDIR)\Oldheadr.obj" \
	"$(INTDIR)\pkware.obj" \
	"$(INTDIR)\PopulateEpoch.obj" \
	"$(INTDIR)\SimpleStringCache.obj" \
	"$(INTDIR)\StringResource.obj" \
	"$(INTDIR)\TextBuffer.obj" \
	"$(INTDIR)\UserList.obj" \
	"$(INTDIR)\version.obj" \
	"$(INTDIR)\Voicetag.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\axabffio32.res" \
	"..\..\lib\Axoutils32.lib" \
	"..\..\Lib\AxonValidation.lib" \
	"..\AxoUtils32\Release\Axoutils32.lib"

"$(OUTDIR)\Axabffio32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetName=Axabffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AxoUtils32 - Win32 Release" "$(OUTDIR)\Axabffio32.dll"
   \AxonDev\Tools\Update .\Release\Axabffio32 dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Axabffio32.dll" "$(OUTDIR)\Axabffio32.bsc"

!ELSE 

ALL : "AxoUtils32 - Win32 Debug" "$(OUTDIR)\Axabffio32.dll" "$(OUTDIR)\Axabffio32.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"AxoUtils32 - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Abffiles.obj"
	-@erase "$(INTDIR)\Abffiles.sbr"
	-@erase "$(INTDIR)\Abfheadr.obj"
	-@erase "$(INTDIR)\Abfheadr.sbr"
	-@erase "$(INTDIR)\Abfhwave.obj"
	-@erase "$(INTDIR)\Abfhwave.sbr"
	-@erase "$(INTDIR)\Abfinfo.obj"
	-@erase "$(INTDIR)\Abfinfo.sbr"
	-@erase "$(INTDIR)\Abfutil.obj"
	-@erase "$(INTDIR)\Abfutil.sbr"
	-@erase "$(INTDIR)\Abfvtags.obj"
	-@erase "$(INTDIR)\Abfvtags.sbr"
	-@erase "$(INTDIR)\Axabffio32.obj"
	-@erase "$(INTDIR)\Axabffio32.pch"
	-@erase "$(INTDIR)\axabffio32.res"
	-@erase "$(INTDIR)\Axabffio32.sbr"
	-@erase "$(INTDIR)\axodebug.obj"
	-@erase "$(INTDIR)\axodebug.sbr"
	-@erase "$(INTDIR)\BufferedArray.obj"
	-@erase "$(INTDIR)\BufferedArray.sbr"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\crc.sbr"
	-@erase "$(INTDIR)\crcmodel.obj"
	-@erase "$(INTDIR)\crcmodel.sbr"
	-@erase "$(INTDIR)\Csynch.obj"
	-@erase "$(INTDIR)\Csynch.sbr"
	-@erase "$(INTDIR)\DACFile.obj"
	-@erase "$(INTDIR)\DACFile.sbr"
	-@erase "$(INTDIR)\Filedesc.obj"
	-@erase "$(INTDIR)\Filedesc.sbr"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\FileIO.sbr"
	-@erase "$(INTDIR)\filepath.obj"
	-@erase "$(INTDIR)\filepath.sbr"
	-@erase "$(INTDIR)\FileReadCache.obj"
	-@erase "$(INTDIR)\FileReadCache.sbr"
	-@erase "$(INTDIR)\Msbincvt.obj"
	-@erase "$(INTDIR)\Msbincvt.sbr"
	-@erase "$(INTDIR)\Oldheadr.obj"
	-@erase "$(INTDIR)\Oldheadr.sbr"
	-@erase "$(INTDIR)\pkware.obj"
	-@erase "$(INTDIR)\pkware.sbr"
	-@erase "$(INTDIR)\PopulateEpoch.obj"
	-@erase "$(INTDIR)\PopulateEpoch.sbr"
	-@erase "$(INTDIR)\SimpleStringCache.obj"
	-@erase "$(INTDIR)\SimpleStringCache.sbr"
	-@erase "$(INTDIR)\StringResource.obj"
	-@erase "$(INTDIR)\StringResource.sbr"
	-@erase "$(INTDIR)\TextBuffer.obj"
	-@erase "$(INTDIR)\TextBuffer.sbr"
	-@erase "$(INTDIR)\UserList.obj"
	-@erase "$(INTDIR)\UserList.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\version.obj"
	-@erase "$(INTDIR)\version.sbr"
	-@erase "$(INTDIR)\Voicetag.obj"
	-@erase "$(INTDIR)\Voicetag.sbr"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(INTDIR)\Wincpp.sbr"
	-@erase "$(OUTDIR)\Axabffio32.bsc"
	-@erase "$(OUTDIR)\Axabffio32.dll"
	-@erase "$(OUTDIR)\Axabffio32.exp"
	-@erase "$(OUTDIR)\Axabffio32.ilk"
	-@erase "$(OUTDIR)\Axabffio32.lib"
	-@erase "$(OUTDIR)\Axabffio32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Axabffio32.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\axabffio32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Axabffio32.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Abffiles.sbr" \
	"$(INTDIR)\Abfheadr.sbr" \
	"$(INTDIR)\Abfhwave.sbr" \
	"$(INTDIR)\Abfinfo.sbr" \
	"$(INTDIR)\Abfutil.sbr" \
	"$(INTDIR)\Abfvtags.sbr" \
	"$(INTDIR)\Axabffio32.sbr" \
	"$(INTDIR)\axodebug.sbr" \
	"$(INTDIR)\BufferedArray.sbr" \
	"$(INTDIR)\crc.sbr" \
	"$(INTDIR)\crcmodel.sbr" \
	"$(INTDIR)\Csynch.sbr" \
	"$(INTDIR)\DACFile.sbr" \
	"$(INTDIR)\Filedesc.sbr" \
	"$(INTDIR)\FileIO.sbr" \
	"$(INTDIR)\filepath.sbr" \
	"$(INTDIR)\FileReadCache.sbr" \
	"$(INTDIR)\Msbincvt.sbr" \
	"$(INTDIR)\Oldheadr.sbr" \
	"$(INTDIR)\pkware.sbr" \
	"$(INTDIR)\PopulateEpoch.sbr" \
	"$(INTDIR)\SimpleStringCache.sbr" \
	"$(INTDIR)\StringResource.sbr" \
	"$(INTDIR)\TextBuffer.sbr" \
	"$(INTDIR)\UserList.sbr" \
	"$(INTDIR)\version.sbr" \
	"$(INTDIR)\Voicetag.sbr" \
	"$(INTDIR)\Wincpp.sbr"

"$(OUTDIR)\Axabffio32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib version.lib ole32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Axabffio32.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /def:".\axabffio32.def" /out:"$(OUTDIR)\Axabffio32.dll" /implib:"$(OUTDIR)\Axabffio32.lib" 
DEF_FILE= \
	".\axabffio32.def"
LINK32_OBJS= \
	"$(INTDIR)\Abffiles.obj" \
	"$(INTDIR)\Abfheadr.obj" \
	"$(INTDIR)\Abfhwave.obj" \
	"$(INTDIR)\Abfinfo.obj" \
	"$(INTDIR)\Abfutil.obj" \
	"$(INTDIR)\Abfvtags.obj" \
	"$(INTDIR)\Axabffio32.obj" \
	"$(INTDIR)\axodebug.obj" \
	"$(INTDIR)\BufferedArray.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\crcmodel.obj" \
	"$(INTDIR)\Csynch.obj" \
	"$(INTDIR)\DACFile.obj" \
	"$(INTDIR)\Filedesc.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\filepath.obj" \
	"$(INTDIR)\FileReadCache.obj" \
	"$(INTDIR)\Msbincvt.obj" \
	"$(INTDIR)\Oldheadr.obj" \
	"$(INTDIR)\pkware.obj" \
	"$(INTDIR)\PopulateEpoch.obj" \
	"$(INTDIR)\SimpleStringCache.obj" \
	"$(INTDIR)\StringResource.obj" \
	"$(INTDIR)\TextBuffer.obj" \
	"$(INTDIR)\UserList.obj" \
	"$(INTDIR)\version.obj" \
	"$(INTDIR)\Voicetag.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\axabffio32.res" \
	"..\..\lib\Axoutils32.lib" \
	"..\..\Lib\AxonValidation.lib" \
	"..\AxoUtils32\Debug\Axoutils32.lib"

"$(OUTDIR)\Axabffio32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetName=Axabffio32
SOURCE="$(InputPath)"
PostBuild_Desc=Copying DLL & LIB
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AxoUtils32 - Win32 Debug" "$(OUTDIR)\Axabffio32.dll" "$(OUTDIR)\Axabffio32.bsc"
   \AxonDev\Tools\Update .\Debug\Axabffio32 dll
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
!IF EXISTS("Axabffio32.dep")
!INCLUDE "Axabffio32.dep"
!ELSE 
!MESSAGE Warning: cannot find "Axabffio32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axabffio32 - Win32 Release" || "$(CFG)" == "axabffio32 - Win32 Debug"
SOURCE=.\Abffiles.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abffiles.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abffiles.obj"	"$(INTDIR)\Abffiles.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Abfheadr.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abfheadr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abfheadr.obj"	"$(INTDIR)\Abfheadr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Abfhwave.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abfhwave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abfhwave.obj"	"$(INTDIR)\Abfhwave.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Abfinfo.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abfinfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abfinfo.obj"	"$(INTDIR)\Abfinfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Abfutil.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abfutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abfutil.obj"	"$(INTDIR)\Abfutil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Abfvtags.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Abfvtags.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Abfvtags.obj"	"$(INTDIR)\Abfvtags.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Axabffio32.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Axabffio32.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Axabffio32.obj"	"$(INTDIR)\Axabffio32.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\axabffio32.rc

"$(INTDIR)\axabffio32.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\Common\axodebug.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\axodebug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\axodebug.obj"	"$(INTDIR)\axodebug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Common\BufferedArray.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\BufferedArray.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\BufferedArray.obj"	"$(INTDIR)\BufferedArray.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Common\crc.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crc.obj"	"$(INTDIR)\crc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Common\crcmodel.c

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crcmodel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crcmodel.obj"	"$(INTDIR)\crcmodel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Csynch.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Csynch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Csynch.obj"	"$(INTDIR)\Csynch.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\DACFile.CPP

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\DACFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\DACFile.obj"	"$(INTDIR)\DACFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Filedesc.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Filedesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Filedesc.obj"	"$(INTDIR)\Filedesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=..\Common\FileIO.CPP

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\FileIO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\FileIO.obj"	"$(INTDIR)\FileIO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Common\filepath.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\filepath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\filepath.obj"	"$(INTDIR)\filepath.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Common\FileReadCache.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\FileReadCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\FileReadCache.obj"	"$(INTDIR)\FileReadCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Msbincvt.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Msbincvt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Msbincvt.obj"	"$(INTDIR)\Msbincvt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Oldheadr.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Oldheadr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Oldheadr.obj"	"$(INTDIR)\Oldheadr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=..\Common\pkware.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\pkware.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\pkware.obj"	"$(INTDIR)\pkware.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\PopulateEpoch.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\PopulateEpoch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\PopulateEpoch.obj"	"$(INTDIR)\PopulateEpoch.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\SimpleStringCache.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\SimpleStringCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\SimpleStringCache.obj"	"$(INTDIR)\SimpleStringCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\StringResource.CPP

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\StringResource.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\StringResource.obj"	"$(INTDIR)\StringResource.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\TextBuffer.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\TextBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\TextBuffer.obj"	"$(INTDIR)\TextBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\UserList.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\UserList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\UserList.obj"	"$(INTDIR)\UserList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=..\Common\version.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\version.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\version.obj"	"$(INTDIR)\version.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Voicetag.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"


"$(INTDIR)\Voicetag.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"


"$(INTDIR)\Voicetag.obj"	"$(INTDIR)\Voicetag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Axabffio32.pch"


!ENDIF 

SOURCE=.\Wincpp.cpp

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /Fp"$(INTDIR)\Axabffio32.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\Axabffio32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "MAK_AXABFFIO32_DLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Axabffio32.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\Wincpp.sbr"	"$(INTDIR)\Axabffio32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "axabffio32 - Win32 Release"

"AxoUtils32 - Win32 Release" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Release" 
   cd "..\AxAbfFio32"

"AxoUtils32 - Win32 ReleaseCLEAN" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\AxAbfFio32"

!ELSEIF  "$(CFG)" == "axabffio32 - Win32 Debug"

"AxoUtils32 - Win32 Debug" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Debug" 
   cd "..\AxAbfFio32"

"AxoUtils32 - Win32 DebugCLEAN" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\AxAbfFio32"

!ENDIF 


!ENDIF 

