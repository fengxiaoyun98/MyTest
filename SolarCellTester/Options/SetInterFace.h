/*
设置对话框的接口
*/
#pragma once
#include "Registry.h"
#include "..\resource.h"
#include "Options\SetBase.h"

class CSetInterFace:public CDialog
{
private:
	CString m_title;
	CRegistry reg;
public:
	CRegistry* OpReg(void){return &reg;}
	CSetInterFace(UINT nIDTemplate,CWnd* pParent =0);
	CString GetThisClassName(void);
	CString GetTitle(void) const{return m_title;}
	void SetTitle(CString title){m_title=title;}
	void Apply(WPARAM wParam);
public://SET对话框共有功能
	virtual ~CSetInterFace(void);
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog(){return CDialog::OnInitDialog();}
	virtual void Serialize(CArchive& ar){};
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
