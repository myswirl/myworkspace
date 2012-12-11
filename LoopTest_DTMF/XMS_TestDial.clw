; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXMS_TestDialDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XMS_TestDial.h"

ClassCount=4
Class1=CXMS_TestDialApp
Class2=CXMS_TestDialDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_XMS_TESTDIAL_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CResult
Resource4=IDD_DIALOG_RESULT

[CLS:CXMS_TestDialApp]
Type=0
HeaderFile=XMS_TestDial.h
ImplementationFile=XMS_TestDial.cpp
Filter=N
LastObject=CXMS_TestDialApp

[CLS:CXMS_TestDialDlg]
Type=0
HeaderFile=XMS_TestDialDlg.h
ImplementationFile=XMS_TestDialDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CXMS_TestDialDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=XMS_TestDialDlg.h
ImplementationFile=XMS_TestDialDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_XMS_TESTDIAL_DIALOG]
Type=1
Class=CXMS_TestDialDlg
ControlCount=32
Control1=IDC_LIST_COUNT,SysListView32,1350631425
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EDIT_ModuleID,edit,1350631552
Control6=IDC_EDIT_Start,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_STATIC_Count,static,1342308864
Control12=IDC_STATIC_Fail,static,1342308864
Control13=IDC_STATIC_SucRate,static,1342308864
Control14=IDC_STATIC,static,1342308864
Control15=IDC_STATIC_TestTimer,static,1342308864
Control16=IDC_STATIC,static,1342308864
Control17=IDC_EDIT_TestTime,edit,1350631552
Control18=IDC_STATIC,static,1342308864
Control19=IDC_STATIC,static,1342308864
Control20=IDC_STATIC_DtmfErr,static,1342308864
Control21=IDC_STATIC,static,1342308866
Control22=IDC_STATIC_AvsSetUp,static,1342308864
Control23=IDC_BUTTON_CHECK,button,1073807360
Control24=IDC_LIST_MSG,listbox,1353777409
Control25=IDC_STATIC,static,1342308864
Control26=IDC_EDIT_VOCPATH,edit,1350631552
Control27=IDC_STATIC,static,1342308864
Control28=IDC_EDIT_IP,edit,1350631552
Control29=IDC_STATIC,static,1342308864
Control30=IDC_EDIT_CALLERE1,edit,1350631552
Control31=IDC_STATIC,static,1342308864
Control32=IDC_EDIT_CALLEDE1,edit,1350631552

[DLG:IDD_DIALOG_RESULT]
Type=1
Class=CResult
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_ERROR,SysListView32,1350631425

[CLS:CResult]
Type=0
HeaderFile=Result.h
ImplementationFile=Result.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CResult

