// InputBox.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "InputBox.h"


// CInputBox dialog

IMPLEMENT_DYNAMIC(CInputBox, CDialog)

CInputBox::CInputBox(CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox::IDD, pParent)
{
	m_string=(_T(""));
	m_swloop=FALSE;
}

CInputBox::~CInputBox()
{
	
}
void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_string);
	DDX_Check(pDX,IDC_CHECKLOOP,m_swloop);
}


BEGIN_MESSAGE_MAP(CInputBox, CDialog)
	ON_WM_SHOWWINDOW()
//	ON_BN_CLICKED(IDC_CHECKLOOP, &CInputBox::OnBnClickedCheckloop)
END_MESSAGE_MAP()


// CInputBox message handlers
BOOL CInputBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CFont * f;
	f = new CFont;
	f->CreateFont(28, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_BOLD, // nWeight
		0, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Arial")); // lpszFac 
	GetDlgItem(IDC_EDIT1)->SetFont(f);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		UpdateData(FALSE);//更新变量到控件
		CEdit* pWnd=(CEdit*)GetDlgItem(IDC_EDIT1);
		pWnd->SetFocus();
		pWnd->SetSel(0,pWnd->GetWindowTextLength());
	}
}

