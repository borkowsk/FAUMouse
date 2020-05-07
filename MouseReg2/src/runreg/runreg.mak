# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=runreg - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to runreg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "runreg - Win32 Release" && "$(CFG)" != "runreg - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "runreg.mak" CFG="runreg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
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
# PROP Target_Last_Scanned "runreg - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "runreg - Win32 Release"

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

ALL : "$(OUTDIR)\runreg.exe"

CLEAN : 
	-@erase "..\..\..\wbin\runreg.exe"
	-@erase ".\Release\winposix.obj"
	-@erase ".\Release\Winmain.obj"
	-@erase ".\Release\Symshell.obj"
	-@erase ".\Release\Runreg.obj"
	-@erase ".\Release\Runreg.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "c:\scc\include" /I "c:\scc\symshell\windows" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INCLUDE_AUTOR" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "c:\scc\include" /I\
 "c:\scc\symshell\windows" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "INCLUDE_AUTOR" /Fp"$(INTDIR)/runreg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /i "c:\scc\symshell\windows" /d "NDEBUG"
RSC_PROJ=/l 0x415 /fo"$(INTDIR)/Runreg.res" /i "c:\scc\symshell\windows" /d\
 "NDEBUG" 
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
LINK32_OBJS= \
	"$(INTDIR)/winposix.obj" \
	"$(INTDIR)/Winmain.obj" \
	"$(INTDIR)/Symshell.obj" \
	"$(INTDIR)/Runreg.obj" \
	"$(INTDIR)/Runreg.res"

"$(OUTDIR)\runreg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"

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

ALL : "$(OUTDIR)\runreg.exe" "$(OUTDIR)\runreg.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\runreg.bsc"
	-@erase ".\Debug\Winmain.sbr"
	-@erase ".\Debug\Symshell.sbr"
	-@erase ".\Debug\Runreg.sbr"
	-@erase ".\Debug\winposix.sbr"
	-@erase ".\Debug\runreg.exe"
	-@erase ".\Debug\Winmain.obj"
	-@erase ".\Debug\Symshell.obj"
	-@erase ".\Debug\Runreg.obj"
	-@erase ".\Debug\winposix.obj"
	-@erase ".\Debug\Runreg.res"
	-@erase ".\Debug\runreg.ilk"
	-@erase ".\Debug\runreg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "c:\scc\include" /I "c:\scc\symshell\windows" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INCLUDE_AUTOR" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "c:\scc\include" /I\
 "c:\scc\symshell\windows" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "INCLUDE_AUTOR" /FR"$(INTDIR)/" /Fp"$(INTDIR)/runreg.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /i "c:\scc\symshell\windows" /d "_DEBUG"
RSC_PROJ=/l 0x415 /fo"$(INTDIR)/Runreg.res" /i "c:\scc\symshell\windows" /d\
 "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/runreg.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Winmain.sbr" \
	"$(INTDIR)/Symshell.sbr" \
	"$(INTDIR)/Runreg.sbr" \
	"$(INTDIR)/winposix.sbr"

"$(OUTDIR)\runreg.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/runreg.pdb" /debug /machine:I386 /out:"$(OUTDIR)/runreg.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Winmain.obj" \
	"$(INTDIR)/Symshell.obj" \
	"$(INTDIR)/Runreg.obj" \
	"$(INTDIR)/winposix.obj" \
	"$(INTDIR)/Runreg.res"

"$(OUTDIR)\runreg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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

# Name "runreg - Win32 Release"
# Name "runreg - Win32 Debug"

!IF  "$(CFG)" == "runreg - Win32 Release"

!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Runreg.cxx
DEP_CPP_RUNRE=\
	"c:\scc\include\uses_wb.hpp"\
	"c:\scc\include\posix.h"\
	"c:\scc\include\platform.hpp"\
	"\Scc\Include\impl_wb.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Timeb.h"\
	"c:\scc\include\gd.h"\
	"\Scc\Include\gdfontl.h"\
	"\Scc\Include\gdfonts.h"\
	"c:\scc\include\symshell.h"\
	"\Scc\Include\wberror.hpp"\
	"\Scc\Include\realpath.h"\
	"\Scc\Include\tnames.h"\
	"\Scc\Include\random.hpp"\
	"\Scc\Include\_protect.hpp"\
	"\Scc\Include\excpbase.hpp"\
	"\Scc\Include\errorhan.hpp"\
	"\Scc\Include\excpmem.hpp"\
	"\Scc\Include\excpothr.hpp"\
	"\Scc\Include\vobject.hpp"\
	"\Scc\Include\_voidnew.hpp"\
	"\Scc\Include\unitype.hpp"\
	"\Scc\Include\htest.h"\
	"\Scc\Include\iosuppor.hpp"\
	"\Scc\Include\soid.hpp"\
	"\Scc\Include\gmhandle.hpp"\
	"\Scc\Include\sosstrin.hpp"\
	"\Scc\Include\compare.hpp"\
	"\Scc\Include\clone.hpp"\
	"\Scc\Include\pix.hpp"\
	"\Scc\Include\endecode.hpp"\
	"\Scc\Include\bounded.hpp"\
	"\Scc\Include\vector2d.hpp"\
	"\Scc\Include\vector3d.hpp"\
	"\Scc\Include\angle.hpp"\
	"\Scc\Include\regex.h"\
	"\Scc\Include\regexpr.hpp"\
	"\Scc\Include\iter_dir.hpp"\
	"\Scc\Include\cticker.hpp"\
	"\Scc\Include\containe.hpp"\
	"\Scc\Include\iterator.hpp"\
	"\Scc\Include\_ikeycon.hpp"\
	"\Scc\Include\file_of.hpp"\
	"\Scc\Include\gd_image.hpp"\
	"\Scc\Include\arr_base.hpp"\
	"\Scc\Include\arrays.hpp"\
	"\Scc\Include\arrays.imp"\
	"\Scc\Include\assobase.hpp"\
	"\Scc\Include\setbase.hpp"\
	"\Scc\Include\assoctab.hpp"\
	"\Scc\Include\assoctab.imp"\
	"\Scc\Include\kptrasso.hpp"\
	"\Scc\Include\kptrasso.imp"\
	"\Scc\Include\dictiona.hpp"\
	"\Scc\Include\set_of.hpp"\
	"\Scc\Include\set_of.imp"\
	"\Scc\Include\enventry.hpp"\
	"\Scc\Include\enventry.imp"\
	"\Scc\Include\envcache.hpp"\
	"\Scc\Include\envcache.imp"\
	"\Scc\Include\configrc.hpp"\
	"\Scc\Include\cgi_conf.hpp"\
	"c:\scc\include\platform.h"\
	"c:\scc\include\soid.imp"\
	"\Scc\Include\gmhandle.imp"\
	
NODEP_CPP_RUNRE=\
	".\..\..\..\scc\include\mousex.h"\
	

!IF  "$(CFG)" == "runreg - Win32 Release"


"$(INTDIR)\Runreg.obj" : $(SOURCE) $(DEP_CPP_RUNRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"


"$(INTDIR)\Runreg.obj" : $(SOURCE) $(DEP_CPP_RUNRE) "$(INTDIR)"

"$(INTDIR)\Runreg.sbr" : $(SOURCE) $(DEP_CPP_RUNRE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Scc\Rtm\Symshell.c
DEP_CPP_SYMSH=\
	"c:\scc\include\platform.h"\
	".\..\..\..\Scc\symshell\symshtxt.c"\
	".\..\..\..\Scc\symshell\symshcur.c"\
	".\..\..\..\Scc\symshell\symshgrx.c"\
	".\..\..\..\Scc\symshell\windows\symshwin.c"\
	".\..\..\..\Scc\symshell\symshx11.c"\
	".\..\..\..\Scc\symshell\platform.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	"c:\scc\include\symshell.h"\
	"c:\scc\include\platform.hpp"\
	"\Scc\symshell\windows\win16ext.h"\
	"c:\scc\symshell\windows\symshwin.h"\
	".\..\..\..\Scc\Rtm\autor.h"\
	".\..\..\..\Scc\symshell\icon.h"\
	

!IF  "$(CFG)" == "runreg - Win32 Release"


"$(INTDIR)\Symshell.obj" : $(SOURCE) $(DEP_CPP_SYMSH) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\Symshell.obj" : $(SOURCE) $(DEP_CPP_SYMSH) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Symshell.sbr" : $(SOURCE) $(DEP_CPP_SYMSH) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Scc\Rtm\winposix.c
DEP_CPP_WINPO=\
	"c:\scc\include\posix.h"\
	

!IF  "$(CFG)" == "runreg - Win32 Release"


"$(INTDIR)\winposix.obj" : $(SOURCE) $(DEP_CPP_WINPO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\winposix.obj" : $(SOURCE) $(DEP_CPP_WINPO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\winposix.sbr" : $(SOURCE) $(DEP_CPP_WINPO) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Scc\Rtm\Winmain.c

!IF  "$(CFG)" == "runreg - Win32 Release"


"$(INTDIR)\Winmain.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "runreg - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\Winmain.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Winmain.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Runreg.rc
DEP_RSC_RUNREG=\
	".\symshwin.ico"\
	"c:\scc\symshell\windows\symshwin.h"\
	

"$(INTDIR)\Runreg.res" : $(SOURCE) $(DEP_RSC_RUNREG) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
