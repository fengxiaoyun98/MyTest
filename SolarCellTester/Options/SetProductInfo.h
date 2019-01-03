#pragma once
#include "..\resource.h"
// CSetProductInfo dialog
#define		MAX_NUM		400	

class CSetProductInfo : public CDialog
{
	DECLARE_DYNAMIC(CSetProductInfo)

public:
	CSetProductInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetProductInfo();

// Dialog Data
	enum { IDD = IDD_DLG_PRODUCTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_no;
	CString m_unit;
	float m_area;
	CString m_size;
	CString m_name;
	CString m_model;
	BOOL m_save_on;
	BOOL m_SwDlg;

	typedef struct Store
	{
	float data[MAX_NUM+1][2];//采集点数据结果
	TCHAR size[22];//规格
	TCHAR model[18];//型号
	TCHAR name[12];//测试人姓名
	TCHAR Serialno[16];//序号
	TCHAR Serial[34];//生产日期
	TCHAR unit[32];//生产厂家

	float xl;//效率
	float rate,ratex;//
	float ftemp;//温度
	float Sun;//光强
	float Reserved[10];//保留区
	}	LABER;
	LABER* pLaber;
public:
	afx_msg void OnBnClickedOk();
	void Save(CString name,CString value);
	CString Get(CString name);
	void SaveSet();
	void GetSet();
	virtual BOOL OnInitDialog();
};
