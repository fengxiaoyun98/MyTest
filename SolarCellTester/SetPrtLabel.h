#pragma once

#include "SetInterFace.h"
#include "afxwin.h"
#include "ActorPrint.h"
// CSetPrtLabel dialog

// 标签打印配置信息结构体
class ConPrtLabel
{
public:
	BOOL m_EnPrtLabel;         // 是否打印标签
	TCHAR m_sFilePath[255];    // 保存文件路径
	TCHAR m_sFileName[255];    // 保存文件名
	BOOL m_saveCSV;            // CSV文件保存选项
	BOOL m_saveTXT;            // TXT文件保存选项
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
	 
	void SetPrtLabelDefault();                                //朱峰改动         恢复默认

public:
	HWND m_hWnd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_print_str,m_print_quick_str;            //改动  识别中英文“打印”字体窗口

};
