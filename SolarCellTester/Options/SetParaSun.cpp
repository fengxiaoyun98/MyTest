// SetParaSun.cpp : implementation file
//

#include "stdafx.h"
#include "SetParaSun.h"
#include "..\Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetParaSun dialog

CSetParaSun::CSetParaSun(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetParaSun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetParaSun)
	m_incur = 0.0f;
	m_xs = 0.0f;
	m_cur = 0.0f;
	m_chkcur = FALSE;
	m_xzvol = 0.0f;
	m_vol = 0.0f;
	m_invol = 0.0f;
	m_chkvol = FALSE;
	m_stdIsc = 0.0f;
	//}}AFX_DATA_INIT
}


void CSetParaSun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetParaSun)
	DDX_Text(pDX, IDC_EDIT_INCUR0, m_incur);
	DDV_MinMaxFloat(pDX, m_incur, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_XS0, m_xs);
	DDV_MinMaxFloat(pDX, m_xs, 0.5f, 1.5f);
	DDX_Text(pDX, IDC_EDIT_OLD0, m_cur);
	DDX_Check(pDX, IDC_CHECKCUR0, m_chkcur);
	DDX_Text(pDX, IDC_EDIT_XZVOL0, m_xzvol);
	DDV_MinMaxFloat(pDX, m_xzvol, 0.5f, 1.5f);
	DDX_Text(pDX, IDC_EDIT_OLDVOL0, m_vol);
	DDX_Text(pDX, IDC_EDIT_INVOL0, m_invol);
	DDV_MinMaxFloat(pDX, m_invol, 0.f, 500.f);
	DDX_Check(pDX, IDC_CHECKVOL0, m_chkvol);
	DDX_Check(pDX, IDC_CHECKSTDSUN, m_stdsun);
	DDX_Text(pDX, IDC_EDIT_STDISC, m_stdIsc);
	DDV_MinMaxFloat(pDX, m_stdIsc, 0.f, 2000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetParaSun, CDialog)
	//{{AFX_MSG_MAP(CSetParaSun)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSetParaSun::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetParaSun message handlers

BOOL CSetParaSun::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetSetParaSun();
	my_xs=m_xs;
	my_xzvol=m_xzvol;
	UpdateData(FALSE);//更新变量到控件
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///保存设置
BOOL CSetParaSun::SaveSet()
{
	UpdateData(TRUE);
	
	//光强设定
	CString strData;
	strData.Format(_T("%f"),m_stdIsc);
	theApp.WriteProfileString(_T("SetParaSun"),_T("STDISC"),strData);

	//电流调整设定
	strData.Format(_T("%f"),m_xs);//
	theApp.WriteProfileString(_T("SetParaSun"),_T("XS"),strData);
	//电压调整设定
	strData.Format(_T("%f"),m_xzvol);//
	theApp.WriteProfileString(_T("SetParaSun"),_T("XZVOL"),strData);

	strData.Format(_T("%f"),m_prev);
	theApp.WriteProfileString(_T("SetParaSun"),_T("PREV"),strData);
	return TRUE;
}


///获取设置输出电压
BOOL CSetParaSun::GetSetParaSun()
{		
	CString strData;
	//光强设定
	strData=theApp.GetProfileString(_T("SetParaSun"),_T("STDISC"),strData);
	m_stdIsc=(float)_tcstod(strData,NULL);

	//电流调整设定
	strData=theApp.GetProfileString(_T("SetParaSun"),_T("XS"),strData);
	m_xs=(float)_tcstod(strData,NULL);
	if(m_xs<0.5||m_xs>1.5)m_xs=1.0;
	//电压调整设定
	strData=theApp.GetProfileString(_T("SetParaSun"),_T("XZVOL"),strData);
	m_xzvol=(float)_tcstod(strData,NULL);
	if(m_xzvol<0.5||m_xzvol>1.5)m_xzvol=1.0;

	strData=theApp.GetProfileString(_T("SetParaSun"),_T("PREV"));
	m_prev=(float)_tcstod(strData,NULL);

	return TRUE;
}

void CSetParaSun::SavePrev()
{
//定电压点电保存
	CString strData;
	strData.Format(_T("%f"),m_prev);
	theApp.WriteProfileString(_T("SetParaSun"),_T("PREV"),strData);
}

void CSetParaSun::OnBnClickedOk()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_chkcur)
	{
		m_chkcur=0;
		if(m_cur>0)	m_xs *= m_incur/m_cur;
		if(m_xs<0.5||m_xs>1.5)m_xs=1.0;
	}
	else m_xs=my_xs;

	if(m_chkvol)
	{
		m_chkvol=0;
		if(m_vol>0)m_xzvol *= m_invol/m_vol;
		if(m_xzvol<0.5||m_xzvol>1.5)m_xzvol=1.0;
	}
	else m_xzvol=my_xzvol;

	UpdateData(FALSE);//更新变量到控件
	SaveSet();//保存设置
}
