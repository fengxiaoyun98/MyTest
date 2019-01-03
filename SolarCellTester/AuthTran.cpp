// AuthTran.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "AuthTran.h"


// CAuthTran dialog

IMPLEMENT_DYNAMIC(CAuthTran, CDialog)

CAuthTran::CAuthTran(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthTran::IDD, pParent)
	, m_str_message(_T(""))
{

}

CAuthTran::~CAuthTran()
{
}

void CAuthTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR_MESSAGE, m_str_message);
}


BEGIN_MESSAGE_MAP(CAuthTran, CDialog)
	ON_BN_CLICKED(IDOK, &CAuthTran::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAuthTran::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CAuthTran::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAuthTran::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAuthTran message handlers

void CAuthTran::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CAuthTran::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_str_message = _T("");
	OnCancel();
}

void CAuthTran::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.LoadString(IDS_STRAUTHMESSAGE_172);
	if (m_encr_temp.GetAuthInf())
	{
		m_str_message += str;
		str.LoadString(IDS_STRAUTHMESSAGE_171);
		if(::MessageBox(NULL,str,_T("Message"),MB_YESNO) == IDYES)
		{
			if(m_encr_temp.ReSet())
			{
				GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
				str.LoadString(IDS_STRAUTHMESSAGE_174);
				m_str_message += str;
			}else
			{
				str.LoadString(IDS_STRAUTHMESSAGE_173);
				m_str_message += str;
			}
		}
	}else
	{
		str.LoadString(IDS_STRAUTHMESSAGE_175);
		m_str_message += str;
	}
	UpdateData(FALSE);

}

void CAuthTran::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString str;
	if (m_encr_temp.SetAuthInf())
	{
		//GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		str.LoadString(IDS_STRAUTHMESSAGE_176);
		m_str_message += str;
	}else
	{
		str.LoadString(IDS_STRAUTHMESSAGE_177);
		m_str_message += str;
	}
	UpdateData(FALSE);
	m_str_message = _T("");
}
