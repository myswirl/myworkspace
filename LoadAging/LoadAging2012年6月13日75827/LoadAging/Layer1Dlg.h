#if !defined(AFX_LAYER1DLG_H__E117A58B_6D4F_4329_B3A3_27F257F2DD87__INCLUDED_)
#define AFX_LAYER1DLG_H__E117A58B_6D4F_4329_B3A3_27F257F2DD87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer1Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer1Dlg dialog

class Layer1Dlg : public CDialog
{
// Construction
public:
	Layer1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer1Dlg)
	enum { IDD = IDD_DIALOG_LAYER1 };
	CComboBox	m_ComboLayer1ParaMode;
	CComboBox	m_ComboLayer1LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer1Loadmode();
	afx_msg void OnSelchangeComboLayer1Parallelmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER1DLG_H__E117A58B_6D4F_4329_B3A3_27F257F2DD87__INCLUDED_)
