// SolarCellTesterMainFrm.h : interface of the CSolarCellTesterMainFrame class
//


#pragma once

#include "SolarCellTesterCtrlBar.h"
#include "DefLevel.h"
#include "DisplayInfo.h"
#include "ColoredStatusBarCtrl.h"

//#include "device/serialport.h"

UINT ThreadMCUKeyCtrl(LPVOID lparam);


class CSolarCellTesterMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CSolarCellTesterMainFrame();
	DECLARE_DYNCREATE(CSolarCellTesterMainFrame)

// Attributes
public:
	CDisplayInfo* GetInfoBox(void){return & m_dispBar;}
	CDisplayInfo m_dispBar;

	CDefLevel m_zoom;


	CSolarCellTesterCtrlBar m_wndSolarCellTesterCtrlBar;
	CColoredStatusBarCtrl m_StatusBar; 

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CSolarCellTesterMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnFileClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnSwitchChartview();
	afx_msg void OnSwitchSetfont();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewZoomdisplay();
	afx_msg void On32780();
	void CheckMCUKey(void);
	afx_msg LRESULT OnCheckMCUkey(WPARAM wParam, LPARAM lParam);   
	afx_msg void OnUpdatestatus(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
public:
	CSerialPort m_comm;
	SocketClient *pcomm;
	int timerEvent, timerEventCLC;

public:
	afx_msg void OnHelp();
};


