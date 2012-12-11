#if !defined(AFX_RESULT_H__2054809C_D156_491E_9B46_5A1885D3E937__INCLUDED_)
#define AFX_RESULT_H__2054809C_D156_491E_9B46_5A1885D3E937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Result.h : header file
//
#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CResult dialog

class CResult : public CDialog
{
// Construction
public:
	char* GetPFBASite(int Iseq);
	void AddList(char *pID, char *pMod,char *pPfba, char *pType, char *pResult1, char *pResult2, char *pResult3);
	void AddResult();

	CResult(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CResult)
	enum { IDD = IDD_DIALOG_RESULT };
	CSortListCtrl	m_ListError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResult)
	public:
	virtual void OnFinalRelease();
	protected:
	char    PFBA[20];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CResult)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULT_H__2054809C_D156_491E_9B46_5A1885D3E937__INCLUDED_)
