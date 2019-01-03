// SQLSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SQLSet.h"


// CSQLSet 对话框

IMPLEMENT_DYNAMIC(CSQLSet, CDialog)

CSQLSet::CSQLSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLSet::IDD, pParent)
	, m_serverip(_T(""))
	, m_password(_T(""))
	, m_database(_T(""))
	, m_username(_T(""))
{

}

CSQLSet::~CSQLSet()
{
}

void CSQLSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_serverip);
	DDX_Text(pDX, IDC_EDIT2, m_database);
	DDX_Text(pDX, IDC_EDIT3, m_username);
	DDX_Text(pDX, IDC_EDIT6, m_password);

}


BEGIN_MESSAGE_MAP(CSQLSet, CDialog)
	ON_BN_CLICKED(IDOK, &CSQLSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSQLSet::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSQLSet 消息处理程序

void CSQLSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}

void CSQLSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
