// SolarCellTesterView.cpp : implementation of the CSolarCellTesterView class
//

#include "stdafx.h"
#include "SolarCellTester.h"

#include "SolarCellTesterDoc.h"
#include "SolarCellTesterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolarCellTesterView

IMPLEMENT_DYNCREATE(CSolarCellTesterView, CView)

BEGIN_MESSAGE_MAP(CSolarCellTesterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_HELP, &CSolarCellTesterView::OnHelp)
END_MESSAGE_MAP()

// CSolarCellTesterView construction/destruction

CSolarCellTesterView::CSolarCellTesterView()
{
	// TODO: add construction code here

}

CSolarCellTesterView::~CSolarCellTesterView()
{
}

BOOL CSolarCellTesterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSolarCellTesterView drawing

void CSolarCellTesterView::OnDraw(CDC* pDC)
{
	CSolarCellTesterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
}


// CSolarCellTesterView printing

BOOL CSolarCellTesterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSolarCellTesterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSolarCellTesterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSolarCellTesterView diagnostics

#ifdef _DEBUG
void CSolarCellTesterView::AssertValid() const
{
	CView::AssertValid();
}

void CSolarCellTesterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSolarCellTesterDoc* CSolarCellTesterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSolarCellTesterDoc)));
	return (CSolarCellTesterDoc*)m_pDocument;
}
#endif //_DEBUG


// CSolarCellTesterView message handlers

void CSolarCellTesterView::OnHelp()
{
	USES_CONVERSION;
	CFileFind   filefind;
	CString sHelpFilePath;
	TCHAR *str= new TCHAR[200];
	GetCurrentDirectoryW(200, str);
	sHelpFilePath= W2T(str);
	sHelpFilePath += _T("\\Help\\Help.chm");
	if(!filefind.FindFile(sHelpFilePath))              // 文件不存在
	{
		CString warn;
		warn.LoadStringW(IDS_STR_OPEN_NOHELPFILE);
		AfxMessageBox(warn);
		return;
	}
	ShellExecuteW(NULL,_T("open"),sHelpFilePath,NULL,NULL,SW_SHOWMAXIMIZED);
}

