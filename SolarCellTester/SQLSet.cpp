// SQLSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SQLSet.h"


// CSQLSet �Ի���

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


// CSQLSet ��Ϣ�������

void CSQLSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}

void CSQLSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
