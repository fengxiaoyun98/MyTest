#pragma once


// CEncrEdit dialog

class CEncrEdit : public CDialog
{
	DECLARE_DYNAMIC(CEncrEdit)

public:
	CEncrEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEncrEdit();

// Dialog Data
	enum { IDD = IDD_DLG_ENCR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cpuid;
	CString m_authcode;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	void SetCpuId(CString cpuid){m_cpuid = cpuid;}
	
};
