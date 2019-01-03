// SetPrtLabel.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SetPrtLabel.h"
#include "dirpath.h"


// CSetPrtLabel dialog

IMPLEMENT_DYNAMIC(CSetPrtLabel, CDialog)
CString g_csv=L".csv";
CSetPrtLabel::CSetPrtLabel(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetPrtLabel::IDD, pParent)
{
	m_EnPrtLabel=(FALSE);
	OpReg()->CreateKey(GetThisClassName());
	m_EnPrtLabelkey=_T("m_EnPrtLabel");
	m_title=L"BarTender";
	CDirPath path;
	m_filepath=path.GetExePath();
	m_filename=_T("Label");	
	m_filepathkey=_T("m_filepathkey");
	m_filenamekey=L"m_filenamekey";
	m_print_str = L"";            
	m_print_quick_str = L"";

	m_saveCSV = FALSE;
	m_saveTXT = FALSE;
}

CSetPrtLabel::~CSetPrtLabel()
{
}

void CSetPrtLabel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, dispBox);
	DDX_Check(pDX,IDC_CHECK1,m_EnPrtLabel);
	DDX_Text(pDX,IDC_EDIT_FILEPATH2,m_filepath);
	DDV_MaxChars(pDX,m_filepath,MAX_PATH);
	DDX_Text(pDX,IDC_EDIT_FILEPATH3,m_filename);
	DDV_MaxChars(pDX,m_filename,MAX_PATH);

	DDX_Check(pDX,IDC_CHECK_CSV,m_saveCSV);
	DDX_Check(pDX,IDC_CHECK_TXT,m_saveTXT);
}

BEGIN_MESSAGE_MAP(CSetPrtLabel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetPrtLabel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CSetPrtLabel::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetPrtLabel::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSetPrtLabel message handlers
//
void CSetPrtLabel::OnBnClickedButton1()
{
	ActPrt.SetTitle(m_title);
	HWND wnd=ActPrt.GetWnd();
	if(!ActPrt.IsWnd())
		return;
	TCHAR str[255];
	::GetWindowText(wnd,str,sizeof(str));
	ActPrt.SetTitle(str);
	if(!IsWindow(dispBox))
		return;
	dispBox.AddString(str);
	CString ids;
	ids.Format(_T("WinID:%X"),wnd);
	dispBox.AddString(ids);
}
void CSetPrtLabel::OnBnClickedButton7()
{
	ActPrt.ReWindow();
	//ActPrt.SendCmdMsg(1,8);        //打开文件
	ActPrt.Ctrl_P();
	SetTimer(0, 100, NULL);
}

void CSetPrtLabel::Print()
{
//	ActPrt.Print();

	ActPrt.ReWindow();
	//ActPrt.SendCmdMsg(6,3);        //更新数据
	//ActPrt.SendCmdMsg(1,8);        //打开文件
	ActPrt.Ctrl_P();
	SetTimer(1, 100, NULL);
}

BOOL CSetPrtLabel::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetPrtLabelSaveReg));
	theApp.RegFunction(L"CSetPrtLabel::SaveReg",str);

	if(theApp.IsDisable(L"CSetPrtLabel::SaveReg"))
		DisableAllSubCtrl(this);

	LANGID LangID=GetSystemDefaultLangID();
	if(PRIMARYLANGID(LangID)==LANG_CHINESE && SUBLANGID(LangID)==SUBLANG_CHINESE_SIMPLIFIED)
	{
		m_print_str = L"打印";
		m_print_quick_str = L"打印(&P)";
	}
	else if(PRIMARYLANGID(LangID)==LANG_ENGLISH)
	{
		m_print_str = L"Print";
		m_print_quick_str = L"Print(&P)";
	}  

//	LoadReg();
	if(m_EnPrtLabel){
		OnBnClickedButton1();
	}
	return CDialog::OnInitDialog();
}


void CSetPrtLabel::OnBnClickedButton2()
{
	BOOL ok=0;
	CDirPath dir;
	ok=dir.SelectFolder();
	UpdateData(1);         //改动

	if(ok!=0)
	{
		m_filepath=dir.GetSelectPath();
		UpdateData(0);
	}
}

CString CSetPrtLabel::GetFullFilePath(void)
{
	if(m_filepath.GetAt(m_filepath.GetLength())==L'\\')
		return m_filepath+m_filename;
	else
		return m_filepath+L"\\"+m_filename;
}

BOOL CSetPrtLabel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
    Apply(wParam);    // 判断控件是否更改
	return CSetInterFace::OnCommand(wParam, lParam);
}


void CSetPrtLabel::LoadReg(void)
{
	ConPrtLabel conPrtLabel;

	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo6.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive)){
		file.Read(&conPrtLabel,sizeof(conPrtLabel));
		file.Close();
	}
	else
		return;

	// 加载配置信息
	m_EnPrtLabel = conPrtLabel.m_EnPrtLabel;
	m_filepath = conPrtLabel.m_sFilePath;
	if( 0 == PathFileExists(m_filepath))
	{
		CDirPath d;
		m_filepath=d.GetExePath();
	}
	m_filename = conPrtLabel.m_sFileName;
	if ( m_filename.IsEmpty() || m_filename.GetLength()>MAX_PATH)
	{
		m_filename = _T("Label");
	}
	m_saveCSV = conPrtLabel.m_saveCSV;
	m_saveTXT = conPrtLabel.m_saveTXT;

	UpdateData(0);
}
void CSetPrtLabel::SaveReg(void)
{
	UpdateData();
	// 更新配置信息
	ConPrtLabel conPrtLabel;
	conPrtLabel.m_EnPrtLabel = m_EnPrtLabel;
	_tcscpy(conPrtLabel.m_sFilePath, m_filepath);
	_tcscpy(conPrtLabel.m_sFileName, m_filename);

	m_saveCSV = conPrtLabel.m_saveCSV;
	m_saveTXT = conPrtLabel.m_saveTXT;


	// 保存配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo6.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		file.Write(&conPrtLabel,sizeof(conPrtLabel));
		file.Close();
	}
}

ConPrtLabel CSetPrtLabel::GetAppliedConfig(void)
{
	UpdateData();
	// 更新配置信息
	ConPrtLabel conPrtLabel;
	conPrtLabel.m_EnPrtLabel = m_EnPrtLabel;
	_tcscpy(conPrtLabel.m_sFilePath, m_filepath);
	_tcscpy(conPrtLabel.m_sFileName, m_filename);

	conPrtLabel.m_saveCSV = m_saveCSV;
	conPrtLabel.m_saveTXT = m_saveTXT;

	return conPrtLabel;
}

void CSetPrtLabel::SetAppliedConfig(ConPrtLabel *conPrtLabel)
{
	// 加载配置信息
	m_EnPrtLabel = conPrtLabel->m_EnPrtLabel;
	m_filepath = conPrtLabel->m_sFilePath;
	if( 0 == PathFileExists(m_filepath))
	{
		CDirPath d;
		m_filepath=d.GetExePath();
	}
	m_filename = conPrtLabel->m_sFileName;
	if ( m_filename.IsEmpty() || m_filename.GetLength()>MAX_PATH)
	{
		m_filename = _T("Label");
	}

	m_saveCSV = conPrtLabel->m_saveCSV;
	m_saveTXT = conPrtLabel->m_saveTXT;

	UpdateData(0);
}

void CSetPrtLabel::SetPrtLabelDefault()                                            //朱峰改动  恢复默认
{
	m_EnPrtLabel=(FALSE);
	CDirPath d;
	m_filepath=d.GetExePath();
	m_filename = _T("Label");
	m_saveCSV = FALSE;
	m_saveTXT = FALSE;

	UpdateData(FALSE);
	
}

void CSetPrtLabel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CSetInterFace::OnTimer(nIDEvent);

	m_hWnd = ::FindWindow(NULL, m_print_str);//得到目标窗口句柄
	if ( m_hWnd != NULL ) 
	{
		HWND hButton;
		hButton = ::FindWindowEx(m_hWnd, NULL, L"button", m_print_str);

		::SendMessage(hButton, WM_LBUTTONDOWN, 0, 0);
		::SendMessage(hButton, WM_LBUTTONUP, 0, 0);

		KillTimer(nIDEvent);
	}
}