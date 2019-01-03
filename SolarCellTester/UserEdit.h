#pragma once
#include "afxwin.h"
#include <vector>
/*
administration
*/
typedef std::vector<CString>VectorString;
class User
{
	CString name;
	CString password;
	//���ù����б�
	VectorString fnl;//class:function
	VectorString ftl;//˵����ǩ,������ʾ
public:
	enum SysCode{AUTOPASS=1,AUTONP,iMAXIM=5};
	//fname=classname.functionname
	User (){};
	CString GetSysName(SysCode code);
	CString GetSysTag(SysCode code);
	void MemerySet(SysCode code,CString fname,CString tag);
	void Addfnl(CString fname){fnl.push_back(fname);}
	void Addftl(CString tag){ftl.push_back(tag);}
	User(CString strn,CString strp){password=strp;name=strn;};
	VectorString &Getftl(void){return ftl;}
	VectorString &Getfnl(void){return fnl;}
	int GetftlID(CString ft){VectorString::iterator s=ftl.begin();int i=0;for(;s!=ftl.end();s++,i++)if(*s==ft)break;return i;}
	CString Getfnl(CString ft){VectorString::iterator s=fnl.begin()+GetftlID(ft);return *s;}
	void AddFunction(CString fname,CString tag);
	void SetPassword(CString str){password=str;};
	void SetName(CString str){name=str;};
	void SetUser(CString strn,CString strp){password=strp;name=strn;};
	CString GetName(void){return name;}
	CString GetPassword(void){return password;}
	BOOL IsDupName(CString strn){if(name==strn)return TRUE;return FALSE;};
	BOOL VerifyUser(CString strn,CString strp){	if(password==strp&&name==strn && (strn != _T("DataPassword"))) return TRUE;	return FALSE;};
	BOOL IsDisable(CString fname,CString tag=_T("")){
		if(fnl.empty())
			return FALSE;
		VectorString::iterator itr;
		if(!fname.IsEmpty()){
			for(itr=fnl.begin();itr!=fnl.end();itr++)
				if(!itr->IsEmpty())
				if(fname==(*itr))
					return TRUE;
		}
		if(!tag.IsEmpty()){
			for(itr=ftl.begin();itr!=ftl.end();itr++)
				if(!itr->IsEmpty())
				if(tag==*itr)
					return TRUE;
		}
		return FALSE;
	};
	void save(CFile &fn,CString pass);//����
};
typedef std::vector<User>UserList;

// CUserEdit dialog
class PowerManager
{
public:
	CString m_UserConfigFile;
	User m_reg;//�ɽ��ù��ܱ��Զ�ע���¼��ȱʡ�û������й��ܽ���
	UserList m_UserList;
	User m_sys;//����Ȩ��ϵͳ������
	User m_edit_act,*m_act;//��ǰ�û�
	//ȫ�������ļ����ݴ���
	enum PMCODE{
		ANAME=0x100,APASS,	
		UNAME=0x1000,UPASS,FNAME,FTAG
	};//���ݸ�ʽ����
public:
	CString GetDataPassword(void);
	void AddUser(User *us);
	void BackupAct(void);
	//�ɹ�����ע��,fname ��tag����Ψһ
	void ChangUser(void);//�л��û�
	void AssginPowerToUser(void);//�û�����
	void Login(void);//��½������������
	BOOL IsDisable(CString fname=_T(""),CString tag=_T("")){return m_act->IsDisable(fname,tag);}
	void RegFunction(CString fname,CString tag){m_reg.AddFunction(fname,tag);};
	PowerManager(void);
	~PowerManager(void);
	void SaveConfig(void);
	void LoadConfig(void);//װ���û�Ȩ�����ݲ���֤
};

class CUserEdit : public CDialog,public PowerManager
{
	DECLARE_DYNAMIC(CUserEdit)
public:
	void UpdateDisplay(void);
	CListBox m_Ldis;
	CListBox m_Lfunc;
	CListBox m_Luser;
	CString m_pw2;
	CString m_pw1;
	CString m_name;
	CUserEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_USERMGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedTodis();
	afx_msg void OnLbnDblclkLfunc();
	afx_msg void OnLbnSelchangeLuser();
	afx_msg void OnLbnDblclkLdis();
	afx_msg void OnLbnDblclkLuser();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedSaveexit();
	virtual BOOL OnInitDialog();
};
