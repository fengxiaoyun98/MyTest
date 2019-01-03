// Options\SetChartView.cpp : implementation file
//

#include "stdafx.h"
#include "..\stdafx.h"
#include "..\SolarCellTester.h"
#include "SetChartView.h"
#include "..\ChartView\ChartView.h"

// CSetChartView dialog

IMPLEMENT_DYNAMIC(CSetChartView, CDialog)

CSetChartView::CSetChartView(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetChartView::IDD, pParent)
{
	OpReg()->CreateKey(GetThisClassName());

	m_Imin=(0)
	, m_Vmin=(0)
	, m_Pmin=(0)
	, m_Imax=(10)
	, m_Vmax=(10)
	, m_Pmax=(10)
	, m_EnableXAuto=(FALSE)
	, m_EnableYAuto=(FALSE)
	, m_hideTitle=(FALSE)
	, m_hideXTitle=(FALSE)
	, m_hideYTitle=(FALSE)
	, m_hideZTitle=(FALSE)
	, m_hidePV=(FALSE)
	, m_hideIRR=(FALSE)
	, m_hideYGrid=(FALSE)
	, m_hideXGrid=(FALSE)
	, m_hideComm=(FALSE);
	m_view=NULL;

}

CSetChartView::~CSetChartView()
{
}

void CSetChartView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_XAUTO,m_EnableXAuto);
	DDX_Check(pDX,IDC_YAUTO,m_EnableYAuto);
	DDX_Check(pDX,IDC_HIDETITLE,m_hideTitle);
	DDX_Check(pDX,IDC_HIDEXTITLE,m_hideXTitle);
	DDX_Check(pDX,IDC_HIDEYTITLE,m_hideYTitle);
	DDX_Check(pDX,IDC_HIDEPV,m_hidePV);
	DDX_Check(pDX,IDC_HIDEIRR,m_hideIRR);
	DDX_Check(pDX,IDC_HIDEYGRID,m_hideYGrid);
	DDX_Check(pDX,IDC_HIDEXGRID,m_hideXGrid);
	DDX_Check(pDX,IDC_HIDECOMMENTS,m_hideComm);

	DDX_Text(pDX, IDC_XMIN, m_Vmin);
	DDX_Text(pDX, IDC_YMIN, m_Imin);
	DDX_Text(pDX, IDC_XMAX, m_Vmax);
	DDX_Text(pDX, IDC_YMAX, m_Imax);
}


BEGIN_MESSAGE_MAP(CSetChartView, CDialog)
	ON_EN_SETFOCUS(IDC_ZMIN, &CSetChartView::OnEnSetfocusZmin)
END_MESSAGE_MAP()

BOOL CSetChartView::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetChartViewSaveReg));
	theApp.RegFunction(L"CSetChartView::SaveReg",str);

	if(theApp.IsDisable(L"CSetChartView::SaveReg"))
		DisableAllSubCtrl(this);
//	LoadReg();
	return CDialog::OnInitDialog();
}


void CSetChartView::OnEnSetfocusZmin()
{
	UpdateData(1);
	m_Pmin=m_Vmin*m_Imin;
	m_Pmax=m_Imax*m_Vmax;
	UpdateData(0);
}

BOOL CSetChartView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
    Apply(wParam);    // 判断控件是否更改
	return __super::OnCommand(wParam, lParam);
}

void CSetChartView::LoadReg(void)
{
	/*	SetChartSet* set;
	set=this;
	OpReg()->Read(_T("SetChartSet"),(byte*)(set),sizeof(SetChartSet));
	if(m_view)
	m_view->ApplySet();
	*/

	SetChartSet *setChart=this;

	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo4.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive)){
		file.Read(setChart,sizeof(SetChartSet));
		file.Close();
	}
	else
		return;

	UpdateData(0);

}

void CSetChartView::SaveReg(void)
{
	/*	UpdateData();
	SetChartSet* set;
	set=this;
	OpReg()->Write(_T("SetChartSet"),(byte*)(set),sizeof(SetChartSet));
	if(m_view)
	m_view->ApplySet();*/

	UpdateData();
	SetChartSet *setChart=this;

	// 保存配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo4.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		file.Write(setChart, sizeof(SetChartSet));
		file.Close();
	}
}

SetChartSet* CSetChartView::GetAppliedConfig(void)
{
	UpdateData();
	SetChartSet *setChart=this;
    
	if(m_view)
	{
		m_view->ApplySet();
	}
	
	return setChart;
}

void CSetChartView::SetAppliedConfig(SetChartSet* setCharSet)
{
	SetChartSet *setChart = this;
	memcpy(setChart, setCharSet, sizeof(SetChartSet));

	if(m_view)
		m_view->ApplySet();

	UpdateData(0);

}

void CSetChartView::SetChartViewDefault()                                          //朱峰改动   恢复默认
{
		SetChartSet *default_setchatview = this;
			default_setchatview->m_Imin=(0);
			default_setchatview-> m_Vmin=(0);
			default_setchatview-> m_Pmin=(0);
			default_setchatview->m_Imax=(10);
			default_setchatview->m_Vmax=(10);
			default_setchatview-> m_Pmax=(10);
			default_setchatview-> m_EnableXAuto=(FALSE);
			default_setchatview-> m_EnableYAuto=(FALSE);
			default_setchatview-> m_hideTitle=(FALSE);
			default_setchatview-> m_hideXTitle=(FALSE);
			default_setchatview-> m_hideYTitle=(FALSE);
			default_setchatview-> m_hideZTitle=(FALSE);
			default_setchatview-> m_hidePV=(FALSE);
			default_setchatview-> m_hideIRR=(FALSE);
			default_setchatview-> m_hideYGrid=(FALSE);
			default_setchatview-> m_hideXGrid=(FALSE);
			default_setchatview-> m_hideComm=(FALSE);

			if(m_view)
				m_view->ApplySet();
		UpdateData(FALSE);
}