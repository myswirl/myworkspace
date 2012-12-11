// ParamEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadAging.h"
#include "ParamEditDlg.h"
#include "LoadAging_Tran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParamEditDlg dialog


ParamEditDlg::ParamEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ParamEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ParamEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ParamEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParamEditDlg)
	DDX_Control(pDX, IDC_LIST_TIMESEIRES, m_ListTimeSeries);
	DDX_Control(pDX, IDC_COMBO_TESTITEM, m_ComboTestItem);
	DDX_Control(pDX, IDC_COMBO_PARAMSETMODE, m_ComboParamSetMode);
	DDX_Control(pDX, IDC_COMBO_LOADPERLAYER, m_ComboLoadPerLayer);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_ComboLayer);
	DDX_Control(pDX, IDC_TAB_PARAMEDIT, m_TabSheetParamEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ParamEditDlg, CDialog)
	//{{AFX_MSG_MAP(ParamEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOADPARAM, OnButtonLoadparam)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CLEARPARAM, OnButtonClearparam)
	ON_CBN_SELCHANGE(IDC_COMBO_PARAMSETMODE, OnSelchangeComboParamsetmode)
	ON_BN_CLICKED(IDC_CHECK_SAVE_RECDATA, OnCheckSaveRecdata)
	ON_BN_CLICKED(IDC_BUTTON_ADDTIMESERIES, OnButtonAddtimeseries)
	ON_BN_CLICKED(IDC_BUTTON_DELTIMESERIES, OnButtonDeltimeseries)
	ON_CBN_SELCHANGE(IDC_COMBO_TESTITEM, OnSelchangeComboTestitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParamEditDlg message handlers

BOOL ParamEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TabSheetParamEdit.AddPage("第1层",&m_Layer1,IDD_DIALOG_LAYER1);
	m_TabSheetParamEdit.AddPage("第2层",&m_Layer2,IDD_DIALOG_LAYER2);
	m_TabSheetParamEdit.AddPage("第3层",&m_Layer3,IDD_DIALOG_LAYER3);
	m_TabSheetParamEdit.AddPage("第4层",&m_Layer4,IDD_DIALOG_LAYER4);
	m_TabSheetParamEdit.AddPage("第5层",&m_Layer5,IDD_DIALOG_LAYER5);
	m_TabSheetParamEdit.AddPage("第6层",&m_Layer6,IDD_DIALOG_LAYER6);
	m_TabSheetParamEdit.Show();
	
	this->m_ComboLayer.AddString("5");
	this->m_ComboLayer.AddString("6");
	this->m_ComboLayer.SetCurSel(1);

	this->m_ComboLoadPerLayer.AddString("10");
	this->m_ComboLoadPerLayer.AddString("8");
	this->m_ComboLoadPerLayer.SetCurSel(1);

	this->m_ComboParamSetMode.AddString("整区设置");
	this->m_ComboParamSetMode.AddString("分层设置");
	this->m_ComboParamSetMode.SetCurSel(0);

	this->SetDlgItemText(IDC_EDIT_LAYER_PERCAR,"6");
	this->SetDlgItemText(IDC_EDIT_LOAD_PERLAYER,"8");
	

	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,"1");
	this->SetDlgItemText(IDC_EDIT_END_LOADNUM,"1");
	
	this->SetDlgItemText(IDC_EDIT_LOADTEST_TOTALTIME,"60");
	
	//添加时序选项
	this->m_ComboTestItem.AddString("");
	this->m_ComboTestItem.AddString("脉冲开关");
	this->m_ComboTestItem.AddString("110V拉载");
	this->m_ComboTestItem.AddString("220V拉载");
	this->m_ComboTestItem.SetCurSel(0);
	this->SetDlgItemText(IDC_EDIT_STARTTIME,"");
	this->SetDlgItemText(IDC_EDIT_TESTTIME,"5");
	this->SetDlgItemText(IDC_EDIT_PULSE_ONTIME,"");
	this->SetDlgItemText(IDC_EDIT_PULSE_OFFTIME,"");
	this->SetDlgItemText(IDC_EDIT_ONOFF_REPEATTIME,"");
	GetDlgItem(IDC_STATIC_TESTTIME)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_TESTTIME)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_TESTTIME_SECOND)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_PULSEONTIME)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_PULSE_ONTIME)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_PULSEONTIME_SECOND)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_PULSEOFFTIME)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_PULSE_OFFTIME)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_PULSEOFFTIME_SECOND)->ShowWindow(false);
	
	GetDlgItem(IDC_STATIC_PULSEREPEATTIME)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_ONOFF_REPEATTIME)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_PULSEREPEATTIME_VAL)->ShowWindow(false);

	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck( 1);
	this->SetDlgItemText(IDC_EDIT_SAVEDATA_INTERVAL,"10");
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ParamEditDlg::OnButtonLoadparam() 
{
	int		number = 0;//每次读到的字节数
	char	tmpStr[256]={0};
	FILE	*pFileHandle;			//文件句柄
	ULONG	nSize = 0;
	TEST_PARAM_STRUCT readSS;
	CFileDialog *lpszOpenFile;		//定义一个CfileDialog对象
	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));

	lpszOpenFile = new	CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, "文件类型(*.psf)|*.psf||");//生成一个对话框
	if(lpszOpenFile->DoModal()== IDOK )	//假如点击对话框确定按钮
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		//this->SetDlgItemText(IDC_EDIT_INPUT_FILE, szGetName);//在窗口标题上显示路径

		//读取bin文件
		pFileHandle = fopen( szGetName, "rb" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"打开文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
		}
		
		nSize = 0x0;
		fseek(pFileHandle, nSize, 0);
		number=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
		sprintf(tmpStr,"文件\"%s\", 位置:0x%X, 读取%d个Byte",szGetName,nSize,number*4);
		if (number>0)//写DSP
		{			
		}
		
		fclose(pFileHandle); 
		delete lpszOpenFile;	//释放分配的对话框

	}else
	{
		delete lpszOpenFile;	//释放分配的对话框
		return;
	}
	
	this->m_ComboLayer.SetCurSel(readSS.layerPerCar);				//层数
	//this->m_ComboLoadPerLayer.SetCurSel( readSS.loadPerLayer );		//个数

	sprintf(tmpStr,"%d",readSS.loadPerLayer);	//负载数
	this->SetDlgItemText(IDC_EDIT_LOAD_PERLAYER,tmpStr);

	sprintf(tmpStr,"%d",readSS.startLoadNum);
	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);			//扫描开始地址
	
	sprintf(tmpStr,"%d",readSS.endLoadNum);
	this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );			//扫描结束地址
	
	this->m_ComboParamSetMode.SetCurSel( readSS.paramSetMode);		//设置模式
	
	sprintf(tmpStr,"%d",readSS.totalTestTime);
	this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );		//总测试时间
	
	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (readSS.isSaveData );//是否保存测试数据
	
	sprintf(tmpStr,"%d",readSS.saveDataInterval);
	this->SetDlgItemText ( IDC_EDIT_SAVEDATA_INTERVAL , tmpStr);//保存数据时间间隔
	
	m_ListTimeSeries.ResetContent();
	for (int i=0;i<MAX_ADD_TIMESERIES;i++)	//插入时序
	{
		if( strlen(readSS.timeSeries[i].timeSeries) != 0)
		{
			m_ListTimeSeries.AddString(readSS.timeSeries[i].timeSeries);
		}
	}

	
	//每个通道的参数, 模式
	this->m_Layer1.m_ComboLayer1LoadMode.SetCurSel(readSS.layerParam[0].loadMode);				//层数
	switch(this->m_Layer1.m_ComboLayer1LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer1.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer1.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer1.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer1.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer1.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer1.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}
	this->m_Layer2.m_ComboLayer2LoadMode.SetCurSel(readSS.layerParam[1].loadMode);				//层数
	switch(this->m_Layer2.m_ComboLayer2LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer2.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer2.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer2.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer2.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer2.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer2.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}
	this->m_Layer3.m_ComboLayer3LoadMode.SetCurSel(readSS.layerParam[2].loadMode);				//层数
	switch(this->m_Layer3.m_ComboLayer3LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer3.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer3.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer3.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer3.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer3.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer3.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}
	this->m_Layer4.m_ComboLayer4LoadMode.SetCurSel(readSS.layerParam[3].loadMode);				//层数
	switch(this->m_Layer4.m_ComboLayer4LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer4.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer4.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer4.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer4.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer4.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer4.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}
	this->m_Layer5.m_ComboLayer5LoadMode.SetCurSel(readSS.layerParam[4].loadMode);				//层数
	switch(this->m_Layer5.m_ComboLayer5LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer5.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer5.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer5.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer5.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer5.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer5.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}
	this->m_Layer6.m_ComboLayer6LoadMode.SetCurSel(readSS.layerParam[5].loadMode);				//层数
	switch(this->m_Layer6.m_ComboLayer6LoadMode.GetCurSel())
	{
	case 0:
		this->m_Layer6.SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->m_Layer6.SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->m_Layer6.SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_Layer6.SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->m_Layer6.SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->m_Layer6.SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}

	//电压、电流固定值
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn1Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn2Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn3Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn4Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn5Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn6Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->SetWindowText( tmpStr);

	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn1Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn2Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn3Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn4Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn5Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn6Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_VALUE)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn1Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn2Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn3Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn4Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn5Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn6Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_VALUE)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn1Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn2Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn3Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn4Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn5Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn6Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_VALUE)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn1Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn2Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn3Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn4Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn5Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn6Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_VALUE)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn1Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn2Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn3Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn4Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn5Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn6Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_VALUE)->SetWindowText( tmpStr);

	//电压、电流上限
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn1Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn2Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn3Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn4Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn5Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn6Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->SetWindowText( tmpStr);

	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn1Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn2Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn3Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn4Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn5Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn6Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_MAX)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn1Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn2Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn3Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn4Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn5Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn6Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_MAX)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn1Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn2Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn3Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn4Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn5Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn6Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_MAX)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn1Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn2Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn3Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn4Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn5Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn6Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_MAX)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn1Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn2Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn3Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn4Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn5Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn6Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_MAX)->SetWindowText( tmpStr);

	//电压、电流下限
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn1Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn2Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn3Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn4Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn5Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn6Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->SetWindowText( tmpStr);

	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn1Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn2Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn3Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn4Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn5Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[1].chn6Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_MIN)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn1Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn2Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn3Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn4Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn5Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[2].chn6Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_MIN)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn1Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn2Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn3Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn4Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn5Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[3].chn6Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_MIN)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn1Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn2Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn3Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn4Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn5Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[4].chn6Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_MIN)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn1Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn2Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn3Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn4Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn5Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[5].chn6Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_MIN)->SetWindowText( tmpStr);
	
	//产品名称
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn1ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn2ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn3ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn4ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn5ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[0].chn6ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_PRONAME)->SetWindowText( tmpStr);

	sprintf(tmpStr,"%s",readSS.layerParam[1].chn1ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[1].chn2ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[1].chn3ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[1].chn4ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[1].chn5ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[1].chn6ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_PRONAME)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn1ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn2ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn3ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn4ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn5ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[2].chn6ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_PRONAME)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn1ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn2ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn3ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn4ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn5ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[3].chn6ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_PRONAME)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn1ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn2ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn3ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn4ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn5ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[4].chn6ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_PRONAME)->SetWindowText( tmpStr);
	
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn1ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn2ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn3ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn4ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn5ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[5].chn6ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_PRONAME)->SetWindowText( tmpStr);
}
/************************************************************************/
/* 根据界面参数，导出配置文件                                           */
/************************************************************************/
void ParamEditDlg::OnButtonExport() 
{
	TEST_PARAM_STRUCT		pes;
	FILE			*pFileHandle;			//文件句柄
	int				numwritten;
	CFileDialog		*lpszOpenFile;		//定义一个CfileDialog对象
	char			tmpStr[64]={0};
	int				iLayerFixed;
	memset(&pes,0,sizeof(TEST_PARAM_STRUCT));
	
	//这台测试车分为多少层
	this->GetDlgItem ( IDC_EDIT_LAYER_PERCAR )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("车型,层数不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.layerPerCar );
	
	this->GetDlgItem ( IDC_EDIT_LOAD_PERLAYER )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("车型,每层负载数不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.loadPerLayer );//每层负载个数

	if (pes.layerPerCar>6)
	{
		AfxMessageBox("车型,层数不能大于6!");
		return;
	}
	if (pes.loadPerLayer*pes.layerPerCar <1 ||pes.loadPerLayer*pes.layerPerCar >60)
	{
		AfxMessageBox("车型,1=<层数*负载数<=60!");
		return;
	}

	this->GetDlgItem ( IDC_EDIT_START_LOADNUM )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("开始地址不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.startLoadNum );//开始地址
	if (pes.startLoadNum<1 || pes.startLoadNum>60)
	{
		AfxMessageBox("开始地址范围,[1,60]!");
		return;
	}

	this->GetDlgItem ( IDC_EDIT_END_LOADNUM )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("结束地址不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.endLoadNum );	//结束地址
	if (pes.endLoadNum<1 || pes.endLoadNum>60)
	{
		AfxMessageBox("结束地址范围,[1,60]!");
		return;
	}
	if (pes.startLoadNum > pes.endLoadNum)
	{
		AfxMessageBox("开始地址不能大于结束地址!");
		return;
	}

	pes.paramSetMode = this->m_ComboParamSetMode.GetCurSel();	//设置模式，整区还是分层
	
	//总测试时间
	this->GetDlgItem ( IDC_EDIT_LOADTEST_TOTALTIME )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("总测试时间不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.totalTestTime );	
	if (pes.totalTestTime > 60*100)
	{
		AfxMessageBox("总测试时间不能超过100个小时!");
		return;
	}

	pes.isSaveData = ((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->GetCheck ( ) ;//是否保存测试数据

	this->GetDlgItem ( IDC_EDIT_SAVEDATA_INTERVAL )->GetWindowText( tmpStr, 30 );
	if (pes.isSaveData==1 && strlen(tmpStr) == 0)
	{
		AfxMessageBox("数据保存时间间隔不能为空!");
		return;
	}
	sscanf ( tmpStr, "%d", &pes.saveDataInterval );	//保存数据时间间隔

	//读取现有ListBox的时序,m_ListTimeSeries.GetCount()为添加的时序数目
	for (int i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		m_ListTimeSeries.GetText(i,bufStr);
		strcpy(pes.timeSeries[i].timeSeries,bufStr);
	}

	//每个通道的参数, 模式
	pes.layerParam[0].loadMode = this->m_Layer1.m_ComboLayer1LoadMode.GetCurSel();				//每层的拉载模式
	pes.layerParam[1].loadMode = this->m_Layer2.m_ComboLayer2LoadMode.GetCurSel();				//每层的拉载模式
	pes.layerParam[2].loadMode = this->m_Layer3.m_ComboLayer3LoadMode.GetCurSel();				//每层的拉载模式
	pes.layerParam[3].loadMode = this->m_Layer4.m_ComboLayer4LoadMode.GetCurSel();				//每层的拉载模式
	pes.layerParam[4].loadMode = this->m_Layer5.m_ComboLayer5LoadMode.GetCurSel();				//每层的拉载模式
	pes.layerParam[5].loadMode = this->m_Layer6.m_ComboLayer6LoadMode.GetCurSel();				//每层的拉载模式

	//每个通道的参数, 固定值
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn1Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn2Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn3Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn4Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn5Value);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn6Value);

	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn1Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn2Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn3Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn4Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn5Value);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn6Value);
	
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn1Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn2Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn3Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn4Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn5Value);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn6Value);
	
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn1Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn2Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn3Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn4Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn5Value);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn6Value);
	
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn1Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn2Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn3Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn4Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn5Value);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn6Value);
	
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn1Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn2Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn3Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn4Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn5Value);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_VALUE)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn6Value);
	
	//判断定值设置是否超出范围
	for (iLayerFixed=0; iLayerFixed<6;iLayerFixed++)
	{
		char tmpStrError[64];
		if (pes.layerParam[iLayerFixed].loadMode==0)//CC模式,恒流
		{
			if (pes.layerParam[iLayerFixed].chn1Value <0 || pes.layerParam[iLayerFixed].chn1Value>10 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,1);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn2Value <0 || pes.layerParam[iLayerFixed].chn2Value>10 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,2);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn3Value <0 || pes.layerParam[iLayerFixed].chn3Value>10 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,3);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn4Value <0 || pes.layerParam[iLayerFixed].chn4Value>10 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,4);
				AfxMessageBox(tmpStrError);
				return;
			}
		}else if (pes.layerParam[iLayerFixed].loadMode==1)//CV模式,恒压
		{
			if (pes.layerParam[iLayerFixed].chn1Value <0 || pes.layerParam[iLayerFixed].chn1Value>100 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,1);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn2Value <0 || pes.layerParam[iLayerFixed].chn2Value>100 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,2);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn3Value <0 || pes.layerParam[iLayerFixed].chn3Value>100 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,3);
				AfxMessageBox(tmpStrError);
				return;
			}
			if (pes.layerParam[iLayerFixed].chn4Value <0 || pes.layerParam[iLayerFixed].chn4Value>100 )
			{
				sprintf(tmpStrError,"第%d层, 通道:%d, 定值超范围!",iLayerFixed+1,4);
				AfxMessageBox(tmpStrError);
				return;
			}
		}
	}
	//每个通道的参数, 上限
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn1Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn2Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn3Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn4Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn5Max);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn6Max);

	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn1Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn2Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn3Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn4Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn5Max);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn6Max);
	
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn1Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn2Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn3Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn4Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn5Max);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn6Max);
	
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn1Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn2Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn3Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn4Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn5Max);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn6Max);
	
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn1Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn2Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn3Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn4Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn5Max);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn6Max);
	
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn1Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn2Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn3Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn4Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn5Max);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_MAX)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn6Max);
	//判断上限值
	for (iLayerFixed=0; iLayerFixed<6;iLayerFixed++)
	{
		char tmpStrError[64];
		if (pes.layerParam[iLayerFixed].chn1Value * pes.layerParam[iLayerFixed].chn1Max <0 ||
			pes.layerParam[iLayerFixed].chn1Value * pes.layerParam[iLayerFixed].chn1Max >85 )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 定值*上限, 范围[0,85]!",iLayerFixed+1,1);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (pes.layerParam[iLayerFixed].chn2Value * pes.layerParam[iLayerFixed].chn2Max <0 ||
			pes.layerParam[iLayerFixed].chn2Value * pes.layerParam[iLayerFixed].chn2Max >85 )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 定值*上限, 范围[0,85]!",iLayerFixed+1,2);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (pes.layerParam[iLayerFixed].chn3Value * pes.layerParam[iLayerFixed].chn3Max <0 ||
			pes.layerParam[iLayerFixed].chn3Value * pes.layerParam[iLayerFixed].chn3Max >85 )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 定值*上限, 范围[0,85]!",iLayerFixed+1,3);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (pes.layerParam[iLayerFixed].chn4Value * pes.layerParam[iLayerFixed].chn4Max <0 ||
			pes.layerParam[iLayerFixed].chn4Value * pes.layerParam[iLayerFixed].chn4Max >85 )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 定值*上限, 范围[0,85]!",iLayerFixed+1,4);
			AfxMessageBox(tmpStrError);
			return;
		}		
	}
	//每个通道的参数, 下限
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn1Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn2Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn3Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn4Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn5Min);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[0].chn6Min);

	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn1Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn2Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn3Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn4Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn5Min);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[1].chn6Min);
	
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn1Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn2Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn3Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn4Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn5Min);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[2].chn6Min);
	
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn1Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn2Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn3Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn4Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn5Min);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[3].chn6Min);
	
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn1Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn2Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn3Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn4Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn5Min);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[4].chn6Min);
	
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn1Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn2Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn3Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn4Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn5Min);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_MIN)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &pes.layerParam[5].chn6Min);
	//判断下限值，必须小于上限
	for (iLayerFixed=0; iLayerFixed<6;iLayerFixed++)
	{
		char tmpStrError[64];
		if ( !(pes.layerParam[iLayerFixed].chn1Max==0 && pes.layerParam[iLayerFixed].chn1Min==0 ) &&
			pes.layerParam[iLayerFixed].chn1Max <= pes.layerParam[iLayerFixed].chn1Min )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 上限<=下限!",iLayerFixed+1,1);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (!(pes.layerParam[iLayerFixed].chn2Max==0 && pes.layerParam[iLayerFixed].chn2Min==0 ) &&
			pes.layerParam[iLayerFixed].chn2Max <= pes.layerParam[iLayerFixed].chn2Min )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 上限<=下限!",iLayerFixed+1,2);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (!(pes.layerParam[iLayerFixed].chn3Max==0 && pes.layerParam[iLayerFixed].chn3Min==0 ) &&
			pes.layerParam[iLayerFixed].chn3Max <= pes.layerParam[iLayerFixed].chn3Min )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 上限<=下限!",iLayerFixed+1,3);
			AfxMessageBox(tmpStrError);
			return;
		}
		if (!(pes.layerParam[iLayerFixed].chn4Max==0 && pes.layerParam[iLayerFixed].chn4Min==0 ) &&
			pes.layerParam[iLayerFixed].chn4Max <= pes.layerParam[iLayerFixed].chn4Min )
		{
			sprintf(tmpStrError,"第%d层, 通道:%d, 上限<=下限!",iLayerFixed+1,4);
			AfxMessageBox(tmpStrError);
			return;
		}
	
	}
	//每个通道的参数, 产品名称
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn1ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn2ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn3ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn4ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn5ProName);
	this->m_Layer1.GetDlgItem(IDC_EDIT_LAYER1_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[0].chn6ProName);

	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn1ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn2ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn3ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn4ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn5ProName);
	this->m_Layer2.GetDlgItem(IDC_EDIT_LAYER2_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[1].chn6ProName);
	
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn1ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn2ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn3ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn4ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn5ProName);
	this->m_Layer3.GetDlgItem(IDC_EDIT_LAYER3_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[2].chn6ProName);
	
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn1ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn2ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn3ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn4ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn5ProName);
	this->m_Layer4.GetDlgItem(IDC_EDIT_LAYER4_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[3].chn6ProName);
	
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn1ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn2ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn3ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn4ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn5ProName);
	this->m_Layer5.GetDlgItem(IDC_EDIT_LAYER5_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[4].chn6ProName);
	
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN1_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn1ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN2_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn2ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN3_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn3ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN4_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn4ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN5_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn5ProName);
	this->m_Layer6.GetDlgItem(IDC_EDIT_LAYER6_CHN6_PRONAME)->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", &pes.layerParam[5].chn6ProName);

	//输入参数校验-------------------------------------
	


	lpszOpenFile = new CFileDialog(TRUE,"","", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "文件类型(*.psf)|*.psf||");//生成一个对话框
	if(lpszOpenFile->DoModal()== IDOK )	//假如点击对话框确定按钮
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		//this->SetDlgItemText(IDC_EDIT_OUTPUT_FILE, szGetName);//在窗口标题上显示路径
		
		//读取bin文件
		pFileHandle = fopen( szGetName, "wb+" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"打开文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
		}
		numwritten = fwrite( &pes, sizeof( TEST_PARAM_STRUCT ), 1, pFileHandle );		
		if (numwritten==1 )//写DSP
		{
			//AfxMessageBox("OK");			
		}		
		fclose(pFileHandle); 
		
	}
	delete lpszOpenFile;	//释放分配的对话框		
}

void ParamEditDlg::OnButtonClearparam() 
{
	
}

void ParamEditDlg::OnSelchangeComboParamsetmode() 
{
	if(this->m_ComboParamSetMode.GetCurSel() == 0)
	{
		this->m_TabSheetParamEdit.SetCurSel(0);
	}

}


void ParamEditDlg::OnCheckSaveRecdata() 
{
	if ( ((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->GetCheck ( ) )
	{
		this->GetDlgItem( IDC_EDIT_SAVEDATA_INTERVAL )->EnableWindow(true);
	}else
	{
		this->GetDlgItem( IDC_EDIT_SAVEDATA_INTERVAL )->EnableWindow(false);
	}
	
}

void ParamEditDlg::OnButtonAddtimeseries() 
{
	char tmpStr[256]={0};
	int totalTestTime;									//总测试时间
	int startTime;										//开始时间
	int testTime;										//110V拉载测试时间
	int pulseOnTime;									//脉冲开时长
	int pulseOffTime;									//脉冲关时长
	int	pulseRepeatTime;								//脉冲开关重复次数
	int timeSeriesStartTime[MAX_ADD_TIMESERIES];		//读取时序中的添加时间
	int timeSeriesTestTime[MAX_ADD_TIMESERIES];			//读取时序中的测试时间

	//总测试时间
	this->GetDlgItem ( IDC_EDIT_LOADTEST_TOTALTIME )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("总测试时间不能为空!");
		return;
	}else
	{
		sscanf ( tmpStr, "%d", &totalTestTime );
		if (totalTestTime==0)
		{			
			AfxMessageBox("总测试时间大于0!");
			return;
		}
	}
	//时序开始时间
	this->GetDlgItem ( IDC_EDIT_STARTTIME )->GetWindowText( tmpStr, 30 );
	if (strlen(tmpStr) == 0)
	{
		AfxMessageBox("时序开始时间不能为空!");
		return;
	}else
	{
		sscanf ( tmpStr, "%d", &startTime );
		if (startTime >= totalTestTime)
		{			
			AfxMessageBox("时序跨度错误!");
			return;
		}
	}
	//时序数目判断
	if (m_ListTimeSeries.GetCount() >= MAX_ADD_TIMESERIES )
	{
		sprintf(tmpStr,"最多添加%d个时序!",MAX_ADD_TIMESERIES);
		AfxMessageBox(tmpStr);
		return;
		
	}
	//读取现有ListBox的时序,m_ListTimeSeries.GetCount()为添加的时序数目
	for (int i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		CString csTemp[4];
		m_ListTimeSeries.GetText(i,bufStr);

		AfxExtractSubString(csTemp[1], (LPCTSTR)bufStr, 1, ','); // 得到测试项
		AfxExtractSubString(csTemp[2], (LPCTSTR)bufStr, 2, ','); // 得到开始时间,单位分
		AfxExtractSubString(csTemp[3], (LPCTSTR)bufStr, 3, ','); // 得到测试时间,单位秒

		sscanf(csTemp[2],"%d",&timeSeriesStartTime[i]);
		sscanf(csTemp[3],"%d",&timeSeriesTestTime[i]);
	}


	//添加当前选中时序操作
	switch (m_ComboTestItem.GetCurSel())
	{
	case 0:
		AfxMessageBox("请选择执行操作!");
		break;
	case 1://脉冲开关
		{
			this->GetDlgItem ( IDC_EDIT_PULSE_ONTIME )->GetWindowText( tmpStr, 30 );
			if (strlen(tmpStr) == 0)
			{
				AfxMessageBox("开时长不能为空!");
				return;
			}else
			{
				sscanf ( tmpStr, "%d", &pulseOnTime );
				
			}
			this->GetDlgItem ( IDC_EDIT_PULSE_OFFTIME )->GetWindowText( tmpStr, 30 );
			if (strlen(tmpStr) == 0)
			{
				AfxMessageBox("关时长不能为空!");
				return;
			}else
			{
				sscanf ( tmpStr, "%d", &pulseOffTime );
				
			}
			if (pulseOffTime == 0 || pulseOnTime==0)
			{			
				AfxMessageBox("脉冲时长设置错误,不能为0!");
				return;
			}
			
			this->GetDlgItem ( IDC_EDIT_ONOFF_REPEATTIME )->GetWindowText( tmpStr, 30 );
			if (strlen(tmpStr) == 0)
			{
				AfxMessageBox("重复次数不能为空!");
				return;
			}else
			{
				sscanf ( tmpStr, "%d", &pulseRepeatTime );	
				if (pulseRepeatTime == 0)
				{			
					AfxMessageBox("重复次数不能为0!");
					return;
				}
			}
			testTime = (pulseOnTime+pulseOffTime)*pulseRepeatTime;//单位秒
			//判断时序跨度是否重叠
			for (int i=0;i<m_ListTimeSeries.GetCount();i++)
			{				
				if (startTime == timeSeriesStartTime[i])
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				if ( startTime*60 >(timeSeriesStartTime[i]*60)
					&& startTime*60 <(timeSeriesStartTime[i]*60+timeSeriesTestTime[i]+5) )
				{
					AfxMessageBox("时序跨度重叠!预留5秒");
					return;
				}
				if ( (startTime*60+testTime) > totalTestTime*60 )
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				
			}
			
			//插入时序
			sprintf(tmpStr,"在%d分,脉冲开关,%d,%d,%d,%d,%d",startTime,startTime,testTime,pulseOffTime,pulseOnTime,pulseRepeatTime);
			if ( strlen(tmpStr) >=128 )
			{
				AfxMessageBox("时序字符串长度不能超过128!");
				return;
			}
			
			m_ListTimeSeries.AddString(tmpStr);

		}
		break;
	case 2://110V拉载
		{
			this->GetDlgItem ( IDC_EDIT_TESTTIME )->GetWindowText( tmpStr, 30 );
			if (strlen(tmpStr) == 0)
			{
				AfxMessageBox("测试时间不能为空!");
				return;
			}else
			{
				sscanf ( tmpStr, "%d", &testTime );
				if (testTime == 0)
				{			
					AfxMessageBox("测试时间必须大于0!");
					return;
				}
			}
			//判断时序跨度是否重叠
			for (int i=0;i<m_ListTimeSeries.GetCount();i++)
			{				
				if (startTime == timeSeriesStartTime[i])
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				if ( startTime*60 >(timeSeriesStartTime[i]*60)
					&& startTime*60 <(timeSeriesStartTime[i]*60+timeSeriesTestTime[i]+5) )
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				if ( (startTime*60+testTime) > totalTestTime*60 )
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
					
			}
			
			//插入时序
			sprintf(tmpStr,"在%d分,110V拉载测试,%d,%d",startTime,startTime,testTime);
			if ( strlen(tmpStr) >=128 )
			{
				AfxMessageBox("时序字符串长度不能超过128!");
				return;
			}
			m_ListTimeSeries.AddString(tmpStr);

		}
		break;
	case 3://220V拉载
		{
			this->GetDlgItem ( IDC_EDIT_TESTTIME )->GetWindowText( tmpStr, 30 );
			if (strlen(tmpStr) == 0)
			{
				AfxMessageBox("测试时间不能为空!");
				return;
			}else
			{
				sscanf ( tmpStr, "%d", &testTime );
				if (testTime == 0)
				{			
					AfxMessageBox("测试时间必须大于0!");
					return;
				}
			}
			//判断时序跨度是否重叠
			for (int i=0;i<m_ListTimeSeries.GetCount();i++)
			{				
				if (startTime == timeSeriesStartTime[i])
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				if ( startTime*60 >(timeSeriesStartTime[i]*60)
					&& startTime*60 <(timeSeriesStartTime[i]*60+timeSeriesTestTime[i]+5) )
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
				if ( (startTime*60+testTime) > totalTestTime*60 )
				{
					AfxMessageBox("时序跨度重叠!");
					return;
				}
					
			}
			
			//插入时序
			sprintf(tmpStr,"在%d分,220V拉载测试,%d,%d",startTime,startTime,testTime);
			if ( strlen(tmpStr) >=128 )
			{
				AfxMessageBox("时序字符串长度不能超过128!");
				return;
			}
			m_ListTimeSeries.AddString(tmpStr);

		}
		break;
	}
	
	
	
	
	
}

void ParamEditDlg::OnButtonDeltimeseries() 
{
	m_ListTimeSeries.DeleteString(m_ListTimeSeries.GetCurSel());	
}

void ParamEditDlg::OnSelchangeComboTestitem() 
{
	switch(this->m_ComboTestItem.GetCurSel())
	{
	case 0:
		
		GetDlgItem(IDC_STATIC_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_TESTTIME_SECOND)->ShowWindow(false);
		
		GetDlgItem(IDC_STATIC_PULSEONTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PULSE_ONTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEONTIME_SECOND)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_PULSEOFFTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PULSE_OFFTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEOFFTIME_SECOND)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_PULSEREPEATTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_ONOFF_REPEATTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEREPEATTIME_VAL)->ShowWindow(false);

		break;
	case 1://脉冲开关
		GetDlgItem(IDC_STATIC_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_TESTTIME_SECOND)->ShowWindow(false);
		
		GetDlgItem(IDC_STATIC_PULSEONTIME)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PULSE_ONTIME)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_PULSEONTIME_SECOND)->ShowWindow(true);
		
		GetDlgItem(IDC_STATIC_PULSEOFFTIME)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PULSE_OFFTIME)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_PULSEOFFTIME_SECOND)->ShowWindow(true);
		
		GetDlgItem(IDC_STATIC_PULSEREPEATTIME)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_ONOFF_REPEATTIME)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_PULSEREPEATTIME_VAL)->ShowWindow(true);
		


		break;
	case 2://110V
	case 3://220V
		GetDlgItem(IDC_STATIC_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_TESTTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_TESTTIME_SECOND)->ShowWindow(false);
		
		GetDlgItem(IDC_STATIC_PULSEONTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PULSE_ONTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEONTIME_SECOND)->ShowWindow(false);
		
		GetDlgItem(IDC_STATIC_PULSEOFFTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PULSE_OFFTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEOFFTIME_SECOND)->ShowWindow(false);
		
		GetDlgItem(IDC_STATIC_PULSEREPEATTIME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_ONOFF_REPEATTIME)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PULSEREPEATTIME_VAL)->ShowWindow(false);

		break;
	}	// TODO: Add your control notification handler code here
	
}
