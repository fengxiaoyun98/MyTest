#pragma once
//

/////////////////////////////////////////////////////////////////////////////
// CDefLevel dialog
class CRegistry;
class CDefLevel : public CDialog
{
// Construction
	DECLARE_DYNAMIC(CDefLevel)
private:
	int m_mode;
public:
	DWORD m_color;
	DWORD m_bcolor;
	CString	m_vcur;
	CString	m_pow;
	CString	m_eff;
	void SetValue(double p,double c,double e);
	CDefLevel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefLevel();
	enum { IDD = IDD_DEFLEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_classname;
	CString m_wndstatestr;
	CRegistry reg;
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnInitDialog(UINT wParam,LONG lParam) ;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
