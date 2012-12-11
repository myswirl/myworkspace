#if !defined(AFX_PARAMEDITDLG_H__D2E0AD86_5B50_4425_AB0B_5209B59D9D94__INCLUDED_)
#define AFX_PARAMEDITDLG_H__D2E0AD86_5B50_4425_AB0B_5209B59D9D94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamEditDlg.h : header file
//

#include "TabSheet.h"
#include "Layer1Dlg.h"
#include "Layer2Dlg.h"
#include "Layer3Dlg.h"
#include "Layer4Dlg.h"
#include "Layer5Dlg.h"
#include "Layer6Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// ParamEditDlg dialog

class ParamEditDlg : public CDialog
{
// Construction
public:
	ParamEditDlg(CWnd* pParent = NULL);   // standard constructor
	Layer1Dlg m_Layer1;
	Layer2Dlg m_Layer2;
	Layer3Dlg m_Layer3;
	Layer4Dlg m_Layer4;
	Layer5Dlg m_Layer5;
	Layer6Dlg m_Layer6;

// Dialog Data
	//{{AFX_DATA(ParamEditDlg)
	enum { IDD = IDD_DIALOG_PARAMEDIT };
	CListBox	m_ListTimeSeries;
	CComboBox	m_ComboTestItem;
	CComboBox	m_ComboParamSetMode;
	CComboBox	m_ComboLoadPerLayer;
	CComboBox	m_ComboLayer;
	CTabSheet	m_TabSheetParamEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParamEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ParamEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadparam();
	afx_msg void OnButtonExport();
	afx_msg void OnButtonClearparam();
	afx_msg void OnSelchangeComboParamsetmode();
	afx_msg void OnCheckSaveRecdata();
	afx_msg void OnButtonAddtimeseries();
	afx_msg void OnButtonDeltimeseries();
	afx_msg void OnSelchangeComboTestitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMEDITDLG_H__D2E0AD86_5B50_4425_AB0B_5209B59D9D94__INCLUDED_)
