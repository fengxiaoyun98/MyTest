// UserEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "UserEdit.h"
#include "Login.h"

//Ȩ��ϵͳ���ü�¼����
CString g_PowerSetName(L"PowerSystemConfigData");
//Ĭ�ϵ�ϵͳ���룬���ڼ���/���������ļ���һ����¼
CString g_password(L"Gsolar");
CString g_null(L"NULL");
CString g_Autopass(L"AUTOPASS");

void CoderString(void *buf,int len,CString &Pass)
{
	if(Pass.IsEmpty())
		Pass=g_password;
	int pl=Pass.GetLength()*sizeof(TCHAR);
	byte *p=new byte[pl];
	memcpy(p,Pass.GetBuffer(),pl);
	byte *b=(byte*)buf;
	int i=0;
	for(;i<len;i++){
		*b=*b^*(p+i%pl);
		b++;
	}
	delete[] p;
}

typedef struct USERCODEINFO{
	int code;
	int len;
}UInfo,*pUInfo;

//code�����ݴ��룬str��Ҫд���ַ�����Pass������
//���ݴ洢��ʽ��code(4byte)length(4byte)str(sizeof(str))
void fWriteString(CFile &fn,int code,CString &str,CString &pass)
{
	//��ֱ�Ӵ��ڴ��޸�CString������
	UInfo ui;
	ui.len=str.GetLength()*sizeof(TCHAR);
	byte *db=new byte[ui.len];
	memcpy(db,str.GetBuffer(),ui.len);//
	CoderString(db,ui.len,pass);
	ui.code=code;
	fn.Write(&ui,sizeof(UInfo));
	fn.Write(db,ui.len);
	delete[] db;
}
//
CString fReadString(CFile &fn,CString &pass,UInfo &ui,int blen=255)
{
	ui.len=ui.code=0;
	fn.Read(&ui,sizeof(UInfo));
	if(ui.len>blen||ui.len<=0||ui.code==0)
		return L"";
	byte *buf=new byte[ui.len];
	ZeroMemory(buf,ui.len);
	fn.Read(buf,ui.len);
	CoderString(buf,ui.len,pass);
	CString str((LPCTSTR)buf,ui.len/sizeof(TCHAR));
	delete[] buf;
	if(str.GetLength()==ui.len/sizeof(TCHAR))
		return str;
	return L"";
}

// CUserEdit dialog

IMPLEMENT_DYNAMIC(CUserEdit, CDialog)

CUserEdit::CUserEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CUserEdit::IDD, pParent)
	, m_pw2(_T(""))
	, m_pw1(_T(""))
	, m_name(_T(""))
{
}

CUserEdit::~CUserEdit()
{
}

void CUserEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LDIS, m_Ldis);
	DDX_Control(pDX, IDC_LFUNC, m_Lfunc);
	DDX_Control(pDX, IDC_LUSER, m_Luser);
	DDX_Text(pDX,IDC_PW2,m_pw2);
	DDX_Text(pDX,IDC_PW1,m_pw1);
	DDX_Text(pDX,IDC_NAME,m_name);
}


BEGIN_MESSAGE_MAP(CUserEdit, CDialog)
	ON_BN_CLICKED(ID_CLEAR, &CUserEdit::OnBnClickedClear)
	ON_BN_CLICKED(ID_TODIS, &CUserEdit::OnBnClickedTodis)
	ON_LBN_DBLCLK(IDC_LFUNC, &CUserEdit::OnLbnDblclkLfunc)
	ON_LBN_SELCHANGE(IDC_LUSER, &CUserEdit::OnLbnSelchangeLuser)
	ON_LBN_DBLCLK(IDC_LDIS, &CUserEdit::OnLbnDblclkLdis)
	ON_LBN_DBLCLK(IDC_LUSER, &CUserEdit::OnLbnDblclkLuser)
	ON_BN_CLICKED(ID_ADD, &CUserEdit::OnBnClickedAdd)
	ON_BN_CLICKED(ID_SAVEEXIT, &CUserEdit::OnBnClickedSaveexit)
END_MESSAGE_MAP()


// CUserEdit message handlers
//ȫ�������ֹ��
void CUserEdit::OnBnClickedClear()
{
	m_Ldis.ResetContent();
}
//�ѵ�ǰ��Ŀ���ӵ���ֹ��
void CUserEdit::OnBnClickedTodis()
{
	CString str;
	int id=m_Lfunc.GetCurSel();
	if(id<0)return;
	m_Lfunc.GetText(id,str);
	m_Ldis.AddString(str);
}

//˫�����ܱ���ӵ���ֹ��
void CUserEdit::OnLbnDblclkLfunc()
{
	OnBnClickedTodis();
}
//˫���ӽ�ֹ����ɾ��һ����Ŀ
void CUserEdit::OnLbnDblclkLdis()
{
	m_Ldis.DeleteString(m_Ldis.GetCurSel());
}

//���µ�ǰ�û���Ϣ���༭��
void CUserEdit::OnLbnSelchangeLuser()
{
	if(!m_Luser.GetCount())
		return;
	UINT id=m_Luser.GetCurSel();
	if(m_Luser.GetTextLen(id)==0)
		return;
	UserList::iterator itr=m_UserList.begin()+id;
	//������ʾ��Ϣ
	m_name=itr->GetName();
	m_pw1=m_pw2=itr->GetPassword();
	m_Ldis.ResetContent();
	VectorString s;
	s=itr->Getftl();
	VectorString::iterator its=s.begin();
	for(;its!=s.end();its++)
		m_Ldis.AddString(*its);
	this->UpdateData(FALSE);
}

//˫��ɾ��һ���û�,�б���������һһ��Ӧ
void CUserEdit::OnLbnDblclkLuser()
{
	UINT id=m_Luser.GetCurSel();
	if(m_Luser.GetTextLen(id)==0)
		return;
	m_Luser.DeleteString(id);
	m_UserList.erase(m_UserList.begin()+id);
}
//���������޸�һ���û�
void CUserEdit::OnBnClickedAdd()
{
	UpdateData();
	//���û������
	if(m_name.IsEmpty())
		return;
	//������֤
	if(m_pw1!=m_pw2){
		AfxMessageBox(IDS_REPEATPASSWORD,MB_OK);
		return;
	}
	if(m_pw1.IsEmpty()){
		AfxMessageBox(IDS_REPEATPASSWORD,MB_OK);
		return;
	}
	//�����û�
	User u(m_name,m_pw1);
	//��д��ִֹ�б�
	CString str;
	for(int i=0;i<m_Ldis.GetCount();i++){
		m_Ldis.GetText(i,str);
		if(str.IsEmpty())
			break;
		u.AddFunction(m_reg.Getfnl(str),str);
	}
	AddUser(&u);
	UpdateDisplay();
}
void PowerManager::BackupAct(void)
{
	//�༭֮ǰҪ������ǰ�û�����Ϣ,�п���ɾ����ǰ�û�
	if(m_edit_act.GetName().IsEmpty()){
		m_edit_act=*m_act;
		m_act=&m_edit_act;
	}
}

CString User::GetSysName(SysCode code)
{
	if(name!=g_PowerSetName||fnl.size()<User::iMAXIM)
		return CString(L"");
	if(fnl[code]!=g_null)
		return fnl[code];
	else
		return CString(L"");
}
CString User::GetSysTag(SysCode code)
{
	if(name!=g_PowerSetName||fnl.size()<User::iMAXIM)
		return CString(L"");
	if(ftl[code]!=g_null)
		return ftl[code];
	else
		return CString(L"");
}

//��������ר���ڼ�¼Ȩ��ϵͳ���õ�
void User::MemerySet(SysCode code,CString fname,CString tag)
{
	if(name!=g_PowerSetName)
		return;	
	fnl.resize(iMAXIM);
	ftl.resize(iMAXIM);
	VectorString::iterator itn;
	itn=fnl.begin();
	while(itn!=fnl.end()){
		if(itn->IsEmpty())
			*itn=g_null;
		itn++;
	};
	fnl[code]=fname;
	ftl[code]=tag;
}

void PowerManager::Login(void)
{
	LoadConfig();
	//����Ȩ��ϵͳ������m_sys
	//��һ��ܼ�¼���ϴε�½���û���������
	CLogin lg;
	if(m_sys.GetSysName(User::AUTOPASS)==g_Autopass){
		lg.SetNP(m_sys.GetSysName(User::AUTONP),m_sys.GetSysTag(User::AUTONP));
		lg.SetAuto(TRUE);
	}
	int tm=0;
	do{
		if(lg.DoModal()!=IDOK)
			break;
		if(lg.GetAuto())
			m_sys.MemerySet(User::AUTOPASS,g_Autopass,L"");
		else
			m_sys.MemerySet(User::AUTOPASS,g_null,L"");
		UserList::iterator itr=	m_UserList.begin();
		for(;itr!=m_UserList.end();itr++){
			if(itr->VerifyUser(lg.Name(),lg.Password())){
				m_act=&(*itr);
				m_sys.MemerySet(User::AUTONP,m_act->GetName(),m_act->GetPassword());
				SaveConfig();
				return;
			}
		}
	}while(tm++<3);
//	::ExitProcess(0);
	m_act=&m_reg;
}
#include "dirpath.h"
PowerManager::PowerManager(void)
{
	CDirPath dir;
	m_UserConfigFile=L"UserConfig.adm";
	m_UserConfigFile=dir.GetExePath()+m_UserConfigFile;
	//ȱʡ�û�
	m_reg.SetUser(L"Demo",L"");
	m_act=NULL;
}
void PowerManager::AddUser(User *us)
{
	//���û������
	CString name=us->GetName();
	if(name.IsEmpty())
		return;
	UserList::iterator fst,end;
	//�������,������ɾ�����ؽ�
	fst=m_UserList.begin();
	end=m_UserList.end();
	while(fst!=m_UserList.end())
		if(fst->IsDupName(name))
			fst=m_UserList.erase(fst);
		else 
			fst++;
	m_UserList.push_back(*us);
}
//�����û������ļ�
void PowerManager::LoadConfig(void)
{
	//��goto�����Щ�����Ҫ����ǰ��
	m_sys.SetName(g_PowerSetName);
	CFile fn;
	CString str,pass;
	User user,*us=NULL;
	BOOL bResult=fn.Open(m_UserConfigFile,CFile::modeRead|CFile::shareExclusive|CFile::typeBinary);
	if(!bResult){
		TRACE("Open error n");
		goto LOADOK;
	}
	fn.SeekToBegin();
	//�ļ�������֤
	ULONGLONG len=fn.GetLength();
	if(len<sizeof(UInfo))
		goto LOADOK;
	UInfo ui;
	//��ȡ����
	pass=fReadString(fn,g_password,ui);
	if(pass.IsEmpty()||(PowerManager::APASS)!=ui.code)
		goto LOADOK;
	int max=255;
	//��һ�����û�����
	while(1){//��ȡһ���û�
	//һ���û������־һ�����µ�UNAME������һ�����ļ�����
	do{
	str=fReadString(fn,pass,ui,max);
	switch(ui.code){
		case PowerManager::UNAME:
			if(str!=m_sys.GetName()){
				m_UserList.push_back(user);
				us=&m_UserList.back();
			}else us=&m_sys;
			us->SetName(str);
			break;
		case PowerManager::UPASS:us->SetPassword(str);break;
		case PowerManager::FNAME:us->Addfnl(str);break;
		case PowerManager::FTAG:us->Addftl(str);break;
		default:goto LOADOK;break;
	}
	if(fn.GetPosition()>=fn.GetLength())
		break;
	}while(1);
	};
LOADOK:
	//��֤���루��������͵�һ�û����룩
	if(!m_UserList.size()){//Ĭ�Ϲ���Ա��������ʱ
		User adm(g_password,g_password);
		m_UserList.push_back(adm);
		m_act=&m_UserList.front();
	}
	else if(pass!=m_UserList.front().GetPassword()){//�����ļ������벻һ��˵���ļ�����
		//PostQuitMessage(0);
		ExitProcess(NULL);
	}
	if(fn.m_hFile!=INVALID_HANDLE_VALUE)
		fn.Close();
	return;
}
void User::save(CFile &fn,CString pass)
{
	fWriteString(fn,PowerManager::UNAME,GetName(),pass);
	fWriteString(fn,PowerManager::UPASS,GetPassword(),pass);
	VectorString::iterator sn,st;
	sn=fnl.begin();
	st=ftl.begin();
	for(;sn!=fnl.end();){
		fWriteString(fn,PowerManager::FNAME,*sn,pass);
		fWriteString(fn,PowerManager::FTAG,*st,pass);
		sn++;
		st++;
	}
}
//�����û������ļ�
void PowerManager::SaveConfig(void)
{
	if(m_UserList.empty())
		return;
	CString pass=m_UserList.front().GetPassword();
	CFile fn;
	BOOL bResult=fn.Open(m_UserConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary);
	if(!bResult){
		TRACE("Open error n");
		return;
	}
	//��������,����Ϊ��һ���û�������
	//��������ȫ���������
	if(pass.IsEmpty())
		pass=g_password;
	fn.SeekToBegin();
	fWriteString(fn,PowerManager::APASS,pass,g_password);
	m_sys.save(fn,pass);
	//��һ�����û�����
	UserList::iterator itr=m_UserList.begin();
	for(;itr!=m_UserList.end();itr++)
		itr->save(fn,pass);
	fn.Close();
}
PowerManager::~PowerManager(void)
{	
}
//�л��û�,�˳�������
void PowerManager::ChangUser(void)
{
	TCHAR szAppName[MAX_PATH+10];
	memset(szAppName,0,sizeof(szAppName));
	:: GetModuleFileName(theApp.m_hInstance,szAppName, MAX_PATH);
	PROCESS_INFORMATION pt;
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	ZeroMemory(&pt,sizeof(pt));
	CreateProcess(szAppName,NULL, NULL, NULL,0,0,NULL,NULL,&si,&pt); 
	theApp.m_pMainWnd->PostMessage(WM_CLOSE);
}

CString PowerManager::GetDataPassword(void)
{
	CString dataname=L"DataPassword";
	//m_act->GetDataPassword(void)
	UserList::iterator i=m_UserList.begin();
	for(;i!=m_UserList.end();i++)
		if(i->GetName()==dataname)
			return i->GetPassword();
	return L"";
}
BOOL CUserEdit::OnInitDialog()
{
	__super::OnInitDialog();
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CUserEdit::UpdateDisplay(void)
{
	//�������ݵ���ʾ��
	//�ɹ����
	m_Lfunc.ResetContent();
	m_Luser.ResetContent();
	VectorString::iterator its=m_reg.Getftl().begin();
	for(;its!=m_reg.Getftl().end();its++)
		m_Lfunc.AddString(*its);
	//�û���
	CString str;
	UserList::iterator itr=m_UserList.begin();
	for(;itr!=m_UserList.end();itr++){
		str=itr->GetName();
		if(str.IsEmpty())break;//�п�˵���д�����
		m_Luser.AddString(str);
	}
	m_Luser.SetCurSel(0);
	OnLbnSelchangeLuser();
}


void CUserEdit::OnBnClickedSaveexit()
{
	SaveConfig();
}

void User::AddFunction(CString fname,CString tag)
{
	VectorString::iterator s=fnl.begin();
	for(;s!=fnl.end();s++)
		if((*s)==fname)//�������
			return;
	fnl.push_back(fname);
	ftl.push_back(tag);
};
