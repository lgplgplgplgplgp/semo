# Microsoft Developer Studio Project File - Name="win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=win32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak" CFG="win32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "win32 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "win32 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "win32 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "win32 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "...\\" /I "...\common" /I "...\front-para\c-front-para" /I "...\mid-para" /I "...\back-para\arm-back-para" /I "...\front-para\lac-front-para" /I "...\back-para\jvm-back-para" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "win32 - Win32 Release"
# Name "win32 - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\common\corenr-text.h"
# End Source File
# Begin Source File

SOURCE=..\common\corenr.c
# End Source File
# Begin Source File

SOURCE=..\common\corenr.h
# End Source File
# Begin Source File

SOURCE=..\common\elf.h
# End Source File
# Begin Source File

SOURCE=..\common\evalor.c
# End Source File
# Begin Source File

SOURCE=..\common\evalor.h
# End Source File
# Begin Source File

SOURCE=..\common\sccl.c
# End Source File
# Begin Source File

SOURCE=..\common\sccl.h
# End Source File
# Begin Source File

SOURCE=..\common\schal.c
# End Source File
# Begin Source File

SOURCE=..\common\schal.h
# End Source File
# End Group
# Begin Group "front-para"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\front-para\c-front-para\c-errors.c"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-errors.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-grammar.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-lexer.c"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-lexer.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-parser.c"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-parser.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-presor.c"
# End Source File
# Begin Source File

SOURCE="..\front-para\c-front-para\c-presor.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\lac-front-para\lac-grammar.h"
# End Source File
# Begin Source File

SOURCE="..\front-para\lac-front-para\lac-lexer.c"
# End Source File
# Begin Source File

SOURCE="..\front-para\lac-front-para\lac-lexer.h"
# End Source File
# End Group
# Begin Group "mid-para"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\mid-para\elf-gen.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\elf-gen.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\java-gentor.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\java-gentor.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lac-gentor.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lac-gentor.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lac.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lac.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lgnosia.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\lgnosia.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\regoc.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\regoc.h"
# End Source File
# Begin Source File

SOURCE="..\mid-para\symbole.c"
# End Source File
# Begin Source File

SOURCE="..\mid-para\symbole.h"
# End Source File
# End Group
# Begin Group "back-para"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-asm-lexer.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-asm-lexer.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-asm.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-asmor.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-asmor.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-assemer.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-assemer.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\arm-back-para\arm-setmap.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\jvm-back-para\jvm-assemer.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\jvm-back-para\jvm-assemer.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\jvm-back-para\jvm-class.c"
# End Source File
# Begin Source File

SOURCE="..\back-para\jvm-back-para\jvm-class.h"
# End Source File
# Begin Source File

SOURCE="..\back-para\jvm-back-para\jvm-setmap.c"
# End Source File
# End Group
# Begin Source File

SOURCE=..\main.c
# End Source File
# Begin Source File

SOURCE=..\sc.c
# End Source File
# Begin Source File

SOURCE=..\sc.h
# End Source File
# End Target
# End Project
