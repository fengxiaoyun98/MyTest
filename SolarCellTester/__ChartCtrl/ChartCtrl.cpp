/*
 *
 *	ChartCtrl.cpp
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
 *	History:
 *		- 18/05/2006: Added support for panning
 *		- 28/05/2006: Bug corrected in RemoveAllSeries
 *		- 28/05/2006: Added support for resizing
 *		- 12/06/2006: Added support for manual zoom
 *		- 10/08/2006: Added SetZoomMinMax and UndoZoom
 *		- 24/03/2007: GDI leak corrected
 *		- 24/03/2007: Invisible series are not taken in account for auto axis 
 *					  and legend (thanks to jerminator-jp).
 *		- 24/03/2007: possibility to specify a margin for the axis. Needs to be improved
 *		- 05/04/2007: ability to change the text color of the axis.
 *		- 05/04/2007: ability to change the color of the border of the drawing area.
 *		- 05/04/2007: Surface series added.
 *		- 26/08/2007: The clipping area of the series is a bit larger (they will be
 *					  drawn over the bottom and right axes).
 *		- 12/01/2007: Ability to change the color of the zoom rectangle.
 *		- 08/02/2008: Added convenience functions to convert from date to value and 
 *					  opposite.
 *		- 21/02/2008: The zoom doesn't do anything if the user only clicks on the control
 *					  (thanks to Eugene Pustovoyt).
 *		- 29/02/2008: The auto axis are now refreshed when a series is removed (thanks to
 *					  Bruno Lavier).
 *		- 08/03/2008: EnableRefresh function added (thanks to Bruno Lavier).
 *		- 21/03/2008: Added support for scrolling.
 *		- 25/03/2008: UndoZoom function added.
 *		- 25/03/2008: A series can now be removed using its pointer.
 *
 */

#include "stdafx.h"
#include "ChartCtrl.h"

#include "ChartSerie.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartSurfaceSerie.h"
#include "ChartBarSerie.h"
#include "ChartGradient.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARTCTRL_CLASSNAME    (_T("ChartCtrl"))  // Window class name


COLORREF pSeriesColorTable[] = { RGB(255,0,0), RGB(0,150,0), RGB(0,0,255), RGB(255,255,0), RGB(0,255,255), 
								 RGB(255,128,0), RGB(128,0,128), RGB(128,128,0), RGB(255,0,255), RGB(64,128,128)};

/////////////////////////////////////////////////////////////////////////////
// CChartCtrl

CChartCtrl::CChartCtrl()
{
	RegisterWindowClass();

	m_bEnableRefresh = true;
	m_bPendingRefresh = false;
	m_BorderColor = RGB(0,0,0);
	m_BackColor = GetSysColor(COLOR_BTNFACE);
	EdgeType = EDGE_RAISED;
	m_BackGradientType = gtVertical;
	m_bBackGradient = false;
	m_BackGradientCol1 = m_BackGradientCol2 = m_BackColor;

	CChartAxis* pBottom = new CChartAxis(this,true);
	CChartAxis* pLeft = new CChartAxis(this,false);
	CChartAxis* pTop = new CChartAxis(this,true);
	pTop->m_bIsVisible = false;
	pTop->SetSecondary(true);
	CChartAxis* pRight = new CChartAxis(this,false);
	pRight->m_bIsVisible = false;
	pRight->SetSecondary(true);

	m_pAxisList.push_back(pBottom);
	m_pAxisList.push_back(pLeft);
	m_pAxisList.push_back(pTop);
	m_pAxisList.push_back(pRight);

	m_pLegend = new CChartLegend(this);
	m_pTitles = new CChartTitle(this);
	
	m_bMemDCCreated = false;
	m_bPanEnabled = true;
	m_bRMouseDown = false;

	m_bZoomEnabled = true;
	m_bLMouseDown = false;
	m_ZoomRectColor = RGB(255,255,255);

	m_bToolBarCreated = false;
}

CChartCtrl::~CChartCtrl()
{
	size_t SeriesCount = m_pSeriesList.size();
	for (size_t i=0;i<SeriesCount;i++)
		delete m_pSeriesList[i];

	size_t AxisCount = m_pAxisList.size();
	for (size_t j=0;j<AxisCount;j++)
		delete m_pAxisList[j];

	if (m_pLegend)
	{
		delete m_pLegend;
		m_pLegend = NULL;
	}
	if (m_pTitles)
	{
		delete m_pTitles;
		m_pTitles = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChartCtrl, CWnd)
	//{{AFX_MSG_MAP(CChartCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChartCtrl message handlers

void CChartCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bMemDCCreated)
	{
		RefreshCtrl();
		m_bMemDCCreated = true;
	}

    // Get Size of Display area
    CRect rect;
    GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), 
			  &m_BackgroundDC, 0, 0, SRCCOPY) ;
	// Draw the zoom rectangle
	if (m_bZoomEnabled && m_bLMouseDown)
	{
		CPen NewPen(PS_SOLID,1,m_ZoomRectColor);
		CPen* pOldPen = dc.SelectObject(&NewPen);

		dc.MoveTo(m_rectZoomArea.TopLeft());
		dc.LineTo(m_rectZoomArea.right,m_rectZoomArea.top);
		dc.LineTo(m_rectZoomArea.BottomRight());
		dc.LineTo(m_rectZoomArea.left,m_rectZoomArea.bottom);
		dc.LineTo(m_rectZoomArea.TopLeft());

		dc.SelectObject(pOldPen);
		DeleteObject(NewPen);
	}
}

BOOL CChartCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// To avoid flickering
//	return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}

bool CChartCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CString(CHARTCTRL_CLASSNAME), &wndcls)))
    {
		memset(&wndcls, 0, sizeof(WNDCLASS));   

		wndcls.hInstance		= hInst;
		wndcls.lpfnWndProc		= ::DefWindowProc;
	//	wndcls.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wndcls.hIcon			= 0;
		wndcls.lpszMenuName		= NULL;
		wndcls.hbrBackground	= (HBRUSH) ::GetStockObject(WHITE_BRUSH);
		wndcls.style			= CS_GLOBALCLASS; // To be modified
		wndcls.cbClsExtra		= 0;
		wndcls.cbWndExtra		= 0;
		wndcls.lpszClassName    = CHARTCTRL_CLASSNAME;

        if (!RegisterClass(&wndcls))
        {
          //  AfxThrowResourceException();
            return false;
        }
    }

    return true;

}

int CChartCtrl::Create(CWnd *pParentWnd, const RECT &rect, UINT nID, DWORD dwStyle)
{
	int Result = CWnd::Create(CHARTCTRL_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);

	if (Result)
		RefreshCtrl();

	return Result;
}

void CChartCtrl::SetBackGradient(COLORREF Col1, COLORREF Col2, EGradientType GradientType)
{
	m_bBackGradient = true;
	m_BackGradientCol1 = Col1;
	m_BackGradientCol2 = Col2;
	m_BackGradientType = GradientType;
	RefreshCtrl();
}

void CChartCtrl::EnableRefresh(bool bEnable)
{
	m_bEnableRefresh = bEnable;
	if (m_bEnableRefresh && m_bPendingRefresh)
	{
		m_bPendingRefresh = false;
		RefreshCtrl();
	}
}

void CChartCtrl::UndoPanZoom()
{
	GetBottomAxis()->UndoZoom();
	GetTopAxis()->UndoZoom();
	GetLeftAxis()->UndoZoom();
	GetRightAxis()->UndoZoom();
}

void CChartCtrl::RefreshCtrl(CDC* pDC,CRect &ClientRect)
{
	if(pDC==NULL)
		return;

	m_PlottingRect = ClientRect;		

	CBrush m_BrushBack;
	m_BrushBack.CreateSolidBrush(m_BackColor) ;

	if (!m_bBackGradient)
	{
		pDC->SetBkColor(m_BackColor);
		pDC->FillRect(ClientRect,&m_BrushBack);
	}
	else
	{
		CChartGradient::DrawGradient(pDC,ClientRect,m_BackGradientCol1,
									 m_BackGradientCol2,m_BackGradientType);
	}

	pDC->DrawEdge(ClientRect,EdgeType,BF_RECT);
	ClientRect.DeflateRect(3,3);

	CSize TitleSize = m_pTitles->GetSize(pDC);
	CRect rcTitle;
	rcTitle = ClientRect;
	rcTitle.bottom = TitleSize.cy;

	ClientRect.top += TitleSize.cy;
	m_pTitles->SetRect(rcTitle);
	m_pTitles->Draw(pDC);

	m_pLegend->ClipArea(ClientRect,pDC);

	//Clip all the margins (axis) of the client rect
	size_t AxisCount = m_pAxisList.size();
	size_t SeriesCount = m_pSeriesList.size();
	
	// TODO: find a better way to handle this.
	GetBottomAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetBottomAxis()->Recalculate();
	GetBottomAxis()->ClipMargin(ClientRect,m_PlottingRect,pDC);
	GetTopAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetTopAxis()->Recalculate();
	GetTopAxis()->ClipMargin(ClientRect,m_PlottingRect,pDC);
	GetLeftAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetLeftAxis()->Recalculate();
	GetLeftAxis()->ClipMargin(ClientRect,m_PlottingRect,pDC);
	GetRightAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetRightAxis()->Recalculate();
	GetRightAxis()->ClipMargin(ClientRect,m_PlottingRect,pDC);
	for (size_t n=0;n<AxisCount;n++)
	{
		m_pAxisList[n]->SetAxisSize(ClientRect,m_PlottingRect);
		m_pAxisList[n]->Recalculate();
		m_pAxisList[n]->Draw(pDC);
	}

	CPen SolidPen(PS_SOLID,0,m_BorderColor);
	CPen* pOldPen = pDC->SelectObject(&SolidPen);

	if (!m_bToolBarCreated)
	{
		std::vector<CChartAxis*>::iterator iter = m_pAxisList.begin();
		for (iter; iter!=m_pAxisList.end(); iter++)
		{
			(*iter)->CreateScrollBar();
		}
		m_bToolBarCreated = true;
	}

	std::vector<CChartAxis*>::iterator iter = m_pAxisList.begin();
	for (iter; iter!=m_pAxisList.end(); iter++)
	{
		(*iter)->UpdateScrollBarPos();
	}

	pDC->MoveTo(m_PlottingRect.left,m_PlottingRect.top);
	pDC->LineTo(m_PlottingRect.right,m_PlottingRect.top);
	pDC->LineTo(m_PlottingRect.right,m_PlottingRect.bottom);
	pDC->LineTo(m_PlottingRect.left,m_PlottingRect.bottom);
	pDC->LineTo(m_PlottingRect.left,m_PlottingRect.top);

	pDC->SelectObject(pOldPen);
	DeleteObject(SolidPen);

	for (size_t z=0;z<SeriesCount;z++)
	{
		CChartSerie* m_NewLine = (CChartSerie*)m_pSeriesList[z];
		CRect drawingRect = m_PlottingRect;
		drawingRect.bottom += 1;
		drawingRect.right += 1;
		m_NewLine->SetRect(drawingRect);
		m_NewLine->DrawAll(pDC);
	}

	// Draw the legend at the end (when floating it should come over the plotting area).
	m_pLegend->Draw(pDC);

}
void CChartCtrl::RefreshCtrl()
{
	if (!m_bEnableRefresh)
	{
		m_bPendingRefresh = true;
		return;
	}

	CClientDC dc(this) ;  
	CRect ClientRect;
	GetClientRect(ClientRect);
	m_PlottingRect = ClientRect;		

	CBrush m_BrushBack;
	m_BrushBack.CreateSolidBrush(m_BackColor) ;

	if (!m_BackgroundDC.GetSafeHdc() )
	{
		CBitmap memBitmap;
		m_BackgroundDC.CreateCompatibleDC(&dc) ;
		memBitmap.CreateCompatibleBitmap(&dc, ClientRect.Width(),ClientRect.Height()) ;
		m_BackgroundDC.SelectObject(&memBitmap) ;
	}
    
	if (!m_bBackGradient)
	{
		m_BackgroundDC.SetBkColor(m_BackColor);
		m_BackgroundDC.FillRect(ClientRect,&m_BrushBack);
	}
	else
	{
		CChartGradient::DrawGradient(&m_BackgroundDC,ClientRect,m_BackGradientCol1,
									 m_BackGradientCol2,m_BackGradientType);
	}

	m_BackgroundDC.DrawEdge(ClientRect,EdgeType,BF_RECT);
	ClientRect.DeflateRect(3,3);

	CSize TitleSize = m_pTitles->GetSize(&m_BackgroundDC);
	CRect rcTitle;
	rcTitle = ClientRect;
	rcTitle.bottom = TitleSize.cy;

	ClientRect.top += TitleSize.cy;
	m_pTitles->SetRect(rcTitle);
	m_pTitles->Draw(&m_BackgroundDC);

	m_pLegend->ClipArea(ClientRect,&m_BackgroundDC);

	//Clip all the margins (axis) of the client rect
	size_t AxisCount = m_pAxisList.size();
	size_t SeriesCount = m_pSeriesList.size();
	
	// TODO: find a better way to handle this.
	GetBottomAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetBottomAxis()->Recalculate();
	GetBottomAxis()->ClipMargin(ClientRect,m_PlottingRect,&m_BackgroundDC);
	GetTopAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetTopAxis()->Recalculate();
	GetTopAxis()->ClipMargin(ClientRect,m_PlottingRect,&m_BackgroundDC);
	GetLeftAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetLeftAxis()->Recalculate();
	GetLeftAxis()->ClipMargin(ClientRect,m_PlottingRect,&m_BackgroundDC);
	GetRightAxis()->SetAxisSize(ClientRect,m_PlottingRect);
	GetRightAxis()->Recalculate();
	GetRightAxis()->ClipMargin(ClientRect,m_PlottingRect,&m_BackgroundDC);
	for (size_t n=0;n<AxisCount;n++)
	{
		m_pAxisList[n]->SetAxisSize(ClientRect,m_PlottingRect);
		m_pAxisList[n]->Recalculate();
		m_pAxisList[n]->Draw(&m_BackgroundDC);
	}

	CPen SolidPen(PS_SOLID,0,m_BorderColor);
	CPen* pOldPen = m_BackgroundDC.SelectObject(&SolidPen);

	if (!m_bToolBarCreated)
	{
		std::vector<CChartAxis*>::iterator iter = m_pAxisList.begin();
		for (iter; iter!=m_pAxisList.end(); iter++)
		{
			(*iter)->CreateScrollBar();
		}
		m_bToolBarCreated = true;
	}

	std::vector<CChartAxis*>::iterator iter = m_pAxisList.begin();
	for (iter; iter!=m_pAxisList.end(); iter++)
	{
		(*iter)->UpdateScrollBarPos();
	}

	m_BackgroundDC.MoveTo(m_PlottingRect.left,m_PlottingRect.top);
	m_BackgroundDC.LineTo(m_PlottingRect.right,m_PlottingRect.top);
	m_BackgroundDC.LineTo(m_PlottingRect.right,m_PlottingRect.bottom);
	m_BackgroundDC.LineTo(m_PlottingRect.left,m_PlottingRect.bottom);
	m_BackgroundDC.LineTo(m_PlottingRect.left,m_PlottingRect.top);

	m_BackgroundDC.SelectObject(pOldPen);
	DeleteObject(SolidPen);

	for (size_t z=0;z<SeriesCount;z++)
	{
		CChartSerie* m_NewLine = (CChartSerie*)m_pSeriesList[z];
		CRect drawingRect = m_PlottingRect;
		drawingRect.bottom += 1;
		drawingRect.right += 1;
		m_NewLine->SetRect(drawingRect);
		m_NewLine->DrawAll(&m_BackgroundDC);
	}

	// Draw the legend at the end (when floating it should come over the plotting area).
	m_pLegend->Draw(&m_BackgroundDC);

	Invalidate();
}


CChartSerie* CChartCtrl::AddSerie(int Type)
{
	size_t Count = m_pSeriesList.size();

	size_t ColIndex = Count%10;
	CChartSerie* pNewSeries = NULL;

	switch (Type)
	{
	case CChartSerie::stLineSerie:
		pNewSeries = new CChartLineSerie(this);
		break;

	case CChartSerie::stPointsSerie:
		pNewSeries = new CChartPointsSerie(this);
		break;

	case CChartSerie::stSurfaceSerie:
		pNewSeries = new CChartSurfaceSerie(this);
		break;

	case CChartSerie::stBarSerie:
		pNewSeries = new CChartBarSerie(this);
		break;

	default:
		pNewSeries = NULL;
		break;
	}

	if (pNewSeries)
	{
		pNewSeries->SetRect(m_PlottingRect);
		pNewSeries->SetColor(pSeriesColorTable[ColIndex]);
		m_pSeriesList.push_back(pNewSeries);
	}

	return pNewSeries;
}

CChartSerie* CChartCtrl::GetSerie(size_t Index) const
{
	size_t Count = m_pSeriesList.size();
	if (Index>=Count)
		return NULL;

	return m_pSeriesList[Index];
}

void CChartCtrl::RemoveSerie(size_t Index)
{
	size_t Count = m_pSeriesList.size();
	if (Index>=Count)
		return;

	CChartSerie* pToDelete = m_pSeriesList[Index];

    std::vector<CChartSerie*>::iterator it = m_pSeriesList.begin() + Index;
	m_pSeriesList.erase(it);
	if (pToDelete)
	{
		EnableRefresh(false);
		pToDelete->m_pVerticalAxis->UnregisterSeries(pToDelete);
		pToDelete->m_pHorizontalAxis->UnregisterSeries(pToDelete);
		pToDelete->m_pVerticalAxis->RefreshAutoAxis();
		pToDelete->m_pHorizontalAxis->RefreshAutoAxis();
		delete pToDelete;
		pToDelete = NULL;
		RefreshCtrl();
		EnableRefresh(true);
	}
}

void CChartCtrl::RemoveSerie(CChartSerie* pToRemove)
{
    std::vector<CChartSerie*>::iterator iter = m_pSeriesList.begin();
	for (iter; iter!=m_pSeriesList.end();iter++)
	{
		if (*iter == pToRemove)
		{
			m_pSeriesList.erase(iter);
			break;
		}
	}

	if (pToRemove)
	{
		EnableRefresh(false);
		pToRemove->m_pVerticalAxis->UnregisterSeries(pToRemove);
		pToRemove->m_pHorizontalAxis->UnregisterSeries(pToRemove);
		pToRemove->m_pVerticalAxis->RefreshAutoAxis();
		pToRemove->m_pHorizontalAxis->RefreshAutoAxis();
		delete pToRemove;
		RefreshCtrl();
		EnableRefresh(true);
	}
}

void CChartCtrl::RemoveAllSeries()
{
	std::vector<CChartSerie*>::iterator iter = m_pSeriesList.begin();
	for (iter; iter != m_pSeriesList.end(); iter++)
	{
		delete (*iter);
	}

	m_pSeriesList.clear();
	RefreshCtrl();
}


CChartAxis* CChartCtrl::GetBottomAxis() const
{
	return (m_pAxisList[0]);
}

CChartAxis* CChartCtrl::GetLeftAxis() const
{
	return (m_pAxisList[1]);
}

CChartAxis* CChartCtrl::GetTopAxis() const
{
	return (m_pAxisList[2]);
}

CChartAxis* CChartCtrl::GetRightAxis() const
{
	return (m_pAxisList[3]);
}


CDC* CChartCtrl::GetDC()
{
	return &m_BackgroundDC;
}


size_t CChartCtrl::GetSeriesCount() const
{
	return m_pSeriesList.size();
}

void CChartCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bRMouseDown && m_bPanEnabled)
	{
		if (point != m_PanAnchor)
		{
			GetLeftAxis()->PanAxis(m_PanAnchor.y,point.y);
			GetRightAxis()->PanAxis(m_PanAnchor.y,point.y);
			GetBottomAxis()->PanAxis(m_PanAnchor.x,point.x);
			GetTopAxis()->PanAxis(m_PanAnchor.x,point.x);

			RefreshCtrl();
			m_PanAnchor = point;
		}
	}

	if (m_bLMouseDown && m_bZoomEnabled)
	{
		m_rectZoomArea.BottomRight() = point;
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChartCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_bRMouseDown = true;
	if (m_bPanEnabled)
		m_PanAnchor = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CChartCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRMouseDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

void CChartCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bZoomEnabled)
	{
		m_bLMouseDown = true;
		m_rectZoomArea.TopLeft() = point;
		m_rectZoomArea.BottomRight() = point;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChartCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLMouseDown = false;
	if (m_bZoomEnabled)
	{
		if ( (m_rectZoomArea.left > m_rectZoomArea.right) ||
			 (m_rectZoomArea.top > m_rectZoomArea.bottom))
		{
			GetBottomAxis()->UndoZoom();
			GetTopAxis()->UndoZoom();
			GetLeftAxis()->UndoZoom();
			GetRightAxis()->UndoZoom();
		}
		else if ( (m_rectZoomArea.left!=m_rectZoomArea.right) &&
				  (m_rectZoomArea.top!=m_rectZoomArea.bottom))
		{
			CChartAxis* pBottom = GetBottomAxis();
			double MinVal = 0;			
			double MaxVal = 0;
			
			if (pBottom->IsInverted())
			{
				MaxVal = pBottom->ScreenToValue(m_rectZoomArea.left);
				MinVal = pBottom->ScreenToValue(m_rectZoomArea.right);
			}
			else
			{
				MinVal = pBottom->ScreenToValue(m_rectZoomArea.left);
				MaxVal = pBottom->ScreenToValue(m_rectZoomArea.right);
			}
			pBottom->SetZoomMinMax(MinVal,MaxVal);

			CChartAxis* pLeft = GetLeftAxis();
			if (pLeft->IsInverted())
			{
				MaxVal = pLeft->ScreenToValue(m_rectZoomArea.bottom);
				MinVal = pLeft->ScreenToValue(m_rectZoomArea.top);
			}
			else
			{
				MinVal = pLeft->ScreenToValue(m_rectZoomArea.bottom);
				MaxVal = pLeft->ScreenToValue(m_rectZoomArea.top);
			}
			pLeft->SetZoomMinMax(MinVal,MaxVal);

			CChartAxis* pTop = GetTopAxis();
			if (pTop->IsInverted())
			{
				MaxVal = pTop->ScreenToValue(m_rectZoomArea.left);
				MinVal = pTop->ScreenToValue(m_rectZoomArea.right);
			}
			else
			{
				MinVal = pTop->ScreenToValue(m_rectZoomArea.left);
				MaxVal = pTop->ScreenToValue(m_rectZoomArea.right);
			}
			pTop->SetZoomMinMax(MinVal,MaxVal);

			CChartAxis* pRight = GetRightAxis();
			if (pRight->IsInverted())
			{
				MaxVal = pRight->ScreenToValue(m_rectZoomArea.bottom);
				MinVal = pRight->ScreenToValue(m_rectZoomArea.top);
			}
			else
			{
				MinVal = pRight->ScreenToValue(m_rectZoomArea.bottom);
				MaxVal = pRight->ScreenToValue(m_rectZoomArea.top);
			}
			pRight->SetZoomMinMax(MinVal,MaxVal);
		}

		RefreshCtrl();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CChartCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// Force recreation of background DC
	if (m_BackgroundDC.GetSafeHdc() )
		m_BackgroundDC.DeleteDC();
	
	RefreshCtrl();
}

double CChartCtrl::DateToValue(const COleDateTime& Date)
{
	return (DATE)Date;
}

COleDateTime CChartCtrl::ValueToDate(double Value)
{
	COleDateTime RetDate((DATE)Value);
	return RetDate;
}

void CChartCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CChartScrollBar* pChartBar = dynamic_cast<CChartScrollBar*>(pScrollBar);
	if (pChartBar)
		pChartBar->OnHScroll(nSBCode, nPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	RefreshCtrl();
}

void CChartCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CChartScrollBar* pChartBar = dynamic_cast<CChartScrollBar*>(pScrollBar);
	if (pChartBar)
		pChartBar->OnVScroll(nSBCode, nPos);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	RefreshCtrl();
}