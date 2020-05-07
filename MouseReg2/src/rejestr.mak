# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=runreg - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to runreg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "rejestr - Win32 Release" && "$(CFG)" !=\
 "rejestr - Win32 Debug" && "$(CFG)" != "runreg - Win32 Release" && "$(CFG)" !=\
 "runreg - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "rejestr.mak" CFG="runreg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rejestr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rejestr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "runreg - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "runreg - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "rejestr - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "rejestr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\mousereg.exe" ".\Release\rejestr.bsc"

CLEAN : 
	-@erase ".\Release\rejestr.bsc"
	-@erase ".\Release\Dispatch.sbr"
	-@erase ".\Release\Rejestr.sbr"
	-@erase ".\Release\Init.sbr"
	-@erase ".\Release\Dib.sbr"
	-@erase ".\Release\Winmain.sbr"
	-@erase "..\..\..\wbin\mousereg.exe"
	-@erase ".\Release\Winmain.obj"
	-@erase ".\Release\Dispatch.obj"
	-@erase ".\Release\Rejestr.obj"
	-@erase ".\Release\Init.obj"
	-@erase ".\Release\Dib.obj"
	-@erase ".\Release\Rejestr.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /w /W0 /Gi /GX /I "c:\scc\include" /I "c:\scc\rtm" /I "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
CPP_PROJ=/nologo /ML /w /W0 /Gi /GX /I "c:\scc\include" /I "c:\scc\rtm" /I\
 "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/rejestr.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
RSC_PROJ=/l 0x415 /fo"$(INTDIR)/Rejestr.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/rejestr.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Dispatch.sbr" \
	"$(INTDIR)/Rejestr.sbr" \
	"$(INTDIR)/Init.sbr" \
	"$(INTDIR)/Dib.sbr" \
	"$(INTDIR)/Winmain.sbr"

".\Release\rejestr.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"c:\wbin\mousereg.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/mousereg.pdb" /machine:I386 /out:"c:\wbin\mousereg.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Winmain.obj" \
	"$(INTDIR)/Dispatch.obj" \
	"$(INTDIR)/Rejestr.obj" \
	"$(INTDIR)/Init.obj" \
	"$(INTDIR)/Dib.obj" \
	"$(INTDIR)/Rejestr.res" \
	"..\wbrtmmvc\Release\wbrtmmvc.lib"

"$(OUTDIR)\mousereg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "rejestr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\mousereg.exe" "$(OUTDIR)\rejestr.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\rejestr.bsc"
	-@erase ".\Debug\Rejestr.sbr"
	-@erase ".\Debug\Init.sbr"
	-@erase ".\Debug\Winmain.sbr"
	-@erase ".\Debug\Dispatch.sbr"
	-@erase ".\Debug\Dib.sbr"
	-@erase ".\debug\mousereg.exe"
	-@erase ".\Debug\Dib.obj"
	-@erase ".\Debug\Rejestr.obj"
	-@erase ".\Debug\Init.obj"
	-@erase ".\Debug\Winmain.obj"
	-@erase ".\Debug\Dispatch.obj"
	-@erase ".\Debug\Rejestr.res"
	-@erase ".\debug\mousereg.ilk"
	-@erase ".\Debug\mousereg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /w /W0 /Gm /Gi /GX /Zi /Gf /Gy /I "c:\scc\include" /I "c:\scc\rtm" /I "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
CPP_PROJ=/nologo /MLd /w /W0 /Gm /Gi /GX /Zi /Gf /Gy /I "c:\scc\include" /I\
 "c:\scc\rtm" /I "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/rejestr.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
RSC_PROJ=/l 0x415 /fo"$(INTDIR)/Rejestr.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/rejestr.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Rejestr.sbr" \
	"$(INTDIR)/Init.sbr" \
	"$(INTDIR)/Winmain.sbr" \
	"$(INTDIR)/Dispatch.sbr" \
	"$(INTDIR)/Dib.sbr"

"$(OUTDIR)\rejestr.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:\msdev\projects\rejestr\debug\mousereg.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/mousereg.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/mousereg.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Dib.obj" \
	"$(INTDIR)/Rejestr.obj" \
	"$(INTDIR)/Init.obj" \
	"$(INTDIR)/Winmain.obj" \
	"$(INTDIR)/Dispatch.obj" \
	"$(INTDIR)/Rejestr.res" \
	"..\wbrtmmvc\Release\wbrtmmvc.lib"

"$(OUTDIR)\mousereg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "runreg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "runreg\Release"
# PROP BASE Intermediate_Dir "runreg\Release"
# PROP BASE Target_Dir "runreg"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "runreg\Release"
# PROP Intermediate_Dir "runreg\Release"
# PROP Target_Dir "runreg"
OUTDIR=.\runreg\Release
INTDIR=.\runreg\Release

ALL : 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /GX /O2 /I "c:\scc\rtm" /I "c:\scc\include" /I "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /GX /O2 /I "c:\scc\rtm" /I "c:\scc\include" /I\
 "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/runreg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\runreg\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/runreg.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"c:\wbin\runreg.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/runreg.pdb" /machine:I386 /out:"c:\wbin\runreg.exe" 
LINK32_OBJS=

!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "runreg\Debug"
# PROP BASE Intermediate_Dir "runreg\Debug"
# PROP BASE Target_Dir "runreg"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "runreg\Debug"
# PROP Intermediate_Dir "runreg\Debug"
# PROP Target_Dir "runreg"
OUTDIR=.\runreg\Debug
INTDIR=.\runreg\Debug

ALL : 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gm /GX /Zi /Od /I "c:\scc\rtm" /I "c:\scc\include" /I "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /Gm /GX /Zi /Od /I "c:\scc\rtm" /I "c:\scc\include" /I\
 "c:\scc\rtm\symshell.oth\windows" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/runreg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\runreg\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/runreg.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/runreg.pdb" /debug /machine:I386 /out:"$(OUTDIR)/runreg.exe" 
LINK32_OBJS=

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "rejestr - Win32 Release"
# Name "rejestr - Win32 Debug"

!IF  "$(CFG)" == "rejestr - Win32 Release"

!ELSEIF  "$(CFG)" == "rejestr - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Rejestr.ico

!IF  "$(CFG)" == "rejestr - Win32 Release"

!ELSEIF  "$(CFG)" == "rejestr - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rejestr.rc
DEP_RSC_REJES=\
	".\Rejestr.ico"\
	".\Resource.h"\
	".\Globals.h"\
	".\Version.h"\
	

"$(INTDIR)\Rejestr.res" : $(SOURCE) $(DEP_RSC_REJES) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Msdev\Projects\wbrtmmvc\Release\wbrtmmvc.lib

!IF  "$(CFG)" == "rejestr - Win32 Release"

!ELSEIF  "$(CFG)" == "rejestr - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Init.cxx
DEP_CPP_INIT_=\
	".\win16ext.h"\
	".\Globals.h"\
	".\Resource.h"\
	

"$(INTDIR)\Init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"

"$(INTDIR)\Init.sbr" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rejestr.cxx

!IF  "$(CFG)" == "rejestr - Win32 Release"

DEP_CPP_REJEST=\
	".\win16ext.h"\
	".\Dib.h"\
	"c:\scc\include\uses_wb.hpp"\
	".\Globals.h"\
	".\Resource.h"\
	"c:\scc\rtm\autor.h"\
	

"$(INTDIR)\Rejestr.obj" : $(SOURCE) $(DEP_CPP_REJEST) "$(INTDIR)"

"$(INTDIR)\Rejestr.sbr" : $(SOURCE) $(DEP_CPP_REJEST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "rejestr - Win32 Debug"

DEP_CPP_REJEST=\
	".\win16ext.h"\
	".\Dib.h"\
	"c:\scc\include\uses_wb.hpp"\
	".\Globals.h"\
	".\Resource.h"\
	"c:\scc\rtm\autor.h"\
	".\..\..\..\Scc\Include\platform.hpp"\
	".\..\..\..\Scc\Include\impl_wb.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\..\..\Scc\Include\platform.h"\
	

"$(INTDIR)\Rejestr.obj" : $(SOURCE) $(DEP_CPP_REJEST) "$(INTDIR)"

"$(INTDIR)\Rejestr.sbr" : $(SOURCE) $(DEP_CPP_REJEST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dispatch.cxx
DEP_CPP_DISPA=\
	".\win16ext.h"\
	".\Globals.h"\
	

"$(INTDIR)\Dispatch.obj" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"

"$(INTDIR)\Dispatch.sbr" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Winmain.cxx
DEP_CPP_WINMA=\
	".\win16ext.h"\
	".\Globals.h"\
	

"$(INTDIR)\Winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"

"$(INTDIR)\Winmain.sbr" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dib.c
DEP_CPP_DIB_C=\
	".\stdwin.h"\
	".\Dib.h"\
	

"$(INTDIR)\Dib.obj" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"

"$(INTDIR)\Dib.sbr" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "runreg - Win32 Release"
# Name "runreg - Win32 Debug"

!IF  "$(CFG)" == "runreg - Win32 Release"

!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"

!ENDIF 

# End Target
# End Project
################################################################################
