// stdafx.cpp : source file that includes just the standard includes
// SolarCellTester.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

double g_FFPrecScale=0;
//全局变量、函数声明
void SetCurExeDir()
{
	CString m_ExePath;
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	m_ExePath = CString(exeFullPath);
	m_ExePath = m_ExePath.Left(m_ExePath.ReverseFind(_T('\\')));
	if(m_ExePath.GetAt(m_ExePath.GetLength()-1)!=_T('\\'))m_ExePath+=_T("\\");
	SetCurrentDirectory(m_ExePath);
}

bool ReadFFPercConfig()
{
	 SetCurExeDir();
	if(PathFileExists(_T("FFPerc.gs")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}