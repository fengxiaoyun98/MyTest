#pragma once

// CLogin dialog
class CPasswordEdit:public CEdit
{
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)
	CPasswordEdit m_edtPassword; 
	CString m_name;
	CString m_Password;
	BOOL m_autopass;
public:
	BOOL GetAuto(void){return m_autopass;}
	void SetAuto(BOOL a){m_autopass=a;}
	void SetNP(CString n,CString p){m_name=n;m_Password=p;};
	CString Name(void){return m_name;}
	CString Password(void){return m_Password;}
	CLogin(CString n=L"",CString p=L"",CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogin();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
