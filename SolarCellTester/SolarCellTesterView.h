// SolarCellTesterView.h : interface of the CSolarCellTesterView class
//


#pragma once


class CSolarCellTesterView : public CView
{
protected: // create from serialization only
	CSolarCellTesterView();
	DECLARE_DYNCREATE(CSolarCellTesterView)

// Attributes
public:
	CSolarCellTesterDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSolarCellTesterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelp();
};

#ifndef _DEBUG  // debug version in SolarCellTesterView.cpp
inline CSolarCellTesterDoc* CSolarCellTesterView::GetDocument() const
   { return reinterpret_cast<CSolarCellTesterDoc*>(m_pDocument); }
#endif

