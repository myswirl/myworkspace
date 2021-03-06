//类头文件

#if !defined(AFX_LED_H__BDAD9442_11BB_4917_8E43_0E2ACC246FE3__INCLUDED_)

#define AFX_LED_H__BDAD9442_11BB_4917_8E43_0E2ACC246FE3__INCLUDED_



#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

// LED.h : header file

//

#define  CIRCLE   0		//圆形
#define  SQUARE   1		//方形
#define  ELLIPSE  2		//椭圆形
extern COLORREF g_ColorSelect;
extern COLORREF g_ColorNotSelect;
extern COLORREF g_ColorGreen;	//绿色，校验通过的	
extern COLORREF g_ColorRed;		//红色
extern COLORREF g_ColorBlue;	//淡蓝色
/////////////////////////////////////////////////////////////////////////////

// CLED window
class CLED : public CStatic
{
	// Construction
	
public:	
	CLED();
	// Attributes	

public:	
	// Operations
	
public:	
	COLORREF m_crBK,m_crOnFG,m_crOffFG;	
	CBrush   *m_brOnFG,*m_brOffFG;	
	int m_iType;					//形状	
	bool m_bOn;	
	CString m_sOnString,m_sOffString;	
	bool  m_bTextOn;				
	
	CRect rect;                  // 存储绘图控件的绘图区域
	CDC *pDC;                  // 控件的屏幕绘图设备指针
	CDC memDC;              // 内存绘图设备
	CBitmap memBitmap;  // 用于内存绘图的位图  
	CBitmap* pOldBmp;    // 备份旧的位图指针 
	CWnd* pWnd;             // 绘图控件的指针
	
	// Overrides	
	// ClassWizard generated virtual function overrides	
	//{{AFX_VIRTUAL(CLED)	
	//}}AFX_VIRTUAL	
	// Implementation
	
public:
	
	int GetType();			//形状
	
	bool GetTextOnOff();
	
	CString GetOnText();
	
	BOOL GetOnOff();
	
	COLORREF GetOnFgColor();
	
	CString GOffText();
	
	COLORREF GetOffFgColor();
	
	COLORREF GetBKColor();
	
	void SetTextOnOff(bool val);
	
	void SetOffText(CString str);
	
	void SetOnText(CString str);
	
	void SetBKColor(COLORREF C);
	
	void SetOnOff(bool val);
	
	void SetType(int type);
	
	void SetOffFgColor(COLORREF C);
	
	void SetOnFgColor(COLORREF C);
	
	virtual ~CLED();
	
	
	
	// Generated message map functions
	
protected:
	
	//{{AFX_MSG(CLED)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	afx_msg void OnPaint();
	
	afx_msg void OnDestroy();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	//}}AFX_MSG
	
	
	
	DECLARE_MESSAGE_MAP()
		
private:
	
	void DrawFigure();
	
};



/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_LED_H__BDAD9442_11BB_4917_8E43_0E2ACC246FE3__INCLUDED_)

/////////////////////////////////////////////////////////////////