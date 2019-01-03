/*
配置841主板接口，完成数据采集、控制设备任务
*/
#pragma once
#include "afxwin.h"
#include "SetInterFace.h"
#include "..\AreaMapping.h"
// CSetDev841 dialog
// 测试计数类
/*#define  ALLFCNUMBER 16*/
class CTestCount
{
public:
	TCHAR m_sCntName1[30];
	TCHAR m_sCntName2[30];
	TCHAR m_sCntName3[30];

	ULONG m_cnt1;         
	ULONG m_cnt2;
	ULONG m_cnt3;
	ULONG m_cntTotal;
public:
	CTestCount()
	{
		m_cnt1 = 0;
		m_cnt2 = 0;
		m_cnt3 = 0;
		m_cntTotal = 0;
		_tcscpy_s(m_sCntName1,_T(""));
		_tcscpy_s(m_sCntName2,_T(""));
		_tcscpy_s(m_sCntName3,_T(""));
	}
};
// 测试设置类
class Reg841Para : public CTestCount{
public:
	BOOL m_externSwitch;
	BOOL m_exterV_I;
	BOOL m_exterV_I_Sec;
	BOOL m_eff_cell;
	UINT m_nocur;
	UINT m_novol;
	UINT m_nslen;
	UINT m_nsweepdirection;
	UINT m_set841sun;
	UINT m_Interval;
	BOOL m_CloseNx;
	ULONG m_Lmt1;         
	ULONG m_Lmt2;
	ULONG m_Lmt3;
	UINT m_model;
	BOOL m_DisCntAlarm;
public:
	Reg841Para(){
			m_set841sun=(1000);
			m_externSwitch=FALSE;
			m_exterV_I = FALSE;
			m_exterV_I_Sec = FALSE;
			m_eff_cell = FALSE;
			m_nocur=0;
			m_novol=0;
			m_nslen=0;
			m_nsweepdirection=0;
			m_Lmt1 = 0;
			m_Lmt2 = 0;
			m_Lmt3 = 0;
			m_Interval=5;//默认测试间隔0秒
			m_CloseNx=FALSE;
			m_model=0;
			m_DisCntAlarm=FALSE;
	}
};


using namespace std;
typedef std::pair<CString,UINT>Model;

class CSetDev841 : public CSetInterFace ,public Reg841Para
{ 
	DECLARE_DYNAMIC(CSetDev841)

public:
	CSetDev841(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDev841();
// Dialog Data
	enum { IDD = IDD_DLGSET_DEV841 };
	CDataExchange *pDXToVar,*pDXToCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void SetDevState(int st,CString str);
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();
	BOOL GetExternSwitch(void)const{return m_externSwitch;}
	void SetExternSwitch(BOOL sw){m_externSwitch=sw;}
	BOOL GetExternV_I(void)const{return m_exterV_I;}
	BOOL GetExternV_I_Sec(void)const{return m_exterV_I_Sec;}
	BOOL GetExternEffCell(void)const{return m_eff_cell;}
	UINT GetSun(void)const{return m_set841sun;}
	UINT GetInterval(void)const{return m_Interval;}
	UINT GetMcuSun(UINT sun=0);
	UINT GetDwCur(void){return m_gpi.GradeAt(m_nocur);}
	UINT GetDwVol(void){return m_gpv.GradeAt(m_novol);}
	double GetDwCurValue(void){return m_gpi.ScaleAt(m_nocur);}
	double GetDwVolValue(void){return m_gpv.ScaleAt(m_novol);}
	byte GetDelay(void){return m_slen.DelayAt(m_nslen);}
	CString GetDelayTitle(void){return m_slen.TitleAt(m_nslen);}
	UINT GetSweepDirection(void){return m_sdirection.DelayAt(m_nsweepdirection);}
	CString GetSweepDirectionTitle(void){return m_sdirection.TitleAt(m_nsweepdirection);}
	void Increment(void);
	void CheckLimit(void);
	BOOL GetCloseNx(void)const{return m_CloseNx;}
	ULONG GetTotalCount(void) { return m_cntTotal; }
	int GetPointCnt(void){return m_pointcnt;}
private://档位设置
	void resetcnt(int no);
	CComboBox m_modlist;
	CComboBox m_cbbvol;
	CComboBox m_cbbslen;
	CComboBox m_cbbcur;
	CComboBox m_SweepDirection;
	GradePairList m_gpv;
	GradePairList m_gpi;
	SunLenPairList m_slen;
	SunLenPairList m_sdirection;
    //光强设置
	AreaMapping m_ss;
	CEdit m_devstate;
	CEdit m_sunarea;
	CEdit m_EC841sun;
	CString m_sCName1;
	CString m_sCName2;
	CString m_sCName3;
	CString Reg841ParaKey;
	CString m_sTestCountKey;
	int m_pointcnt;
	std::vector<Model>m_mlist;
public:
	afx_msg void OnCbnSelchangeComboCurr();
	afx_msg void OnCbnSelchangeComboVolo();
	afx_msg void OnCbnSelchangeComboSLen();
	afx_msg void OnCbnSelchangeComboSweepDirection();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedVocIsc();
	afx_msg void OnBnClickedEffCell();
	afx_msg void OnBnClickedVocIscSec();


protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	Reg841Para* GetAppliedConfig(void);
	void SetAppliedConfig(Reg841Para* tReg841Para);
	void LoadCntReg(void);
	void SaveCntReg(void);
public:
	void SetDev841Default();                                  //朱峰改动    恢复默认函数
	int m_testcnt;
public:
	CString m_atype;
	CString m_atime;
// 	//v到i，i到v，sunsvoc，暗特性，光谱响应，高效i到v，高效v到i，智能接线，selfcheck，脉宽到150，反射系数
// 	enum ALLFC{VI_FC=0,IV_FC,SUNSVOC_FC,DARK_FC,QE_FC,EFF_IV_FC,EFF_VI_FC,SMART_VI_FC,SELFCHECK_FC,PULSE_TO150,REFLECTANCE};
// 	UCHAR m_fc[ALLFCNUMBER];
};
