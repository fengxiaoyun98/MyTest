// SetRun.cpp : implementation file
//

#include "stdafx.h"
#include "Registry.h"
#include "SetRunPara.h"


// CSetRunPara dialog

IMPLEMENT_DYNAMIC(CSetRunPara, CDialog)

CSetRunPara::CSetRunPara(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRunPara::IDD, pParent)
	, m_enno(FALSE)
	, m_swpv(FALSE)
	, m_swrshrs(FALSE)
	, m_useTemp(FALSE)
	, m_SavePic(FALSE)
	, m_jpg(FALSE)
	, m_swst(FALSE)
	, m_swvip(FALSE)
	, m_TestMode(FALSE)
	, m_autosn(FALSE)
{

}

CSetRunPara::~CSetRunPara()
{
}

void CSetRunPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_NO, m_enno);
	DDX_Check(pDX, IDC_CHECK_SWPV, m_swpv);
	DDX_Check(pDX, IDC_CHECK_SWRSHRS, m_swrshrs);
	DDX_Check(pDX, IDC_CHECK_SWTMP, m_useTemp);
	DDX_Check(pDX, IDC_CHECK_SWSAVEPIC, m_SavePic);
	DDX_Check(pDX, IDC_CHECK_SWSAVEPIC2, m_jpg);
	DDX_Check(pDX, IDC_CHECK_ST, m_swst);
	DDX_Check(pDX, IDC_CHECK_VIP, m_swvip);
	DDX_Check(pDX, IDC_CHECK_MODE, m_TestMode);
	DDX_Check(pDX, IDC_CHKAUTOSN, m_autosn);
}


BEGIN_MESSAGE_MAP(CSetRunPara, CDialog)
	ON_BN_CLICKED(IDOK, &CSetRunPara::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetRunPara message handlers

BOOL CSetRunPara::SaveSet()
{
	UpdateData(TRUE);
	Save(_T("USETEMP"),m_useTemp);
	Save(_T("ENRSRSH"),m_swrshrs);
	Save(_T("DISPV"),m_swpv);
	Save(_T("ENNO"),m_enno);
	Save(_T("SAVEPIC"),m_SavePic);
	Save(_T("SAVEJPG"),m_jpg);
	Save(_T("SWST"),m_swst);
	Save(_T("SWVIP"),m_swvip);
	Save(_T("AUTOSN"),m_autosn);
	Save(_T("TESTMODE"),m_TestMode);
	return TRUE;
}
BOOL CSetRunPara::GetSet()
{
	m_useTemp=Get(_T("USETEMP"));;
	m_swrshrs=Get(_T("ENRSRSH"));
	m_swpv=Get(_T("DISPV"));
	m_enno=Get(_T("ENNO"));
	m_SavePic=Get(_T("SAVEPIC"));
	m_jpg=Get(_T("SAVEJPG"));
	m_swst=Get(_T("SWST"));
	m_swvip=Get(_T("SWVIP"));
	m_autosn=Get(_T("AUTOSN"));
	m_TestMode=Get(_T("TESTMODE"));
	UpdateData(FALSE);
	return TRUE;
}
void CSetRunPara::SetTestTimes(long n)
{
	if(n ==1)m_allTimes++;//自动加1
	else m_allTimes=n;

	CString strData;
	strData.Format(_T("%d"),m_allTimes);
//	SavaStr();
//	theApp.WriteProfileString(_T("VolSet"),_T("ALLTIMES"),strData);
}

long CSetRunPara::GetTestTimes()
{
	CString strData;
//	strData=theApp.GetProfileString(_T("VolSet"),_T("ALLTIMES"));
	m_allTimes=_ttoi(strData);
	return m_allTimes;
}
CString CSetRunPara::AutoSN(void)
{
	AutoSer++;
	CString str=_T("");
	if(m_autosn)
		str.Format(_T("-%04d"),AutoSer);
	return str;
}



void CSetRunPara::OnBnClickedOk()
{
	//更新变量到控件
	SaveSet();//保存设置
}
void CSetRunPara::Save(CString name,BOOL value)
{
	CString strData;
	CRegistry reg;
	if(reg.Open(_T("Software\0")))
	{
		reg.CreateKey(_T("SetRun"));
		reg.Write(name,value);
	}
}
BOOL CSetRunPara::Get(CString name)
{
	BOOL ret;
	CRegistry reg;
	if(reg.Open(_T("Software\\SetRun\0")))
	{
		reg.Read(name,&ret);
		return ret;
	}
	else
		AfxMessageBox(_T("cannot open registry!"));
}