// Set.cpp : implementation file
//

#include "stdafx.h"
#include "SetParameter.h"
#include "..\resource.h"
#include "Registry.h"

// CSetParameter dialog

IMPLEMENT_DYNAMIC(CSetParameter, CDialog)

CSetParameter::CSetParameter(CWnd* pParent /*=NULL*/)
	: CDialog(CSetParameter::IDD, pParent)
	, m_incur(0)
	, m_xs(0)
	, m_cur(0)
	, m_chkcur(FALSE)
	, m_chkls(FALSE)
	, m_xzvol(0)
	, m_vol(0)
	, m_invol(0)
	, m_chkvol(FALSE)
	, m_chktmp(FALSE)
	, m_intmp(0)
	, m_tmp(0)
	, m_xztmp(0)
	, m_ls(0)
	, AutoSer(0)
	, m_sccurr(0)
	, m_scvolt(0)
{

}

CSetParameter::~CSetParameter()
{
}

void CSetParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INCUR, m_incur);
	DDV_MinMaxFloat(pDX, m_incur, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_XS, m_xs);
	DDV_MinMaxFloat(pDX, m_xs, 0.5, 1.5);
	DDX_Text(pDX, IDC_EDIT_OLD, m_cur);
	DDX_Check(pDX, IDC_CHECKCUR, m_chkcur);
	DDX_Check(pDX, IDC_CHECKLS, m_chkls);
	DDX_Text(pDX, IDC_EDIT_XZVOL, m_xzvol);
	DDV_MinMaxFloat(pDX, m_xzvol, 0.5, 1.5);
	DDX_Text(pDX, IDC_EDIT_OLDVOL, m_vol);
	DDX_Text(pDX, IDC_EDIT_INVOL, m_invol);
	DDV_MinMaxFloat(pDX, m_invol, 0.f, 500.f);
	DDX_Check(pDX, IDC_CHECKVOL, m_chkvol);
	DDX_Check(pDX, IDC_CHECKTMP, m_chktmp);
	DDX_Text(pDX, IDC_EDIT_INTMP, m_intmp);
	DDX_Text(pDX, IDC_EDIT_OLDTMP, m_tmp);
	DDX_Text(pDX, IDC_EDIT_XZTMP, m_xztmp);
	DDV_MinMaxFloat(pDX, m_xztmp, 0.25, 3);
	DDX_Text(pDX, IDC_EDIT_LQ, m_ls);
	DDV_MinMaxFloat(pDX, m_ls, 200, 1200);
	DDX_Text(pDX, IDC_EDIT_AUTO, AutoSer);
	DDV_MinMaxUInt(pDX, AutoSer, 0, 999999);
	DDX_Text(pDX, IDC_EDIT_SCCURR, m_sccurr);
	DDX_Text(pDX, IDC_EDIT_SCVOLT, m_scvolt);
}


BEGIN_MESSAGE_MAP(CSetParameter, CDialog)
	ON_BN_CLICKED(IDOK, &CSetParameter::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetParameter message handlers




void CSetParameter::OnOK() 
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

	if(m_chktmp)
	{
		m_chktmp=0;
		if(m_tmp!=-273)
		{
			m_xztmp *=( m_intmp+273)/(m_tmp+273);
			if(m_xztmp<0.25||m_xztmp>3)m_xztmp=1.0;
		}else m_xztmp=1.0;
	}
	else m_xztmp=my_xztmp;

	if(m_chkls)
	{
		m_chkls=0;
		if((m_ls<200)||(m_ls>1200))m_ls=1000;
		m_ls=(m_ls/100)*100;
		m_oldls=m_ls*3;//mcu输出3000对应光强1000
		m_lsUp=1; //光强设置改变
	}
	else
	{
		m_lsUp=0;m_ls=my_ls;
	}

	if((m_sccurr<0)||(m_sccurr>0.01))
		m_sccurr=0.06/100;
	if((m_scvolt>0.01)||(m_scvolt<-0.01))
		m_scvolt=-0.004;

//	ChangeChk();

	UpdateData(FALSE);//更新变量到控件
	SaveSet();//保存设置

}


///保存设置
BOOL CSetParameter::SaveSet()
{
	UpdateData(TRUE);	
	Save(_T("LS"),m_ls);
	//电流调整设定
	Save(_T("XS"),m_xs);
	//电压调整设定
	Save(_T("XZVOL"),m_xzvol);
	//温度调整设定
	Save(_T("XZTMP"),m_xztmp);
	Save(_T("PREV"),m_prev);
	Save(_T("SCCURR"),m_sccurr);
	Save(_T("SCVOLT"),m_scvolt);

	return TRUE;
}


///获取设置输出电压
BOOL CSetParameter::GetSet()
{		
	CString strData;
	//光强设定
	m_ls = Get(_T("LS"));
	if(m_ls<200||m_ls>1200)m_ls=1000;

	//电流调整设定
	m_xs = Get(_T("XS"));
	if(m_xs<0.5||m_xs>1.5)m_xs=1.0;
	//电压调整设定
	m_xzvol = Get(_T("XZVOL"));
	if(m_xzvol<0.5||m_xzvol>1.5)m_xzvol=1.0;
	//温度调整设定
	m_xztmp = Get(_T("XZTMP"));
	if(m_xztmp<0.25||m_xzvol>3)m_xztmp=1.0;

	m_sccurr = Get(_T("SCCURR"));
	m_scvolt = Get(_T("SCVOLT"));
	m_prev = Get(_T("PREV"));
//	m_unit=theApp.GetProfileString(_T("Set"),_T("UNIT"));
	m_oldls=m_ls*3;//转换成mcu输出值
	UpdateData(false);
	return TRUE;
}

void CSetParameter::SavePrev()
{
//定电压点电保存
//	CString strData;
//	strData.Format(_T("%f"),m_prev);
//	theApp.WriteProfileString(_T("VolSet"),_T("PREV"),strData);

//	theApp.WriteProfileString(_T("VolSet"),_T("UNIT"),m_unit);//单位
}

void CSetParameter::OnBnClickedOk()
{
	OnOK();
}

BOOL CSetParameter::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetSet();
	my_ls=m_ls;
	my_xs=m_xs;
	my_xzvol=m_xzvol;
	my_xztmp=m_xztmp;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetParameter::Save(CString name,float value)
{	
	CString strData;
	CRegistry reg;
	if(reg.Open(_T("Software\0")))
	{
		reg.CreateKey(_T("SetParameter"));
		strData.Format(_T("%ud"),value);
		reg.Write(name,strData);
	}
}

float CSetParameter::Get(CString name)
{	
	float ret;
	TCHAR buf[255];
	CString strData(buf);
	CRegistry reg;
	if(reg.Open(_T("Software\\SetParameter\0")))
	{
		reg.Read(name,strData);
		ret=_tcstod(strData,NULL);
		return ret;
	}
}
