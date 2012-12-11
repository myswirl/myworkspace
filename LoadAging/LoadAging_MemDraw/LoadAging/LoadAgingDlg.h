// LoadAgingDlg.h : header file
//
//#include "BtnST.h"
#include "LED.h"
#include "BtnST.h"

#if !defined(AFX_LOADAGINGDLG_H__0035CE09_73E8_4A15_8F49_E29416DEA777__INCLUDED_)
#define AFX_LOADAGINGDLG_H__0035CE09_73E8_4A15_8F49_E29416DEA777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLoadAgingDlg dialog

class CLoadAgingDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CLoadAgingDlg(CWnd* pParent = NULL);	// standard constructor
	CToolTipCtrl m_tooltip;
	CRect  m_rect;

// Dialog Data
	//{{AFX_DATA(CLoadAgingDlg)
	enum { IDD = IDD_LOADAGING_DIALOG };
	CBrush m_brshBack;
	CButtonST	m_BtnSysSet;
	CButtonST	m_BtnParamImport;
	CButtonST	m_BtnParamEdit;
	CButtonST	m_BtnPassword;
	CButtonST	m_BtnHelp;
	CButtonST	m_BtnExit;
	CButtonST	m_BtnTest;
	CLED		m_LED[252];
	CTabCtrl	m_TabCtrlTestAllCar;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadAgingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoadAgingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonPassword();
	afx_msg void OnButtonSysconfig();
	afx_msg void OnButtonParamedit();
	afx_msg void OnButtonParamimport();
	afx_msg void OnButtonHelp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonTest();
	afx_msg void OnButtonClear();
	afx_msg void OnSelchangeTabTestingAllcar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADAGINGDLG_H__0035CE09_73E8_4A15_8F49_E29416DEA777__INCLUDED_)
