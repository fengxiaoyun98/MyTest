/*
工具条对话框
*/
#pragma once
#include   <atlimage.h> 

// CSolarCellTesterCtrlBar dialog

class CSolarCellTesterCtrlBar : public CDialogBar
{
	DECLARE_DYNAMIC(CSolarCellTesterCtrlBar)

public:
	CSolarCellTesterCtrlBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSolarCellTesterCtrlBar();
	CImage m_emf;

// Dialog Data
	enum { IDD = IDD_DIALOG_CTRLBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnInitDialog(UINT wParam,LONG lParam) ;
	afx_msg void OnPaint();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();

public:
	CStatic m_staticTel;
	HWND hWnd;
};
