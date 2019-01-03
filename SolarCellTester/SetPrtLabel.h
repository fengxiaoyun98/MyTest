#pragma once

#include "SetInterFace.h"
#include "afxwin.h"
#include "ActorPrint.h"
// CSetPrtLabel dialog

// ��ǩ��ӡ������Ϣ�ṹ��
class ConPrtLabel
{
public:
	BOOL m_EnPrtLabel;         // �Ƿ��ӡ��ǩ
	TCHAR m_sFilePath[255];    // �����ļ�·��
	TCHAR m_sFileName[255];    // �����ļ���
	BOOL m_saveCSV;            // CSV�ļ�����ѡ��
	BOOL m_saveTXT;            // TXT�ļ�����ѡ��
};

class CSetPrtLabel : public CSetInterFace
{
	DECLARE_DYNAMIC(CSetPrtLabel)

public:
	CSetPrtLabel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetPrtLabel();

// Dialog Data
	enum { IDD = IDD_DLGSET_LABEL };
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();
	BOOL GetEnPrt(void){return m_EnPrtLabel;}
	CString GetFullFilePath(void);
	void Print();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ActorPrint ActPrt;
	CListBox dispBox;
	BOOL m_EnPrtLabel;
	CString m_EnPrtLabelkey;
	CString m_title;
	CString m_filepath;
	CString m_filepathkey;
	CString m_filename;
	CString m_filenamekey;

	BOOL m_saveCSV;
	BOOL m_saveTXT;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton2();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	ConPrtLabel GetAppliedConfig(void);
	void SetAppliedConfig(ConPrtLabel *conPrtLabel);
	 
	void SetPrtLabelDefault();                                //���Ķ�         �ָ�Ĭ��

public:
	HWND m_hWnd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_print_str,m_print_quick_str;            //�Ķ�  ʶ����Ӣ�ġ���ӡ�����崰��

};
