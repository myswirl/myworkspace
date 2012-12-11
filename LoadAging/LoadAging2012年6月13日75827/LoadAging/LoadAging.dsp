# Microsoft Developer Studio Project File - Name="LoadAging" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LoadAging - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadAging.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadAging.mak" CFG="LoadAging - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoadAging - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LoadAging - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoadAging - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "LoadAging - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LoadAging - Win32 Release"
# Name "LoadAging - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\hangePasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer1Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer4Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer5Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer6Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LED.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadAging.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadAging.rc
# End Source File
# Begin Source File

SOURCE=.\LoadAging_Tran.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadAgingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LongDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultimediaTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\MyLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NT77.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSheet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\hangePasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer1Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer4Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer5Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Layer6Dlg.h
# End Source File
# Begin Source File

SOURCE=.\LED.h
# End Source File
# Begin Source File

SOURCE=.\LoadAging.h
# End Source File
# Begin Source File

SOURCE=.\LoadAging_Tran.h
# End Source File
# Begin Source File

SOURCE=.\LoadAgingDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\LongDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultimediaTimer.h
# End Source File
# Begin Source File

SOURCE=.\MyLog.h
# End Source File
# Begin Source File

SOURCE=.\NT77Api.h
# End Source File
# Begin Source File

SOURCE=.\ParamEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\SLinkList.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabSheet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ChangePassword.ico
# End Source File
# Begin Source File

SOURCE=.\res\Exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\exit_foc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_nor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help.ico
# End Source File
# Begin Source File

SOURCE=.\res\historyData.ico
# End Source File
# Begin Source File

SOURCE=.\res\LoadAging.ico
# End Source File
# Begin Source File

SOURCE=.\res\LoadAging.rc2
# End Source File
# Begin Source File

SOURCE=.\res\max_focu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max_norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min_focu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min_norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ParamEdit.ico
# End Source File
# Begin Source File

SOURCE=.\res\ParamImport.ico
# End Source File
# Begin Source File

SOURCE=.\res\res_focu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\res_norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\Stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\SysSet.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\NT77.lib
# End Source File
# End Target
# End Project
