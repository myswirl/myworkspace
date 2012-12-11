// Result.cpp : implementation file
//

#include "stdafx.h"
#include "XMS_TestDial.h"
#include "Result.h"
#include "XMS_Dial_Sub.h"
#include "XMS_Dial_Event.h"
#include "XMS_Dial_String.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalTrunk;
extern int						g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];
/////////////////////////////////////////////////////////////////////////////
// CResult dialog


CResult::CResult(CWnd* pParent /*=NULL*/)
	: CDialog(CResult::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CResult::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResult)
	DDX_Control(pDX, IDC_LIST_ERROR, m_ListError);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResult, CDialog)
	//{{AFX_MSG_MAP(CResult)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CResult, CDialog)
	//{{AFX_DISPATCH_MAP(CResult)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IResult to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {057A0E2B-230C-4D21-AC45-3CD525E591C8}
static const IID IID_IResult =
{ 0x57a0e2b, 0x230c, 0x4d21, { 0xac, 0x45, 0x3c, 0xd5, 0x25, 0xe5, 0x91, 0xc8 } };

BEGIN_INTERFACE_MAP(CResult, CDialog)
	INTERFACE_PART(CResult, IID_IResult, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResult message handlers

BOOL CResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ListError.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListError.SetHeadings(_T("ID,60; Mod/Ch,100; PFBA ,100; Type,100; Total,50;CallFail,50;CheckFail,50"));
	AddResult();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResult::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CResult::OnOK() 
{
	// TODO: Add extra validation here
//  	DWORD dwStyle;
	CDialog::OnOK();
}

void CResult::AddResult()
{
	TRUNK_STRUCT	*pOneTrunk = NULL;
	int  i, s8ModID, s16ChID,PFBAChnl;
	char ID[10];
	char Mod[20];
	char PFBA[20];
	char Mtype[20];
	char Result1[50];
	char Result2[50];
	char Result3[50];

	
	for (i=0; i<g_iTotalTrunk; i++)
	{
		s8ModID = MapTable_Trunk[i].m_s8ModuleID;
		s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
			
		if (AllDeviceRes[s8ModID].pTrunk == NULL)
			break;

	    pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
		if(pOneTrunk->iCallErrCnt!=0||pOneTrunk->iCheckErrCnt!=0)
		{
			sprintf(ID," %d",pOneTrunk->iSeqID);
            sprintf(Mod," %d,%d",pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
			PFBAChnl=pOneTrunk->deviceID.m_s16ChannelID%32-(pOneTrunk->deviceID.m_s16ChannelID%32)/16;
			sprintf(PFBA," %s,%d",GetPFBASite(pOneTrunk->iSeqID),PFBAChnl);
			sprintf(Mtype," %s",GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub));
			sprintf(Result1," %d", pOneTrunk->iTotalCnt);
			sprintf(Result2," %d", pOneTrunk->iCallErrCnt);
			sprintf(Result3," %d", pOneTrunk->iCheckErrCnt);			
			AddList(ID,Mod,PFBA,Mtype,Result1,Result2,Result3);
		}
	}
	
}

void CResult::AddList(char *pID, char *pMod,char *pPfba, char *pType, char *pResult1, char *pResult2, char *pResult3)
{
	m_ListError.AddItem(pID,pMod,pPfba,pType,pResult1,pResult2,pResult3);
}

char* CResult::GetPFBASite(int Iseq)
{
    int  Isite;

	Isite=Iseq/32+1;
	
	if(0==Isite%2)
	sprintf(PFBA,"MB%d",Isite/2+1);

	else
	sprintf(PFBA,"MA%d",Isite/2+2);

	return PFBA;
}
