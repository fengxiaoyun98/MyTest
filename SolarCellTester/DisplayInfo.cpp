// DisplayInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "DisplayInfo.h"
#include "Options\Registry.h"


// CDisplayInfo dialog

IMPLEMENT_DYNAMIC(CDisplayInfo, CDialog)

CDisplayInfo::CDisplayInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayInfo::IDD, pParent)
{
	m_classname=GetRuntimeClass()->m_lpszClassName;
	reg.CreateKey(m_classname);
	m_wndstatestr=_T("WINDOWPLACEMENT");
}

CDisplayInfo::~CDisplayInfo()
{
	m_fnt.DeleteObject();
}

void CDisplayInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayInfo, CDialog)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
//	ON_COMMAND(ID_RESIZE, &CDisplayInfo::OnResize)
//ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CDisplayInfo message handlers
LRESULT CDisplayInfo::OnInitDialog(UINT wParam,LONG lParam) 
{
	SetMsgFont(30);

	HWND hwnd=GetSafeHwnd();
	SetWindowLong(hwnd,GWL_EXSTYLE, GetWindowLong(hwnd,GWL_EXSTYLE)|0x80000L);//   ���÷ֲ���չ���  
	SetLayeredWindowAttributes(0,(255*80)/100,0x2);//   80%   alpha  
	// TODO: Add extra initialization here
	LRESULT bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("Init FloatDisplayBar Failed��");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//��һ��������д��
void CDisplayInfo::DrawTextRgn(CDC* pDC,CRect& rect,CStringArray* sta)
{
	if((sta==NULL)||sta->IsEmpty()||rect.IsRectEmpty()||pDC==NULL)
		return;
	//�ֿ�͸�����ʻ���Χ�ǵ�ɫ
	INT_PTR cnt=sta->GetCount();

	pDC->SetBkMode(TRANSPARENT);
	CFont *pf=pDC->SelectObject(&m_fnt);
    
/*	if (((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.IsWindowVisible())
	{
		CRect tRect;
        ((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.GetClientRect(tRect);
		rect.top += tRect.bottom;
		AfxMessageBox(L"1");
	}
*/
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rect);
	pDC->SelectClipRgn(&rgn);     

	//ȡ������߶�
	CString str(_T("�л����񹲺͹�"));
	CSize sz=pDC->GetTextExtent(str);
	int high=sz.cy;
	int space=high/4;//�о�
	for(int i=0;i<cnt;i++){
		str=sta->GetAt(i);
		if (-1 == str.Find(_T("!")))
			pDC->SetTextColor(RGB(0,0,255));
		else
			pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOut(rect.left,rect.top+i*high+space,str,str.GetLength());
	}
	rgn.DeleteObject();
	pf=pDC->SelectObject(pf);

}
//���ñ���ɫΪ��ɫ
void CDisplayInfo::SetBoardColor(CDC* pDC,COLORREF bcolor)
{
	CBrush m_BrushBack;
	m_BrushBack.CreateSolidBrush(bcolor) ;
	pDC->SetBkColor(bcolor);
	CRect ClientRect;
	GetClientRect(ClientRect);
	pDC->FillRect(ClientRect,&m_BrushBack);
}
void CDisplayInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	SetBoardColor(&dc,RGB(255,255,255));
	CRect ClientRect;
	GetClientRect(ClientRect);
	ClientRect.DeflateRect(5,5);
	DrawTextRgn(&dc,ClientRect,&m_list);

}
//���Ӷ���
void CDisplayInfo::DisplayMessage(CStringArray* msglist)
{
	m_list.Append(*msglist);
	this->Invalidate();
}
//����һ��
void CDisplayInfo::AddString(CString item)
{
	m_list.Add(item);
	this->Invalidate();
}
//ɾ��һ�л���ȫ��
void CDisplayInfo::ClearBoard(int at)
{
	if(at<0)
		m_list.RemoveAll();
	else if(at<m_list.GetCount())
		m_list.RemoveAt(at);
	this->Invalidate();
}
//
//�ı���ʾ���塢�ߴ�
void CDisplayInfo::SetMsgFont( int high,CString fontname)
{
	//ֻ�ܸı��࣬���ܸı��С
	CFont* pf=this->GetFont();
	if(!fontname){
		LOGFONT lgfnt;
		pf->GetLogFont(&lgfnt);
		fontname=lgfnt.lfFaceName;
	}
	m_fnt.DeleteObject();
	CClientDC dc(this);
	m_fnt.CreatePointFont(high*10,fontname,&dc);//nPointSize=high=10*dot
	this->Invalidate();
}
//�ޱ߿򴰿��϶�
void CDisplayInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(10,10);
	if(rt.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}
//��¼����״̬
void CDisplayInfo::OnDestroy()
{
    WINDOWPLACEMENT   WndStatus;  
    WndStatus.length=sizeof(WINDOWPLACEMENT);  
    GetWindowPlacement(&WndStatus);   
	reg.Write(m_wndstatestr,(BYTE*)&WndStatus,sizeof(WINDOWPLACEMENT));
	CDialog::OnDestroy();
}
//װ����ʷ״̬
int CDisplayInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

static int slist[5]={30,60,120,240,540};    //�Ķ�  �Ըı������С
static byte sloop=0;

void CDisplayInfo::OnRButtonDown(UINT nFlags, CPoint point)
{
	sloop++;
	SetMsgFont(slist[sloop%5]);
	Invalidate();
	CDialog::OnRButtonDown(nFlags, point);
}
void CDisplayInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
}

