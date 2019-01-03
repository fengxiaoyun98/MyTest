#pragma once

#include "ChartSerie.h"
#include "ChartAxis.h"
#include "ChartGrid.h"
#include "ChartLegend.h"
#include "ChartTitle.h"
#include "ChartGradient.h"
#include "ChartAxisLabel.h"
#include "ChartComment.h"
#include "..\SolarCellTesterDoc.h"
#include <vector>
#include "..\SunAds\CurveIV.h"
#include "..\Options\SetChartView.h"
#include "..\stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CChartView window

class CChartView : public CView
{
	DECLARE_DYNCREATE(CChartView)
protected:
	CSetChartView* m_set;
	//从Wazzied创建的视类手动添加以下三条代码和相应的消息和响应代码就可可以支持框架打印支持
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
protected:
	CChartView();           // protected constructor used by dynamic creation
	virtual ~CChartView();

public:
	void UpdataParaDisplay(CurveIV& iv);
	void AddIV(CDoubleXY& xy, UINT cnt=0);
	void AddIV(CurveIV& iv);
	void AddPV(CurveIV& iv);
	void ApplySet(void);
	void SetCoordinate(CDC * pDC,CSize objwin);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	CRect GetPlottingRect()  const { return m_PlottingRect; }
	CRect GetOldRect()  const { return m_OldRect; }         //改动 界面


	//该对象表示图表的图例
	CChartComment* GetComment() const  { return m_pComment; }
	CChartLegend* GetLegend() const  { return m_pLegend; }
	CChartTitle*  GetTitle()  const  { return m_pTitles; }

	CChartAxis* GetBottomAxis() const;
	CChartAxis* GetLeftAxis() const;
	CChartAxis* GetTopAxis() const;
	CChartAxis* GetRightAxis() const;

	UINT GetEdgeType() const        { return EdgeType;    }
	void SetEdgeType(UINT NewEdge)  
	{ 
		EdgeType = NewEdge; 
	}

	COLORREF GetBackColor() const			{ return m_BackColor;   }
	void SetBackColor(COLORREF NewCol)		
	{ 
		m_BackColor = NewCol;  
		m_bBackGradient = false; 
	}
	COLORREF GetBorderColor() const			{ return m_BorderColor;   }
	void SetBorderColor(COLORREF NewCol)	{ m_BorderColor = NewCol; }
	COLORREF GetZoomRectColor() const		{ return m_ZoomRectColor;   }
	void SetZoomRectColor(COLORREF NewCol)	{ m_ZoomRectColor = NewCol; }
	void SetBackGradient(COLORREF Col1, COLORREF Col2, EGradientType GradientType);

	CChartSerie* AddSerie(int Type);
	CChartSerie* GetSerie(size_t Index) const;
	void RemoveSerie(size_t Index);
	void RemoveSerie(CChartSerie* pToRemove);
	void RemoveAllSeries();
	size_t GetSeriesCount() const;
	
	void SetPanEnabled(bool bEnabled)  { m_bPanEnabled = bEnabled;  }
	bool GetPanEnabled() const		   { return m_bPanEnabled;	    }
	void SetZoomEnabled(bool bEnabled) { m_bZoomEnabled = bEnabled; }
	bool GetZoomEnabled() const		   { return m_bZoomEnabled;	    }
	void UndoPanZoom();

	void RefreshDraw(CDC* pDC,CRect &ClientRect);
	int Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle=WS_VISIBLE);
	
	static double DateToValue(const COleDateTime& Date);
	static COleDateTime ValueToDate(double Value);

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	bool RegisterWindowClass();

	bool m_bMemDCCreated;

	bool m_bBackGradient;	// Specifies if the background is gradient or solid
	COLORREF m_BackGradientCol1;
	COLORREF m_BackGradientCol2;
	EGradientType m_BackGradientType;
	COLORREF m_BackColor;	
	COLORREF m_BorderColor;
	UINT EdgeType;		

	CRect m_PlottingRect;	// Zone in wich the series will be plotted
	CRect m_OldRect;	    //改动 界面

	std::vector<CChartSerie*> m_pSeriesList;		// Table containing all the series (dynamic)
	std::vector<CChartAxis*>  m_pAxisList;			// Table containing all the axes (dynamic)

	CChartComment* m_pComment;
	CChartLegend* m_pLegend;	// Chart legend
	CChartTitle*  m_pTitles;	// Chart titles

	// Support for mouse panning
	bool m_bPanEnabled;
	bool m_bRMouseDown;		// If the right mouse button is pressed
	CPoint m_PanAnchor;

	// Support for manual zoom
	bool  m_bZoomEnabled;
	bool  m_bLMouseDown;	// If the left mouse button is pressed
	CRect m_rectZoomArea;
	COLORREF m_ZoomRectColor;

	// Specifies if the toolbars have already been created
	bool m_bToolBarCreated;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();
};

/////////////////////////////////////////////////////////////////////////////


