#if !defined(AFX_LAYER3DLG_H__64378B42_C77B_47D5_91EF_4A3E200D2A1E__INCLUDED_)
#define AFX_LAYER3DLG_H__64378B42_C77B_47D5_91EF_4A3E200D2A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer3Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer3Dlg dialog

class Layer3Dlg : public CDialog
{
// Construction
public:
	Layer3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer3Dlg)
	enum { IDD = IDD_DIALOG_LAYER3 };
	CComboBox	m_ComboLayer3ParaMode;
	CComboBox	m_ComboLayer3LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer3Loadmode();
	afx_msg void OnSelchangeComboLayer3Paramode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER3DLG_H__64378B42_C77B_47D5_91EF_4A3E200D2A1E__INCLUDED_)
