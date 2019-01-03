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
//�������ˣ�ȡ���س���ESCAPEĬ���Ƴ�����
BOOL CSetInterFace::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN){
		if(pMsg->wParam==VK_ESCAPE){//Esc�����˳�����
			return   FALSE;
		}else if(pMsg->wParam==VK_RETURN){
			return FALSE;//�Ի����ڲ��ؼ����Խ��յ��س���Ϣ!! 
			//return TRUE;//�Ի����ڲ��ؼ������Խ��յ��س���Ϣ!!
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetInterFace::Apply(WPARAM wParam)
{
	CString   strClass;   
	::GetClassName(GetFocus()->GetSafeHwnd(),strClass.GetBufferSetLength(255),255);   
	strClass.ReleaseBuffer(); 

	if ( strClass == L"Edit")                        // Edit,�޸ļ���Ӧ��
	{
		if (HIWORD(wParam) == EN_CHANGE)
		{
			(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
		}

	}
	else if ( strClass == L"ComboBox")               // ComboBox,�޸�ѡ�񼤻�Ӧ��
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(1);
		}

	}
	else if ( strClass == L"ListBox")                // ListBox,˫������Ӧ��
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