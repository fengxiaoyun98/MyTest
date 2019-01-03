#pragma once


// CInputBox dialog

class CInputBox : public CDialog
{
	DECLARE_DYNAMIC(CInputBox)

public:
	void SetString(CString str){m_string=str;}
	BOOL GetEnableLoop(void)const{return m_swloop;}
	void SetEnableLoop(BOOL sw){m_swloop=sw;}

	CString GetString(void){return m_string;}
	CInputBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputBox();
// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT };
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_swloop;
	CString m_string;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
