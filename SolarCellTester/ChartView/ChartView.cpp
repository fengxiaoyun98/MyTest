// ..\ChartVew\ChartView.cpp : implementation file
//

#include "stdafx.h"
#include "ChartView.h"
#include "ChartSerie.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartSurfaceSerie.h"
#include "ChartBarSerie.h"
#include "ChartGradient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ChartView_CLASSNAME    (_T("ChartView"))  // Window class name


COLORREF pSeriesColorTable[] = { RGB(255,0,0), RGB(0,150,0), RGB(0,0,255), RGB(255,255,0), RGB(0,255,255), 
								 RGB(255,128,0), RGB(128,0,128), RGB(128,128,0), RGB(255,0,255), RGB(64,128,128)};


// CChartView

IMPLEMENT_DYNCREATE(CChartView, CView)


BEGIN_MESSAGE_MAP(CChartView, CView)
	//{{AFX_MSG_MAP(CChartView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)   
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)   
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,CView::OnFilePrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CChartView::OnPreparePrinting(CPrintInfo* pInfo)
{   
	//   default   preparation   
	return   DoPreparePrinting(pInfo);   
}   
void CChartView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{ 
	CView::OnPrepareDC(pDC, pInfo);
// 	pDC->SetMapMode(MM_ANISOTROPIC); //转换坐标映射方式
// 	CSize size = CSize(800, 560);
//     pDC->SetWindowExt(size);//确定窗口大小 //得到实际设备每逻辑英寸的象素数量 　　 
// 	 int xLogPixPerInch = pDC-> GetDeviceCaps(LOGPIXELSX);
// 	 int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);//得到设备坐标和逻辑坐标的比例
// 	 long xExt = (long)size.cx * xLogPixPerInch/96;
// 	 long yExt = (long)size.cy * yLogPixPerInch/96;
// 	 pDC->SetViewportExt((int)xExt, (int)yExt);//确定视口大小			　　 
}
void CChartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	//   TODO:   add   extra   initialization   before   printing 
}   

void CChartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	//   TODO:   add   cleanup   after   printing   
}   
//void CChartView::AddPV(CDoubleXY& xy,double im,double pm)
void CChartView::AddPV(CurveIV& iv)
{
	if(!GetRightAxis()->IsVisible())
		return;
	if(iv.x.empty())
		return;
	CChartLineSerie* pIV = (CChartLineSerie*)AddSerie(CChartSerie::stLineSerie);
	pIV->SetVerticalAxis(1);//设置和右边坐标轴关联
	CDoubleXY pow;
	pow.EquB(&iv);
	pow.MultiXYToY();
	pIV->SetSeriesOrdering(CChartSerie::soNoOrdering);
	pIV->AddPoints(pow.x,pow.y,pow.GetDataSize());

	double min,max;
	GetLeftAxis()->GetMinMax(min,max);
	double vm=iv.GetVmax();

	GetRightAxis()->SetMinMax(min*vm,max*vm);
}
void CChartView::AddIV(CDoubleXY& xy, UINT cnt)
{
	if(xy.x.empty())
		return;
	if(cnt==0||cnt>xy.GetDataSize())
		cnt=xy.GetDataSize();
	CChartLineSerie* pIV = (CChartLineSerie*)AddSerie(CChartSerie::stLineSerie);
	pIV->SetSeriesOrdering(CChartSerie::soNoOrdering);
	pIV->AddPoints(xy.x,xy.y,cnt);
}
void CChartView::AddIV(CurveIV& iv)
{
	if(iv.x.empty())
		return;
	CChartLineSerie* pIV = (CChartLineSerie*)AddSerie(CChartSerie::stLineSerie);
	pIV->AddPoints(iv.x,iv.y,iv.GetDataSize());
	pIV->SetSeriesOrdering(CChartSerie::soNoOrdering);
	UpdataParaDisplay(iv);
}
void CChartView::UpdataParaDisplay(CurveIV& iv)
{
	CString str=iv.GetCustomParaListString();
	GetComment()->SetComment(str);//只能显示最后一条曲线的
}

// CChartView drawing
void CChartView::SetCoordinate(CDC * pDC,CSize objwin)
{
	CRect rt;
	CSize sw;
	pDC->SetMapMode (MM_ISOTROPIC);//(MM_ANISOTROPIC);//
	//确定物理尺寸,自动调整尺寸
	if(pDC->IsPrinting ()){
		sw.cx=pDC->GetDeviceCaps(HORZRES);
		sw.cy=pDC->GetDeviceCaps(VERTRES);
	}else{
		GetClientRect (&rt);
		sw=rt.Size ();//像素单位
	}
	pDC->SetWindowExt(objwin);
	pDC->SetViewportExt(sw);//以像素为单位
	pDC->SetWindowOrg (0,0);
	pDC->SetViewportOrg(0,0);
}
void CChartView::ApplySet(void)
{
	if(!m_set){
			m_set=theApp.m_opt.GetSetChartView();
			m_set->SetCustomView(this);
	}

	CChartAxis* xaxis=GetBottomAxis();
	xaxis->SetMinMax(m_set->GetXmin(),m_set->GetXmax());
	xaxis->SetAutomatic(m_set->GetXAuto());
	xaxis->GetLabel()->SetVisible(m_set->GetHideXTitle());
	xaxis->GetGrid()->SetVisible(m_set->GetHideXGrid());
	CChartAxis* yaxis=GetLeftAxis();
	yaxis->SetMinMax(m_set->GetYmin(),m_set->GetYmax());
	yaxis->SetAutomatic(m_set->GetYAuto());
	yaxis->GetLabel()->SetVisible(m_set->GetHideYTitle());
	yaxis->GetGrid()->SetVisible(m_set->GetHideYGrid());
	CChartAxis* zaxis=GetRightAxis();
	zaxis->SetMinMax(m_set->GetZmin(),m_set->GetZmax());
	zaxis->SetAutomatic(true);
	zaxis->GetLabel()->SetVisible(m_set->GetHideZTitle());
	zaxis->SetVisible(m_set->GetHidePV());              
	
	GetTitle()->SetVisible(m_set->GetHideTitle());
	GetComment()->SetVisible(m_set->GetHideComm());  

	CClientDC dc(this);
	OnDraw(&dc);         
}
void CChartView::OnDraw(CDC* pDC)
{
	CSolarCellTesterDoc* pDoc = (CSolarCellTesterDoc*)GetDocument();
	// TODO: add draw code here

	CRect rect;
	GetClientRect(&rect);

	/*/--------------------------------------------------------------//mark
	CRect rectCopy;
	rectCopy=rect;
	//双缓冲技术
	CClientDC dc(this);
	// Step 1:为屏幕DC创建兼容的  内存
	CDC memDC;
	pDC=&memDC;

	pDC->CreateCompatibleDC(&dc);
	// Step 2:创建位图
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,rectCopy.Width(),rectCopy.Height());	

	// Step 3:把位图选人设备环境 SelectObject(),可以理解为选择画布
	CBitmap *pOldBitmap = pDC->SelectObject(&bmp);

	//--------------------------------------------------------------*/

	SetCoordinate(pDC,rect.Size());

	if(pDC->IsPrinting ())
		rect.DeflateRect(rect.Width()/20,rect.Width()/20);

	pDoc->OnPreDraw(pDC);
	SetBackColor (RGB(255,255,255));

	rect.top +=50;
	rect.right -=50;
	RefreshDraw(pDC,rect);
	pDoc->OnEndDraw(pDC);

	/*/-------------------------------------------------------------//mark
	//绘图双缓冲技术
	dc.BitBlt(0,0,rectCopy.Width(),rectCopy.Height(),pDC,0,0,SRCCOPY);
	pDC->SelectObject(pOldBitmap);
	//-------------------------------------------------------------*/
}

void CChartView::RefreshDraw(CDC* pDC,CRect &ClientRect)    //改动 界面 
{
	if(pDC==NULL)
		return; 


	CBrush m_BrushBack;
	m_BrushBack.CreateSolidBrush(m_BackColor) ;

	if (m_bBackGradient)
	{
		CChartGradient::DrawGradient(pDC,ClientRect,m_BackGradientCol1,
									 m_BackGradientCol2,m_BackGradientType);
	}
	else
	{
		pDC->SetBkColor(m_BackColor);
		CRect allRect = ClientRect;
		allRect.top -=50;
		allRect.right +=50;
		pDC->FillRect(allRect,&m_BrushBack);
	}
	m_OldRect = ClientRect;         //改动 界面
	m_PlottingRect = ClientRect;
	m_PlottingRect.right = ClientRect.right - 430;	 //改动

	CSize TitleSize = m_pTitles->GetSize(pDC);
	CRect rcTitle;
	rcTitle = m_PlottingRect;           //改动 标题字体位置
//	rcTitle.bottom = TitleSize.cy;

	// 标题
//	ClientRect.top += TitleSize.cy;
	if (m_set->GetHideTitle())
	{
		GetTitle()->RemoveAll();
		GetTitle()->AddString(theApp.m_opt.GetSetCurveIV()->GetManufacture()); //改动  标题
		GetTitle()->SetRect(rcTitle);
		GetTitle()->Draw(pDC);
	}

	m_pComment->ClipArea(ClientRect,pDC);    
	m_pLegend->ClipArea(ClientRect,pDC);
	m_pLegend->SetVisible(true);

	GetRightAxis()->GetLabel()->SetText (_T("Power(W)"));
	GetRightAxis()->RefreshAutoAxis();
	GetLeftAxis()->GetLabel()->SetText (_T("Current(A)"));
	GetLeftAxis()->RefreshAutoAxis();
	if (Iselfcheck)
	{
		GetBottomAxis()->GetLabel()->SetText (_T("Time(S)"));    //mark  改动
		GetBottomAxis()->RefreshAutoAxis();
	}else
	{
		GetBottomAxis()->GetLabel()->SetText (_T("Voltage(V)"));    //mark  改动
		GetBottomAxis()->RefreshAutoAxis();
	}

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

	pDC->SelectObject(pOldPen);
	DeleteObject(SolidPen);

	for (size_t z=0;z<SeriesCount;z++)
	{
 		CChartSerie* m_NewLine = (CChartSerie*)m_pSeriesList[z];
		CRect drawingRect = m_PlottingRect;
		drawingRect.bottom += 1;
		drawingRect.right += 1;
		m_NewLine->SetRect(drawingRect);

		if (z == 1 && !m_set->GetHidePV())
		{
			break;
		}
		if (z == 2 && !m_set->GetHideIRR())
		{
			break;
		}
		m_NewLine->DrawAll(pDC);    
	}

	// Draw the legend at the end (when doubleing it should come over the plotting area).
	m_pLegend->Draw(pDC);
	m_pComment->Draw(pDC);

}

/////////////////////////////////////////////////////////////////////////////
// CChartView

CChartView::CChartView()
{
	//RegisterWindowClass();

	m_set=NULL;
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
	pTop->SetSecondary(true);                 //改动  决定是否显示哪个轴   总共四个轴
	CChartAxis* pRight = new CChartAxis(this,false);
	pRight->SetSecondary(true);

	m_pAxisList.push_back(pBottom);
	m_pAxisList.push_back(pLeft);
	m_pAxisList.push_back(pTop);
	m_pAxisList.push_back(pRight);

	m_pComment = new CChartComment(this);
	m_pLegend = new CChartLegend(this);
	m_pTitles = new CChartTitle(this);
	
	m_bMemDCCreated = false;
	m_bPanEnabled = true;
	m_bRMouseDown = false;

	m_bZoomEnabled = true;
	m_bLMouseDown = false;
	m_ZoomRectColor = RGB(0,0,0);//RGB(255,255,255);

	m_bToolBarCreated = false;  

}

CChartView::~CChartView()
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
	if (m_pComment)
	{
		delete m_pComment;
		m_pComment = NULL;
	}	
	if (m_pTitles)
	{
		delete m_pTitles;
		m_pTitles = NULL;
	}

}



/////////////////////////////////////////////////////////////////////////////
// CChartView message handlers

BOOL CChartView::OnEraseBkgnd(CDC* pDC) 
{
	// To avoid flickering
//	return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}

bool CChartView::RegisterWindowClass()
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CString(ChartView_CLASSNAME), &wndcls)))
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
		wndcls.lpszClassName    = ChartView_CLASSNAME;

        if (!RegisterClass(&wndcls))
        {
          //  AfxThrowResourceException();
            return false;
        }
    }

    return true;

}

int CChartView::Create(CWnd *pParentWnd, const RECT &rect, UINT nID, DWORD dwStyle)
{
	int Result = CWnd::Create(ChartView_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);


	return Result;
}

void CChartView::SetBackGradient(COLORREF Col1, COLORREF Col2, EGradientType GradientType)
{
	m_bBackGradient = true;
	m_BackGradientCol1 = Col1;
	m_BackGradientCol2 = Col2;
	m_BackGradientType = GradientType;
}

void CChartView::UndoPanZoom()
{
	GetBottomAxis()->UndoZoom();
	GetTopAxis()->UndoZoom();
	GetLeftAxis()->UndoZoom();
	GetRightAxis()->UndoZoom();
}


CChartSerie* CChartView::AddSerie(int Type)
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

CChartSerie* CChartView::GetSerie(size_t Index) const
{
	size_t Count = m_pSeriesList.size();
	if (Index>=Count)
		return NULL;

	return m_pSeriesList[Index];
}

void CChartView::RemoveSerie(size_t Index)
{
	size_t Count = m_pSeriesList.size();
	if (Index>=Count)
		return;

	CChartSerie* pToDelete = m_pSeriesList[Index];

    std::vector<CChartSerie*>::iterator it = m_pSeriesList.begin() + Index;
	m_pSeriesList.erase(it);
	if (pToDelete)
	{
		pToDelete->m_pVerticalAxis->UnregisterSeries(pToDelete);
		pToDelete->m_pHorizontalAxis->UnregisterSeries(pToDelete);
		pToDelete->m_pVerticalAxis->RefreshAutoAxis();
		pToDelete->m_pHorizontalAxis->RefreshAutoAxis();
		delete pToDelete;
		pToDelete = NULL;
	}
}

void CChartView::RemoveSerie(CChartSerie* pToRemove)
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
		pToRemove->m_pVerticalAxis->UnregisterSeries(pToRemove);
		pToRemove->m_pHorizontalAxis->UnregisterSeries(pToRemove);
		pToRemove->m_pVerticalAxis->RefreshAutoAxis();
		pToRemove->m_pHorizontalAxis->RefreshAutoAxis();
		delete pToRemove;
	}
}

void CChartView::RemoveAllSeries()
{
	std::vector<CChartSerie*>::iterator iter = m_pSeriesList.begin();
	for (iter; iter != m_pSeriesList.end(); iter++)
	{
		delete (*iter);
	}
	m_pSeriesList.clear();
}


CChartAxis* CChartView::GetBottomAxis() const
{
	return (m_pAxisList[0]);
}

CChartAxis* CChartView::GetLeftAxis() const
{
	return (m_pAxisList[1]);
}

CChartAxis* CChartView::GetTopAxis() const
{
	return (m_pAxisList[2]);
}

CChartAxis* CChartView::GetRightAxis() const
{
	return (m_pAxisList[3]);
}

size_t CChartView::GetSeriesCount() const
{
	return m_pSeriesList.size();
}

void CChartView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bRMouseDown && m_bPanEnabled)
	{
		if (point != m_PanAnchor)
		{
			GetLeftAxis()->PanAxis(m_PanAnchor.y,point.y);
			GetRightAxis()->PanAxis(m_PanAnchor.y,point.y);
			GetBottomAxis()->PanAxis(m_PanAnchor.x,point.x);
			GetTopAxis()->PanAxis(m_PanAnchor.x,point.x);
			Invalidate();
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

void CChartView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_bRMouseDown = true;
	if (m_bPanEnabled)
		m_PanAnchor = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CChartView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRMouseDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

void CChartView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bZoomEnabled)
	{
		m_bLMouseDown = true;
		m_rectZoomArea.TopLeft() = point;
		m_rectZoomArea.BottomRight() = point;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChartView::OnLButtonUp(UINT nFlags, CPoint point) 
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

		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CChartView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

double CChartView::DateToValue(const COleDateTime& Date)
{
	return (DATE)Date;
}

COleDateTime CChartView::ValueToDate(double Value)
{
	COleDateTime RetDate((DATE)Value);
	return RetDate;
}

void CChartView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CChartScrollBar* pChartBar = dynamic_cast<CChartScrollBar*>(pScrollBar);
	if (pChartBar)
		pChartBar->OnHScroll(nSBCode, nPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}

void CChartView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CChartScrollBar* pChartBar = dynamic_cast<CChartScrollBar*>(pScrollBar);
	if (pChartBar)
		pChartBar->OnVScroll(nSBCode, nPos);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}


// CChartView diagnostics

#ifdef _DEBUG
void CChartView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


void CChartView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if(!m_set){
			m_set=theApp.m_opt.GetSetChartView();
			m_set->SetCustomView(this);
	}
}
