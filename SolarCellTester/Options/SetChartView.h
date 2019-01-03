/*
图表设置选项，控制IV曲线显示
*/
#pragma once

#include "SetInterFace.h"
#include "afxwin.h"

class CChartView;
// CSetChartView dialog
typedef struct SetChartSet{
	double m_Imin,m_Imax;
	double m_Vmin,m_Vmax;
	double m_Pmin,m_Pmax;
	BOOL m_EnableXAuto;
	BOOL m_EnableYAuto;
	BOOL m_hideTitle;
	BOOL m_hideXTitle;
	BOOL m_hideYTitle;
	BOOL m_hideZTitle;
	BOOL m_hideXGrid;
	BOOL m_hideYGrid;
	BOOL m_hidePV;
	BOOL m_hideIRR;
	BOOL m_hideComm;
}SetChartSet;

class CSetChartView : public CSetInterFace,private SetChartSet
{
	DECLARE_DYNAMIC(CSetChartView)
public:
	void SetCustomView(CChartView* view){m_view=view;}
	CSetChartView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetChartView();

// Dialog Data
	enum { IDD = IDD_DLGSET_CHARTVIEW };
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();

protected:
//	CChartView* m_view;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CChartView* m_view;
	double GetXmin(void)const{return m_Vmin;}
	double GetXmax(void)const{return m_Vmax;}
	double GetYmin(void)const{return m_Imin;}
	double GetYmax(void)const{return m_Imax;}
	double GetZmin(void)const{return m_Pmin;}
	double GetZmax(void)const{return m_Pmax;}
	bool GetXAuto(void)const{return m_EnableXAuto==TRUE;}
	bool GetYAuto(void)const{return m_EnableYAuto==TRUE;}
	bool GetHideTitle(void)const{return m_hideTitle==FALSE;}
	bool GetHideXTitle(void)const{return m_hideXTitle==FALSE;}
	bool GetHideYTitle(void)const{return m_hideYTitle==FALSE;}
	bool GetHideZTitle(void)const{return m_hideZTitle==FALSE;}
	bool GetHideXGrid(void)const{return m_hideXGrid==FALSE;}
	bool GetHideYGrid(void)const{return m_hideYGrid==FALSE;}
	bool GetHidePV(void)const{return m_hidePV==FALSE;}   
	bool GetHideIRR(void)const{return m_hideIRR==FALSE;}     
	bool GetHideComm(void)const{return m_hideComm==FALSE;}
public:
	afx_msg void OnEnSetfocusZmin();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	SetChartSet* GetAppliedConfig(void);
	void SetAppliedConfig(SetChartSet* setCharSet);
public:
	void  SetChartViewDefault();                              //朱峰改动  恢复默认
};
