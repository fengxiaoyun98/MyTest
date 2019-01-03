// SolarCellTesterMainFrm.cpp : implementation of the CSolarCellTesterMainFrame class
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SolarCellTesterMainFrm.h"
#include "ChartView\ChartView.h"
#include "ChartView\ChartLineSerie.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolarCellTesterMainFrame

IMPLEMENT_DYNCREATE(CSolarCellTesterMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CSolarCellTesterMainFrame, CFrameWnd)
	ON_COMMAND(ID_FILE_CLOSE, &CSolarCellTesterMainFrame::OnFileClose)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_1,OnUpdatestatus)
	ON_MESSAGE( WM_CHECKMCUKEY, &CSolarCellTesterMainFrame::OnCheckMCUkey)
	ON_COMMAND(ID_SWITCH_CHARTVIEW, &CSolarCellTesterMainFrame::OnSwitchChartview)
	ON_COMMAND(ID_SWITCH_SETFONT, &CSolarCellTesterMainFrame::OnSwitchSetfont)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_ZOOMDISPLAY, &CSolarCellTesterMainFrame::OnViewZoomdisplay)
	ON_COMMAND(ID_32780, &CSolarCellTesterMainFrame::On32780)
	ON_COMMAND(ID_HELP, &CSolarCellTesterMainFrame::OnHelp)
END_MESSAGE_MAP()


// CSolarCellTesterMainFrame construction/destruction
static UINT indicators[] =
{
	ID_INDICATOR_1,
//	ID_SEPARATOR,           // 状态行指示器
// 	ID_INDICATOR_NUM,
//  ID_INDICATOR_CAPS,
//  ID_INDICATOR_SCRL,
};

CSolarCellTesterMainFrame::CSolarCellTesterMainFrame()
{
	// TODO: add member initialization code here
	pThreadMCUKeyCtrl = NULL;

}

CSolarCellTesterMainFrame::~CSolarCellTesterMainFrame()
{

}


BOOL CSolarCellTesterMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}


// CSolarCellTesterMainFrame diagnostics

#ifdef _DEBUG
void CSolarCellTesterMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CSolarCellTesterMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CSolarCellTesterMainFrame message handlers



BOOL CSolarCellTesterMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pMainWnd == NULL)
		pApp->m_pMainWnd = this;

	// replace Exit option on File menu with Close for secondary windows
	if (AfxGetApp()->m_pMainWnd != this)
	{
		CMenu *pMenu = GetMenu();
		ASSERT(pMenu);
		pMenu->ModifyMenu(ID_APP_EXIT, MF_BYCOMMAND | MF_STRING, ID_FILE_CLOSE, _T("&Close"));
	}

	return TRUE;
}

void CSolarCellTesterMainFrame::OnFileClose()
{
   DestroyWindow();
}

int CSolarCellTesterMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
   

	if (!m_wndSolarCellTesterCtrlBar.Create(this,CSolarCellTesterCtrlBar::IDD,WS_VISIBLE|WS_CHILD|CBRS_BORDER_3D|CBRS_SIZE_FIXED|CBRS_RIGHT,CSolarCellTesterCtrlBar::IDD))
	{
		TRACE0("Failed to create SolarCellTesterCtrlBar bar!");
		return -1;
	}

	if(!m_StatusBar.Create(this)|| !m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))  
	{  
		TRACE0("Failed to create status bar\n");  
		return false;  
	}  

	m_dispBar.Create(CDisplayInfo::IDD,this);
	m_dispBar.ShowWindow(SW_SHOW);

	m_zoom.Create(CDefLevel::IDD,this);
	m_zoom.ShowWindow(SW_SHOW);

	m_wndSolarCellTesterCtrlBar.EnableWindow(TRUE);     //变灰不可用  改动
	ShowWindow(SW_SHOWMAXIMIZED);
	//设置字体
	LOGFONT logfont;
	GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(logfont),&logfont); 
	logfont.lfCharSet=GB2312_CHARSET; 
	logfont.lfWeight = FW_NORMAL; 
	logfont.lfHeight = 24; 
	lstrcpy(logfont.lfFaceName, _T("微软雅黑"));
	static CFont sfontStatusBar;
	sfontStatusBar.CreateFontIndirect(&logfont);
	m_StatusBar.SetFont(&sfontStatusBar);


	m_StatusBar.m_szStatusString = _T("Gsolar Power Co.,Ltd.");
	m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 1, SBT_OWNERDRAW); 
	m_StatusBar.SetPaneInfo( 0,ID_INDICATOR_1, SBPS_NORMAL, 1000 );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_comm.Open( 2,9600,m_comm.EvenParity,8,m_comm.OneStopBit );
	if( !m_comm.IsOpen() )
	return 0;
	m_comm.ConfigCom();
	timerEvent = SetTimer( 16, 750, NULL );
	timerEventCLC = SetTimer(18, 1500, NULL);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//	SetTimer(1,10,0);
	return 0;
}
void CSolarCellTesterMainFrame::OnUpdatestatus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}
BOOL CSolarCellTesterMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_wndSolarCellTesterCtrlBar.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return    TRUE;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CSolarCellTesterMainFrame::OnSwitchChartview()
{
	CChartView* pV;
	pV=(CChartView*)GetActiveView();
	pV->RemoveAllSeries();
	return ;
	m_dispBar.SetMsgFont(16);
	CStringArray sta;
	sta.Add(_T("CStringArray sta;"));
	sta.Add(_T("sta.Add(_T(\"\"));"));
	sta.Add(_T("m_dispBar.DisplayMessage(&sta);"));
	sta.Add(_T("中华人民共和国"));
	sta.Add(_T("万岁！"));
	m_dispBar.DisplayMessage(&sta);

}
void CSolarCellTesterMainFrame::OnSwitchSetfont()
{
	CString str;
	CChartView* pV;
	pV=(CChartView*)GetActiveView();
	double x[10]={0,50,100,150,200,300,500,700};
	double y[10]={0,50,100,150,200,300,500,700};

	str=_T("Line 1");
	for(int i=0;i<10;i++)
	{
		CChartLineSerie* pIV = (CChartLineSerie*)pV->AddSerie(CChartSerie::stLineSerie);
		pIV->SetSeriesOrdering(CChartSerie::soNoOrdering);
			pIV->AddPoints(x,y,8);
		pIV->SetName(str);//
		pV->GetLegend()->DockLegend(CChartLegend::dsDockLeft);
	}
	pV->Invalidate();
}
//检测单片机按键启动测试
void CSolarCellTesterMainFrame::CheckMCUKey(void)
{
	CDocument* doc=this->GetActiveDocument();
	if(doc->IsKindOf(RUNTIME_CLASS(CSolarCellTesterDoc)))
		((CSolarCellTesterDoc*)doc)->WaitMcuKey();
}

LRESULT CSolarCellTesterMainFrame::OnCheckMCUkey(WPARAM wParam, LPARAM lParam)
{
	CheckMCUKey();
	return 1;
}
UINT ThreadMCUKeyCtrl(LPVOID lparam)    //改动
{
	UINT ret=0;
	while(1)
	{
		if(g_testing)
			continue;

		if(!theApp.m_opt.GetSetDev841()->GetExternSwitch()||!m_com.IsOpen())
			continue;

		if(0 == m_com.BytesWaiting())
			continue;

		byte recvbuf[10];
		memset(recvbuf, 0,(sizeof(byte)*10) );
		m_com.Read(recvbuf,10);
		if ((recvbuf[0] == 0xA5) 
			&& (recvbuf[1] == 0xA5) 
			&& (recvbuf[2] == 0xA5)
			&& (recvbuf[3] == 0x5A))
		{
			USHORT j = MAKEWORD(recvbuf[4],recvbuf[5]);

			if ((recvbuf[6] == 0xB1)             //判断子命令01是否返回相同的命令组合
				&& (recvbuf[7] == 0x01)
				&& j == 4)
			{
				//m_com.ClearALL();
				memset(recvbuf, 0,(sizeof(byte)*10) );
				ret++;
			}
		}
		if(ret==1)
		{
			ret=0;
			m_com.ClearALL();
			g_testing = true;
			AfxGetMainWnd()->SendMessageW(WM_COMMAND,IDC_BUTTON_START,0);
		}
	}
	return 0;
}
void CSolarCellTesterMainFrame::OnViewZoomdisplay()
{
	if(::IsWindowVisible(m_zoom.GetSafeHwnd()))
	{
		m_zoom.ShowWindow(SW_HIDE);
	}
	else
	{
		m_zoom.ShowWindow(SW_SHOW);
	}

}


void CSolarCellTesterMainFrame::On32780()
{
	CRect rc,rt;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	m_dispBar.GetWindowRect(&rt);
	m_dispBar.MoveWindow(rc.left,rc.top,rt.Width(),rt.Height());

	CRect   temprect(0,0,400,300);
	m_dispBar.SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);
}

void CSolarCellTesterMainFrame::OnHelp()
{
	USES_CONVERSION;
	CFileFind   filefind;
	CString sHelpFilePath;
	TCHAR *str= new TCHAR[200];
	GetCurrentDirectoryW(200, str);
	sHelpFilePath= W2T(str);
	delete []str;
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
void CSolarCellTesterMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 16:
		{
			switch(g_state)
			{
			case material:
				//if (m_comm.ReadPLC(0) == 65)
				{
					m_comm.WritePLC(2,45);
					g_state = sendgrade;
					//AfxGetMainWnd()->SendMessageW(WM_COMMAND,IDC_BUTTON_START,0);
				}
				break;		
			case sendgrade:
				m_comm.WritePLC(24,g_grade);
				g_state = testOK;
				break;
			case testOK:
				m_comm.WritePLC(1,66);
				g_state = 100;
				break;
			case Out_material:
				m_comm.WritePLC(58,33);
				g_state = 100;
				g_grade = 0;
				break;
			}
		}
		break;
	case 18:
		m_comm.WritePLC(57,1);
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CSolarCellTesterMainFrame::OnDestroy()
{
	if( timerEventCLC != 0 )
	{
		KillTimer( timerEventCLC );
	}
	if( timerEvent != 0 )
	{
		KillTimer( timerEvent );
	}
	if( pThreadMCUKeyCtrl != NULL )
	{
		pThreadMCUKeyCtrl->SuspendThread();
		pThreadMCUKeyCtrl->Delete();
		pThreadMCUKeyCtrl = NULL;
	}
	if(m_comm.IsOpen())
		m_comm.Close();
	if (m_com.IsOpen())
		m_com.Close();

	CloseHandle( dlgHandle );
	exit(1);
	CFrameWnd::OnDestroy();
}