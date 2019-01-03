/*
页控件，用于展示设置对话框页面
*/
#pragma once
#include "SetInterFace.h"
/////////////////////////////////////////////////////////////////////////////
// CSetTabSheet window

class CSetTabSheet : public CTabCtrl
{
// Construction
public:
	enum{MAXPAGE= 16};
	CSetTabSheet();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTabSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	CSetInterFace* GetCurDialog(void);
	int GetCurSel();
	int SetCurSel(int nItem);
	void Show();
	void SetRect();
	BOOL AddPage(LPCTSTR title, CSetInterFace *pDialog, UINT ID);
	virtual ~CSetTabSheet();

	// Generated message map functions
protected:
	LPCTSTR m_Title[MAXPAGE];
	UINT m_IDD[MAXPAGE];
	CSetInterFace* m_pPages[MAXPAGE];
	int m_nNumOfPages;
	int m_nCurrentPage;
	//{{AFX_MSG(CSetTabSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

