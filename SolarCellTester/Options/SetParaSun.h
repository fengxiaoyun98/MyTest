#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVolSet dialog
#include "..\resource.h"
#include "SetInterFace.h"

class CSetParaSun : public CSetInterFace
{
// Construction
public:
	float	my_xs;//电流修正系数
	float	my_xzvol;//电压修正系数
	CString m_curPath;
	void SavePrev();
	float m_prev;
	BOOL GetSetParaSun();
	BOOL SaveSet();
	CSetParaSun(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_DLG_SETSUN };
	float	m_incur;
	float	m_xs;
	float	m_cur;
	BOOL	m_chkcur;
	float	m_xzvol;
	float	m_vol;
	float	m_invol;
	BOOL	m_chkvol;
	BOOL	m_stdsun;
	float	m_stdIsc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
