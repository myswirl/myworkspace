#if !defined(AFX_IMPORTPARAMDLG_H__FB883478_68CB_4107_913D_55FBFFD8CDAC__INCLUDED_)
#define AFX_IMPORTPARAMDLG_H__FB883478_68CB_4107_913D_55FBFFD8CDAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ImportParamDlg dialog

class ImportParamDlg : public CDialog
{
// Construction
public:
	ImportParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ImportParamDlg)
	enum { IDD = IDD_DIALOG_IMPORT_PARAM };
	CListBox	m_ListTimeSeries;
	CTabCtrl	m_TabCtrlAllLayer;
	CTabCtrl	m_TabCtrlAllCar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImportParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ImportParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadparam();
	afx_msg void OnSelchangeTabAllcar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabAlllayer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOk();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTPARAMDLG_H__FB883478_68CB_4107_913D_55FBFFD8CDAC__INCLUDED_)
