/*
分档和报警设置对话框
*/
#pragma once
#include "SetInterFace.h"
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#include "..\SunAds\SunPara.h"
// CSetParaAlarm dialog

#define ALARMNUM 16             // 报警参数总数
#define CLASSNUM 20             // 分档总数

class GradeAlarm
{
public:
	BYTE m_number;
	TCHAR m_GAList[10][255];
};
class CSetParaAlarm : public CSetInterFace
{
	DECLARE_DYNAMIC(CSetParaAlarm)
public:
	BOOL GetDisableAlarm(void);
	BOOL GetErrorNoSave(void);
	BOOL GetNoDisplay(void);
	BOOL GetTModNOFlash(void);
	BOOL FilterSeter( CSunPara& sc,CStringArray* inf=NULL);
	BOOL FilterSeterTmod( CSunPara& sc,CStringArray* inf=NULL);

	int GetClassType(void)   { return m_pSet->type; }
	CSetParaAlarm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetParaAlarm();
	enum { IDD = IDD_DLGSET_PARAALARM };
private:
	virtual void Serialize(CArchive &ar);
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();
// Dialog Data
public:
	static const int VERFIRST=1000;
	enum FILEOP{SAVE=0,LOAD=1};
	enum SYS{SWALARM=0,SWDISPLAY,SWRING,SWNOSAVE,SWTMODNOFLASH};
	enum MINMAX{MIN=0,MAX=1,NAMELENGTH=10};
	enum ITEM{SURFTEMP=0,ISC,VOC,PM,FF,I1,EFF,IM,VM,RS,RSH,IREV,ENVTEMP,SUN};


	class AlarmItem{
	public:
		TCHAR name[NAMELENGTH];      //分档名
		double area[4][2];              //档位上下限      //改动  分档
		CString m_class[4];             //改动 分档
		CString m_Gname[4];
	};
	typedef std::vector<AlarmItem>AlarmList;
	class _Seter{
	public:
		UINT ver;                    //文件头中版本
		UINT size;                   //AlarmItem的数量
		int type;                    //分档依据类型
		BOOL sys[CLASSNUM];          //系统设置
		double item[ALARMNUM][2];          //报警参数上下限，每档位一组
		AlarmList list;              //AlarmItem列表，可以为零
	};
	typedef std::vector<_Seter>Seter;
private:
	CStringArray m_gaInfo;          //Grade&Alarm 信息
	Seter m_appset;                 //用于保存当前所有效的_Seter，和每个文件对应，和listbox对应用于管理
	_Seter *m_pSet;                 //保存一组实例用于存储和显示、交互制作
	Seter::iterator m_iSeter;       //当前配置实例，用于临时迭代
	AlarmList::iterator m_iItem;    //用于临时迭代
	AlarmList* m_pList;
	AlarmItem m_item;               //当前显示条目，用于交互制作
	CListBox m_listbox;
	CString m_filenamekey,m_filecountkey;
	CListBox m_filelist;
private:
	void DisplayInfo(CStringArray& gaInfo);
	AlarmItem* GradeDeciding( CSunPara& sc,_Seter& set,CStringArray& inf);
	BOOL AlarmDeciding( CSunPara& sc,_Seter& set,CStringArray& inf);
	BOOL AlarmDecidingTmod( CSunPara& sc,_Seter& set,CStringArray& inf);
	BOOL ApplayFilterSeter( CSunPara& sc,_Seter& set,CStringArray& inf);
	BOOL ApplayFilterSeterTmod( CSunPara& sc,_Seter& set,CStringArray& inf);
	BOOL SeterValidityCheck( AlarmList& list);
	BOOL AlarmItemValidityCheckNoBeep( AlarmItem& Item);
	BOOL AlarmItemValidityCheckNoBeep_arm();

	BOOL AlarmItemValidityCheck( AlarmItem& item);
	CString GetFileName(BOOL o);
	BOOL ConfigFileOperation(CString fn, BOOL o);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAppend();
	afx_msg void OnBnClickedUpdata();
	afx_msg void OnLbnSelchangeItemlist();
	afx_msg void OnLbnDblclkItemlist();
	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnDblclkFilelist();
	afx_msg void OnBnClickedModify();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	GradeAlarm GetAppliedConfig(void);
	void SetAppliedConfig(GradeAlarm* gradeAlarm);
public:
	void  SetParaAlarmDefault();                               //朱峰改动   恢复默认
	CDragListBox m_TypeList;                           //改动   分档
	double   ReGetWhichData(CString Bname,CSunPara& sc);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedGStyle();

};
