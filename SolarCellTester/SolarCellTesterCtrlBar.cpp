// SolarCellTesterCtrlBar.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SolarCellTesterCtrlBar.h"

#include "SolarCellTesterMainFrm.h"
#include "SolarCellTesterDoc.h"
#include "SolarCellTesterView.h"

// CSolarCellTesterCtrlBar dialog

IMPLEMENT_DYNAMIC(CSolarCellTesterCtrlBar, CDialogBar)

CSolarCellTesterCtrlBar::CSolarCellTesterCtrlBar(CWnd* pParent /*=NULL*/)
//	: CDialog(CSolarCellTesterCtrlBar::IDD, pParent)
{

}

CSolarCellTesterCtrlBar::~CSolarCellTesterCtrlBar()
{
}

void CSolarCellTesterCtrlBar::DoDataExchange(CDataExchange* pDX)
{
//	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TEL, m_staticTel);

}


BEGIN_MESSAGE_MAP(CSolarCellTesterCtrlBar, CDialogBar)
   ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
   ON_WM_PAINT()
   ON_MESSAGE(WM_HOTKEY,OnHotKey)
   ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSolarCellTesterCtrlBar message handlers
LRESULT CSolarCellTesterCtrlBar::OnInitDialog(UINT wParam,LONG lParam) 
{
	// 注册热键
	hWnd = GetSafeHwnd();
// 	CWnd* wnd = GetForegroundWindow();
// 	hWnd = wnd->GetSafeHwnd();
 
	RegisterHotKey(hWnd, 10001, 0, VK_F4);
	RegisterHotKey(hWnd, 10002, 0, VK_F3); 

   
	//CDialogBar::OnInitDialog();
	// TODO: Add extra initialization here


	//加载资源文件中的EMF格式徽标数据
	HRSRC hRsrc=::FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MARK),_T("mark"));
    if(hRsrc!=NULL)
	{
		HGLOBAL hImgData=::LoadResource(AfxGetResourceHandle(),hRsrc);
		if(hImgData==NULL)
		{
			::FreeResource(hImgData);
		}
		else
		{
			LPVOID lpVoid = ::LockResource(hImgData);
			LPSTREAM pStream=NULL;
			DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
			HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
			LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
			::memcpy(lpByte, lpVoid, dwSize);
			GlobalUnlock(hNew);
			HRESULT ht = CreateStreamOnHGlobal(hNew, TRUE, &pStream);
			if( ht != S_OK )
			{
				//ASSERT( FALSE );
				GlobalFree(hNew);
			}
			else
			{
				m_emf.Load(pStream);
				GlobalFree(hNew);
			}
			::FreeResource(hImgData);
		}
	}
 
   LRESULT bRet = HandleInitDialog(wParam,lParam);
   if (!UpdateData(FALSE))
   {
          TRACE("Init SolarCellTesterCtrlBar Failed！");
   }
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CSolarCellTesterCtrlBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rt;
	GetDlgItem(IDC_BUTTON_OPTIONS4)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	int nWidth;
	int nHeight;
	int nfar=30;

	//显示徽标
	if(!m_emf.IsNull()){
	nWidth=m_emf.GetWidth();
	nHeight=m_emf.GetHeight();
	m_emf.Draw(dc.m_hDC ,rt.left,rt.bottom+nfar , rt.Width(), nHeight*rt.Width()/nWidth, 0, 0,nWidth, nHeight);
	}

	if (hWnd == NULL)
	{
		CWnd* wnd = GetForegroundWindow();
		hWnd = wnd->GetSafeHwnd();
	}

	RegisterHotKey(hWnd, 10001, 0, VK_F4);
	RegisterHotKey(hWnd, 10002, 0, VK_F3); 
}

LRESULT CSolarCellTesterCtrlBar::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	// 当前应用程序是否在顶层，是则热键有效
	CWnd* wnd = GetForegroundWindow();
	CString str;
	wnd->GetWindowText(str);
	if (str != L"太阳电池测试设置" && str != L"Solar Simulator")
	{
		return FALSE;
	}
	// 为“测试”按钮添加快捷键SPACE和ENTER
	switch(wParam) 
	{
	case 10001:          //空格键
		SendMessage(WM_COMMAND, IDC_BUTTON_START,0);
		break;
	case 10002:
		SendMessage(WM_COMMAND, IDC_BUTTON_OPTIONS2,0);
		break;
	}

	return TRUE;
}

void CSolarCellTesterCtrlBar::OnDestroy()
{
	CDialogBar::OnDestroy();
	UnregisterHotKey(hWnd,10001);
	UnregisterHotKey(hWnd,10002);
}

