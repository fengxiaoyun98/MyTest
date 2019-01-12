/*
设置IV曲线的数据生成
*/
#pragma once

#include "SetInterFace.h"
#include "afxwin.h"
#include "..\SunAds\SunPara.h"
#include <vector>
#include "SetBase.h"
// CSetCurveIV dialog

class IVParaList{
public:
	int id;
	TCHAR name[100];        //改动   字符串长度
public:
	IVParaList() { id = 0; memset(name, 0, 100*sizeof(TCHAR));};
};

class ZDYList              //改动  自定义
{
public:
	TCHAR fied[100];
	TCHAR data[100];
public:
	ZDYList()
	{
		memset(fied, 0, 100*sizeof(TCHAR));
		memset(data, 0, 100*sizeof(TCHAR));
	}
};
class CurveSet{
public:
	BOOL m_SQLserver;                        //朱峰改动

	BOOL m_swAuto;
	BOOL m_swSerial;
	BOOL m_swSNLength;
	BOOL m_swSerialBox;
	BOOL m_swSaveJpg;
	BOOL m_swSaveDaq;
	BOOL m_swSaveCaliDaq;
	BOOL m_swSavePara;
	BOOL m_swSaveXLS;
	BOOL m_swSaveParaMDB;
	BOOL m_swSerialUnique;
	BOOL m_swSaveInClass;
	BOOL m_swBackInput;
	BOOL m_fnstyle;
	BOOL m_swUpdateFnDate;
	BOOL m_DontClearInput;
	LONG m_auto;
	LONG m_TotalTest;
	TCHAR m_fileSavePath[255];
	TCHAR m_manuf[30];
	TCHAR m_ParaFileName[30];
	TCHAR m_InputBoxSerial[31];
	int  m_fCount;
	IVParaList  m_Tofile[50];
	int  m_dCount;
	IVParaList  m_ToDisplay[50];
	ZDYList m_usToDisplay[20];
public:
	double  m_area1;                            //改动
	double  m_areaeff1;
	BOOL	m_eorc;
public:           
	TCHAR m_serverip_this[50];                //朱峰改动
	TCHAR m_database_this[50];
	TCHAR m_username_this[50];     
	TCHAR m_password_this[50];
};//;CurveSet;

class CSetCurveIV:public CSetInterFace,public CurveSet        //改动  私有变公有
{
	DECLARE_DYNAMIC(CSetCurveIV)
private:
	BOOL m_state;//序列号对话框是ok或cancel
	BOOL m_swloop;
public:
	BOOL GetEnableLoop(void)const{return m_swloop;}
	ULONG TotalTest(void){return m_TotalTest;}
	void InputSerial(void);
	CString BuildParaFileName(CSunPara* plist);
	CString BuildParaClassFileName(CSunPara* plist);
	CString BuildSerial(CSunPara* plist);
	CString BuildDisplay(CSunPara* plist);
	CString BuildSaveTitle(CSunPara* plist);
	CString BuildSaveItem(CSunPara* plist);
	CString BuildField(CSunPara* plist,int type=0);
	CString BuildItem(CSunPara* plist);
	CString BuildFileName(CSunPara* plist);
	BOOL GetSwAuto(void)const{return m_swAuto;}
	BOOL GetSwSerial(void)const{return m_swSerial;}
	BOOL GetSwSNLength(void)const{return m_swSNLength;}
	BOOL GetSwSerialBox(void)const{return m_swSerialBox;}
	BOOL GetSwSaveJpg(void)const{return m_swSaveJpg;}
	BOOL GetSwSaveDaq(void)const{return m_swSaveDaq;}
	BOOL GetSwSaveCaliDaq(void)const{return m_swSaveCaliDaq;}
	BOOL GetSwSavePara(void)const{return m_swSavePara&&m_state;}
	BOOL GetSwSaveXLS(void)const{return m_swSaveXLS;}
	BOOL GetSwSaveMDB(void)const{return m_swSaveParaMDB;}
	BOOL GetSwSerialUnique(void)const{return m_swSerialUnique;}
	BOOL GetSwSaveInClass(void)const{return m_swSaveInClass;}
	BOOL GetSwBackInput(void)const{return m_swBackInput;}
	BOOL GetSwFnStyle(void)const{return m_fnstyle;}
	unsigned long  GetAuto(void)const{return m_auto;}
	CString GetManufacture() { CString str; str = m_manuf; return str; }

	CString SignificanceParam(int mdot,double value);

	BOOL GetSwSaveSQLserver(void)const{return m_SQLserver;}      //朱峰改动
	CString   GetSwSQLserverip(){CString str; str  = m_serverip_this;return str;} 
	CString   GetSwSQLdatabase(){CString str; str = m_database_this;return str;}
	CString   GetSwSQLusername(){CString str; str  = m_username_this;return str;}   
	CString   GetSwSQLpassword(){CString str; str  = m_password_this;return str;} 

	double GetArea1(void){return m_area1;}                               //改动
	void SetArea1(double area1_s){m_area1 = area1_s;}
	double GetAreaEff1(void){return m_areaeff1;}
	void SetAreaEff1(double areaeff1_s){m_areaeff1 = areaeff1_s;}
	BOOL  GetSweorc(void)const{return m_eorc;}

	enum PARALISTNO{OLEDATE=0,SERIAL};
public:
	CSetCurveIV(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetCurveIV();
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	class Pair{
	public:
		UINT id;
		CString name;
	};
	typedef std::vector<Pair>PairList;
// Dialog Data
	enum { IDD = IDD_DLGSET_CURVEIV };
	void LabelCSV(CSunPara* pList,CString fn);
	void LabelTXT(CSunPara* pList,CString fn);
private:
	bool ExsitPair(PairList* pl,Pair& p);
	PairList m_ParaPair,m_SavePair,m_DispPair;
	CString GetParaString(UINT id);
	CString GetParaString(CSunPara* pList,UINT id,int n=5);
	CString m_filepathkey;
	CString m_CurveSetkey;
	CString SaveDispKey;
	CString SaveDispCountKey;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CListBox m_ParaList;
	CDragListBox m_SaveList;                           //朱峰改动
	CDragListBox m_DispList;
	CString m_filepath;
	afx_msg void OnLbnDblclkList2();
	afx_msg void OnLbnDblclkList3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonSort();                                          //朱峰改动
	afx_msg void OnBnClickedButtonAddUsfied();                                          //朱峰改动 自定义
	afx_msg void OnBnClickedButtonDelUsfied();                                          //朱峰改动 自定义
	afx_msg void OnDestroy();
	afx_msg void OnradioC();
	afx_msg void OnradioE();
	afx_msg void OnFiedSelchange();                       //改动 自定义
	afx_msg void OnDataSelchange();                       //改动 自定义
	afx_msg void OnBnClickedCheckSQL();            //改动


protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CurveSet* GetAppliedConfig(void);
	void SetAppliedConfig(CurveSet* curveSet);
public:
	void  SetCurveIVDefault();                                            //朱峰改动            恢复默认
	////////////////////////自定义 改动////////////////////////////////////////
	CString m_usfied;      
	CString m_usdata;
	CComboBox m_usfiedB;
	CComboBox m_usdataB;
	CString GetvalueZDY(int n);
	CString GetTEXTZDY(int n);
	CString GetFormatZDY();
	//////////////////////////////////////////////////////////////////////////
};
