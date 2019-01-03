/*
修正和校准对话框
*/
#pragma once

#include "SetInterFace.h"
#include "afxwin.h"

class CurveIV;

//typedef struct __CalibrationParaList{
class CalibrationParaList{
public:
	double m_area;
	double m_trueIsc;
	double m_curIsc;
	double m_scaleIsc;
	double m_preIsc;
	double m_scaleIsc1;
	double m_trueVoc;
	double m_curVoc;
	double m_scaleVoc;
	double m_preVoc;
	double m_scaleVoc1;
	double m_truePmax;
	double m_curPmax;
	double m_scalePmax;
	double m_prePmax;
	double m_trueTemp;
	double m_curTemp;
	double m_scaleTemp;
	double m_preTemp;
	double m_trueIRTemp;
	double m_curIRTemp;
	double m_scaleIRTemp;
	double m_preIRTemp;
	double m_StdCellIsc;
	double m_tempScaleCur;
	int    m_solarcount;
	double m_tempScaleVol;
	double m_ObjectTemp;
	double m_Vdot;
	BOOL m_swTempToObj;
	BOOL m_swIrrToObj;
	BOOL m_swDisableCali;
	BOOL m_swReflectScale;
	BOOL m_reckonIsc;
	BOOL m_reckonVoc;
	BOOL m_reckonPmax;
	BOOL m_reckonTemp;
	BOOL m_reckonIRTemp;
	int m_TempType;
	double m_areaEff;
};//CalibrationParaList;
// CSetSunPara dialog
class CSetSunPara :public CSetInterFace,public CalibrationParaList
{
	DECLARE_DYNAMIC(CSetSunPara)

public:
	CSetSunPara(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSunPara();
	virtual void LoadReg(void);
	virtual void SaveReg(void);
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DLGSET_SUNPARA };
public:
	void UpdataFromSunPara(CurveIV *piv);//Updata current Isc,Voc,Sun,Temp value
	BOOL GetDisableCaliSw(void){return m_swDisableCali;}
	BOOL GetReflectScaleSw(void){return m_swReflectScale;}
	BOOL GetTempToObjSw(void){return m_swTempToObj;}
	BOOL GetIrrToObjSw(void){return m_swIrrToObj;}
	double GetScaleVoc(void){return m_mirrorVocScale;}
	double GetScaleIsc(void){return m_mirrorIscScale;}
	double GetScalePmax(void){return m_mirrorPmaxScale;}

	double GetScaleIsc1(void){return m_scaleIsc1;}
	double GetScaleVoc1(void){return m_scaleVoc1;}

	double GetScaleTemp(void){return m_mirrorTempScale;}
	double GetScaleIRTemp(void){return m_mirrorIRTempScale;}
	double GetStdCellIsc(void){return m_StdCellIsc;}
	double GetObjectTemp(void){return m_ObjectTemp;}
	double GetScaleCurTemp(void){return m_tempScaleCur;}
	double GetScaleVolTemp(void){return m_tempScaleVol;}
	int	   GetSolarCount(void){return m_solarcount;}
	double GetArea(void){return m_area;}
	double GetAreaEff(void){return m_areaEff;}
	double GetVdot(void){return m_Vdot;}
	bool GetTempType(void){return m_TempType==0;}//==0为环境温度
    
	double GetTrueVoc(void){return m_trueVoc;}   //改动
	double GetTrueIsc(void){return m_trueIsc;}
public:
	void SetScaleVoc(double scalevoc){ m_mirrorVocScale = scalevoc;}
	void SetScaleIsc(double scaleIsc){ m_mirrorIscScale = scaleIsc;}
	void SetScalePmax(double scalePmax){ m_mirrorPmaxScale = scalePmax;}
	void SetScaleTemp(double scaleTemp){ m_mirrorTempScale = scaleTemp;}
	void SetScaleIRTemp(double scaleIRTemp){ m_mirrorIRTempScale = scaleIRTemp;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	double m_mirrorVocScale;
	double m_mirrorIscScale;
	double m_mirrorPmaxScale;
	double m_mirrorTempScale;
	double m_mirrorIRTempScale;
	CString CalibrationParaListKey;
public:
	afx_msg void OnBnClickedButtonCalcscaleisc();
	afx_msg void OnBnClickedButtonCalcscalevoc();
	afx_msg void OnBnClickedButtonCalcscalePmax();
	afx_msg void OnBnClickedButtonCalcscaletemp();
	afx_msg void OnBnClickedButtonCalcscaleIRtemp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckReflectScale(); 
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CalibrationParaList* GetAppliedConfig(void);
	void SetAppliedConfig(CalibrationParaList *caliList);
public:
	void SetSunParaDefault();               //改动
public:
	COLORREF m_colorEditText; // edit控件的字体颜色
	COLORREF m_colorEditBK;  // edit控件的背景颜色
	CBrush* m_pEditBkBrush;
};
