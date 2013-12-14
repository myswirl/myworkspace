; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=ParamEditDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "loadaging.h"
LastPage=0

ClassCount=16
Class1=ChangePasswordDlg
Class2=ImportParamDlg
Class3=Layer1Dlg
Class4=Layer2Dlg
Class5=Layer3Dlg
Class6=Layer4Dlg
Class7=Layer5Dlg
Class8=Layer6Dlg
Class9=CLED
Class10=CLoadAgingApp
Class11=CAboutDlg
Class12=CLoadAgingDlg
Class13=LongDlg
Class14=ParamEditDlg
Class15=SysConfigDlg
Class16=CTabSheet

ResourceCount=13
Resource1=IDD_DIALOG_LAYER4
Resource2=IDD_LOADAGING_DIALOG
Resource3=IDD_DIALOG_LAYER2
Resource4=IDD_DIALOG_LAYER6
Resource5=IDD_DIALOG_LAYER5
Resource6=IDD_DIALOG_LOGIN
Resource7=IDD_ABOUTBOX
Resource8=IDD_DIALOG_LAYER1
Resource9=IDD_DIALOG_PARAMEDIT
Resource10=IDD_DIALOG_LAYER3
Resource11=IDD_DIALOG_CHANGEPASSWORD
Resource12=IDD_DIALOG_SYSCONFIG
Resource13=IDD_DIALOG_IMPORT_PARAM

[CLS:ChangePasswordDlg]
Type=0
BaseClass=CDialog
HeaderFile=hangePasswordDlg.h
ImplementationFile=hangePasswordDlg.cpp

[CLS:ImportParamDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImportParamDlg.h
ImplementationFile=ImportParamDlg.cpp
LastObject=IDC_LIST_TIMESEIRES
Filter=D
VirtualFilter=dWC

[CLS:Layer1Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer1Dlg.h
ImplementationFile=Layer1Dlg.cpp
LastObject=IDC_COMBO_LAYER1_LOADMODE
Filter=D
VirtualFilter=dWC

[CLS:Layer2Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer2Dlg.h
ImplementationFile=Layer2Dlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_LAYER2_PARAMODE

[CLS:Layer3Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer3Dlg.h
ImplementationFile=Layer3Dlg.cpp
LastObject=IDC_COMBO_LAYER3_PARAMODE
Filter=D
VirtualFilter=dWC

[CLS:Layer4Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer4Dlg.h
ImplementationFile=Layer4Dlg.cpp
LastObject=IDC_COMBO_LAYER4_PARAMODE
Filter=D
VirtualFilter=dWC

[CLS:Layer5Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer5Dlg.h
ImplementationFile=Layer5Dlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_LAYER5_PARAMODE

[CLS:Layer6Dlg]
Type=0
BaseClass=CDialog
HeaderFile=Layer6Dlg.h
ImplementationFile=Layer6Dlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_LAYER6_PARAMODE

[CLS:CLED]
Type=0
BaseClass=CStatic
HeaderFile=LED.h
ImplementationFile=LED.cpp

[CLS:CLoadAgingApp]
Type=0
BaseClass=CWinApp
HeaderFile=LoadAging.h
ImplementationFile=LoadAging.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoadAgingDlg.cpp
ImplementationFile=LoadAgingDlg.cpp

[CLS:CLoadAgingDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoadAgingDlg.h
ImplementationFile=LoadAgingDlg.cpp
LastObject=IDC_BUTTON_HELP
Filter=D
VirtualFilter=dWC

[CLS:LongDlg]
Type=0
BaseClass=CDialog
HeaderFile=LongDlg.h
ImplementationFile=LongDlg.cpp

[CLS:ParamEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=ParamEditDlg.h
ImplementationFile=ParamEditDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_LAYER0_PARALLELMODE

[CLS:SysConfigDlg]
Type=0
BaseClass=CDialog
HeaderFile=SysConfigDlg.h
ImplementationFile=SysConfigDlg.cpp
LastObject=SysConfigDlg
Filter=D
VirtualFilter=dWC

[CLS:CTabSheet]
Type=0
BaseClass=CTabCtrl
HeaderFile=TabSheet.h
ImplementationFile=TabSheet.cpp

[DLG:IDD_DIALOG_CHANGEPASSWORD]
Type=1
Class=ChangePasswordDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_CURRENTPASSWORD,edit,1350631584
Control7=IDC_EDIT_NEWPASSWORD,edit,1350631584
Control8=IDC_EDIT_NEWPASSWORD2,edit,1350631584
Control9=IDC_STATIC_CHANGEERRORMSG,static,1342308352

[DLG:IDD_DIALOG_IMPORT_PARAM]
Type=1
Class=ImportParamDlg
ControlCount=69
Control1=IDC_STATIC,button,1342177287
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CHECK_SAVE_RECDATA,button,1476460547
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_SAVEDATA_INTERVAL,edit,1484849280
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_START_LOADNUM,edit,1484849280
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_END_LOADNUM,edit,1484849280
Control16=IDC_STATIC,static,1342308352
Control17=IDC_BUTTON_LOADPARAM,button,1342242816
Control18=IDC_STATIC,static,1342308352
Control19=IDC_TAB_ALLCAR,SysTabControl32,1342179328
Control20=IDC_TAB_ALLLAYER,SysTabControl32,1073741824
Control21=IDC_EDIT_LAYER,edit,1484849280
Control22=IDC_EDIT_LOADPERLAYER,edit,1484849280
Control23=IDC_EDIT_PARAMSETMODE,edit,1484849280
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC_VALUE,static,1342308352
Control27=IDC_STATIC_MAX,static,1342308352
Control28=IDC_STATIC_MIN,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1073872896
Control35=IDC_STATIC,static,1073872896
Control36=IDC_EDIT_LAYER1_CHN1_VALUE,edit,1484849280
Control37=IDC_EDIT_LAYER1_CHN2_VALUE,edit,1484849280
Control38=IDC_EDIT_LAYER1_CHN3_VALUE,edit,1484849280
Control39=IDC_EDIT_LAYER1_CHN4_VALUE,edit,1484849280
Control40=IDC_EDIT_LAYER1_CHN5_VALUE,edit,1216413824
Control41=IDC_EDIT_LAYER1_CHN6_VALUE,edit,1216413824
Control42=IDC_EDIT_LAYER1_CHN1_MAX,edit,1484849280
Control43=IDC_EDIT_LAYER1_CHN2_MAX,edit,1484849280
Control44=IDC_EDIT_LAYER1_CHN3_MAX,edit,1484849280
Control45=IDC_EDIT_LAYER1_CHN4_MAX,edit,1484849280
Control46=IDC_EDIT_LAYER1_CHN5_MAX,edit,1216413824
Control47=IDC_EDIT_LAYER1_CHN6_MAX,edit,1216413824
Control48=IDC_EDIT_LOAD_FILENAME,edit,1484849280
Control49=IDC_EDIT_LAYER1_CHN2_MIN,edit,1484849280
Control50=IDC_EDIT_LAYER1_CHN3_MIN,edit,1484849280
Control51=IDC_EDIT_LAYER1_CHN4_MIN,edit,1484849280
Control52=IDC_EDIT_LAYER1_CHN5_MIN,edit,1216413824
Control53=IDC_EDIT_LAYER1_CHN6_MIN,edit,1216413824
Control54=IDC_EDIT_LAYER1_CHN1_PRONAME,edit,1484849280
Control55=IDC_EDIT_LAYER1_CHN2_PRONAME,edit,1484849280
Control56=IDC_EDIT_LAYER1_CHN3_PRONAME,edit,1484849280
Control57=IDC_EDIT_LAYER1_CHN4_PRONAME,edit,1484849280
Control58=IDC_EDIT_LAYER1_CHN5_PRONAME,edit,1216413824
Control59=IDC_EDIT_LAYER1_CHN6_PRONAME,edit,1216413824
Control60=IDC_EDIT_LOADMODE,edit,1484849280
Control61=IDC_STATIC,static,1342308352
Control62=IDC_EDIT_LAYER1_CHN1_MIN,edit,1484849280
Control63=IDOK,button,1342242816
Control64=IDC_STATIC,static,1342308352
Control65=IDC_EDIT_LOADTEST_TOTALTIME,edit,1484857472
Control66=IDC_STATIC,static,1342308352
Control67=IDC_LIST_TIMESEIRES,listbox,1352728835
Control68=IDC_STATIC,static,1342308352
Control69=IDC_EDIT_PARAMODE,edit,1484849280

[DLG:IDD_DIALOG_LAYER1]
Type=1
Class=Layer1Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER1_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER1_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER1_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER1_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER1_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER1_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER1_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER1_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER1_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER1_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER1_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER1_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER1_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER1_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER1_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER1_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER1_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER1_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER1_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER1_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER1_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER1_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER1_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER1_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER1_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC_PARALLEL,static,1342308352
Control39=IDC_COMBO_LAYER1_PARALLELMODE,combobox,1344339970

[DLG:IDD_DIALOG_LAYER2]
Type=1
Class=Layer2Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER2_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER2_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER2_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER2_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER2_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER2_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER2_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER2_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER2_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER2_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER2_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER2_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER2_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER2_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER2_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER2_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER2_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER2_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER2_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER2_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER2_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER2_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER2_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER2_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER2_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC,static,1342308352
Control39=IDC_COMBO_LAYER2_PARAMODE,combobox,1344339970

[DLG:IDD_DIALOG_LAYER3]
Type=1
Class=Layer3Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER3_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER3_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER3_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER3_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER3_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER3_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER3_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER3_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER3_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER3_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER3_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER3_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER3_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER3_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER3_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER3_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER3_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER3_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER3_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER3_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER3_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER3_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER3_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER3_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER3_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC,static,1342308352
Control39=IDC_COMBO_LAYER3_PARAMODE,combobox,1344339970

[DLG:IDD_DIALOG_LAYER4]
Type=1
Class=Layer4Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER4_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER4_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER4_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER4_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER4_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER4_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER4_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER4_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER4_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER4_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER4_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER4_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER4_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER4_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER4_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER4_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER4_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER4_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER4_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER4_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER4_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER4_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER4_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER4_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER4_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC,static,1342308352
Control39=IDC_COMBO_LAYER4_PARAMODE,combobox,1344339970

[DLG:IDD_DIALOG_LAYER5]
Type=1
Class=Layer5Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER5_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER5_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER5_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER5_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER5_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER5_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER5_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER5_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER5_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER5_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER5_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER5_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER5_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER5_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER5_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER5_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER5_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER5_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER5_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER5_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER5_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER5_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER5_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER5_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER5_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC,static,1342308352
Control39=IDC_COMBO_LAYER5_PARAMODE,combobox,1344339970

[DLG:IDD_DIALOG_LAYER6]
Type=1
Class=Layer6Dlg
ControlCount=39
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_LAYER6_LOADMODE,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_VALUE,static,1342308352
Control5=IDC_STATIC_MAX,static,1342308352
Control6=IDC_STATIC_MIN,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1073872896
Control13=IDC_STATIC,static,1073872896
Control14=IDC_EDIT_LAYER6_CHN1_VALUE,edit,1350631552
Control15=IDC_EDIT_LAYER6_CHN2_VALUE,edit,1350631552
Control16=IDC_EDIT_LAYER6_CHN3_VALUE,edit,1350631552
Control17=IDC_EDIT_LAYER6_CHN4_VALUE,edit,1350631552
Control18=IDC_EDIT_LAYER6_CHN5_VALUE,edit,1216413824
Control19=IDC_EDIT_LAYER6_CHN6_VALUE,edit,1216413824
Control20=IDC_EDIT_LAYER6_CHN1_MAX,edit,1350631552
Control21=IDC_EDIT_LAYER6_CHN2_MAX,edit,1350631552
Control22=IDC_EDIT_LAYER6_CHN3_MAX,edit,1350631552
Control23=IDC_EDIT_LAYER6_CHN4_MAX,edit,1350631552
Control24=IDC_EDIT_LAYER6_CHN5_MAX,edit,1216413824
Control25=IDC_EDIT_LAYER6_CHN6_MAX,edit,1216413824
Control26=IDC_EDIT_LAYER6_CHN1_MIN,edit,1350631552
Control27=IDC_EDIT_LAYER6_CHN2_MIN,edit,1350631552
Control28=IDC_EDIT_LAYER6_CHN3_MIN,edit,1350631552
Control29=IDC_EDIT_LAYER6_CHN4_MIN,edit,1350631552
Control30=IDC_EDIT_LAYER6_CHN5_MIN,edit,1216413824
Control31=IDC_EDIT_LAYER6_CHN6_MIN,edit,1216413824
Control32=IDC_EDIT_LAYER6_CHN1_PRONAME,edit,1350631552
Control33=IDC_EDIT_LAYER6_CHN2_PRONAME,edit,1350631552
Control34=IDC_EDIT_LAYER6_CHN3_PRONAME,edit,1350631552
Control35=IDC_EDIT_LAYER6_CHN4_PRONAME,edit,1350631552
Control36=IDC_EDIT_LAYER6_CHN5_PRONAME,edit,1216413824
Control37=IDC_EDIT_LAYER6_CHN6_PRONAME,edit,1216413824
Control38=IDC_STATIC,static,1342308352
Control39=IDC_COMBO_LAYER6_PARAMODE,combobox,1344339970

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOADAGING_DIALOG]
Type=1
Class=CLoadAgingDlg
ControlCount=253
Control1=IDCANCEL,button,1073807360
Control2=IDC_BUTTON_SYSCONFIG,button,1342242816
Control3=IDC_BUTTON_PARAMEDIT,button,1342242816
Control4=IDC_BUTTON_PARAMIMPORT,button,1342242816
Control5=IDC_BUTTON_HELP,button,1342242816
Control6=IDC_BUTTON_PASSWORD,button,1342242816
Control7=IDC_TAB_TESTING_ALLCAR,SysTabControl32,1342177280
Control8=IDC_STATIC_CHN1,static,1342308608
Control9=IDC_STATIC_CHN2,static,1342308608
Control10=IDC_STATIC_CHN3,static,1342308608
Control11=IDC_STATIC_CHN4,static,1342308608
Control12=IDC_STATIC_CHN5,static,1342308608
Control13=IDC_STATIC_CHN6,static,1342308608
Control14=IDC_STATIC_CHN7,static,1342308608
Control15=IDC_STATIC_CHN8,static,1342308608
Control16=IDC_STATIC_CHN9,static,1342308608
Control17=IDC_STATIC_CHN10,static,1342308608
Control18=IDC_STATIC_CHN11,static,1342308608
Control19=IDC_STATIC_CHN12,static,1342308608
Control20=IDC_STATIC_CHN13,static,1342308608
Control21=IDC_STATIC_CHN14,static,1342308608
Control22=IDC_STATIC_CHN15,static,1342308608
Control23=IDC_STATIC_CHN16,static,1342308608
Control24=IDC_STATIC_CHN17,static,1342308608
Control25=IDC_STATIC_CHN18,static,1342308608
Control26=IDC_STATIC_CHN19,static,1342308608
Control27=IDC_STATIC_CHN20,static,1342308608
Control28=IDC_STATIC_CHN21,static,1342308608
Control29=IDC_STATIC_CHN22,static,1342308608
Control30=IDC_STATIC_CHN23,static,1342308608
Control31=IDC_STATIC_CHN24,static,1342308608
Control32=IDC_STATIC_CHN25,static,1342308608
Control33=IDC_STATIC_CHN26,static,1342308608
Control34=IDC_STATIC_CHN27,static,1342308608
Control35=IDC_STATIC_CHN28,static,1342308608
Control36=IDC_STATIC_CHN29,static,1342308608
Control37=IDC_STATIC_CHN30,static,1342308608
Control38=IDC_STATIC_CHN31,static,1342308608
Control39=IDC_STATIC_CHN32,static,1342308608
Control40=IDC_STATIC_CHN33,static,1342308608
Control41=IDC_STATIC_CHN34,static,1342308608
Control42=IDC_STATIC_CHN35,static,1342308608
Control43=IDC_STATIC_CHN36,static,1342308608
Control44=IDC_STATIC_CHN37,static,1342308608
Control45=IDC_STATIC_CHN38,static,1342308608
Control46=IDC_STATIC_CHN39,static,1342308608
Control47=IDC_STATIC_CHN40,static,1342308608
Control48=IDC_STATIC_CHN41,static,1342308608
Control49=IDC_STATIC_CHN42,static,1342308608
Control50=IDC_STATIC_CHN43,static,1342308608
Control51=IDC_STATIC_CHN44,static,1342308608
Control52=IDC_STATIC_CHN45,static,1342308608
Control53=IDC_STATIC_CHN46,static,1342308608
Control54=IDC_STATIC_CHN47,static,1342308608
Control55=IDC_STATIC_CHN48,static,1342308608
Control56=IDC_STATIC_CHN49,static,1342308608
Control57=IDC_STATIC_CHN50,static,1342308608
Control58=IDC_STATIC_CHN51,static,1342308608
Control59=IDC_STATIC_CHN52,static,1342308608
Control60=IDC_STATIC_CHN53,static,1342308608
Control61=IDC_STATIC_CHN54,static,1342308608
Control62=IDC_STATIC_CHN55,static,1342308608
Control63=IDC_STATIC_CHN56,static,1342308608
Control64=IDC_STATIC_CHN57,static,1342308608
Control65=IDC_STATIC_CHN58,static,1342308608
Control66=IDC_STATIC_CHN59,static,1342308608
Control67=IDC_STATIC_CHN60,static,1342308608
Control68=IDC_STATIC_CHN61,static,1342308608
Control69=IDC_STATIC_CHN62,static,1342308608
Control70=IDC_STATIC_CHN63,static,1342308608
Control71=IDC_STATIC_CHN64,static,1342308608
Control72=IDC_STATIC_CHN65,static,1342308608
Control73=IDC_STATIC_CHN66,static,1342308608
Control74=IDC_STATIC_CHN67,static,1342308608
Control75=IDC_STATIC_CHN68,static,1342308608
Control76=IDC_STATIC_CHN69,static,1342308608
Control77=IDC_STATIC_CHN70,static,1342308608
Control78=IDC_STATIC_CHN71,static,1342308608
Control79=IDC_STATIC_CHN72,static,1342308608
Control80=IDC_STATIC_CHN73,static,1342308608
Control81=IDC_STATIC_CHN74,static,1342308608
Control82=IDC_STATIC_CHN75,static,1342308608
Control83=IDC_STATIC_CHN76,static,1342308608
Control84=IDC_STATIC_CHN77,static,1342308608
Control85=IDC_STATIC_CHN78,static,1342308608
Control86=IDC_STATIC_CHN79,static,1342308608
Control87=IDC_STATIC_CHN80,static,1342308608
Control88=IDC_STATIC_CHN81,static,1342308608
Control89=IDC_STATIC_CHN82,static,1342308608
Control90=IDC_STATIC_CHN83,static,1342308608
Control91=IDC_STATIC_CHN84,static,1342308608
Control92=IDC_STATIC_CHN85,static,1342308608
Control93=IDC_STATIC_CHN86,static,1342308608
Control94=IDC_STATIC_CHN87,static,1342308608
Control95=IDC_STATIC_CHN88,static,1342308608
Control96=IDC_STATIC_CHN89,static,1342308608
Control97=IDC_STATIC_CHN90,static,1342308608
Control98=IDC_STATIC_CHN91,static,1342308608
Control99=IDC_STATIC_CHN92,static,1342308608
Control100=IDC_STATIC_CHN93,static,1342308608
Control101=IDC_STATIC_CHN94,static,1342308608
Control102=IDC_STATIC_CHN95,static,1342308608
Control103=IDC_STATIC_CHN96,static,1342308608
Control104=IDC_STATIC_CHN97,static,1342308608
Control105=IDC_STATIC_CHN98,static,1342308608
Control106=IDC_STATIC_CHN99,static,1342308608
Control107=IDC_STATIC_CHN100,static,1342308608
Control108=IDC_STATIC_CHN101,static,1342308608
Control109=IDC_STATIC_CHN102,static,1342308608
Control110=IDC_STATIC_CHN103,static,1342308608
Control111=IDC_STATIC_CHN104,static,1342308608
Control112=IDC_STATIC_CHN105,static,1342308608
Control113=IDC_STATIC_CHN106,static,1342308608
Control114=IDC_STATIC_CHN107,static,1342308608
Control115=IDC_STATIC_CHN108,static,1342308608
Control116=IDC_STATIC_CHN109,static,1342308608
Control117=IDC_STATIC_CHN110,static,1342308608
Control118=IDC_STATIC_CHN111,static,1342308608
Control119=IDC_STATIC_CHN112,static,1342308608
Control120=IDC_STATIC_CHN113,static,1342308608
Control121=IDC_STATIC_CHN114,static,1342308608
Control122=IDC_STATIC_CHN115,static,1342308608
Control123=IDC_STATIC_CHN116,static,1342308608
Control124=IDC_STATIC_CHN117,static,1342308608
Control125=IDC_STATIC_CHN118,static,1342308608
Control126=IDC_STATIC_CHN119,static,1342308608
Control127=IDC_STATIC_CHN120,static,1342308608
Control128=IDC_STATIC_CHN121,static,1342308608
Control129=IDC_STATIC_CHN122,static,1342308608
Control130=IDC_STATIC_CHN123,static,1342308608
Control131=IDC_STATIC_CHN124,static,1342308608
Control132=IDC_STATIC_CHN125,static,1342308608
Control133=IDC_STATIC_CHN126,static,1342308608
Control134=IDC_STATIC_CHN127,static,1342308608
Control135=IDC_STATIC_CHN128,static,1342308608
Control136=IDC_STATIC_CHN129,static,1342308608
Control137=IDC_STATIC_CHN130,static,1342308608
Control138=IDC_STATIC_CHN131,static,1342308608
Control139=IDC_STATIC_CHN132,static,1342308608
Control140=IDC_STATIC_CHN133,static,1342308608
Control141=IDC_STATIC_CHN134,static,1342308608
Control142=IDC_STATIC_CHN135,static,1342308608
Control143=IDC_STATIC_CHN136,static,1342308608
Control144=IDC_STATIC_CHN137,static,1342308608
Control145=IDC_STATIC_CHN138,static,1342308608
Control146=IDC_STATIC_CHN139,static,1342308608
Control147=IDC_STATIC_CHN140,static,1342308608
Control148=IDC_STATIC_CHN141,static,1342308608
Control149=IDC_STATIC_CHN142,static,1342308608
Control150=IDC_STATIC_CHN143,static,1342308608
Control151=IDC_STATIC_CHN144,static,1342308608
Control152=IDC_STATIC_CHN145,static,1342308608
Control153=IDC_STATIC_CHN146,static,1342308608
Control154=IDC_STATIC_CHN147,static,1342308608
Control155=IDC_STATIC_CHN148,static,1342308608
Control156=IDC_STATIC_CHN149,static,1342308608
Control157=IDC_STATIC_CHN150,static,1342308608
Control158=IDC_STATIC_CHN151,static,1342308608
Control159=IDC_STATIC_CHN152,static,1342308608
Control160=IDC_STATIC_CHN153,static,1342308608
Control161=IDC_STATIC_CHN154,static,1342308608
Control162=IDC_STATIC_CHN155,static,1342308608
Control163=IDC_STATIC_CHN156,static,1342308608
Control164=IDC_STATIC_CHN157,static,1342308608
Control165=IDC_STATIC_CHN158,static,1342308608
Control166=IDC_STATIC_CHN159,static,1342308608
Control167=IDC_STATIC_CHN160,static,1342308608
Control168=IDC_STATIC_CHN161,static,1342308608
Control169=IDC_STATIC_CHN162,static,1342308608
Control170=IDC_STATIC_CHN163,static,1342308608
Control171=IDC_STATIC_CHN164,static,1342308608
Control172=IDC_STATIC_CHN165,static,1342308608
Control173=IDC_STATIC_CHN166,static,1342308608
Control174=IDC_STATIC_CHN167,static,1342308608
Control175=IDC_STATIC_CHN168,static,1342308608
Control176=IDC_STATIC_CHN169,static,1342308608
Control177=IDC_STATIC_CHN170,static,1342308608
Control178=IDC_STATIC_CHN171,static,1342308608
Control179=IDC_STATIC_CHN172,static,1342308608
Control180=IDC_STATIC_CHN173,static,1342308608
Control181=IDC_STATIC_CHN174,static,1342308608
Control182=IDC_STATIC_CHN175,static,1342308608
Control183=IDC_STATIC_CHN176,static,1342308608
Control184=IDC_STATIC_CHN177,static,1342308608
Control185=IDC_STATIC_CHN178,static,1342308608
Control186=IDC_STATIC_CHN179,static,1342308608
Control187=IDC_STATIC_CHN180,static,1342308608
Control188=IDC_STATIC_CHN181,static,1342308608
Control189=IDC_STATIC_CHN182,static,1342308608
Control190=IDC_STATIC_CHN183,static,1342308608
Control191=IDC_STATIC_CHN184,static,1342308608
Control192=IDC_STATIC_CHN185,static,1342308608
Control193=IDC_STATIC_CHN186,static,1342308608
Control194=IDC_STATIC_CHN187,static,1342308608
Control195=IDC_STATIC_CHN188,static,1342308608
Control196=IDC_STATIC_CHN189,static,1342308608
Control197=IDC_STATIC_CHN190,static,1342308608
Control198=IDC_STATIC_CHN191,static,1342308608
Control199=IDC_STATIC_CHN192,static,1342308608
Control200=IDC_STATIC_CHN193,static,1342308608
Control201=IDC_STATIC_CHN194,static,1342308608
Control202=IDC_STATIC_CHN195,static,1342308608
Control203=IDC_STATIC_CHN196,static,1342308608
Control204=IDC_STATIC_CHN197,static,1342308608
Control205=IDC_STATIC_CHN198,static,1342308608
Control206=IDC_STATIC_CHN199,static,1342308608
Control207=IDC_STATIC_CHN200,static,1342308608
Control208=IDC_STATIC_CURCAR,static,1342309120
Control209=IDC_BUTTON_TEST,button,1342242816
Control210=IDC_STATIC_CHN201,static,1342308608
Control211=IDC_STATIC_CHN202,static,1342308608
Control212=IDC_STATIC_CHN203,static,1342308608
Control213=IDC_STATIC_CHN204,static,1342308608
Control214=IDC_STATIC_CHN205,static,1342308608
Control215=IDC_STATIC_CHN206,static,1342308608
Control216=IDC_STATIC_CHN207,static,1342308608
Control217=IDC_STATIC_CHN208,static,1342308608
Control218=IDC_STATIC_CHN209,static,1342308608
Control219=IDC_STATIC_CHN210,static,1342308608
Control220=IDC_STATIC_CHN211,static,1342308608
Control221=IDC_STATIC_CHN212,static,1342308608
Control222=IDC_STATIC_CHN213,static,1342308608
Control223=IDC_STATIC_CHN214,static,1342308608
Control224=IDC_STATIC_CHN215,static,1342308608
Control225=IDC_STATIC_CHN216,static,1342308608
Control226=IDC_STATIC_CHN217,static,1342308608
Control227=IDC_STATIC_CHN218,static,1342308608
Control228=IDC_STATIC_CHN219,static,1342308608
Control229=IDC_STATIC_CHN220,static,1342308608
Control230=IDC_STATIC_CHN221,static,1342308608
Control231=IDC_STATIC_CHN222,static,1342308608
Control232=IDC_STATIC_CHN223,static,1342308608
Control233=IDC_STATIC_CHN224,static,1342308608
Control234=IDC_STATIC_CHN225,static,1342308608
Control235=IDC_STATIC_CHN226,static,1342308608
Control236=IDC_STATIC_CHN227,static,1342308608
Control237=IDC_STATIC_CHN228,static,1342308608
Control238=IDC_STATIC_CHN229,static,1342308608
Control239=IDC_STATIC_CHN230,static,1342308608
Control240=IDC_STATIC_CHN231,static,1342308608
Control241=IDC_STATIC_CHN232,static,1342308608
Control242=IDC_STATIC_CHN233,static,1342308608
Control243=IDC_STATIC_CHN234,static,1342308608
Control244=IDC_STATIC_CHN235,static,1342308608
Control245=IDC_STATIC_CHN236,static,1342177536
Control246=IDC_STATIC_CHN237,static,1342177536
Control247=IDC_STATIC_CHN238,static,1342177536
Control248=IDC_STATIC_CHN239,static,1342177536
Control249=IDC_STATIC_CHN240,static,1342177536
Control250=IDC_BUTTON_CLEAR,button,1342242816
Control251=IDC_STATIC_COMMAND,button,1342177287
Control252=IDC_STATIC_SERPORT_TEST,static,1073872896
Control253=IDC_BUTTON_HISTORYDATA,button,1342242816

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=LongDlg
ControlCount=7
Control1=IDC_EDIT_PASSWORD,edit,1350631584
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_COMBO_USERNAME,combobox,1344339970
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC_LOGINRESULT,static,1342308352

[DLG:IDD_DIALOG_PARAMEDIT]
Type=1
Class=ParamEditDlg
ControlCount=78
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT_STARTTIME,edit,1350639744
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC_PULSEONTIME,static,1342308352
Control7=IDC_STATIC_PULSEOFFTIME,static,1342308352
Control8=IDC_STATIC_PULSEREPEATTIME,static,1342308352
Control9=IDC_EDIT_PULSE_ONTIME,edit,1350639744
Control10=IDC_EDIT_PULSE_OFFTIME,edit,1350639744
Control11=IDC_EDIT_ONOFF_REPEATTIME,edit,1350639744
Control12=IDC_STATIC_PULSEONTIME_SECOND,static,1342308352
Control13=IDC_STATIC_PULSEOFFTIME_SECOND,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_CHECK_SAVE_RECDATA,button,1342242819
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_SAVEDATA_INTERVAL,edit,1350639744
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_COMBO_LAYER,combobox,1075904770
Control21=IDC_STATIC,static,1342308352
Control22=IDC_COMBO_LOADPERLAYER,combobox,1075904770
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_START_LOADNUM,edit,1350639744
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_END_LOADNUM,edit,1350639744
Control28=IDC_STATIC,static,1342308352
Control29=IDC_COMBO_PARAMSETMODE,combobox,1344339970
Control30=IDC_TAB_PARAMEDIT,SysTabControl32,1073741824
Control31=IDC_BUTTON_LOADPARAM,button,1342242816
Control32=IDC_BUTTON_EXPORT,button,1342242816
Control33=IDC_BUTTON_CLEARPARAM,button,1073807360
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_EDIT_LOADTEST_TOTALTIME,edit,1350639744
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352
Control39=IDC_STATIC,static,1342308352
Control40=IDC_STATIC_PULSEREPEATTIME_VAL,static,1342308352
Control41=IDC_COMBO_TESTITEM,combobox,1344339970
Control42=IDC_STATIC_TESTTIME,static,1342308352
Control43=IDC_EDIT_TESTTIME,edit,1350639744
Control44=IDC_STATIC_TESTTIME_SECOND,static,1342308352
Control45=IDC_LIST_TIMESEIRES,listbox,1352728835
Control46=IDC_BUTTON_DELTIMESERIES,button,1342242816
Control47=IDC_BUTTON_ADDTIMESERIES,button,1342242816
Control48=IDC_EDIT_LAYER_PERCAR,edit,1350639744
Control49=IDC_EDIT_LOAD_PERLAYER,edit,1350639744
Control50=IDC_STATIC_LAYER0_LOADMODE,static,1342308352
Control51=IDC_COMBO_LAYER0_LOADMODE,combobox,1344339970
Control52=IDC_STATIC_LAYER0_CHN,static,1342308352
Control53=IDC_STATIC_LAYER0_VALUE,static,1342308352
Control54=IDC_STATIC_LAYER0_MAX,static,1342308352
Control55=IDC_STATIC_LAYER0_MIN,static,1342308352
Control56=IDC_STATIC_LAYER0_PRONAME,static,1342308352
Control57=IDC_STATIC_LAYER0_CHN1,static,1342308352
Control58=IDC_STATIC_LAYER0_CHN2,static,1342308352
Control59=IDC_STATIC_LAYER0_CHN3,static,1342308352
Control60=IDC_STATIC_LAYER0_CHN4,static,1342308352
Control61=IDC_EDIT_LAYER0_CHN1_VALUE,edit,1350631552
Control62=IDC_EDIT_LAYER0_CHN2_VALUE,edit,1350631552
Control63=IDC_EDIT_LAYER0_CHN3_VALUE,edit,1350631552
Control64=IDC_EDIT_LAYER0_CHN4_VALUE,edit,1350631552
Control65=IDC_EDIT_LAYER0_CHN1_MAX,edit,1350631552
Control66=IDC_EDIT_LAYER0_CHN2_MAX,edit,1350631552
Control67=IDC_EDIT_LAYER0_CHN3_MAX,edit,1350631552
Control68=IDC_EDIT_LAYER0_CHN4_MAX,edit,1350631552
Control69=IDC_EDIT_LAYER0_CHN1_MIN,edit,1350631552
Control70=IDC_EDIT_LAYER0_CHN2_MIN,edit,1350631552
Control71=IDC_EDIT_LAYER0_CHN3_MIN,edit,1350631552
Control72=IDC_EDIT_LAYER0_CHN4_MIN,edit,1350631552
Control73=IDC_EDIT_LAYER0_CHN1_PRONAME,edit,1350631552
Control74=IDC_EDIT_LAYER0_CHN2_PRONAME,edit,1350631552
Control75=IDC_EDIT_LAYER0_CHN3_PRONAME,edit,1350631552
Control76=IDC_EDIT_LAYER0_CHN4_PRONAME,edit,1350631552
Control77=IDC_STATIC_LAYER0_PARALLEL,static,1342308352
Control78=IDC_COMBO_LAYER0_PARALLELMODE,combobox,1344339970

[DLG:IDD_DIALOG_SYSCONFIG]
Type=1
Class=SysConfigDlg
ControlCount=42
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_COMBO_COMLIST_A1,combobox,1344339970
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_COMLIST_B1,combobox,1344339970
Control9=IDC_STATIC,static,1342308352
Control10=IDC_COMBO_COMLIST_C1,combobox,1344339970
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO_COMLIST_D1,combobox,1344339970
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO_COMLIST_A2,combobox,1344339970
Control15=IDC_STATIC,static,1342308352
Control16=IDC_COMBO_COMLIST_B2,combobox,1344339970
Control17=IDC_STATIC,static,1342308352
Control18=IDC_COMBO_COMLIST_C2,combobox,1344339970
Control19=IDC_STATIC,static,1342308352
Control20=IDC_COMBO_COMLIST_D2,combobox,1344339970
Control21=IDC_STATIC,static,1342308352
Control22=IDC_COMBO_COMLIST_A3,combobox,1344339970
Control23=IDC_STATIC,static,1342308352
Control24=IDC_COMBO_COMLIST_B3,combobox,1344339970
Control25=IDC_STATIC,static,1342308352
Control26=IDC_COMBO_COMLIST_C3,combobox,1344339970
Control27=IDC_STATIC,static,1342308352
Control28=IDC_COMBO_COMLIST_D3,combobox,1344339970
Control29=IDC_STATIC,static,1342308352
Control30=IDC_COMBO_COMLIST_A4,combobox,1344339970
Control31=IDC_STATIC,static,1342308352
Control32=IDC_COMBO_COMLIST_B4,combobox,1344339970
Control33=IDC_STATIC,static,1342308352
Control34=IDC_COMBO_COMLIST_C4,combobox,1344339970
Control35=IDC_STATIC,static,1342308352
Control36=IDC_COMBO_COMLIST_D4,combobox,1344339970
Control37=IDC_STATIC,static,1342308352
Control38=IDC_EDIT_DATARECORD_PATH,edit,1350633600
Control39=IDC_BUTTON_BROWSE,button,1342242816
Control40=IDC_STATIC,button,1342177287
Control41=IDC_CHECK_ENABLEBEEP,button,1342242819
Control42=IDC_CHECK_ENABLELIGHT,button,1342242819

