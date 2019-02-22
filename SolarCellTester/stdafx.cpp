// stdafx.cpp : source file that includes just the standard includes
// SolarCellTester.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

double g_FFPrecScale=0;
CString g_ModbusTcpServerIP;
CString g_ModbusTcpPort;
bool  ReadMODBUSTCPConfig();
CSerialPort m_comm;
CWinThread      * pThreadPLCCtrl;
bool g_autoTest;

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

CString GetAppPath()
{
	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	::GetModuleFileName(NULL,chBuf,nBufSize);
	TCHAR* lpStrPath = chBuf;
	PathRemoveFileSpec(lpStrPath);
	CString pathname = lpStrPath;
	return pathname;
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

bool  ReadMODBUSTCPConfig()
{
	 SetCurExeDir();
	if(PathFileExists(_T("MODBUSTCP.gs")))
	{
		USES_CONVERSION;
		CString strPath=GetAppPath();
		strPath+="\\MODBUSTCP.gs";
		CString str;
		GetPrivateProfileString(L"Config",L"ServerIP",L"192.168.2.1",str.GetBuffer(MAX_PATH),MAX_PATH,strPath);
		g_ModbusTcpServerIP=str;
		GetPrivateProfileString(L"Config",L"Port",L"502",str.GetBuffer(MAX_PATH),MAX_PATH,strPath);
		g_ModbusTcpPort=str;
		return 1;
	}
	else
	{
		return 0;
	}
}