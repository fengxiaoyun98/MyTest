#pragma once
#include "Encryption/Encryption.h"       //º”√‹ ⁄»®¿‡

// CAuthTran dialog

class CAuthTran : public CDialog
{
	DECLARE_DYNAMIC(CAuthTran)

public:
	CAuthTran(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAuthTran();

// Dialog Data
	enum { IDD = IDD_DIALOG_AUTHTRAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	CEncryption m_encr_temp;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

public:
	CString m_str_message;
};
