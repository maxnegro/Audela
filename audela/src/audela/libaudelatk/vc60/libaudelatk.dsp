# Microsoft Developer Studio Project File - Name="libaudelatk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libaudelatk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "libaudelatk.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "libaudelatk.mak" CFG="libaudelatk - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "libaudelatk - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libaudelatk - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libaudelatk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAUDELATK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\src" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\external\include\win" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_TCL_STUBS" /D "USE_TK_STUBS" /D "USE_COMPAT_CONST" /D "TCL_THREADS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 user32.lib tk85t.lib tkstub85.lib tclstub85.lib libaudela.lib /nologo /dll /machine:I386 /nodefaultlib:"msvcrt" /out:"..\..\..\..\bin\libaudelatk.dll" /libpath:"..\..\..\external\lib" /libpath:"..\..\lib"

!ELSEIF  "$(CFG)" == "libaudelatk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAUDELATK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\src" /I "..\..\include" /I "..\..\..\include" /I "..\..\..\external\include\win" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_TCL_STUBS" /D "USE_TK_STUBS" /D "USE_COMPAT_CONST" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib tk85t.lib tkstub85.lib tclstub85.lib libaudela.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"msvcrt" /out:"..\..\..\..\bin\libaudelatk.dll" /pdbtype:sept /libpath:"..\..\..\external\lib" /libpath:"..\..\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF

# Begin Target

# Name "libaudelatk - Win32 Release"
# Name "libaudelatk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\cvisu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\libaudelatk.cpp
# End Source File
# Begin Source File

SOURCE=.\libaudelatk.def
# End Source File
# Begin Source File

SOURCE=..\src\tkimgvideo.c
# End Source File
# Begin Source File

SOURCE=..\src\visu_tcl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\cbuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cdevice.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cerror.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cpixels.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cpool.h
# End Source File
# Begin Source File

SOURCE=..\src\cvisu.h
# End Source File
# Begin Source File

SOURCE=..\..\include\fitskw.h
# End Source File
# Begin Source File

SOURCE=..\..\include\palette.h
# End Source File
# Begin Source File

SOURCE=..\..\..\external\include\win\pthread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\external\include\win\sched.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\sysexp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
