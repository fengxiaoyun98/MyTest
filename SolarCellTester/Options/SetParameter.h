#pragma once
#include "..\resource.h"

// CSetParameter dialog

class CSetParameter : public CDialog
{
	DECLARE_DYNAMIC(CSetParameter)

public:
	CSetParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetParameter();

// Dialog Data
	enum { IDD = IDD_DLG_SETPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_incur;
	float m_xs;
	float m_cur;
	BOOL m_chkcur;
	BOOL m_chkls;
	float m_xzvol;
	float m_vol;
	float m_invol;
	BOOL m_chkvol;
	BOOL m_chktmp;
	float m_intmp;
	float m_tmp;
	float m_xztmp;
	float m_ls;
	UINT AutoSer;
	double m_sccurr;
	double m_scvolt;
	
	UINT	my_ls;
	float	my_xs;//电流修正系数
	float	my_xzvol;//电压修正系数
	float	my_xztmp;//温度修正系数

	UINT m_oldls;
	BOOL m_lsUp;

	float m_prev;

protected:
	BOOL SaveSet(void);
	CString AutoSN();
	void SavePrev();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	BOOL GetSet(void);
	void Save(CString name,float value);
	float Get(CString name);
};
