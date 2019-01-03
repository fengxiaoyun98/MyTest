#pragma once


// CSQLSet �Ի���

class CSQLSet : public CDialog
{
	DECLARE_DYNAMIC(CSQLSet)

public:
	CSQLSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSQLSet();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	CString Getserverip(void){ return m_serverip;}
	CString Getdatabase(void){return m_database;}
	CString Getusername(void){ return m_username;}
	CString Getpassword(void){return m_password;}
public:
	CString m_serverip;
	CString m_database;
	CString m_username;
	CString m_password;		
};
