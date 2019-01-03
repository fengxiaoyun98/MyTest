// DefLevel.cpp : implementation file
//

#include "stdafx.h"
#include "DefLevel.h"


/////////////////////////////////////////////////////////////////////////////
// CDefLevel dialog

IMPLEMENT_DYNAMIC(CDefLevel, CDialog)

CDefLevel::CDefLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CDefLevel::IDD, pParent)
	, m_mode(0)
{
	m_pow=_T("Pmax:");
	m_vcur=_T("ThePoint:");
	m_eff=_T("Eff:");
	m_color=RGB(0,0,255);
	m_bcolor=RGB(255,255,255);
	m_classname=GetRuntimeClass()->m_lpszClassName;
	reg.CreateKey(m_classname);
	m_wndstatestr=_T("WINDOWPLACEMENT");
}
CDefLevel::~CDefLevel()
{
}


void CDefLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_RADIODEF1,m_mode);
}


BEGIN_MESSAGE_MAP(CDefLevel, CDialog)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_RADIODEF1, &CDefLevel::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIODEF2, &CDefLevel::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIODEF3, &CDefLevel::OnBnClickedRadio3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefLevel message handlers

void CDefLevel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CFont *ft=dc.GetCurrentFont();
	LOGFONT lt;
	memset(&lt,0,sizeof(LOGFONT));
	ft->GetLogFont (&lt);
	_tcscpy(lt.lfFaceName ,_T("Arial"));
	lt.lfHeight = 48;
	lt.lfWidth =lt.lfHeight/2;
	HFONT hFont=::CreateFontIndirect(&lt);
	HFONT oldf=(HFONT)dc.SelectObject (hFont);
	DWORD cr=dc.SetTextColor (m_color);

	int radiohigh=30;
	CRect rect;
	GetClientRect(rect);
	rect.top+=radiohigh;//无限按钮区域
	dc.FillSolidRect(rect,m_bcolor); //设置背景
	CString str;
	switch (m_mode){
		case 0:str=m_eff;break;
		case 1:str=m_pow;break;
		case 2:str=m_vcur;break;
		default:str=m_eff;break;
	}
	int step=10;
	CSize sz=dc.GetTextExtent(str);
	sz.cy+=radiohigh+step*2;
//	MoveWindow(0,0,600,sz.cy  );
	dc.TextOut (step,radiohigh+step,str);

	dc.SelectObject (oldf);
	dc.SetTextColor (cr);
	DeleteObject(hFont);
	ft->DeleteObject ();

	// Do not call CDialog::OnPaint() for painting messages
}

void CDefLevel::SetValue(double p,double c,double e)
{
	CString str;
	str.Format (_T("Pmax:%0.4f"),p);
	m_pow=str;
	str.Format (_T("ThePoint:%0.4f"),c);
	m_vcur=str;
	if(e<1)
		e*=100;
	str.Format (_T("Eff:%0.4f"),e);
	m_eff=str;
	this->UpdateData (false);
}

void CDefLevel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}


void CDefLevel::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	static BYTE ref=0;
	switch(ref)
	{
	case 0:m_color=RGB(0,0,0);break;
	case 1:m_color=RGB(255,0,0);break;
	case 2:m_color=RGB(0,255,0);break;
	case 3:m_color=RGB(0,0,255);break;
	case 4:m_color=RGB(255,255,0);break;
	case 5:m_color=RGB(255,0,255);break;
	case 6:m_color=RGB(0,255,255);break;
	case 7:m_color=RGB(255,255,255);break;
	}
	if(ref<7)ref++;
	else ref=0;
	this->Invalidate ();
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDefLevel::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	static BYTE ref=0;
	switch(ref)
	{
	case 0:m_bcolor=RGB(0,0,0);break;
	case 1:m_bcolor=RGB(255,0,0);break;
	case 2:m_bcolor=RGB(0,255,0);break;
	case 3:m_bcolor=RGB(0,0,255);break;
	case 4:m_bcolor=RGB(255,255,0);break;
	case 5:m_bcolor=RGB(255,0,255);break;
	case 6:m_bcolor=RGB(0,255,255);break;
	case 7:m_bcolor=RGB(255,255,255);break;
	}
	if(ref<7)ref++;
	else ref=0;
	this->Invalidate ();

	CDialog::OnRButtonDblClk(nFlags, point);
}

void CDefLevel::OnBnClickedRadio1()
{
	UpdateData();
	Invalidate();
}

void CDefLevel::OnBnClickedRadio2()
{
	UpdateData();
	Invalidate();
}

void CDefLevel::OnBnClickedRadio3()
{
	UpdateData();
	Invalidate();
}
LRESULT CDefLevel::OnInitDialog(UINT wParam,LONG lParam) 
{
//	CDialog::OnInitDialog();

	//SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOSIZE);
	HWND hwnd=GetSafeHwnd();
	SetWindowLong(hwnd,GWL_EXSTYLE, GetWindowLong(hwnd,GWL_EXSTYLE)|0x80000L);//   设置分层扩展标记  
	SetLayeredWindowAttributes(0,(255*80)/100,0x2);//   80%   alpha  
	// TODO: Add extra initialization here
	LRESULT bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("Init FloatDisplayBar Failed！");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//记录窗口状态
void CDefLevel::OnDestroy()
{
	WINDOWPLACEMENT   WndStatus;  
	WndStatus.length=sizeof(WINDOWPLACEMENT);  
	GetWindowPlacement(&WndStatus);   
	reg.Write(m_wndstatestr,(BYTE*)&WndStatus,sizeof(WINDOWPLACEMENT));
	CDialog::OnDestroy();
}
//装入历史状态
int CDefLevel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	WINDOWPLACEMENT   WndStatus;  
	memset(&WndStatus,0,sizeof(WINDOWPLACEMENT));
	if(reg.Read(m_wndstatestr,(BYTE*)&WndStatus,sizeof(WINDOWPLACEMENT)))
		SetWindowPlacement(&WndStatus);  

	return 0;
}