// ColoredStatusBarCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "dlgviewtest.h"
#include "ColoredStatusBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrl

CColoredStatusBarCtrl::CColoredStatusBarCtrl()
{
	m_szStatusString = "";
}

CColoredStatusBarCtrl::~CColoredStatusBarCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredStatusBarCtrl, CStatusBar)
	//{{AFX_MSG_MAP(CColoredStatusBarCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrl message handlers

void CColoredStatusBarCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Attach to a CDC object
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	dc.SetBkMode(TRANSPARENT);

	// Get the pane rectangle and calculate text coordinates
	CRect rect(&lpDrawItemStruct->rcItem);

	if (m_szStatusString.Compare(_T("Gsolar Power Co.,Ltd.")) == 0)
	{	
		dc.SetTextColor(RGB(0, 0, 0));
	}else
		dc.SetTextColor(RGB(255, 0, 0));

	
	dc.TextOut(rect.left+2, rect.top, m_szStatusString);
	// Detach from the CDC object, otherwise the hDC will be
	// destroyed when the CDC object goes out of scope
	dc.Detach();
}
