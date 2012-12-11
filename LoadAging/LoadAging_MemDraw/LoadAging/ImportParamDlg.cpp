// ImportParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "loadaging.h"
#include "ImportParamDlg.h"
#include "LoadAging_Tran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImportParamDlg dialog


ImportParamDlg::ImportParamDlg(CWnd* pParent /*=NULL*/)
: CDialog(ImportParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ImportParamDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ImportParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ImportParamDlg)
	DDX_Control(pDX, IDC_LIST_TIMESEIRES, m_ListTimeSeries);
	DDX_Control(pDX, IDC_TAB_ALLLAYER, m_TabCtrlAllLayer);
	DDX_Control(pDX, IDC_TAB_ALLCAR, m_TabCtrlAllCar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ImportParamDlg, CDialog)
//{{AFX_MSG_MAP(ImportParamDlg)
ON_BN_CLICKED(IDC_BUTTON_LOADPARAM, OnButtonLoadparam)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ALLCAR, OnSelchangeTabAllcar)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ALLLAYER, OnSelchangeTabAlllayer)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ImportParamDlg message handlers
/************************************************************************/
/* 参数导入窗口初始化                                                   */
/************************************************************************/
BOOL ImportParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TabCtrlAllCar.InsertItem(0, _T("A1区"));
	m_TabCtrlAllCar.InsertItem(1, _T("A2区"));
	m_TabCtrlAllCar.InsertItem(2, _T("A3区"));
	m_TabCtrlAllCar.InsertItem(3, _T("A4区"));
	m_TabCtrlAllCar.InsertItem(4, _T("B1区"));
	m_TabCtrlAllCar.InsertItem(5, _T("B2区"));
	m_TabCtrlAllCar.InsertItem(6, _T("B3区"));
	m_TabCtrlAllCar.InsertItem(7, _T("B4区"));
	m_TabCtrlAllCar.InsertItem(8, _T("C1区"));
	m_TabCtrlAllCar.InsertItem(9, _T("C2区"));
	m_TabCtrlAllCar.InsertItem(10, _T("C3区"));
	m_TabCtrlAllCar.InsertItem(11, _T("C4区"));
	m_TabCtrlAllCar.InsertItem(12, _T("D1区"));
	m_TabCtrlAllCar.InsertItem(13, _T("D2区"));
	m_TabCtrlAllCar.InsertItem(14, _T("D3区"));
	m_TabCtrlAllCar.InsertItem(15, _T("D4区"));
	
	m_TabCtrlAllLayer.InsertItem(0, _T("第1层"));
	m_TabCtrlAllLayer.InsertItem(1, _T("第2层"));
	m_TabCtrlAllLayer.InsertItem(2, _T("第3层"));
	m_TabCtrlAllLayer.InsertItem(3, _T("第4层"));
	m_TabCtrlAllLayer.InsertItem(4, _T("第5层"));
	m_TabCtrlAllLayer.InsertItem(5, _T("第6层"));
	
	char	tmpStr[64]={0};
	int		selTab,selLayerTab;						
	
	selTab=m_TabCtrlAllCar.GetCurSel();					//当前选中的测试区
	if(strlen(g_AllCar[selTab].paramFileName) == 0)		//未进行参数导入
	{
		this->SetDlgItemText( IDC_EDIT_LAYER,"");
		this->SetDlgItemText( IDC_EDIT_LOADPERLAYER,"");
		this->SetDlgItemText( IDC_EDIT_START_LOADNUM,"");
		this->SetDlgItemText( IDC_EDIT_END_LOADNUM,"" );
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"");
		this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,"" );	
		this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,"" );
		((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (false);
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"" );
		m_ListTimeSeries.ResetContent();
		//电压、电流固定值
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//电压、电流上限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//电压、电流下限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		//产品名称
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( "");
		
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//在窗口标题上显示路径
		
	}else 
	{
		//界面显示,已导入的参数
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, g_AllCar[selTab].paramFileName);				//在窗口标题上显示路径
		
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.layerPerCar);	//层数
		this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);		
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.loadPerLayer);	//每层负载数
		this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.startLoadNum);	//扫描开始地址
		this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.endLoadNum);		//扫描结束地址
		this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.totalTestTime);	//总测试时间
		this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
		if (g_AllCar[selTab].testParam.paramSetMode == 0)
		{
			this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"整区设置");
		}else if (g_AllCar[selTab].testParam.paramSetMode == 1)
		{
			this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"分层设置");
		}	

		((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (g_AllCar[selTab].testParam.isSaveData );
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.saveDataInterval);		//保存数据时间间隔
		this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

		//插入时序
		m_ListTimeSeries.ResetContent();
		for (int i=0;i<MAX_ADD_TIMESERIES;i++)
		{
			if( strlen(g_AllCar[selTab].testParam.timeSeries[i].timeSeries) != 0)
			{
				m_ListTimeSeries.AddString(g_AllCar[selTab].testParam.timeSeries[i].timeSeries);
			}
		}
		//每个通道的参数, 模式
		selLayerTab = m_TabCtrlAllLayer.GetCurSel();		//当前选层
		if (g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 0)
		{
			this->SetDlgItemText( IDC_EDIT_LOADMODE,"CC模式" );
		}else if(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 1)
		{
			this->SetDlgItemText( IDC_EDIT_LOADMODE,"CV模式" );
		}	
		switch(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode)
		{
		case 0://CC
			this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
			this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
			this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
			break;
		case 1://CV
			this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
			this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
			this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
			break;
		}	
		
		//电压、电流固定值
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Value);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Value);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Value);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Value);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[0].chn5Value);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[0].chn6Value);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->SetWindowText( tmpStr);
		
		//电压、电流上限
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Max);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Max);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Max);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Max);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn5Max);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn6Max);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->SetWindowText( tmpStr);
		
		//电压、电流下限
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Min);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Min);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Min);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Min);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn5Min);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->SetWindowText( tmpStr);
		//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn6Min);
		//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->SetWindowText( tmpStr);
		//产品名称
		sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1ProName);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2ProName);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3ProName);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( tmpStr);
		sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4ProName);
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( tmpStr);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
/************************************************************************/
/* 载入参数,只是在界面显示，并未进行真正的参数导入                      */
/************************************************************************/
void ImportParamDlg::OnButtonLoadparam() 
{
	int		numberRead = -1;				//每次读到的字节数
	int		selLayerTab;					//当前选中的Tab
	char	tmpStr[256]={0};
	FILE	*pFileHandle;					//文件句柄
	ULONG	nSize = 0;
	TEST_PARAM_STRUCT readSS;
	
	CFileDialog *lpszOpenFile;				//定义一个CfileDialog对象
	CString szGetName;

	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	
	lpszOpenFile = new	CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, "文件类型(*.psf)|*.psf||");//生成一个对话框
	if(lpszOpenFile->DoModal()== IDOK )	//假如点击对话框确定按钮
	{		
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		
		//读取psf文件
		pFileHandle = fopen( szGetName, "rb" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"打开文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
			return;
		}
		fseek(pFileHandle, nSize, 0);//文件指针移动到文件头
		numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
		if (numberRead<0)//
		{			
			sprintf(tmpStr,"读取文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
			return;
		}		
		fclose(pFileHandle); 
		delete lpszOpenFile;	//释放分配的对话框	
		
	}else
	{
		delete lpszOpenFile;	//释放分配的对话框	
		return;
	}
	
	//界面显示------------------------------------------------------------------
	this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, szGetName);				//在窗口标题上显示文件全路径名称

	sprintf(tmpStr,"%d",readSS.layerPerCar);	//层数
	this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);

	sprintf(tmpStr,"%d",readSS.loadPerLayer);	//负载数
	this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);

	sprintf(tmpStr,"%d",readSS.startLoadNum);	//扫描开始地址
	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
	sprintf(tmpStr,"%d",readSS.endLoadNum);		//扫描结束地址
	this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
	
	if (readSS.paramSetMode == 0)//设置方式
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"整区设置");
	}else if (readSS.paramSetMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"分层设置");
	}	
	sprintf(tmpStr,"%d",readSS.totalTestTime);		//总测试时间
	this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
	
	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (readSS.isSaveData );	//是否保存数据
	
	sprintf(tmpStr,"%d",readSS.saveDataInterval);		//保存数据时间间隔
	this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

	//插入时序
	m_ListTimeSeries.ResetContent();
	for (int i=0;i<MAX_ADD_TIMESERIES;i++)
	{
		if( strlen(readSS.timeSeries[i].timeSeries) != 0)
		{
			m_ListTimeSeries.AddString(readSS.timeSeries[i].timeSeries);
		}
	}
	//通道参数显示，当前选中层
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();//当前选中层
	if (readSS.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CC模式" );
	}else if(readSS.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CV模式" );
	}	
	switch(readSS.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
	
	//电压、电流固定值
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn5Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn6Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->SetWindowText( tmpStr);
	
	//电压、电流上限
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn5Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn6Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->SetWindowText( tmpStr);
	
	//电压、电流下限
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn5Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn6Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->SetWindowText( tmpStr);
	
	//产品名称
	sprintf(tmpStr,"%s",readSS.layerParam[selLayerTab].chn1ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[selLayerTab].chn2ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[selLayerTab].chn3ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",readSS.layerParam[selLayerTab].chn4ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( tmpStr);

	
}

void ImportParamDlg::OnSelchangeTabAllcar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char	tmpStr[64]={0};
	int		selTab,selLayerTab;	
	
	selTab=m_TabCtrlAllCar.GetCurSel();//当前选中测试区
	//参数导入按钮
	sprintf(tmpStr, "%s载入参数", GetTestCarName(selTab));
	this->SetDlgItemText( IDC_BUTTON_LOADPARAM,tmpStr);

	if(strlen(g_AllCar[selTab].paramFileName) == 0)		//未进行参数导入
	{
		this->SetDlgItemText( IDC_EDIT_LAYER,"");
		this->SetDlgItemText( IDC_EDIT_LOADPERLAYER,"");
		this->SetDlgItemText( IDC_EDIT_START_LOADNUM,"");
		this->SetDlgItemText( IDC_EDIT_END_LOADNUM,"" );
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"");
		this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,"" );
		this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,"" );
		m_ListTimeSeries.ResetContent();
		((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (false);
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"" );
		//电压、电流固定值
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//电压、电流上限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//电压、电流下限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		//产品名称
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( "");

		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//在窗口标题上显示参数文件全路径名称
		
		return;
	}
	//界面显示,已导入的参数
	this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, g_AllCar[selTab].paramFileName);				//在窗口标题上显示路径

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.layerPerCar);	//层数
	this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.loadPerLayer);	//层数
	this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);


	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.startLoadNum);	//扫描开始地址
	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.endLoadNum);		//扫描结束地址
	this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
	
	if (g_AllCar[selTab].testParam.paramSetMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"整区设置");
	}else if (g_AllCar[selTab].testParam.paramSetMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"分层设置");
	}	

	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (g_AllCar[selTab].testParam.isSaveData );
	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.saveDataInterval);		//保存数据时间间隔
	this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.totalTestTime);		//总测试时间
	this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
	//插入时序
	m_ListTimeSeries.ResetContent();
	for (int i=0;i<MAX_ADD_TIMESERIES;i++)
	{
		if( strlen(g_AllCar[selTab].testParam.timeSeries[i].timeSeries) != 0)
		{
			m_ListTimeSeries.AddString(g_AllCar[selTab].testParam.timeSeries[i].timeSeries);
		}
	}
	//每个通道的参数, 模式
	//当前选层
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();
	if (g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CC模式" );
	}else if(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CV模式" );
	}	
	switch(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
	
	//电压、电流固定值
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[0].chn5Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[0].chn6Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->SetWindowText( tmpStr);
	
	//电压、电流上限
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn5Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn6Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->SetWindowText( tmpStr);
	
	//电压、电流下限
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn5Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn6Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->SetWindowText( tmpStr);
	
	//产品名称
	sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn1ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn2ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn3ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%s",g_AllCar[selTab].testParam.layerParam[selLayerTab].chn4ProName);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( tmpStr);
	
	
	*pResult = 0;
}
/************************************************************************/
/* 参数导入界面，不同层切换                                             */
/************************************************************************/
void ImportParamDlg::OnSelchangeTabAlllayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char	tmpStr[64]={0};
	int		selTab,selLayerTab;		//当前选中的Tab
	int		numberRead = -1;				//每次读到的字节数
	FILE	*pFileHandle;			//文件句柄
	ULONG	nSize = 0;
	TEST_PARAM_STRUCT readSS;	
	CString szGetName;

	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	selTab=m_TabCtrlAllCar.GetCurSel();				//当前层
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();	//当前测试区

	this->GetDlgItemText(IDC_EDIT_LOAD_FILENAME,szGetName);//获取导入的参数文件全路径名称
 
	//界面显示------------------------------------------------------------------
	//每个通道的参数, 模式，当前选层
	if(strlen(szGetName) == 0)//未进行参数导入
	{
		
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"" );
		//电压、电流固定值
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//电压、电流上限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//电压、电流下限
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//在窗口标题上显示路径
		
		return;
	}
	
	pFileHandle = fopen( szGetName, "rb" );
	if (pFileHandle == NULL)
	{
		sprintf(tmpStr,"打开文件失败, %s",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}	
	nSize = 0x0;
	fseek(pFileHandle, nSize, 0);
	numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
	if (numberRead<0)
	{		
		sprintf(tmpStr,"读取文件失败, %s",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}		
	fclose(pFileHandle);

	if (readSS.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CC模式" );
	}else if(readSS.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CV模式" );
	}	
	switch(readSS.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
	
	//电压、电流固定值
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Value);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn5Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_VALUE)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[0].chn6Value);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_VALUE)->SetWindowText( tmpStr);
	
	//电压、电流上限
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Max);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn5Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MAX)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn6Max);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MAX)->SetWindowText( tmpStr);
	
	//电压、电流下限
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn1Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn2Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn3Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( tmpStr);
	sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn4Min);
	this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn5Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN5_MIN)->SetWindowText( tmpStr);
	//sprintf(tmpStr,"%.3f",readSS.layerParam[selLayerTab].chn6Min);
	//this->GetDlgItem(IDC_EDIT_LAYER1_CHN6_MIN)->SetWindowText( tmpStr);	
	*pResult = 0;
}
/************************************************************************/
/* 导入参数，将参数文件中的内容读取到全局结构体中                       */
/************************************************************************/
void ImportParamDlg::OnOK() 
{
	int			i;
	int			numberRead=-1;
	int			selTab;					//当前选中的Tab
	int			testItemNum=0;			//时序测试项，数目，经计算得到
	CString		szGetName;				//参数文件全路径名称
	FILE		*pFileHandle;			//文件句柄
	ULONG		nSize = 0;
	char		tmpStr[256]={0};
	TEST_PARAM_STRUCT readSS;
	
	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	
	//当前选中工作区,当前选中测试车
	selTab = m_TabCtrlAllCar.GetCurSel();
	if (g_AllCar[selTab].m_CarState == CAR_STATE_TESTING) //当前车没有在测试中才可进行参数导入
	{
		sprintf(tmpStr,"%s, Please Stop Test First!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;
	}	
	this->GetDlgItemText(IDC_EDIT_LOAD_FILENAME,szGetName);
	if(strlen(szGetName) == 0)
	{
		sprintf(tmpStr,"%s, 请先导入参数!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;	
	}
	pFileHandle = fopen( szGetName, "rb" );
	if (pFileHandle == NULL)
	{
		sprintf(tmpStr,"打开文件失败, %s!",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}
	fseek(pFileHandle, nSize, 0);	//文件指针移动到文件头
	numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
	if (numberRead<0)	//文件读取失败
	{			
		sprintf(tmpStr,"读取文件失败, %s!",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}		
	fclose(pFileHandle); 	
	
	
	strcpy(g_AllCar[selTab].paramFileName, szGetName);						//记录导入文件全路径名称
	
	//导入文件参数结构体
	memcpy(&g_AllCar[selTab].testParam, &readSS,sizeof(TEST_PARAM_STRUCT));	
	
	//时序转换---------------------------------------------------------
	//读取现有ListBox的时序,计算测试项
	for ( i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		CString csTemp[5];
		int pulseRepeatTime;	//脉冲开关
		m_ListTimeSeries.GetText(i,bufStr);

		AfxExtractSubString(csTemp[1], (LPCTSTR)bufStr, 1, ','); // 得到测试项
		AfxExtractSubString(csTemp[2], (LPCTSTR)bufStr, 2, ','); // 得到开始时间,单位分
		AfxExtractSubString(csTemp[3], (LPCTSTR)bufStr, 3, ','); // 得到测试时间,单位秒
		if(csTemp[1] == "脉冲开关")
		{
			AfxExtractSubString(csTemp[4], (LPCTSTR)bufStr, 6, ','); //脉冲重复次数
			sscanf(csTemp[4],"%d",&pulseRepeatTime);
			testItemNum += pulseRepeatTime*2;

		}else if(csTemp[1] == "110V拉载测试")
		{
			testItemNum++;
		}else if(csTemp[1] == "220V拉载测试")
		{
			testItemNum++;
		}
	}
	g_AllCar[selTab].pTimeSeires = new TIMESEIRES_STRUCT[testItemNum];
	if(g_AllCar[selTab].pTimeSeires == NULL)
	{
		AfxMessageBox("Error, new TIMESERIES_STRUCT fail!");
		return;
	}
	//读取现有ListBox的时序,填写测试项
	for ( i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		CString csTemp[7];
		int pulseRepeatTime;		//脉冲开关
		int timeSeriesStartTime;	//开始测试时间
		int timeSeriesTestTime;		//测试时间
		int pulseOnTime;
		int pulseOffTime;
		m_ListTimeSeries.GetText(i,bufStr);

		AfxExtractSubString(csTemp[1], (LPCTSTR)bufStr, 1, ','); // 得到测试项
		AfxExtractSubString(csTemp[2], (LPCTSTR)bufStr, 2, ','); // 得到开始时间,单位分
		AfxExtractSubString(csTemp[3], (LPCTSTR)bufStr, 3, ','); // 得到测试时间,单位秒
		sscanf(csTemp[2],"%d",&timeSeriesStartTime);
		sscanf(csTemp[3],"%d",&timeSeriesTestTime);

		if(csTemp[1] == "脉冲开关")
		{
			AfxExtractSubString(csTemp[4], (LPCTSTR)bufStr, 4, ','); //脉冲开时长
			AfxExtractSubString(csTemp[5], (LPCTSTR)bufStr, 5, ','); //脉冲关时长
			AfxExtractSubString(csTemp[6], (LPCTSTR)bufStr, 6, ','); //脉冲重复次数
			sscanf(csTemp[4],"%d",&pulseOffTime);
			sscanf(csTemp[5],"%d",&pulseOnTime);
			sscanf(csTemp[6],"%d",&pulseRepeatTime);
			for(int j=0;j<pulseRepeatTime;j++)
			{
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_PULSEOFF;
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60 + j*(pulseOnTime+pulseOffTime);
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = pulseOnTime;
				g_AllCar[selTab].testItemNum++;
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_PULSEON;
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60+pulseOffTime+j*(pulseOnTime+pulseOffTime);
				g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = pulseOffTime;
				g_AllCar[selTab].testItemNum++;
			}

		}else if(csTemp[1] == "110V拉载测试")
		{
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_110V;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = timeSeriesTestTime;
			g_AllCar[selTab].testItemNum++;
		}else if(csTemp[1] == "220V拉载测试")
		{
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_220V;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = timeSeriesTestTime;
			g_AllCar[selTab].testItemNum++;
		}
	}

	//全局负载，
	for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)
	{
		g_AllCar[selTab].m_Load[loadIndex-1].m_LayerID = (loadIndex-1)/readSS.loadPerLayer+1;//所属层数计算
		g_AllCar[selTab].m_Load[loadIndex-1].m_LoadState = LOAD_STATE_SELECT;		//更改负载状态为选中				
	}
	
	//根据通道测试参数对全局结构体进行设置
	if (readSS.paramSetMode == 0)//整区设置
	{
		for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)
		{
			g_AllCar[selTab].m_Load[loadIndex-1].m_LoadMode = readSS.layerParam[0].loadMode;//第一层参数赋值给每个通道
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetValue=readSS.layerParam[0].chn1Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetMax=readSS.layerParam[0].chn1Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetMin=readSS.layerParam[0].chn1Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetProName,readSS.layerParam[0].chn1ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetValue=readSS.layerParam[0].chn2Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetMax=readSS.layerParam[0].chn2Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetMin=readSS.layerParam[0].chn2Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetProName,readSS.layerParam[0].chn2ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetValue=readSS.layerParam[0].chn3Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetMax=readSS.layerParam[0].chn3Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetMin=readSS.layerParam[0].chn3Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetProName,readSS.layerParam[0].chn3ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetValue=readSS.layerParam[0].chn4Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetMax=readSS.layerParam[0].chn4Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetMin=readSS.layerParam[0].chn4Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetProName,readSS.layerParam[0].chn4ProName);
			
		}
		
	}else if (readSS.paramSetMode == 1)//分层设置
	{
		for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)//全局负载参数设置，根据全局负载所属的层，分别对通道参数赋值
		{
			int layerID = g_AllCar[selTab].m_Load[loadIndex-1].m_LayerID-1;//属于哪一层

			g_AllCar[selTab].m_Load[loadIndex-1].m_LoadMode = readSS.layerParam[layerID].loadMode;
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetValue=readSS.layerParam[layerID].chn1Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetMax=readSS.layerParam[layerID].chn1Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetMin=readSS.layerParam[layerID].chn1Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[0].m_SetProName,readSS.layerParam[layerID].chn1ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetValue=readSS.layerParam[layerID].chn2Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetMax=readSS.layerParam[layerID].chn2Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetMin=readSS.layerParam[layerID].chn2Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[1].m_SetProName,readSS.layerParam[layerID].chn2ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetValue=readSS.layerParam[layerID].chn3Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetMax=readSS.layerParam[layerID].chn3Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetMin=readSS.layerParam[layerID].chn3Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[2].m_SetProName,readSS.layerParam[layerID].chn3ProName);
			
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetValue=readSS.layerParam[layerID].chn4Value;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetMax=readSS.layerParam[layerID].chn4Max;
			g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetMin=readSS.layerParam[layerID].chn4Min;
			strcpy(g_AllCar[selTab].m_Load[loadIndex-1].m_Channel[3].m_SetProName,readSS.layerParam[layerID].chn4ProName);
			
		}
		
	}
			
	//测试车状态更改为
	g_AllCar[selTab].m_CarState = CAR_STATE_IMPORTED;	
	
	CDialog::OnOK();
	
}

