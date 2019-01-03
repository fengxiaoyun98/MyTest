// MCMsgTipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "InOutTip.h"

const UINT_PTR BLAND_IN = 4;
const UINT_PTR BLAND_OUT = 5;

const UINT IN_ELAPSE = 1;
const UINT OUT_ELAPSE = 5000;

// CInOutTipDlg dialog

IMPLEMENT_DYNAMIC(CInOutTipDlg, CDialog)

CInOutTipDlg::CInOutTipDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInOutTipDlg::IDD, pParent)
, m_strTipInfo(_T(""))
{

}

CInOutTipDlg::~CInOutTipDlg()
{
}

void CInOutTipDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInOutTipDlg, CDialog)
    ON_WM_TIMER()
	ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDOK, &CInOutTipDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CInOutTipDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CInOutTipDlg message handlers
void CInOutTipDlg::ShowMsgWindow(CWnd* pParent, const CString& strTipInfo)
{
    m_strTipInfo = strTipInfo;
    Create(IDD, pParent);
    ShowWindow(SW_HIDE);
}
HBRUSH CInOutTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	UINT id=pWnd->GetDlgCtrlID();
	if (id==IDC_TIP_INFO)   //改变 id为IDC_EMAIL static 的颜色
	{
		pDC->SetTextColor(m_TextColor);
	//	pDC->SelectObject(&f);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
BOOL CInOutTipDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    SetDlgItemText(IDC_TIP_INFO, m_strTipInfo);

    InitDlgPosition();

    //消息渐入渐出效果
    SetTimer(BLAND_IN, IN_ELAPSE, NULL);

    return TRUE;
}

void CInOutTipDlg::InitDlgPosition()
{
    CRect rectInit;
    GetWindowRect(&rectInit);

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    int cy = rect.bottom-rect.top;
    int cx = rect.right-rect.left;

    int nx = rect.right - rectInit.Width();
    int ny = cy - rectInit.Height();

    rectInit.MoveToXY(nx, ny);

    MoveWindow(rectInit);
}

void CInOutTipDlg::OnTimer(UINT_PTR nIDEvent)
{
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
    int cy = rect.bottom-rect.top;
    int cx = rect.right-rect.left;

    CRect rectTip;
    GetWindowRect(&rectTip);

    switch (nIDEvent)
    {
    case BLAND_IN:
        {
            KillTimer(BLAND_IN);
            AnimateWindow(1000, AW_BLEND); 
            SetTimer(BLAND_OUT, OUT_ELAPSE, NULL);
            break;
        }
    case BLAND_OUT:
        {
            KillTimer(BLAND_OUT);
            AnimateWindow(1000, AW_BLEND|AW_HIDE);
			//PostMessage(WM_CLOSE);
			CDialog::OnClose();
            break;
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CInOutTipDlg::OnCancel()
{
    DestroyWindow();
}

void CInOutTipDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();

    //窗口销毁时，删除该对象
    delete this;
}

void CInOutTipDlg::OnBnClickedOk()
{
    OnOK();
}

void CInOutTipDlg::OnBnClickedCancel()
{
    OnCancel();
}
