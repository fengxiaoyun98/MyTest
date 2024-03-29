// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//EXCEL
#include <afxdb.h> 
#include <odbcinst.h>

//头文件
#include "SolarCellTester.h"
#include "Options\SetBase.h"
#include "SolarCellTesterMainFrm.h"


//全局声明
extern CSolarCellTesterApp theApp;
extern CSerial m_com;
extern void DisableAllSubCtrl(CWnd *wnd);
extern void WriteUicodeBOM(CFile &f);
extern void SetCurExeDir();
extern bool ReadFFPercConfig();
extern double g_FFPrecScale;
extern CString g_ModbusTcpServerIP;
extern CString g_ModbusTcpPort;
extern bool  ReadMODBUSTCPConfig();
extern 	CSerialPort m_comm;
extern CWinThread      * pThreadPLCCtrl;
extern bool g_autoTest;

enum{THREAD_PLC_FREE=1000,THREAD_PLC_O_S,THREAD_PLC_R_P,THREAD_PLC_T_OK,THREAD_PLC_T_NG,THREAD_PLC_ALARMTIME,THREAD_PLC_R_C,THREAD_PLC_ERROR,THREAD_PLC_TESTING};
