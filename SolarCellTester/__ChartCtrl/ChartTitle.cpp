/*
 *
 *	ChartTitle.cpp
 *
 *	Written by C�dric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *
 */

#include "stdafx.h"
#include "ChartTitle.h"
#include "ChartCtrl.h"
#include "Math.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartTitle::CChartTitle(CChartCtrl* pParent) : CChartObject(pParent)
{
	m_ObjectColor = RGB(0,0,0);
	m_iFontSize = 100;
	m_strFontName = "Microsoft Sans Serif";
}

CChartTitle::~CChartTitle()
{

}

void CChartTitle::SetFont(int iPointSize,CString strFaceName)
{
	m_iFontSize = iPointSize;
	m_strFontName = strFaceName;
	m_pParent->RefreshCtrl();
}

void CChartTitle::AddString(CString NewString)
{
	m_StringArray.push_back(NewString);
	m_pParent->RefreshCtrl();
}

size_t CChartTitle::GetStringCount() const
{
	return m_StringArray.size();
}

CString CChartTitle::GetString(size_t Index) const
{
	if ( (Index<0) || (Index>=m_StringArray.size()) )
		return _T("");
	return m_StringArray[Index];
}

void CChartTitle::RemoveAll()
{
	m_StringArray.clear();
	m_pParent->RefreshCtrl();
}

void CChartTitle::Draw(CDC *pDC)
{
	if (!pDC->GetSafeHdc())
		return;
	if (!m_bIsVisible)
		return;

	CFont* pOldFont;
	CFont NewFont;
	NewFont.CreatePointFont(m_iFontSize,m_strFontName,pDC);

	COLORREF OldColor = pDC->SetTextColor(m_ObjectColor);
	pOldFont = pDC->SelectObject(&NewFont);
	int iPrevMode = pDC->SetBkMode(TRANSPARENT);

	//Draw all entries
	int YPos = 4;
	size_t TitleCount = m_StringArray.size();
	for (size_t i=0;i<TitleCount;i++)
	{
		//Draw Text
		int TextWidth = pDC->GetTextExtent(m_StringArray[i]).cx;
		int TextHeigh = pDC->GetTextExtent(m_StringArray[i]).cy;

		int XPos = m_ObjectRect.left + (int)fabs((m_ObjectRect.left-m_ObjectRect.right)/2.0) - TextWidth/2;

		if (m_bShadow)
		{
			pDC->SetTextColor(m_ShadowColor);
			pDC->ExtTextOut(XPos+m_iShadowDepth,m_ObjectRect.top+YPos+m_iShadowDepth,
							ETO_CLIPPED,NULL,m_StringArray[i],NULL);
			pDC->SetTextColor(m_ObjectColor);
		}
		pDC->ExtTextOut(XPos,m_ObjectRect.top+YPos,ETO_CLIPPED,NULL,m_StringArray[i],NULL);

		YPos += TextHeigh + 2;
	}

	pDC->SelectObject(pOldFont);
	DeleteObject(NewFont);
	pDC->SetTextColor(OldColor);
	pDC->SetBkMode(iPrevMode);
}

CSize CChartTitle::GetSize(CDC *pDC)
{
	CSize TitleSize;

	if (!m_bIsVisible)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	int Height = 4;		//Upper space
	CSize TextSize = 0;
	int MaxTextWidth = 0;

	size_t TitleCount = m_StringArray.size();
	if (TitleCount==0)
	{
		TitleSize.cx = TitleSize.cy = 0;
		return TitleSize;
	}

	CFont* pOldFont;
	CFont NewFont;
	NewFont.CreatePointFont(m_iFontSize,m_strFontName,pDC);
	pOldFont = pDC->SelectObject(&NewFont);

	for (size_t i=0;i<TitleCount;i++)
	{
		TextSize = pDC->GetTextExtent(m_StringArray[i]);
		Height += TextSize.cy + 2;
		if (TextSize.cx > MaxTextWidth)
			MaxTextWidth = TextSize.cx;
	}

	TitleSize.cx = MaxTextWidth + 2;
	TitleSize.cy = Height;

	m_ObjectRect.bottom = m_ObjectRect.top + Height;

	pDC->SelectObject(pOldFont);
	DeleteObject(NewFont);
	return TitleSize;
}

