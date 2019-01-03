// SolarCellTester.h : main header file for the SolarCellTester application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//#include "vld.h"
#include "LogTrace.h"
#include "resource.h"       // main symbols
#include "Options\SetBase.h"
#include "UserEdit.h"
#include "SolarCellTesterDoc.h"
#include "SolarCellTesterView.h"
#include "DumpFile.h"
#include "Encryption/Encryption.h"       //加密授权类
// CSolarCellTesterApp:
// See SolarCellTester.cpp for the implementation of this class
//

#define  WM_CHECKMCUKEY				(WM_USER+1)

#define material     1000000        //是否有物料
#define Out_material 1000001        //出料
#define sendgrade    1000002        //发送档位
#define testOK       1000003        //测试完成


#define  ALLFCNUMBER 16

typedef struct UserConfig{
	CalibrationParaList Calibration;//用户名
	Reg841Para Acquisition;
	GradeAlarm DataFilter;
	SetChartSet ChartAxis;
	CurveSet SaveData;
	ConPrtLabel PrintLabel;
}USERCONFIG;

class CSolarCellTesterApp : public CWinApp
{
public:
	CSolarCellTesterApp();

protected:
	CMultiDocTemplate* m_pDocTemplate;
	CRegistry reg;
public:
	CRegistry* OpTheAppReg(void){return &reg;}
public:
	CString GetDataPassword(void){return m_um.GetDataPassword();}
	void RegFunction(CString fname,CString tag){m_um.RegFunction(fname,tag);};
	BOOL IsDisable(CString fname=_T(""),CString tag=_T("")){return m_um.IsDisable(fname,tag);}
	CSetBase* GetSetBase(void){return &m_opt;}
	CSetBase m_opt;
	CUserEdit m_um;
	CDocument* m_doc;

	CSolarCellTesterDoc *m_solarDoc;
	CSolarCellTesterView *m_solarView;

	USERCONFIG m_userConfig;
	CDumpFile m_dump;
	CEncryption m_encr;
	CString m_regnoshow;
// Overrides
public:
	virtual BOOL InitInstance();
// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnAppHelp();
	afx_msg void OnAppVideo();


	afx_msg void OnFileNewFrame();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitApplication();
	void SetOptRun(void);
	afx_msg void OnBnClickedButtonOptions();
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnAssginpower();
	afx_msg void OnChangeuser();
};

// extern double IAttenuatio;
extern bool   Iselfcheck;
extern CWinThread		* pThreadMCUKeyCtrl;	//脚踩控制线程
extern void SingleSleep(DWORD nMilliseconds);       //不阻塞MFC界面的延时函数  朱峰 2014-10-13
extern void Getpath(CString &szPath);
extern double g_irr_yscale;
extern CLogTrace m_LogTrace;
extern int g_grade;
extern int g_state;
extern bool g_testing;
extern CString g_strvalue;
extern HANDLE dlgHandle;

//v到i，i到v，sunsvoc，暗特性，光谱响应，高效i到v，高效v到i，智能接线，selfcheck，脉宽到150，反射系数
enum ALLFC{VI_FC=0,IV_FC,SUNSVOC_FC,DARK_FC,QE_FC,EFF_IV_FC,EFF_VI_FC,SMART_VI_FC,SELFCHECK_FC,PULSE_TO150,REFLECTANCE};
extern UCHAR m_fc[ALLFCNUMBER];


