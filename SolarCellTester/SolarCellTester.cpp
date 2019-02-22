// SolarCellTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SolarCellTesterMainFrm.h"

#include "SolarCellTesterDoc.h"
#include "SolarCellTesterView.h"
#include "ChartView\ChartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <io.h>
#include <fcntl.h>
//################################################################################控制台调用程序开始
	void InitConsoleWindow()
	{
		int nCrt = 0;
		FILE* fp;
		AllocConsole();
		nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		fp = _fdopen(nCrt, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
	}
//################################################################################控制台调用程序结束
// CSolarCellTesterApp

BEGIN_MESSAGE_MAP(CSolarCellTesterApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSolarCellTesterApp::OnAppAbout)
	ON_COMMAND(ID_APP_HELP, &CSolarCellTesterApp::OnAppHelp)
	ON_COMMAND(ID_APP_VIDEO, &CSolarCellTesterApp::OnAppVideo)

	ON_COMMAND(ID_FILE_NEW_FRAME, &CSolarCellTesterApp::OnFileNewFrame)
	ON_COMMAND(ID_FILE_NEW, &CSolarCellTesterApp::OnFileNew)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, &CSolarCellTesterApp::OnBnClickedButtonOptions)
	ON_COMMAND(ID_ASSGINPOWER, &CSolarCellTesterApp::OnAssginpower)
	ON_COMMAND(ID_CHANGEUSER, &CSolarCellTesterApp::OnChangeuser)
END_MESSAGE_MAP()


// CSolarCellTesterApp construction
CSolarCellTesterApp::CSolarCellTesterApp()
{
	m_solarDoc = NULL;
	m_solarView = NULL;
}


// The one and only CSolarCellTesterApp object

CSolarCellTesterApp theApp;
CLogTrace m_LogTrace;
CSerial m_com;
// double IAttenuatio;            //改动
bool   Iselfcheck;
CWinThread		* pThreadMCUKeyCtrl;	//脚踩控制线程
double g_irr_yscale;
int g_grade;
int g_state;
bool g_testing;
CString g_strvalue;
HANDLE dlgHandle;

UCHAR m_fc[ALLFCNUMBER];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

public:
	CString m_strReadMe;
	CString m_strtel;
	BOOL m_noshow;

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strReadMe(_T(""))
, m_strtel(_T(""))
, m_noshow(FALSE)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_README, m_strReadMe);
	DDX_Text(pDX, IDC_EDIT_TEL, m_strtel);
	DDX_Check(pDX, IDC_CHECK_NOSHOW, m_noshow);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str=L"";

	str=L"Tel:029-89150751-803\r\n";
	m_strtel += str;

	str=L"http:www.gsola.cn\r\n";
	m_strtel += str;

	str=L"(c)Gsolar Power Co.,Ltd.All rights reserved.";
	m_strtel += str;

	//使用必读
	HRSRC hRes = FindResource(theApp.m_hInstance, MAKEINTRESOURCE(IDR_TXT_README), _T("TXT"));    
	DWORD len  = SizeofResource(theApp.m_hInstance, hRes);  
	HGLOBAL hg = LoadResource(theApp.m_hInstance, hRes);  
	LPVOID lp = (LPSTR)LockResource(hg);   

	char * cp = (char *)lp;  
	USES_CONVERSION;  
	m_strReadMe = A2T(cp);
	FreeResource(hg); 

	theApp.OpTheAppReg()->Read(theApp.m_regnoshow,&m_noshow);


	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control

}
void CAboutDlg::OnDestroy()
{
	UpdateData(TRUE);

	if(m_noshow)
		theApp.OpTheAppReg()->Write(theApp.m_regnoshow,1);
	else
		theApp.OpTheAppReg()->Write(theApp.m_regnoshow,0);

	CDialog::OnDestroy();
}
// App command to run the dialog
void CSolarCellTesterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CSolarCellTesterApp::OnAppHelp()
{
	CString temp;  
	CString path;
	Getpath(path);
	temp = path + _T("document\\IV-help.chm");   
	::ShellExecute(NULL,_T("open"),temp,NULL,NULL,SW_SHOW);  
}

void CSolarCellTesterApp::OnAppVideo()
{
	CString temp;  
	CString path;
	Getpath(path);
	temp = path + _T("video");   
	::ShellExecute( NULL, _T("open"), _T("explorer.exe"), temp, NULL, SW_SHOWNORMAL );  
}
// CSolarCellTesterApp initialization
#include "InputBox.h"

BOOL CSolarCellTesterApp::InitInstance()
{
	CString strname;
	strname = _T("Gsola-11A");
	dlgHandle = ::CreateMutex( NULL, FALSE, strname);
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		AfxMessageBox(_T("The application has already existed!"));
		return FALSE; 
	}
	//CloseHandle( dlgHandle );

	CString file_logname(_T(""));
	file_logname = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d")) + _T("-log.txt");
	m_LogTrace.m_strAppName = _T("Gsolar11A+");
	m_LogTrace.SetFileName(file_logname);
	m_LogTrace.OnStartup(TRUE, TRUE);
	m_LogTrace.WriteLine(_T("Program already running"));	
	//加载用户配置文件并登陆
	m_um.Login();           //登录的同时告知禁用列表
	CString str(MAKEINTRESOURCE(IDS_CSolarCellTesterAppOnAssginpower));
	m_um.RegFunction(L"CSolarCellTesterApp::OnAssginpower",str);

	CWinApp::InitInstance();
    
	AfxOleInit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSolarCellTesterDoc),
		RUNTIME_CLASS(CSolarCellTesterMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CChartView));
	if (!pDocTemplate)
		return FALSE;
	m_pDocTemplate = pDocTemplate;
	AddDocTemplate(pDocTemplate);

/*
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSolarCellTesterDoc),
		RUNTIME_CLASS(CSolarCellTesterMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSolarCellTesterView));
	if (!pDocTemplate)
		return FALSE;
	m_pDocTemplate = pDocTemplate;
	AddDocTemplate(pDocTemplate);
*/

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
//	m_pMainWnd->UpdateWindow();
	AfxGetMainWnd()->SetWindowText(_T("Solar Simulator"));
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	m_um.Create(CUserEdit::IDD);

	memset(m_fc,0,sizeof(m_fc));
	g_strvalue = _T("");

	if(PathFileExists(_T("DEBUG.gs")))
	{
		InitConsoleWindow();
	}

	//一打开软件就读取功能码
	if( !m_com.AutoOpenPort(g_strvalue))
	{
		m_com.Close();
		m_LogTrace.WriteLine(_T("The motherboard is not detected!"));
	}else
	{
		if(g_strvalue.Mid(0,2) == _T("EC"))
		{
			USHORT L_read_fc;
			L_read_fc = m_encr.ReadFC();

			for (int i = 0; i < 16; i++)
			{
				m_fc[i] = L_read_fc >> i & 1;
			}
		}else
			AfxMessageBox(_T("E00B3:Lower machine recognition code is incorrect!"));
	}
	//初始化Options
	//登记可管理功能在OnInitDialog中,检查权限禁止控件
	m_opt.Create(CSetBase::IDD);

// 	IAttenuatio = 0;      //改动
	Iselfcheck  = false;
	g_irr_yscale = 0.0;
	g_grade = 0;
	g_state = 100;
	g_testing = false;

	m_dump.Initialize();
	m_regnoshow=_T("NoShowAboutXJCM11AZ");

#ifdef _DEBUG
	if (pThreadMCUKeyCtrl == NULL)
	{
		pThreadMCUKeyCtrl = AfxBeginThread(ThreadMCUKeyCtrl,NULL,0,0,0,0);
	}
#endif
	////////////////////about mark///////////////////////////
	BOOL noshow = 0;
	theApp.OpTheAppReg()->Read(theApp.m_regnoshow,&noshow);
	if (!noshow)
	{
		CAboutDlg aboutdlg;
		aboutdlg.DoModal();
	}
	return TRUE;
}


// CSolarCellTesterApp message handlers

void CSolarCellTesterApp::OnFileNewFrame() 
{
	ASSERT(m_pDocTemplate != NULL);

	CDocument* pDoc = NULL;
	CFrameWnd* pFrame = NULL;

	// Create a new instance of the document referenced
	// by the m_pDocTemplate member. 
	pDoc = m_pDocTemplate->CreateNewDocument();
	if (pDoc != NULL)
	{
		// If creation worked, use create a new frame for
		// that document.
		pFrame = m_pDocTemplate->CreateNewFrame(pDoc, NULL);
		if (pFrame != NULL)
		{
			// Set the title, and initialize the document.
			// If document initialization fails, clean-up
			// the frame window and document.

			m_pDocTemplate->SetDefaultTitle(pDoc);
			if (!pDoc->OnNewDocument())
			{
				pFrame->DestroyWindow();
				pFrame = NULL;
			}
			else
			{
				// Otherwise, update the frame
				m_pDocTemplate->InitialUpdateFrame(pFrame, pDoc, TRUE);
			}
		}
	}

	// If we failed, clean up the document and show a
	// message to the user.

	if (pFrame == NULL || pDoc == NULL)
	{
		delete pDoc;
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
	}
}

void CSolarCellTesterApp::OnFileNew() 
{
	CDocument* pDoc = NULL;
	CFrameWnd* pFrame;
	pFrame = DYNAMIC_DOWNCAST(CFrameWnd, CWnd::GetActiveWindow());
	
	if (pFrame != NULL)
		pDoc = pFrame->GetActiveDocument();

	if (pFrame == NULL || pDoc == NULL)
	{
		// if it's the first document, create as normal
		CWinApp::OnFileNew();
	}
	else
	{
		// Otherwise, see if we have to save modified, then
		// ask the document to reinitialize itself.
		if (!pDoc->SaveModified())
			return;

		CDocTemplate* pTemplate = pDoc->GetDocTemplate();
		ASSERT(pTemplate != NULL);

		pTemplate->SetDefaultTitle(pDoc);
		pDoc->OnNewDocument();
	}
}

BOOL CSolarCellTesterApp::InitApplication()
{
	//TODO: Add your specialized code here and/or call the base class
	//Log/Demo/Script
	//limits of authority,user
	//Options

	return CWinApp::InitApplication();
}

void CSolarCellTesterApp::SetOptRun(void)
{
	if(m_opt.GetSafeHwnd()==NULL)
		m_opt.Create(IDD_DLG_BASE);

	m_opt.AnimateWindow(100,AW_SLIDE|AW_HOR_NEGATIVE);
	m_opt.ShowWindow(SW_SHOW);

	//m_opt.FlashWindowEx(FLASHW_ALL ,10,500);
}

void CSolarCellTesterApp::OnBnClickedButtonOptions()
{
	// TODO: Add your control notification handler code here

	SetOptRun();
}
//检测下位机事件
//
#include "TimerUs.h"
#include "afxwin.h"
BOOL CSolarCellTesterApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
    //不如定时器
	static TimerUS us;
	if(!us.IsLost(1000000))//间隔
		return TRUE;
	us.Reset();
	// TODO: Add your specialized code here and/or call the base class
	if(m_doc==NULL){
	POSITION pos=m_pDocTemplate->GetFirstDocPosition();
	CDocument* doc=m_pDocTemplate->GetNextDoc(pos);
	while(doc){
		if(doc->IsKindOf(RUNTIME_CLASS(CSolarCellTesterDoc))){
			m_doc=doc;
			break;
		}
		doc=m_pDocTemplate->GetNextDoc(pos);
	}}

	//不需要(CSolarCellTesterDoc*)，下边语句可以执行
	((CSolarCellTesterDoc*)m_doc)->WaitMcuKey();

	return CWinApp::OnIdle(lCount);
}

void CSolarCellTesterApp::OnAssginpower()
{
	if(m_um.IsDisable(L"CSolarCellTesterApp::OnAssginpower"))
		return;
	m_um.BackupAct();
	m_um.UpdateDisplay();
	m_um.ShowWindow(SW_SHOW);
}

void CSolarCellTesterApp::OnChangeuser()
{
	m_um.ChangUser();
}

#pragma comment(lib, "version.lib")

CString GetThisFile(HMODULE hModule)
{
    TCHAR fname[MAX_PATH];
	CString str=L"";
    if (::GetModuleFileName(hModule, fname, MAX_PATH))
		str=fname;
	return str;
}
CString GetFileVersion(CString  FileName)
{
	int iVerInfoSize;
	TCHAR *pBuf;
	CString asVer=L"";
	VS_FIXEDFILEINFO *pVsInfo;
	unsigned int iFileInfoSize=sizeof(VS_FIXEDFILEINFO);
	iVerInfoSize=GetFileVersionInfoSize(FileName,NULL);
	if(iVerInfoSize!=0){
		pBuf=new TCHAR[iVerInfoSize];
		if(GetFileVersionInfo(FileName,0,iVerInfoSize,pBuf)){
			if(VerQueryValue(pBuf,L"\\",(void **)&pVsInfo,&iFileInfoSize)){
				asVer.Format(L"%d.%d.%d.%d",HIWORD(pVsInfo->dwFileVersionMS),LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),LOWORD(pVsInfo->dwFileVersionLS));
			}
		}
		delete []pBuf;
	}
	return asVer;
}    

CString GetModifyTime(CString appname)   
{   
    WIN32_FIND_DATA ffd ;   
    HANDLE hFind = FindFirstFile(appname,&ffd);   
    SYSTEMTIME stUTC, stLocal;   
    FileTimeToSystemTime(&(ffd.ftLastWriteTime), &stUTC);   
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);   
    CString strTmp;   
	strTmp.Format(L"%d-%d-%d,%d:%d:%d", stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond);   

	return strTmp;   
}  

void SingleSleep(DWORD nMilliseconds)
{
	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	LARGE_INTEGER liDuring;
	liDuring.QuadPart = - (__int64)nMilliseconds * 10000; //单位是100纳秒

	if (hTimer == NULL)
		RaiseException(EXCEPTION_INVALID_HANDLE, 0, 0, 0);

	SetWaitableTimer(hTimer, &liDuring, 0, 0, 0, 0);

	for(;;) {
		DWORD dwRet = MsgWaitForMultipleObjects(1, &hTimer, FALSE, INFINITE, QS_ALLEVENTS);
		if (dwRet == WAIT_OBJECT_0)
			break;
		else if (dwRet == WAIT_OBJECT_0 + 1) {
			MSG msg;
			while(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) != 0) {
				if (AfxGetThread()->PumpMessage() == 0) {
					AfxPostQuitMessage(0);
					break;
				}
			}
		}
	}

	CloseHandle(hTimer);

}
void Getpath(CString &szPath)
{
	TCHAR buf[256] = {0};  
	GetModuleFileName(NULL, buf, sizeof(buf)/sizeof(TCHAR));  
	szPath = buf;         
	szPath = szPath.Left(szPath.ReverseFind('\\') + 1); 
}