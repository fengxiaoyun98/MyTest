#pragma once
#include "afxwin.h"
#include "..\resource.h"
// CSetSelect dialog

class CSetSelect : public CDialog
{
	DECLARE_DYNAMIC(CSetSelect)

public:
	CSetSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSelect();

// Dialog Data-+

	enum { IDD = IDD_DLG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CString LvlName(CString name);
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedAppend();
	void DisLevel(CDC* pDC, float I, float Pm, float Eff);
	afx_msg void OnLbnDblclkListDislevel();
	void AddItem(CString name, float level);
	BOOL OSLVL(CString  fn, bool o);
	CString GetFileName(bool o);
	virtual BOOL OnInitDialog();

	CString m_name;
	CString	m_filename;
	float m_level;
	int m_class;
	CListBox m_disp;

	UINT rn;
	enum{ NUM=100};//最大分档数
	CString nstr[NUM];//name of level
	float nrge[NUM]; //range maxim value
};
