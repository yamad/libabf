# Microsoft Developer Studio Generated NMAKE File, Based on HEADER.DSP
!IF "$(CFG)" == ""
CFG=Header - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Header - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Header - Win32 Release" && "$(CFG)" != "Header - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Header - Win32 Release"

OUTDIR=.\HeaderRelease
INTDIR=.\HeaderRelease
# Begin Custom Macros
OutDir=.\HeaderRelease
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\HEADER.exe"

!ELSE 

ALL : "AxoUtils32 - Win32 Release" "Abflib - Win32 Release" "$(OUTDIR)\HEADER.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Abflib - Win32 ReleaseCLEAN" "AxoUtils32 - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axodebug.obj"
	-@erase "$(INTDIR)\Cparam.obj"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\crcmodel.obj"
	-@erase "$(INTDIR)\FILEPATH.OBJ"
	-@erase "$(INTDIR)\Header.obj"
	-@erase "$(INTDIR)\HEADER.pch"
	-@erase "$(INTDIR)\header.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Version.obj"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\HEADER.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MT /W4 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\HEADER.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\header.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HEADER.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=oldnames.lib kernel32.lib user32.lib gdi32.lib winmm.lib winspool.lib version.lib setargv.obj ole32.lib /nologo /stack:0x1400 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\HEADER.pdb" /machine:IX86 /out:"$(OUTDIR)\HEADER.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Axodebug.obj" \
	"$(INTDIR)\Cparam.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\crcmodel.obj" \
	"$(INTDIR)\FILEPATH.OBJ" \
	"$(INTDIR)\Header.obj" \
	"$(INTDIR)\Version.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\header.res" \
	"..\..\Lib\Abflib.lib" \
	"..\..\Lib\Axoutils32.lib" \
	"..\..\Lib\AxonValidation.lib" \
	".\LibRelease\Abflib.lib" \
	"..\AxoUtils32\Release\Axoutils32.lib"

"$(OUTDIR)\HEADER.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\HeaderRelease
TargetName=HEADER
SOURCE="$(InputPath)"
PostBuild_Desc=Copying EXE to bin
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\HeaderRelease
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AxoUtils32 - Win32 Release" "Abflib - Win32 Release" "$(OUTDIR)\HEADER.exe"
   \AxonDev\Tools\Update .\HeaderRelease\HEADER exe
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

OUTDIR=.\HeaderDebug
INTDIR=.\HeaderDebug
# Begin Custom Macros
OutDir=.\HeaderDebug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\HEADER.exe"

!ELSE 

ALL : "AxoUtils32 - Win32 Debug" "Abflib - Win32 Debug" "$(OUTDIR)\HEADER.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Abflib - Win32 DebugCLEAN" "AxoUtils32 - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axodebug.obj"
	-@erase "$(INTDIR)\Cparam.obj"
	-@erase "$(INTDIR)\crc.obj"
	-@erase "$(INTDIR)\crcmodel.obj"
	-@erase "$(INTDIR)\FILEPATH.OBJ"
	-@erase "$(INTDIR)\Header.obj"
	-@erase "$(INTDIR)\HEADER.pch"
	-@erase "$(INTDIR)\header.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Version.obj"
	-@erase "$(INTDIR)\Wincpp.obj"
	-@erase "$(OUTDIR)\HEADER.exe"
	-@erase "$(OUTDIR)\HEADER.ilk"
	-@erase "$(OUTDIR)\HEADER.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\HEADER.pch" /Yu"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\header.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HEADER.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ole32.lib oldnames.lib kernel32.lib user32.lib gdi32.lib winmm.lib winspool.lib version.lib setargv.obj /nologo /stack:0x1400 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\HEADER.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\HEADER.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Axodebug.obj" \
	"$(INTDIR)\Cparam.obj" \
	"$(INTDIR)\crc.obj" \
	"$(INTDIR)\crcmodel.obj" \
	"$(INTDIR)\FILEPATH.OBJ" \
	"$(INTDIR)\Header.obj" \
	"$(INTDIR)\Version.obj" \
	"$(INTDIR)\Wincpp.obj" \
	"$(INTDIR)\header.res" \
	"..\..\Lib\Abflib.lib" \
	"..\..\Lib\Axoutils32.lib" \
	"..\..\Lib\AxonValidation.lib" \
	".\LibDebug\Abflib.lib" \
	"..\AxoUtils32\Debug\Axoutils32.lib"

"$(OUTDIR)\HEADER.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\HeaderDebug
TargetName=HEADER
SOURCE="$(InputPath)"
PostBuild_Desc=Copying EXE to bin
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\HeaderDebug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AxoUtils32 - Win32 Debug" "Abflib - Win32 Debug" "$(OUTDIR)\HEADER.exe"
   \AxonDev\Tools\Update .\HeaderDebug\HEADER exe
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
!IF EXISTS("HEADER.DEP")
!INCLUDE "HEADER.DEP"
!ELSE 
!MESSAGE Warning: cannot find "HEADER.DEP"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Header - Win32 Release" || "$(CFG)" == "Header - Win32 Debug"
SOURCE=..\Common\Axodebug.cpp

"$(INTDIR)\Axodebug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HEADER.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Cparam.cpp

"$(INTDIR)\Cparam.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HEADER.pch"


SOURCE=..\Common\crc.cpp

!IF  "$(CFG)" == "Header - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Common\crcmodel.c

!IF  "$(CFG)" == "Header - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crcmodel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\crcmodel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Common\FILEPATH.CPP

"$(INTDIR)\FILEPATH.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HEADER.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Header.cpp

"$(INTDIR)\Header.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HEADER.pch"


SOURCE=.\header.rc

"$(INTDIR)\header.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\Common\Version.cpp

"$(INTDIR)\Version.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HEADER.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Wincpp.cpp

!IF  "$(CFG)" == "Header - Win32 Release"

CPP_SWITCHES=/nologo /Zp4 /MT /W4 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\HEADER.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\HEADER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

CPP_SWITCHES=/nologo /Zp4 /MTd /W4 /Gm /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "WIN32_LEAN_AND_MEAN" /Fp"$(INTDIR)\HEADER.pch" /Yc"wincpp.hpp" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Wincpp.obj"	"$(INTDIR)\HEADER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "Header - Win32 Release"

"Abflib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\Abflib.mak CFG="Abflib - Win32 Release" 
   cd "."

"Abflib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\Abflib.mak CFG="Abflib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

"Abflib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\Abflib.mak CFG="Abflib - Win32 Debug" 
   cd "."

"Abflib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\Abflib.mak CFG="Abflib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "Header - Win32 Release"

"AxoUtils32 - Win32 Release" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Release" 
   cd "..\AxAbfFio32"

"AxoUtils32 - Win32 ReleaseCLEAN" : 
   cd "\AxonDev\Comp\AxoUtils32"
   $(MAKE) /$(MAKEFLAGS) /F .\Axoutils32.mak CFG="AxoUtils32 - Win32 Release" RECURSE=1 CLEAN 
   cd "..\AxAbfFio32"

!ELSEIF  "$(CFG)" == "Header - Win32 Debug"

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

