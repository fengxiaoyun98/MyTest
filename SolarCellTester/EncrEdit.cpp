// EncrEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "EncrEdit.h"


// CEncrEdit dialog

IMPLEMENT_DYNAMIC(CEncrEdit, CDialog)

CEncrEdit::CEncrEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CEncrEdit::IDD, pParent)
	, m_cpuid(_T(""))
	, m_authcode(_T(""))
{

}

CEncrEdit::~CEncrEdit()
{
}

void CEncrEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cpuid);
	DDX_Text(pDX, IDC_EDIT2, m_authcode);
}


BEGIN_MESSAGE_MAP(CEncrEdit, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CEncrEdit::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CEncrEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEncrEdit::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEncrEdit message handlers

void CEncrEdit::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	CString str(m_cpuid);
	if (OpenClipboard())   
	{
		EmptyClipboard();  
	
		HGLOBAL hClip;
		hClip = GlobalAlloc(GMEM_MOVEABLE, (str.GetLength() * 2) + 2); 
		TCHAR *pBuf;
		pBuf = (TCHAR *)GlobalLock(hClip);
		lstrcpy(pBuf, str);
		GlobalUnlock(hClip);
		SetClipboardData(CF_UNICODETEXT, hClip);
		CloseClipboard();
	} 
}

void CEncrEdit::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnOK();
}

void CEncrEdit::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
