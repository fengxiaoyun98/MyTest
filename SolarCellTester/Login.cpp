// Login.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "Login.h"

BOOL g_bSenderIdentity;
// CLogin dialog


LRESULT CPasswordEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//对Edit的内容获取必须通过以下两个消息之一 
	if((message==WM_GETTEXT)||(message==EM_GETLINE)){
		//检查是否为合法 
		if(!g_bSenderIdentity)//非法获取,显示信息AfxMessageBox(_T ("报告：正在试图窃取密码！"));
			return 0;
		//合法获取 
		g_bSenderIdentity=FALSE;
	}
	return CEdit::DefWindowProc(message, wParam, lParam);
}


IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CString n,CString p,CWnd* pParent/*=NULL*/)   // standard constructor
	: CDialog(CLogin::IDD, pParent)
{
	m_name=n;m_Password=p;
	m_autopass=FALSE;
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	//如果获取数据 
	//注意：对于CPropertyPage类这里不需要 if (pDX－>m_bSaveAndValidate)条件
	if(pDX->m_bSaveAndValidate)
		g_bSenderIdentity=TRUE; 
	CDialog::DoDataExchange(pDX);
	DDX_Text (pDX,IDC_EDIT_PASSWORD,m_Password);
	DDX_Text (pDX,IDC_EDIT_NAME,m_name);
	DDX_Check(pDX,IDC_MPASS,m_autopass);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
//	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogin message handlers

BOOL CLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_edtPassword.SubclassDlgItem(IDC_EDIT_PASSWORD,this); 
	m_Password=L"";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
