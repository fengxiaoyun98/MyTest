// Login.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "Login.h"

BOOL g_bSenderIdentity;
// CLogin dialog


LRESULT CPasswordEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��Edit�����ݻ�ȡ����ͨ������������Ϣ֮һ 
	if((message==WM_GETTEXT)||(message==EM_GETLINE)){
		//����Ƿ�Ϊ�Ϸ� 
		if(!g_bSenderIdentity)//�Ƿ���ȡ,��ʾ��ϢAfxMessageBox(_T ("���棺������ͼ��ȡ���룡"));
			return 0;
		//�Ϸ���ȡ 
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
	//�����ȡ���� 
	//ע�⣺����CPropertyPage�����ﲻ��Ҫ if (pDX��>m_bSaveAndValidate)����
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
