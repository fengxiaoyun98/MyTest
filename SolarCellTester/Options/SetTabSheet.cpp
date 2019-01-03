#include "stdafx.h"
#include "SetTabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTabSheet

CSetTabSheet::CSetTabSheet()
{
	m_nNumOfPages = 0;
	m_nCurrentPage = 0;
}

CSetTabSheet::~CSetTabSheet()
{
}


BEGIN_MESSAGE_MAP(CSetTabSheet, CTabCtrl)
	//{{AFX_MSG_MAP(CSetTabSheet)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTabSheet message handlers

BOOL CSetTabSheet::AddPage(LPCTSTR title, CSetInterFace* pDialog,UINT ID)
{
	if( MAXPAGE == m_nNumOfPages )
		return FALSE;


	m_pPages[m_nNumOfPages] = pDialog;
	m_IDD[m_nNumOfPages] = ID;
	m_Title[m_nNumOfPages] = title;
	m_pPages[m_nNumOfPages]->Create( ID, this );
	InsertItem( m_nNumOfPages, title );

	m_nNumOfPages++;

	return TRUE;
}

void CSetTabSheet::SetRect()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-2;
	nYc=tabRect.bottom-nY-2;

	m_pPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for( int nCount=1; nCount < m_nNumOfPages; nCount++ )
		m_pPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

}

void CSetTabSheet::Show()
{

	m_pPages[0]->ShowWindow(SW_SHOW);
	for( int i=1; i < m_nNumOfPages; i++)
		m_pPages[i]->ShowWindow(SW_HIDE);

	SetRect();

}

void CSetTabSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_nCurrentPage != GetCurFocus())
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage=GetCurFocus();
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		
//		m_pPages[m_nCurrentPage]->SetFocus();
	}
}

int CSetTabSheet::SetCurSel(int nItem)
{
	if( nItem < 0 || nItem >= m_nNumOfPages)
		return -1;

	int ret = m_nCurrentPage;

	if(m_nCurrentPage != nItem )
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = nItem;
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
//		m_pPages[m_nCurrentPage]->SetFocus();
		CTabCtrl::SetCurSel(nItem);
	}

	return ret;
}
CSetInterFace* CSetTabSheet::GetCurDialog(void)
{
	return m_pPages[CTabCtrl::GetCurSel()];
}

int CSetTabSheet::GetCurSel()
{
	return CTabCtrl::GetCurSel();
}
