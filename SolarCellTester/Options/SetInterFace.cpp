#include "StdAfx.h"
#include "..\stdafx.h"
#include "SetInterFace.h"

CSetInterFace::CSetInterFace(UINT nIDTemplate,CWnd* pParent )
	: CDialog(nIDTemplate, pParent)
{
	GetThisClassName();
	m_title=_T("Option Pages");
}
CString CSetInterFace::GetThisClassName(void)
{
	return CString(GetRuntimeClass()->m_lpszClassName);
}
void CSetInterFace::LoadReg(void)
{
}
void CSetInterFace::SaveReg(void)
{
}

CSetInterFace::~CSetInterFace(void)
{
}
//按键过滤，取消回车和ESCAPE默认推出动作
BOOL CSetInterFace::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN){
		if(pMsg->wParam==VK_ESCAPE){//Esc键不退出程序
			return   FALSE;
		}else if(pMsg->wParam==VK_RETURN){
			return FALSE;//对话框内部控件可以接收到回车消息!! 
			//return TRUE;//对话框内部控件不可以接收到回车消息!!
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetInterFace::Apply(WPARAM wParam)
{
	CString   strClass;   
	::GetClassName(GetFocus()->GetSafeHwnd(),strClass.GetBufferSetLength(255),255);   
	strClass.ReleaseBuffer(); 

	if ( strClass == L"Edit")                        // Edit,修改激活应用
	{
		if (HIWORD(wParam) == EN_CHANGE)
		{
			(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
		}

	}
	else if ( strClass == L"ComboBox")               // ComboBox,修改选择激活应用
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
		}

	}
	else if ( strClass == L"ListBox")                // ListBox,双击激活应用
	{
		if (HIWORD(wParam) == LBN_DBLCLK)
		{
			(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
		}
	}
	else
	{
		(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
	}
}