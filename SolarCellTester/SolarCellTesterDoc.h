// SolarCellTesterDoc.h : interface of the CSolarCellTesterDoc class
//


#pragma once
#include "device\serial.h"
#include "DevSolarSimulator.h"
#include "SunAds\CurveIV.h"
#include "ChartView\ChartView.h"
#include "Options\SetDev841.h"
#include "Options\Setbase.h"

#include "Encryption/Encryption.h"       //º”√‹ ⁄»®¿‡
#include "EncrEdit.h"
#include "AuthTran.h"

class CSolarCellTesterDoc : public CDocument
{
protected: // create from serialization only
	CSolarCellTesterDoc();
	DECLARE_DYNCREATE(CSolarCellTesterDoc)

// Attributes
public:
	CChartView* m_ivcurve;
	DevSolarSimulator m_sim;
	MultiCurveIV m_MultiIV;
	CurveIV m_iv;
	CurveIV m_ivOriginal;
	CurveIV m_irradiance;
	CSetBase* m_setbase;
	CSetSunPara* m_setcal;
	USHORT m_cmd;

	int m_ThreadPLCSTA;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSolarCellTesterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetEnable(BOOL fot);
	void WaitMcuKey();
	void SingleIV();
	void SetVGetI();
	void InitChart(void);
	void OnPreDraw(CDC* pDC);
	void OnEndDraw(CDC* pDC);
	afx_msg void OnBnClickedButtonAutoCacl();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonOptions2();
	afx_msg void OnBnClickedButtonOptions3();
	afx_msg void OnBnClickedButtonOptions4();
	afx_msg void OnHelpSelfcheck();
	afx_msg void OnAuthorization();
	afx_msg void OnAuthorizationMig();
public:
	CEncryption m_encr;
	CEncrEdit   m_encr_edit;
	CAuthTran   m_auth_mig;
};


