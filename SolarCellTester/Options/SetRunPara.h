#pragma once

#include "..\resource.h"
// CSetRunPara dialog

class CSetRunPara : public CDialog
{
	DECLARE_DYNAMIC(CSetRunPara)

public:
	CSetRunPara(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetRunPara();

// Dialog Data
	enum { IDD = IDD_DLG_SETRUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_enno;
	BOOL m_swpv;
	BOOL m_swrshrs;
	BOOL m_useTemp;
	BOOL m_SavePic;
	BOOL m_jpg;
	BOOL m_swst;
	BOOL m_swvip;
	BOOL m_TestMode;
	BOOL m_autosn;


	UINT AutoSer;
	CString AutoSN(void);
	long m_allTimes;
	long GetTestTimes();
	void SetTestTimes(long n);
//	CString LvlName(CString name=_T(""));
	BOOL SaveSet(void);
	BOOL GetSet(void);
public:
	afx_msg void OnBnClickedOk();
	
	void Save(CString name,BOOL value);
	BOOL Get(CString name);
};
