// SolarCellTesterDoc.cpp : implementation of the CSolarCellTesterDoc class
//

#include "stdafx.h"
#include "SolarCellTester.h"
#include "SolarCellTesterDoc.h"
#include "ChartView\ChartLineSerie.h"
#include "ChartView\ChartAxisLabel.h"
#include "SolarCellTesterMainFrm.h"
#include "math.h"
#include "Sqlsave.h"      //改动
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSolarCellTesterDoc

IMPLEMENT_DYNCREATE(CSolarCellTesterDoc, CDocument)

BEGIN_MESSAGE_MAP(CSolarCellTesterDoc, CDocument)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSolarCellTesterDoc::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS2, &CSolarCellTesterDoc::OnBnClickedButtonOptions2)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS3, &CSolarCellTesterDoc::OnBnClickedButtonOptions3)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS4, &CSolarCellTesterDoc::OnBnClickedButtonOptions4)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS5, &CSolarCellTesterDoc::OnBnClickedButtonAutoCacl)
	ON_COMMAND(ID_HELP_SELFCHECK, &CSolarCellTesterDoc::OnHelpSelfcheck)
	ON_COMMAND(ID_AUTH_CODE, &CSolarCellTesterDoc::OnAuthorization)
	ON_COMMAND(ID_AUTH_MIG, &CSolarCellTesterDoc::OnAuthorizationMig)

END_MESSAGE_MAP()

// CSolarCellTesterDoc construction/destruction

CSolarCellTesterDoc::CSolarCellTesterDoc()
/*:m_iv(410)*/
{
	// 测试功能权限分配
	m_cmd=0;
	m_ivcurve=NULL;
	m_setbase=&theApp.m_opt;
	m_setcal=m_setbase->GetSetSunPara();
	CString str;
	str.LoadString(IDS_CSolarCellTesterDocOnHelpSelfcheck);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnHelpSelfcheck",str);
	str.LoadString(IDS_CSolarCellTesterDocOnAuthorization );
	theApp.RegFunction(L"CSolarCellTesterDoc::OnAuthorization",str);
	str.LoadString(IDS_CSolarCellTesterDocOnBnClickedButtonOptions4);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnBnClickedButtonOptions4",str);
	str.LoadString(IDS_CSolarCellTesterDocOnBnClickedButtonOptions3);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnBnClickedButtonOptions3",str);
	str.LoadString(IDS_CSolarCellTesterDocOnBnClickedButtonOptions2);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnBnClickedButtonOptions2",str);
	str.LoadString(IDS_CSolarCellTesterDocOnBnClickedButtonStart);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnBnClickedButtonStart",str);
	str.LoadString(IDS_SetAutomaticCalibration);
	theApp.RegFunction(L"CSolarCellTesterDoc::OnBnClickedButtonAutoCacl",str);
}

CSolarCellTesterDoc::~CSolarCellTesterDoc()
{
}

BOOL CSolarCellTesterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// CSolarCellTesterDoc serialization

void CSolarCellTesterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSolarCellTesterDoc diagnostics

#ifdef _DEBUG
void CSolarCellTesterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSolarCellTesterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CSolarCellTesterDoc::InitChart()
{
	if(m_ivcurve!=NULL)
		return;

	POSITION pos = GetFirstViewPosition();
	CView* pv=GetNextView(pos);
	while(pv){
		if(pv->IsKindOf(RUNTIME_CLASS(CChartView)))
		{
			m_ivcurve=(CChartView*)pv;
			break;
		}
		pv=GetNextView(pos);
	}
}
//以下两个尽量不要使用
void CSolarCellTesterDoc::OnPreDraw(CDC* pDC)
{
	if(m_ivcurve==NULL)
		return;
}
void CSolarCellTesterDoc::OnEndDraw(CDC* pDC)
{
	if(m_ivcurve==NULL)
	{
		AfxGetMainWnd()->SetWindowText(_T("Solar Simulator"));
		return;
	}
}
void CSolarCellTesterDoc::OnBnClickedButtonAutoCacl()
{
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnBnClickedButtonAutoCacl"))
		return;
	do 
	{
		SingleIV();
		if (!m_setcal->m_reckonIsc || m_setcal->GetScaleIsc() == 1)
			m_setcal->OnBnClickedButtonCalcscaleisc();
		if (!m_setcal->m_reckonVoc || m_setcal->GetScaleVoc() == 1)
			m_setcal->OnBnClickedButtonCalcscalevoc();

		if (m_setcal->m_reckonIsc && m_setcal->m_reckonVoc)
		{
			if (!m_setcal->m_reckonPmax || m_setcal->GetScalePmax() == 1)
				m_setcal->OnBnClickedButtonCalcscalePmax();

			if (m_setcal->m_reckonPmax)
			{
				CString str;
				str.LoadString(IDS_STRCALI_167);
				AfxMessageBox(str);
				break;
			}
		}

		m_setbase->OnBnClickedSetbaseApply();
	} while (1);
}
void CSolarCellTesterDoc::OnBnClickedButtonStart()
{
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnBnClickedButtonStart"))
		return;

	g_testing = true;

	for(int i = 0; i < m_setbase->GetSetDev841()->m_testcnt; i++)
	{
		Iselfcheck  = false;
		SingleIV();
	}

	g_testing = false;

	if(m_iv.GetEnableLoop())
		AfxGetMainWnd()->PostMessage(WM_COMMAND,IDC_BUTTON_START,0);
}
//检测单片机按键事件，启动单片机测试数据
void CSolarCellTesterDoc::WaitMcuKey()
{
	if(m_sim.DevKeyTriggleCheck())
		SingleIV();
}

//强制重画屏幕
void ReDrawWnd(CWnd *pWnd)
{
	pWnd->InvalidateRect(0);
	//pWnd->Invalidate(FALSE);
	pWnd->UpdateWindow();
}
//单次测试
#include "TimerUS.h"
#include "SolarCellTesterMainFrm.h"
////////////////读外部数据///////////////////
#include <iostream>
#include <fstream>
#include <string>
void CSolarCellTesterDoc::SingleIV()
{
	TimerUS us;

	int isDAQSave;
	CString logtest(_T(""));
	isDAQSave = 0;
	m_cmd = 0;

	SetEnable(FALSE);
	InitChart();

	m_iv.InitialIV();
	m_irradiance.InitialIV();

	logtest = _T("Gsolar Power Co.,Ltd.");
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
	
	isDAQSave = m_iv.InputBoxSerialRun();

	if (0 == isDAQSave)        //测试计数，输入序列号,为空则不进行测试和保存
		return;
	else if (1 == isDAQSave)
		us.Reset();

	m_iv.SetSTime(m_setbase->GetSetDev841()->GetDelayTitle());
	m_iv.SetSDirection(m_setbase->GetSetDev841()->GetSweepDirectionTitle());
	
	m_irradiance.SetSerial(m_iv.GetSerial());
	logtest = m_iv.GetSerial() + _T("\t") + m_iv.GetSTime() +  _T("\t") + m_iv.GetSDirection();
	m_LogTrace.WriteLine(logtest);

	if (m_setbase->GetSetParaAlarm()->GetTModNOFlash())
	{
		m_sim.SurfTemp(&m_iv);
		if (m_iv.FilteringParaAlarmTmod())
		{
			m_ivcurve->UpdataParaDisplay(m_iv);
			m_ivcurve->RemoveAllSeries();
			ReDrawWnd(m_ivcurve);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.SetValue(0,0,0);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.Invalidate();
			g_grade = 13;
			g_state = material;
			return;
		}
	}

	if (1 == m_setbase->GetSetDev841()->GetSweepDirection())
	{
		if (m_setbase->GetSetDev841()->GetExternEffCell())
		{
			m_cmd = 64;
		}else
			m_cmd = 1;
		if (m_setbase->GetSetDev841()->GetExternV_I())
			m_cmd = 128;
		if (m_setbase->GetSetDev841()->GetExternV_I_Sec())
			m_cmd = 1;
	}
	if (0 == m_setbase->GetSetDev841()->GetSweepDirection())
	{
		if (m_setbase->GetSetDev841()->GetExternEffCell())
		{
			m_cmd = 32;
		}else
			m_cmd = 2;
	}

	INT_PTR ret = 1;
	ret=m_sim.GetARMDATA(&m_iv,&m_irradiance,0, m_cmd);

	logtest = _T("flash over!");
	m_LogTrace.WriteLine(logtest);

	if(ret==0)
	{
		logtest = _T("W0001:Failed To Get Data!");
		m_LogTrace.WriteLine(logtest);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.SetValue(0,0,0);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.Invalidate();
		m_iv.FilteringParaAlarm();

		g_grade = 13;
		g_state = material;

		if (us.GetLost() < (double)m_setbase->GetSetDev841()->GetInterval())
		{
			SingleSleep(((double)m_setbase->GetSetDev841()->GetInterval()-us.GetLost())*1000);
		}
		SetEnable(TRUE);
		return;
	}
	if (m_iv.m_setiv->GetSwSaveDaq())
	{
		m_iv.SaveDaq(_T(".daq"));   //保存原始数据
		m_irradiance.SaveDaq(_T("-irr.daq"));

		logtest = _T("save daq over!");       //mark 1129
		m_LogTrace.WriteLine(logtest);
	}

	///////////////////////voc-isc逆向的值翻转///////////////////////////////////
	if (1 == m_setbase->GetSetDev841()->GetSweepDirection())
	{
		double m_tempdata;
		int first = -1;
		int last = m_iv.GetDataSize();
		while(++first < --last)
		{
			m_tempdata = m_iv.x[first];
			m_iv.x[first] = m_iv.x[last];
			m_iv.x[last] = m_tempdata;

			m_tempdata = m_iv.y[first];
			m_iv.y[first] = m_iv.y[last];
			m_iv.y[last] = m_tempdata;
		}
	}

	/////////////////////////////////////////////////////////////////////////*/

	//处理IV数据异常
	int nIzeroNum = 0;
	for (int i = 0; i < m_iv.GetDataSize(); i++)
	{
		if (m_iv.y[i] < 0.000001)
		{
			nIzeroNum++;
		}
		if (nIzeroNum > 50)
		{
			logtest = _T("W0002:IV data anomaly,more than 50 of the current values are 0!");
			m_LogTrace.WriteLine(logtest);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
			m_ivcurve->UpdataParaDisplay(m_iv);
			m_ivcurve->RemoveAllSeries();
			ReDrawWnd(m_ivcurve);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.SetValue(0,0,0);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.Invalidate();
			m_iv.FilteringParaAlarm();
			g_grade = 13;
			g_state = material;
			if (us.GetLost() < (double)m_setbase->GetSetDev841()->GetInterval())
			{
				SingleSleep(((double)m_setbase->GetSetDev841()->GetInterval()-us.GetLost())*1000);
			}
			SetEnable(TRUE);
			return;
		}
	}
	//处理标准电池数据异常
	double m_irradiance_imax ,m_irradiance_imin, m_irradiance_stability ;
	m_irradiance_imax= 0.0;
	m_irradiance_imin= 0.0;
	if (m_irradiance.GetDataSize() != 0)
		m_irradiance_imin= m_irradiance.y[0];
	m_irradiance_stability = 0.0;
	for(int n = 0;n < m_irradiance.GetDataSize();n++)
	{
		if (m_irradiance_imax < m_irradiance.y[n])
			m_irradiance_imax = m_irradiance.y[n];
		if (m_irradiance_imin > m_irradiance.y[n])
			m_irradiance_imin = m_irradiance.y[n];
	}
	m_irradiance_stability = fabs((m_irradiance_imax-m_irradiance_imin)/(m_irradiance_imax+m_irradiance_imin));
	if(m_irradiance_stability > 0.01)
	{
		logtest = _T("W0003:Reference cell data anomaly!");
		m_LogTrace.WriteLine(logtest);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
	}

	////////////////////////标准电池温度修正////////////////////////////////*/
	double I1,I2;
	double a = 0.0005;
	if (m_iv.GetRCTemp() > -0.000001 && m_iv.GetRCTemp() < 0.000001)
	{
		logtest = _T("W0004:Reference cell temperature anomaly!");
		m_LogTrace.WriteLine(logtest);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
	}else
	{
		for (int irr_n = 0 ;irr_n < m_irradiance.GetDataSize() ;irr_n++)
		{
			I1=m_irradiance.y[irr_n];
			I2=m_irradiance.y[irr_n]=I1*(1+a*(25.0-m_iv.GetRCTemp()));
		}
	}

	double stdisc_sum=0;
	int nstart,nend;
	if (m_setbase->GetSetDev841()->GetExternEffCell())
	{
		nstart = 10;
		nend = 40;
	}else
	{
		nstart = 100;
		nend = 130;
	}

	for(int stdisc_n = nstart; stdisc_n < nend;stdisc_n++)
		stdisc_sum += m_irradiance.y[stdisc_n];

	m_iv.SetStdCellIscTesting(stdisc_sum/30);
	/////////////////////////////////////////////////////////////////////////*/
	logtest = _T("begin cal!");
	m_LogTrace.WriteLine(logtest);

	m_iv.CalSunPara();//修正和计算和存储
	
	logtest = _T("cal over!");
	m_LogTrace.WriteLine(logtest);
	//处理pmax为0的状况
	if (m_iv.GetPmax() > -0.000001 && m_iv.GetPmax() < 0.000001)
	{
		logtest = _T("W0005:The Pmax value is 0,please test again!");
		m_LogTrace.WriteLine(logtest);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
	}

	if(m_iv.m_setiv->GetSwSaveCaliDaq())
	{
		m_iv.SaveDaq(_T("-cali.daq"));   //保存校准后数据
		m_irradiance.SaveDaq(_T("-irr-cali.daq"));

		logtest = _T("save cal daq over!");
		m_LogTrace.WriteLine(logtest);
	}

	//////////////////辐照度曲线位置和长短处理//////////////////////////*/
	double m_yscale;                          
	m_yscale = m_iv.GetIsc()/m_iv.GetStdCellIscTesting();

	for (int irr_n = 0 ;irr_n < m_irradiance.GetDataSize() ;irr_n++)
	{
		m_irradiance.x[irr_n] = m_iv.x[irr_n];
		m_irradiance.y[irr_n] *= m_yscale/2;
	}
	m_irradiance.SetDataSize(m_iv.GetDataSize());	
	//////////////////////////////////////////////////////////////////////////
	m_ivcurve->RemoveAllSeries();
	m_ivcurve->AddIV(m_iv);
	m_ivcurve->AddPV(m_iv);
	if (m_setbase->GetSetChartView()->GetHideIRR())
		m_ivcurve->AddIV(m_irradiance,0);
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.SetValue(m_iv.GetPmax(),m_iv.GetIdot(),m_iv.GetEff());
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_zoom.Invalidate();

	if (theApp.m_opt.GetSetCurveIV()->GetSwBackInput())     // 先测后输入则在输入框之前显示测试结果
	{
		ReDrawWnd(m_ivcurve);
	}
	isDAQSave = m_iv.InputBoxSerialRun(1);
	if (0 == isDAQSave)//测试计数，输入序列号,为空则不进行测试和保存
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		ReDrawWnd(m_ivcurve);
		return;
	}else if (1 == isDAQSave)
		us.Reset();

	m_iv.SaveParaRun();//存参数,打标签,Class信息
	m_ivcurve->UpdataParaDisplay(m_iv);
	ReDrawWnd(m_ivcurve);
	SingleSleep(30);
	if (m_ivcurve->IsWindowVisible())
	{
		m_iv.SaveJpg(m_ivcurve);
	}
	else
	{
		m_iv.SaveJpg(m_ivcurve);
		AfxMessageBox(L"Visible == FALSE");
	}

	USES_CONVERSION;
	g_grade = atoi(T2A(m_iv.GetGrade()));
	if (m_iv.GetGrade().CompareNoCase(_T("NONAME")) == 0)
		g_grade = 13;
	g_state = material;

	if (us.GetLost() < (double)m_setbase->GetSetDev841()->GetInterval())
	{
		SingleSleep(((double)m_setbase->GetSetDev841()->GetInterval()-us.GetLost())*1000);
	}

	logtest.Format(_T("This test takes %f seconds."),us.GetLost());
	m_LogTrace.WriteLine(logtest);
	SetEnable(TRUE);
}
//光强曲线
void CSolarCellTesterDoc::SetVGetI()
{
	CString logtest(_T(""));
	SetEnable(FALSE);
	InitChart();
	m_iv.InitialIV();
	m_irradiance.InitialIV();

	logtest = _T("Gsolar Power Co.,Ltd.");
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 

	m_cmd = 256;

	logtest = _T("selfcheck Test");
	m_LogTrace.WriteLine(logtest);

	TimerUS us;
	us.Reset();

	INT_PTR ret=m_sim.GetARMDATA(&m_iv,&m_irradiance,0, m_cmd);
	if(ret==0)
	{
		logtest = _T("Failed to Get Data!");
		m_LogTrace.WriteLine(logtest);
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		return;
	}

	m_iv.SetX(0,1.0);//修改x轴

	double k=0,b=0;
	UINT s=5,n=400-2*s;
	m_iv.TransToLine(k,b,n,s);
	m_ivcurve->RemoveAllSeries();
	m_ivcurve->AddIV(m_iv,0);
	CString str1,str2,str3,strtemp;
	str1.Format(_T("K:%f,B=%f,(%d-%d)"),k,b,s,s+n);
	double avr=m_iv.GetAverage(0);
	double min=m_iv.GetMin(0);
	double max=m_iv.GetMax(0);
	str2.Format(L"MAX:%0.4fAVR:%0.4fMIN:%0.4f",max,avr,min);
	for(int i = 1;i < 400;i = i + 20)
	{
		strtemp.Format(L"Current[%d]:%0.4f\n",i,m_iv.y[i]);
		str3 += strtemp;
	}

	m_ivcurve->GetComment()->SetComment(str2+L"\n"+str1+L"\n"+str3);

	ReDrawWnd(m_ivcurve);
}

//弱光响应测试
void CSolarCellTesterDoc::OnBnClickedButtonOptions2()
{
	CString str_temp_filepath;
	CString str_open_filepath;

	if (m_iv.m_setiv->m_filepath.IsEmpty())
	{
		Getpath(str_temp_filepath);
		str_open_filepath = str_temp_filepath;
	}else
		str_open_filepath = m_iv.m_setiv->m_filepath;

	::ShellExecute( NULL, _T("open"), _T("explorer.exe"), str_open_filepath, NULL, SW_SHOWNORMAL ); 
	/*/
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnBnClickedButtonOptions2"))
		return;
	Iselfcheck  = false;
	SetEnable(FALSE);
	InitChart();
	m_iv.InitialIV();
	m_ivcurve->RemoveAllSeries ();
	USHORT sun=1200;
	USHORT i=0;
	CString comt,str;
	for(;i<6;i++)                   
	{
		INT_PTR ret=m_sim.GetIVAtSun(&m_iv,sun);             //改动新建线程并等待 WaitForSingleObject()
		if(ret==0)
		{
			m_ivcurve->UpdataParaDisplay(m_iv);
			m_ivcurve->RemoveAllSeries();
			ReDrawWnd(m_ivcurve);
			return;
		}
		m_iv.CalSunPara();//修正和计算和存储
		m_ivcurve->AddIV(m_iv);
		m_ivcurve->UpdataParaDisplay(m_iv);
		//ReDrawWnd(m_ivcurve);
		str.Format(_T("Sun:%.0f, Vmp:%.3f, Pmax:%.3f\n"),m_iv.GetSun(),m_iv.GetVmax(),m_iv.GetPmax());
		comt+=str;
		sun-=200;
		SingleSleep(1000);
	}
	m_ivcurve->GetComment()->SetComment(comt);
	ReDrawWnd(m_ivcurve);
	int isDAQSave;
	if (0 == (isDAQSave = m_iv.InputBoxSerialRun()))         //测试计数，输入序列号,为空则不进行测试和保存
		return;
	if (m_ivcurve->IsWindowVisible())
	{
		m_iv.SaveJpg(m_ivcurve);
	}
	else
	{
		m_iv.SaveJpg(m_ivcurve);
		AfxMessageBox(L"Visible == FALSE");
	}
	//*/
}
//双光强测量Ｒｓ
void CSolarCellTesterDoc::OnBnClickedButtonOptions3()
{
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnBnClickedButtonOptions3"))
		return;
	Iselfcheck  = false;
	SetEnable(FALSE);
	InitChart();
	m_iv.InitialIV();
	m_ivcurve->RemoveAllSeries ();
	CurveIV iv800(m_iv.size());

	if (0 == m_iv.InputBoxSerialRun())//测试计数，输入序列号,为空则不进行测试和保存
		return;

	INT_PTR ret=0;
	ret=m_sim.GetIVAtSun(&iv800,800);
	if(ret==0)
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		return;
	}
	iv800.CalSunPara();
	m_ivcurve->AddIV(iv800);
	//	ReDrawWnd(m_ivcurve);

	SingleSleep(1000);

	ret=m_sim.GetIVAtSun(&m_iv,1000);
	if(ret==0)
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		return;
	}
	m_iv.CalSunPara();
	m_iv.EnvalRs(&iv800);

	m_ivcurve->AddIV(m_iv);
	//	ReDrawWnd(m_ivcurve);
	if (0 == m_iv.InputBoxSerialRun(1))//测试计数，输入序列号,为空则不进行测试和保存
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		ReDrawWnd(m_ivcurve);
		return;
	}
	m_iv.SaveParaRun();//存参数,打标签,Class信息
	m_ivcurve->UpdataParaDisplay(m_iv);
	ReDrawWnd(m_ivcurve);
	if (m_ivcurve->IsWindowVisible())
	{
		m_iv.SaveJpg(m_ivcurve);
	}
	else
	{
		m_iv.SaveJpg(m_ivcurve);
		AfxMessageBox(L"Visible == FALSE");
	}
}
#include <conio.h>
//双曲线测试Pmax，第二次聚焦测试Pmax
void CSolarCellTesterDoc::OnBnClickedButtonOptions4()
{
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnBnClickedButtonOptions4"))
		return;
	Iselfcheck  = false;
	SetEnable(FALSE);
	InitChart();
	m_iv.InitialIV();
	m_ivcurve->RemoveAllSeries ();

	CurveIV ivmax(m_iv.size());
	if (0 == m_iv.InputBoxSerialRun())//测试计数，输入序列号,为空则不进行测试和保存
		return;
	INT_PTR ret=0;
	ret=m_sim.GetIVAtSun(&m_iv,1000);
	if(ret==0)
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		return;
	}
	UINT at;
	double pmax,vol;
	pmax=m_iv.GetMaxXY(at);
	vol=m_iv.GetX(at);
	ret=m_sim.GetIVAtPmax(&ivmax,1000,vol);

	if(ret==0)
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		m_ivcurve->RemoveAllSeries();
		ReDrawWnd(m_ivcurve);
		return;
	}
	m_iv.EnvalPmax(&ivmax,80);

	m_ivcurve->AddIV(ivmax);

	m_ivcurve->AddIV(m_iv);

	ReDrawWnd(m_ivcurve);
	if (0 == m_iv.InputBoxSerialRun(1))//测试计数，输入序列号,为空则不进行测试和保存
	{
		m_ivcurve->UpdataParaDisplay(m_iv);
		ReDrawWnd(m_ivcurve);
		return;
	}
	m_iv.SaveParaRun();//存参数,打标签,Class信息
	m_ivcurve->UpdataParaDisplay(m_iv);
	ReDrawWnd(m_ivcurve);
	if (m_ivcurve->IsWindowVisible())
	{
		m_iv.SaveJpg(m_ivcurve);
	}
	else
	{
		m_iv.SaveJpg(m_ivcurve);
		AfxMessageBox(L"Visible == FALSE");
	}
	//	m_iv.SaveJpg(m_ivcurve);
}
//
void CSolarCellTesterDoc::OnHelpSelfcheck()
{	
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnHelpSelfcheck"))
		return;
	m_iv.InitialIV();
	Iselfcheck = true;     //改动
	//校准和修正
	SetVGetI();
}
void CSolarCellTesterDoc::OnAuthorization()
{	
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnAuthorization"))
		return;
	CString str;
	str.LoadString(IDS_STRAUTHMESSAGE_168);

	if(MessageBox(NULL,str,_T("Message"),MB_YESNO) == IDYES)
	{
		m_encr_edit.SetCpuId(m_encr.GetCPUID());
		if (m_encr_edit.DoModal() == IDOK)
		{
			if (m_encr_edit.m_authcode.IsEmpty())
			{
				str.LoadString(IDS_STRAUTHMESSAGE_170);
				AfxMessageBox(str);
				return;
			}
			if (!m_encr.WriteRegitInfToBoard(m_encr_edit.m_authcode))
				return;
		}else
			return;
	}else
	{
		str.LoadString(IDS_STRAUTHMESSAGE_169);
		if (MessageBox(NULL,str,_T("Message"),MB_YESNO) == IDYES)
		{
			m_encr_edit.SetCpuId(m_encr.GetCPUID());
			if (m_encr_edit.DoModal() == IDOK)
			{
				if (m_encr_edit.m_authcode.IsEmpty())
				{
					str.LoadString(IDS_STRAUTHMESSAGE_170);
					AfxMessageBox(str);
					return;
				}
				if (!m_encr.WriteRegitInfToBoard(m_encr_edit.m_authcode))
					return;
			}else
				return;
		}else
			return;

	}

}
void CSolarCellTesterDoc::OnAuthorizationMig()
{
	if(theApp.IsDisable(L"CSolarCellTesterDoc::OnAuthorization"))
		return;
	m_auth_mig.DoModal();
}
void CSolarCellTesterDoc::SetEnable(BOOL fot)
{
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_START)->EnableWindow(fot); //改动
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS2)->EnableWindow(fot);//改动
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS3)->EnableWindow(fot);//改动
	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS4)->EnableWindow(fot);//改动
//	((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->EnableWindow(fot);//改动

}