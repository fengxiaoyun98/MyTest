/*
����ҳ�������������öԻ���
*/
#pragma once

#include "SetInterFace.h"
#include "SetTabSheet.h"
#include "SetDev841.h"
#include "SetChartView.h"
#include "SetCurveIV.h"
#include "SetSunPara.h"
#include "SetParaAlarm.h"
#include "SetPrtLabel.h"
#include "Markup.h"

// CSetBase dialog
#define   GETVARNAME(var,namebuf,value) namebuf = #var; value = var             // ��ȡ�������ƺͱ���ֵ

class CSetBase : public CSetInterFace
{
	DECLARE_DYNAMIC(CSetBase)

	void Destroy(CDialog* dlg);
public:
	CSetBase(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetBase();
// Dialog Data
	enum { IDD = IDD_DLG_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CSetTabSheet m_sheet;

	CSetSunPara* m_sunpara;
	CSetDev841* m_841;
	CSetParaAlarm* m_alarm;
	CSetCurveIV* m_curve;
	CSetChartView* m_chart;
	CSetPrtLabel * m_PrtLabel;

	CString m_sConfigFileName;
	CString m_sConfigFileKey;              // �����ļ�ע�����Ϣ

public:
	CSetSunPara* GetSetSunPara(void)const{return m_sunpara;}
	CSetDev841* GetSetDev841(void)const{return m_841;}
	CSetParaAlarm* GetSetParaAlarm(void){return m_alarm;}
	CSetCurveIV* GetSetCurveIV(void){return m_curve;}
	CSetChartView* GetSetChartView(void){return m_chart;}
	CSetPrtLabel * GetSetPrtLabel(void){return m_PrtLabel;}
	void SaveUserConfig(CString filename);
	BOOL ReadUserConfig(CString filename);
	CString GetConfigFileKey(void) { return m_sConfigFileKey; };
    
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetbaseApply();
	afx_msg void OnBnClickedSetbaseDefault();                                  //�ָ�Ĭ��   ���Ķ�
	afx_msg void OnBnClickedApplyConfigFile();
	afx_msg void OnBnClickedBrowseConfigFile();
	afx_msg void OnEnChangeEditConfigfile();
};
