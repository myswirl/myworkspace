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
/* �������봰�ڳ�ʼ��                                                   */
/************************************************************************/
BOOL ImportParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TabCtrlAllCar.InsertItem(0, _T("A1��"));
	m_TabCtrlAllCar.InsertItem(1, _T("A2��"));
	m_TabCtrlAllCar.InsertItem(2, _T("A3��"));
	m_TabCtrlAllCar.InsertItem(3, _T("A4��"));
	m_TabCtrlAllCar.InsertItem(4, _T("B1��"));
	m_TabCtrlAllCar.InsertItem(5, _T("B2��"));
	m_TabCtrlAllCar.InsertItem(6, _T("B3��"));
	m_TabCtrlAllCar.InsertItem(7, _T("B4��"));
	m_TabCtrlAllCar.InsertItem(8, _T("C1��"));
	m_TabCtrlAllCar.InsertItem(9, _T("C2��"));
	m_TabCtrlAllCar.InsertItem(10, _T("C3��"));
	m_TabCtrlAllCar.InsertItem(11, _T("C4��"));
	m_TabCtrlAllCar.InsertItem(12, _T("D1��"));
	m_TabCtrlAllCar.InsertItem(13, _T("D2��"));
	m_TabCtrlAllCar.InsertItem(14, _T("D3��"));
	m_TabCtrlAllCar.InsertItem(15, _T("D4��"));
	
	m_TabCtrlAllLayer.InsertItem(0, _T("��1��"));
	m_TabCtrlAllLayer.InsertItem(1, _T("��2��"));
	m_TabCtrlAllLayer.InsertItem(2, _T("��3��"));
	m_TabCtrlAllLayer.InsertItem(3, _T("��4��"));
	m_TabCtrlAllLayer.InsertItem(4, _T("��5��"));
	m_TabCtrlAllLayer.InsertItem(5, _T("��6��"));
	
	char	tmpStr[64]={0};
	int		selTab,selLayerTab;						
	
	selTab=m_TabCtrlAllCar.GetCurSel();					//��ǰѡ�еĲ�����
	if(strlen(g_AllCar[selTab].paramFileName) == 0)		//δ���в�������
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
		//��ѹ�������̶�ֵ
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		//��Ʒ����
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( "");
		
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//�ڴ��ڱ�������ʾ·��
		
	}else 
	{
		//������ʾ,�ѵ���Ĳ���
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, g_AllCar[selTab].paramFileName);				//�ڴ��ڱ�������ʾ·��
		
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.layerPerCar);	//����
		this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);		
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.loadPerLayer);	//ÿ�㸺����
		this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.startLoadNum);	//ɨ�迪ʼ��ַ
		this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.endLoadNum);		//ɨ�������ַ
		this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.totalTestTime);	//�ܲ���ʱ��
		this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
		if (g_AllCar[selTab].testParam.paramSetMode == 0)
		{
			this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"��������");
		}else if (g_AllCar[selTab].testParam.paramSetMode == 1)
		{
			this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"�ֲ�����");
		}	

		((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (g_AllCar[selTab].testParam.isSaveData );
		sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.saveDataInterval);		//��������ʱ����
		this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

		//����ʱ��
		m_ListTimeSeries.ResetContent();
		for (int i=0;i<MAX_ADD_TIMESERIES;i++)
		{
			if( strlen(g_AllCar[selTab].testParam.timeSeries[i].timeSeries) != 0)
			{
				m_ListTimeSeries.AddString(g_AllCar[selTab].testParam.timeSeries[i].timeSeries);
			}
		}
		//ÿ��ͨ���Ĳ���, ģʽ
		selLayerTab = m_TabCtrlAllLayer.GetCurSel();		//��ǰѡ��
		if (g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 0)
		{
			this->SetDlgItemText( IDC_EDIT_LOADMODE,"CCģʽ" );
		}else if(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 1)
		{
			this->SetDlgItemText( IDC_EDIT_LOADMODE,"CVģʽ" );
		}	
		switch(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode)
		{
		case 0://CC
			this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
			this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
			this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
			break;
		case 1://CV
			this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
			this->SetDlgItemText(IDC_STATIC_MAX,"��������");
			this->SetDlgItemText(IDC_STATIC_MIN,"��������");
			break;
		}	
		
		//��ѹ�������̶�ֵ
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
		
		//��ѹ����������
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
		
		//��ѹ����������
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
		//��Ʒ����
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
/* �������,ֻ���ڽ�����ʾ����δ���������Ĳ�������                      */
/************************************************************************/
void ImportParamDlg::OnButtonLoadparam() 
{
	int		numberRead = -1;				//ÿ�ζ������ֽ���
	int		selLayerTab;					//��ǰѡ�е�Tab
	char	tmpStr[256]={0};
	FILE	*pFileHandle;					//�ļ����
	ULONG	nSize = 0;
	TEST_PARAM_STRUCT readSS;
	
	CFileDialog *lpszOpenFile;				//����һ��CfileDialog����
	CString szGetName;

	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	
	lpszOpenFile = new	CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, "�ļ�����(*.psf)|*.psf||");//����һ���Ի���
	if(lpszOpenFile->DoModal()== IDOK )	//�������Ի���ȷ����ť
	{		
		szGetName = lpszOpenFile->GetPathName();//�õ����ļ���·��
		
		//��ȡpsf�ļ�
		pFileHandle = fopen( szGetName, "rb" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"���ļ�ʧ��, %s",szGetName);
			AfxMessageBox(tmpStr);
			return;
		}
		fseek(pFileHandle, nSize, 0);//�ļ�ָ���ƶ����ļ�ͷ
		numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
		if (numberRead<0)//
		{			
			sprintf(tmpStr,"��ȡ�ļ�ʧ��, %s",szGetName);
			AfxMessageBox(tmpStr);
			return;
		}		
		fclose(pFileHandle); 
		delete lpszOpenFile;	//�ͷŷ���ĶԻ���	
		
	}else
	{
		delete lpszOpenFile;	//�ͷŷ���ĶԻ���	
		return;
	}
	
	//������ʾ------------------------------------------------------------------
	this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, szGetName);				//�ڴ��ڱ�������ʾ�ļ�ȫ·������

	sprintf(tmpStr,"%d",readSS.layerPerCar);	//����
	this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);

	sprintf(tmpStr,"%d",readSS.loadPerLayer);	//������
	this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);

	sprintf(tmpStr,"%d",readSS.startLoadNum);	//ɨ�迪ʼ��ַ
	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
	sprintf(tmpStr,"%d",readSS.endLoadNum);		//ɨ�������ַ
	this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
	
	if (readSS.paramSetMode == 0)//���÷�ʽ
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"��������");
	}else if (readSS.paramSetMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"�ֲ�����");
	}	
	sprintf(tmpStr,"%d",readSS.totalTestTime);		//�ܲ���ʱ��
	this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
	
	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (readSS.isSaveData );	//�Ƿ񱣴�����
	
	sprintf(tmpStr,"%d",readSS.saveDataInterval);		//��������ʱ����
	this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

	//����ʱ��
	m_ListTimeSeries.ResetContent();
	for (int i=0;i<MAX_ADD_TIMESERIES;i++)
	{
		if( strlen(readSS.timeSeries[i].timeSeries) != 0)
		{
			m_ListTimeSeries.AddString(readSS.timeSeries[i].timeSeries);
		}
	}
	//ͨ��������ʾ����ǰѡ�в�
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();//��ǰѡ�в�
	if (readSS.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CCģʽ" );
	}else if(readSS.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CVģʽ" );
	}	
	switch(readSS.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
		this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��������");
		this->SetDlgItemText(IDC_STATIC_MIN,"��������");
		break;
	}	
	
	//��ѹ�������̶�ֵ
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
	
	//��ѹ����������
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
	
	//��ѹ����������
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
	
	//��Ʒ����
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
	
	selTab=m_TabCtrlAllCar.GetCurSel();//��ǰѡ�в�����
	//�������밴ť
	sprintf(tmpStr, "%s�������", GetTestCarName(selTab));
	this->SetDlgItemText( IDC_BUTTON_LOADPARAM,tmpStr);

	if(strlen(g_AllCar[selTab].paramFileName) == 0)		//δ���в�������
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
		//��ѹ�������̶�ֵ
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		//��Ʒ����
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_PRONAME)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_PRONAME)->SetWindowText( "");

		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//�ڴ��ڱ�������ʾ�����ļ�ȫ·������
		
		return;
	}
	//������ʾ,�ѵ���Ĳ���
	this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, g_AllCar[selTab].paramFileName);				//�ڴ��ڱ�������ʾ·��

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.layerPerCar);	//����
	this->SetDlgItemText(IDC_EDIT_LAYER,tmpStr);

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.loadPerLayer);	//����
	this->SetDlgItemText(IDC_EDIT_LOADPERLAYER,tmpStr);


	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.startLoadNum);	//ɨ�迪ʼ��ַ
	this->SetDlgItemText(IDC_EDIT_START_LOADNUM,tmpStr);
	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.endLoadNum);		//ɨ�������ַ
	this->SetDlgItemText( IDC_EDIT_END_LOADNUM,tmpStr );
	
	if (g_AllCar[selTab].testParam.paramSetMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"��������");
	}else if (g_AllCar[selTab].testParam.paramSetMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_PARAMSETMODE,"�ֲ�����");
	}	

	((CButton *)this->GetDlgItem (IDC_CHECK_SAVE_RECDATA))->SetCheck (g_AllCar[selTab].testParam.isSaveData );
	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.saveDataInterval);		//��������ʱ����
	this->SetDlgItemText( IDC_EDIT_SAVEDATA_INTERVAL,tmpStr );

	sprintf(tmpStr,"%d",g_AllCar[selTab].testParam.totalTestTime);		//�ܲ���ʱ��
	this->SetDlgItemText( IDC_EDIT_LOADTEST_TOTALTIME,tmpStr );
	//����ʱ��
	m_ListTimeSeries.ResetContent();
	for (int i=0;i<MAX_ADD_TIMESERIES;i++)
	{
		if( strlen(g_AllCar[selTab].testParam.timeSeries[i].timeSeries) != 0)
		{
			m_ListTimeSeries.AddString(g_AllCar[selTab].testParam.timeSeries[i].timeSeries);
		}
	}
	//ÿ��ͨ���Ĳ���, ģʽ
	//��ǰѡ��
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();
	if (g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CCģʽ" );
	}else if(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CVģʽ" );
	}	
	switch(g_AllCar[selTab].testParam.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
		this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��������");
		this->SetDlgItemText(IDC_STATIC_MIN,"��������");
		break;
	}	
	
	//��ѹ�������̶�ֵ
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
	
	//��ѹ����������
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
	
	//��ѹ����������
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
	
	//��Ʒ����
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
/* ����������棬��ͬ���л�                                             */
/************************************************************************/
void ImportParamDlg::OnSelchangeTabAlllayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char	tmpStr[64]={0};
	int		selTab,selLayerTab;		//��ǰѡ�е�Tab
	int		numberRead = -1;				//ÿ�ζ������ֽ���
	FILE	*pFileHandle;			//�ļ����
	ULONG	nSize = 0;
	TEST_PARAM_STRUCT readSS;	
	CString szGetName;

	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	selTab=m_TabCtrlAllCar.GetCurSel();				//��ǰ��
	selLayerTab = m_TabCtrlAllLayer.GetCurSel();	//��ǰ������

	this->GetDlgItemText(IDC_EDIT_LOAD_FILENAME,szGetName);//��ȡ����Ĳ����ļ�ȫ·������
 
	//������ʾ------------------------------------------------------------------
	//ÿ��ͨ���Ĳ���, ģʽ����ǰѡ��
	if(strlen(szGetName) == 0)//δ���в�������
	{
		
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"" );
		//��ѹ�������̶�ֵ
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_VALUE)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_VALUE)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MAX)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MAX)->SetWindowText( "");
		
		//��ѹ����������
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN1_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN2_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN3_MIN)->SetWindowText( "");
		this->GetDlgItem(IDC_EDIT_LAYER1_CHN4_MIN)->SetWindowText( "");
		
		this->SetDlgItemText(IDC_EDIT_LOAD_FILENAME, "");				//�ڴ��ڱ�������ʾ·��
		
		return;
	}
	
	pFileHandle = fopen( szGetName, "rb" );
	if (pFileHandle == NULL)
	{
		sprintf(tmpStr,"���ļ�ʧ��, %s",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}	
	nSize = 0x0;
	fseek(pFileHandle, nSize, 0);
	numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
	if (numberRead<0)
	{		
		sprintf(tmpStr,"��ȡ�ļ�ʧ��, %s",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}		
	fclose(pFileHandle);

	if (readSS.layerParam[selLayerTab].loadMode == 0)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CCģʽ" );
	}else if(readSS.layerParam[selLayerTab].loadMode == 1)
	{
		this->SetDlgItemText( IDC_EDIT_LOADMODE,"CVģʽ" );
	}	
	switch(readSS.layerParam[selLayerTab].loadMode)
	{
	case 0://CC
		this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
		this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
		break;
	case 1://CV
		this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��������");
		this->SetDlgItemText(IDC_STATIC_MIN,"��������");
		break;
	}	
	
	//��ѹ�������̶�ֵ
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
	
	//��ѹ����������
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
	
	//��ѹ����������
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
/* ����������������ļ��е����ݶ�ȡ��ȫ�ֽṹ����                       */
/************************************************************************/
void ImportParamDlg::OnOK() 
{
	int			i;
	int			numberRead=-1;
	int			selTab;					//��ǰѡ�е�Tab
	int			testItemNum=0;			//ʱ��������Ŀ��������õ�
	CString		szGetName;				//�����ļ�ȫ·������
	FILE		*pFileHandle;			//�ļ����
	ULONG		nSize = 0;
	char		tmpStr[256]={0};
	TEST_PARAM_STRUCT readSS;
	
	memset(&readSS,0,sizeof(TEST_PARAM_STRUCT));
	
	//��ǰѡ�й�����,��ǰѡ�в��Գ�
	selTab = m_TabCtrlAllCar.GetCurSel();
	if (g_AllCar[selTab].m_CarState == CAR_STATE_TESTING) //��ǰ��û���ڲ����вſɽ��в�������
	{
		sprintf(tmpStr,"%s, Please Stop Test First!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;
	}	
	this->GetDlgItemText(IDC_EDIT_LOAD_FILENAME,szGetName);
	if(strlen(szGetName) == 0)
	{
		sprintf(tmpStr,"%s, ���ȵ������!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;	
	}
	pFileHandle = fopen( szGetName, "rb" );
	if (pFileHandle == NULL)
	{
		sprintf(tmpStr,"���ļ�ʧ��, %s!",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}
	fseek(pFileHandle, nSize, 0);	//�ļ�ָ���ƶ����ļ�ͷ
	numberRead=fread(&readSS,sizeof(TEST_PARAM_STRUCT), 1,pFileHandle);
	if (numberRead<0)	//�ļ���ȡʧ��
	{			
		sprintf(tmpStr,"��ȡ�ļ�ʧ��, %s!",szGetName);
		AfxMessageBox(tmpStr);
		return;
	}		
	fclose(pFileHandle); 	
	
	
	strcpy(g_AllCar[selTab].paramFileName, szGetName);						//��¼�����ļ�ȫ·������
	
	//�����ļ������ṹ��
	memcpy(&g_AllCar[selTab].testParam, &readSS,sizeof(TEST_PARAM_STRUCT));	
	
	//ʱ��ת��---------------------------------------------------------
	//��ȡ����ListBox��ʱ��,���������
	for ( i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		CString csTemp[5];
		int pulseRepeatTime;	//���忪��
		m_ListTimeSeries.GetText(i,bufStr);

		AfxExtractSubString(csTemp[1], (LPCTSTR)bufStr, 1, ','); // �õ�������
		AfxExtractSubString(csTemp[2], (LPCTSTR)bufStr, 2, ','); // �õ���ʼʱ��,��λ��
		AfxExtractSubString(csTemp[3], (LPCTSTR)bufStr, 3, ','); // �õ�����ʱ��,��λ��
		if(csTemp[1] == "���忪��")
		{
			AfxExtractSubString(csTemp[4], (LPCTSTR)bufStr, 6, ','); //�����ظ�����
			sscanf(csTemp[4],"%d",&pulseRepeatTime);
			testItemNum += pulseRepeatTime*2;

		}else if(csTemp[1] == "110V���ز���")
		{
			testItemNum++;
		}else if(csTemp[1] == "220V���ز���")
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
	//��ȡ����ListBox��ʱ��,��д������
	for ( i=0;i<m_ListTimeSeries.GetCount();i++)
	{
		CString bufStr;
		CString csTemp[7];
		int pulseRepeatTime;		//���忪��
		int timeSeriesStartTime;	//��ʼ����ʱ��
		int timeSeriesTestTime;		//����ʱ��
		int pulseOnTime;
		int pulseOffTime;
		m_ListTimeSeries.GetText(i,bufStr);

		AfxExtractSubString(csTemp[1], (LPCTSTR)bufStr, 1, ','); // �õ�������
		AfxExtractSubString(csTemp[2], (LPCTSTR)bufStr, 2, ','); // �õ���ʼʱ��,��λ��
		AfxExtractSubString(csTemp[3], (LPCTSTR)bufStr, 3, ','); // �õ�����ʱ��,��λ��
		sscanf(csTemp[2],"%d",&timeSeriesStartTime);
		sscanf(csTemp[3],"%d",&timeSeriesTestTime);

		if(csTemp[1] == "���忪��")
		{
			AfxExtractSubString(csTemp[4], (LPCTSTR)bufStr, 4, ','); //���忪ʱ��
			AfxExtractSubString(csTemp[5], (LPCTSTR)bufStr, 5, ','); //�����ʱ��
			AfxExtractSubString(csTemp[6], (LPCTSTR)bufStr, 6, ','); //�����ظ�����
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

		}else if(csTemp[1] == "110V���ز���")
		{
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_110V;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = timeSeriesTestTime;
			g_AllCar[selTab].testItemNum++;
		}else if(csTemp[1] == "220V���ز���")
		{
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testItem = TEST_FLOW_220V;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].startTime = timeSeriesStartTime*60;
			g_AllCar[selTab].pTimeSeires[g_AllCar[selTab].testItemNum].testTime = timeSeriesTestTime;
			g_AllCar[selTab].testItemNum++;
		}
	}

	//ȫ�ָ��أ�
	for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)
	{
		g_AllCar[selTab].m_Load[loadIndex-1].m_LayerID = (loadIndex-1)/readSS.loadPerLayer+1;//������������
		g_AllCar[selTab].m_Load[loadIndex-1].m_LoadState = LOAD_STATE_SELECT;		//���ĸ���״̬Ϊѡ��				
	}
	
	//����ͨ�����Բ�����ȫ�ֽṹ���������
	if (readSS.paramSetMode == 0)//��������
	{
		for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)
		{
			g_AllCar[selTab].m_Load[loadIndex-1].m_LoadMode = readSS.layerParam[0].loadMode;//��һ�������ֵ��ÿ��ͨ��
			
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
		
	}else if (readSS.paramSetMode == 1)//�ֲ�����
	{
		for (int loadIndex=readSS.startLoadNum; loadIndex<=readSS.endLoadNum; loadIndex++)//ȫ�ָ��ز������ã�����ȫ�ָ��������Ĳ㣬�ֱ��ͨ��������ֵ
		{
			int layerID = g_AllCar[selTab].m_Load[loadIndex-1].m_LayerID-1;//������һ��

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
			
	//���Գ�״̬����Ϊ
	g_AllCar[selTab].m_CarState = CAR_STATE_IMPORTED;	
	
	CDialog::OnOK();
	
}

