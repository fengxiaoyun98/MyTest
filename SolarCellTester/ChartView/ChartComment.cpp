
#include "stdafx.h"
#include "ChartComment.h"
#include "ChartSerie.h"
#include "ChartView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartComment::CChartComment(CChartView* pParent):CChartObject(pParent)
{
	m_ObjectColor = RGB(255,255,255);
	m_mode=0;//GetComment()->SetPosStyle(0);
	m_iFontSize = 140;
	m_strFontName = _T("courier new");
	//m_strFontName = _T("Times New Roman");
	m_rectpos.SetRectEmpty ();
	m_comet=_T("");

	m_bIsVisible = true;

	m_iLeftPos = m_iTopPos = 0;
	m_bIsTransparent = false;
	m_bIsHorizontal = false;

	m_bShadow = true;
	m_iShadowDepth = 3;
	m_BitmapSize.cx = 16;
	m_BitmapSize.cy = 16;
}

CChartComment::~CChartComment()
{
}

void CChartComment::SetFont(int iPointSize,CString strFaceName)
{
	m_iFontSize = iPointSize;
	m_strFontName = strFaceName;
}

void CChartComment::SetTransparent(bool bTransparent)
{ 
	m_bIsTransparent = bTransparent; 
}

void  CChartComment::SetComment(CString str)
{
	m_comet=str;
}

void CChartComment::ClipArea(CRect& rcControl, CDC* pDC)
{
	CSize sz=pDC->GetTextExtent (m_comet,m_comet.GetLength ());
	m_ObjectRect=CRect(0,0,sz.cx,sz.cy);
	if (m_ObjectRect.IsRectEmpty())
		return;
}
void CChartComment::SetRect(void)
{
	m_rectpos=m_pParent->GetPlottingRect();

//	int space=m_rectpos.Width()/20;         //改动
	int x,y;
	m_pParent->GetRightAxis()->Clip(x,y);//左上角
	int off=y;
	off=y+m_rectpos.right;
	
	switch(m_mode)
	{
		case 0:m_rectpos.OffsetRect(m_rectpos.Width()+70,0); break;   //改动   数据显示位置
		case 1:m_rectpos.OffsetRect(off,0);break;
	}

}
void CChartComment::SetPosStyle(int mode)
{
	m_mode=mode;
}
void CChartComment::Draw(CDC *pDC)
{
	if (!pDC->GetSafeHdc())
		return;
	if (!m_bIsVisible)
		return;
	if (m_ObjectRect.IsRectEmpty())
		return;

	SetRect();
    
	CPen SolidPen(PS_SOLID,0,RGB(0,0,0));
	CPen* pOldPen;
	CFont* pOldFont;
	CFont NewFont;
	if (pDC->IsPrinting())     //important change
	{
		m_iFontSize = 80;
	}else
		m_iFontSize = 140;
	NewFont.CreatePointFont(m_iFontSize,m_strFontName,pDC);

	pOldFont = pDC->SelectObject(&NewFont);
	pOldPen = pDC->SelectObject(&SolidPen);

	int iPrevMode = pDC->SetBkMode(TRANSPARENT);
	CRect rectBitmap(m_ObjectRect.left+2,m_ObjectRect.top+5,
					 m_ObjectRect.left+2+m_BitmapSize.cx,
					 m_ObjectRect.top+6+m_BitmapSize.cy);

	pDC->DrawText(m_comet,m_comet.GetLength(),m_rectpos,DT_EXPANDTABS);

	pDC->SetBkMode(iPrevMode);
	pDC->SelectObject(pOldFont);
	DeleteObject(NewFont);
	pDC->SelectObject(pOldPen);
	DeleteObject(SolidPen);
}

