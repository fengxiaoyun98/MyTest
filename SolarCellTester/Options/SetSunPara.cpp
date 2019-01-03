// Options\SetSunPara.cpp : implementation file
//
#include "stdafx.h"

#include "..\stdafx.h"
#include "..\SolarCellTester.h"
#include "SetSunPara.h"
#include "..\SunAds\CurveIV.h"

// CSetSunPara dialog

IMPLEMENT_DYNAMIC(CSetSunPara, CDialog)

BEGIN_MESSAGE_MAP(CSetSunPara, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CALCSCALEISC, &CSetSunPara::OnBnClickedButtonCalcscaleisc)
	ON_BN_CLICKED(IDC_BUTTON_CALCSCALEVOC, &CSetSunPara::OnBnClickedButtonCalcscalevoc)
	ON_BN_CLICKED(IDC_BUTTON_CALCSCALEPMAX, &CSetSunPara::OnBnClickedButtonCalcscalePmax)
	ON_BN_CLICKED(IDC_BUTTON_CALCSCALETEMP, &CSetSunPara::OnBnClickedButtonCalcscaletemp)
	ON_BN_CLICKED(IDC_BUTTON_CALCSCALEIRTEMP, &CSetSunPara::OnBnClickedButtonCalcscaleIRtemp)
	ON_BN_CLICKED(IDC_CHECK_REFLECT, &CSetSunPara::OnBnClickedCheckReflectScale) 
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CSetSunPara::CSetSunPara(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetSunPara::IDD, pParent)
{
	OpReg()->CreateKey(GetThisClassName());
	m_trueIsc=(0)
	, m_TempType=(1)
	, m_curIsc=(0)
	, m_scaleIsc=(1)
	, m_preIsc=(0.2)
	, m_scaleIsc1=(1)
	, m_trueVoc=(0)
	, m_curVoc=(0)
	, m_scaleVoc=(1)
	, m_preVoc=(0.2)
	, m_scaleVoc1=(1)
	, m_truePmax=(0)
	, m_curPmax=(0)
	, m_scalePmax=(1)
	, m_prePmax=(0.2)
	, m_trueTemp=(0)
	, m_curTemp=(0)
	, m_scaleTemp=(1)
	, m_preTemp=(0.2)
	, m_trueIRTemp=(0)
	, m_curIRTemp=(0)
	, m_scaleIRTemp=(1)
	, m_preIRTemp=(0.2)
	, m_StdCellIsc=(0)
	, m_tempScaleCur=(0.02/100)
	, m_tempScaleVol=(-0.0037)
	, m_solarcount=(0)
	, m_swTempToObj=(FALSE)
	, m_swIrrToObj=(FALSE)
	, m_ObjectTemp=(25)
	, m_swDisableCali=(FALSE)
	, m_swReflectScale=(FALSE)
	, m_reckonIsc = (TRUE)
	, m_reckonVoc = (TRUE)
	, m_reckonPmax = (TRUE)
	, m_reckonTemp = (TRUE)
	, m_reckonIRTemp = (TRUE)
	, m_Vdot=(0)
	, m_area=(0)
	, m_areaEff=(0);
	m_mirrorVocScale=1;
	m_mirrorIscScale=1;
	m_mirrorPmaxScale=1;
	m_mirrorTempScale=1;
	m_mirrorIRTempScale = 1;

	CalibrationParaListKey=_T("CalibrationParaList");

	m_colorEditText = RGB(0,0,0);
	m_colorEditBK = RGB(240,240,240);
	m_pEditBkBrush = new CBrush(RGB(240,240,240));
}
////
CSetSunPara::~CSetSunPara()
{
}

void CSetSunPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_EDIT_TRUEISC, m_trueIsc);
	DDV_MinMaxDouble(pDX, m_trueIsc, 0.0, 100.0);
	DDX_Text(pDX, IDC_EDIT_CURISC, m_curIsc);
	DDV_MinMaxDouble(pDX, m_curIsc, 0.0, 100.0);
	DDX_Text(pDX, IDC_EDIT_SCALEISC, m_mirrorIscScale);
	DDV_MinMaxDouble(pDX, m_mirrorIscScale, 0.5, 2.0);
	DDX_Text(pDX, IDC_EDIT_PREISC, m_preIsc);
	DDV_MinMaxDouble(pDX, m_preIsc, 0, 4);
	DDX_Text(pDX, IDC_EDIT_SCALEISC1, m_scaleIsc1);
	DDV_MinMaxDouble(pDX, m_scaleIsc1, 1.0, 1.3);

	DDX_Text(pDX, IDC_EDIT_TRUEVOC, m_trueVoc);
	DDV_MinMaxDouble(pDX, m_trueVoc, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT_CURVOC, m_curVoc);
	DDV_MinMaxDouble(pDX, m_curVoc, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT_SCALEVOC, m_mirrorVocScale);
	DDV_MinMaxDouble(pDX, m_mirrorVocScale, 0.5, 2.0);
	DDX_Text(pDX, IDC_EDIT_PREVOC, m_preVoc);
	DDV_MinMaxDouble(pDX, m_preVoc, 0, 4);
	DDX_Text(pDX, IDC_EDIT_SCALEVOC1, m_scaleVoc1);
	DDV_MinMaxDouble(pDX, m_scaleVoc1, 1.0, 1.3);

	DDX_Text(pDX, IDC_EDIT_TRUEPMAX, m_truePmax);
	DDV_MinMaxDouble(pDX, m_truePmax, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT_CURPMAX, m_curPmax);
	DDV_MinMaxDouble(pDX, m_curPmax, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT_SCALEPMAX, m_mirrorPmaxScale);
	DDV_MinMaxDouble(pDX, m_mirrorPmaxScale, 0.5, 2.0);
	DDX_Text(pDX, IDC_EDIT_PREPMAX, m_prePmax);
	DDV_MinMaxDouble(pDX, m_prePmax, 0, 4);

	DDX_Text(pDX, IDC_EDIT_TRUETEMP, m_trueTemp);
//	DDV_MinMaxDouble(pDX, m_trueTemp, -273, 300.0);
	DDX_Text(pDX, IDC_EDIT_CURTEMP, m_curTemp);
//	DDV_MinMaxDouble(pDX, m_curTemp, -273, 300.0);
	DDX_Text(pDX, IDC_EDIT_SCALETEMP, m_mirrorTempScale);
//	DDV_MinMaxDouble(pDX, m_mirrorTempScale, 0.5, 2.0);
	DDX_Text(pDX, IDC_EDIT_PRETEMP, m_preTemp);
//	DDV_MinMaxDouble(pDX, m_preTemp, 0, 4);

	DDX_Text(pDX, IDC_EDIT_TRUEIRTEMP, m_trueIRTemp);
	DDV_MinMaxDouble(pDX, m_trueIRTemp, -273, 300.0);
	DDX_Text(pDX, IDC_EDIT_CURIRTEMP, m_curIRTemp);
	DDV_MinMaxDouble(pDX, m_curIRTemp, -273, 300.0);
	DDX_Text(pDX, IDC_EDIT_SCALEIRTEMP, m_mirrorIRTempScale);
	DDV_MinMaxDouble(pDX, m_mirrorIRTempScale, 0.5, 2.0);
	DDX_Text(pDX, IDC_EDIT_PREIRTEMP, m_preIRTemp);
	DDV_MinMaxDouble(pDX, m_preIRTemp, 0, 4);

	DDX_Text(pDX, IDC_EDIT_STDCELLISC, m_StdCellIsc);
	DDV_MinMaxDouble(pDX, m_StdCellIsc, 0.0, 100);

	DDX_Text(pDX, IDC_EDIT_TEMPSCALECURR, m_tempScaleCur);
	DDV_MinMaxDouble(pDX, m_tempScaleCur, -1.0,1.0);
	DDX_Text(pDX, IDC_EDIT_TEMPSCALEVOLT, m_tempScaleVol);
	DDV_MinMaxDouble(pDX, m_tempScaleVol, -1.0, 1.0);
	DDX_Text(pDX, IDC_EDIT_SOLARCOUNT, m_solarcount);

	DDX_Text(pDX, IDC_EDIT_OBJECTTEMP, m_ObjectTemp);
	DDV_MinMaxDouble(pDX, m_ObjectTemp, -50.0, 100);


	DDX_Text(pDX, IDC_EDIT_AREA, m_area);
	DDV_MinMaxDouble(pDX, m_area, 0, 999999999);
	DDX_Text(pDX, IDC_EDIT_EFFAREA, m_areaEff);
	DDV_MinMaxDouble(pDX, m_areaEff, 0, 999999999);

	DDX_Text(pDX, IDC_EDIT_VDOT, m_Vdot);
	DDV_MinMaxDouble(pDX, m_Vdot, 0, 999999999);

	DDX_Check(pDX, IDC_CHECKTEMPCALI, m_swTempToObj);
	DDX_Check(pDX, IDC_CHECKIRR, m_swIrrToObj);

	DDX_Check(pDX, IDC_CHECKDISABLECALI, m_swDisableCali);

	DDX_Check(pDX, IDC_CHECK_REFLECT, m_swReflectScale);

	DDX_Radio(pDX,IDC_RADIO1,m_TempType);
}

void CSetSunPara::UpdataFromSunPara(CurveIV* piv)
{
	m_curIsc=piv->GetIsc();
	m_curVoc=piv->GetVoc();
	m_curPmax = piv->GetPmax();
	m_curTemp=piv->GetEnvTemp();
	m_curIRTemp=piv->GetSurfTemp();
	UpdateData(0);
}
HBRUSH CSetSunPara::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (pWnd->GetDlgCtrlID())
	{
	 case IDC_EDIT_CURISC:
		 if (m_reckonIsc)
		 {
			 m_colorEditText = RGB(0,0,0);

		 }else
		 {
			 m_colorEditText = RGB(255,0,0);

		 }
		 break;
	 case IDC_EDIT_CURVOC:
		 if (m_reckonVoc)
		 {
			 m_colorEditText = RGB(0,0,0);

		 }else
		 {
			 m_colorEditText = RGB(255,0,0);

		 }
		 break;
	 case IDC_EDIT_CURPMAX:
		 if (m_reckonPmax)
		 {
			 m_colorEditText = RGB(0,0,0);

		 }else
		 {
			 m_colorEditText = RGB(255,0,0);

		 }
		 break;
		
	 case IDC_EDIT_CURTEMP:
		 if (m_reckonTemp)
		 {
			 m_colorEditText = RGB(0,0,0);

		 }else
		 {
			 m_colorEditText = RGB(255,0,0);

		 }
		 break;
	 case IDC_EDIT_CURIRTEMP:
		 if (m_reckonIRTemp)
		 {
			 m_colorEditText = RGB(0,0,0);

		 }else
		 {
			 m_colorEditText = RGB(255,0,0);
		 }
		 break;

	 default:
		 return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	pDC->SetTextColor(m_colorEditText);
	pDC->SetBkColor(m_colorEditBK);
	return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
}
void CSetSunPara::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	delete m_pEditBkBrush;

}
void DisableAllSubCtrl(CWnd *wnd)
{
	CWnd* pWnd = wnd->GetWindow(GW_CHILD);
	TCHAR str[20];

	while(pWnd){
		::GetClassName(pWnd->GetSafeHwnd(),str,10);
		if(CString(str)!=CString(L"Static")){
			LONG style=GetWindowLong(pWnd->GetSafeHwnd(),GWL_STYLE);
			if((style&BS_GROUPBOX)!=BS_GROUPBOX)
				pWnd->EnableWindow(FALSE);
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

BOOL CSetSunPara::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetSunParaSaveReg));
	theApp.RegFunction(L"CSetSunPara::SaveReg",str);
	str.LoadString(IDS_SetCurePmax);
	theApp.RegFunction(L"CSetSunPara::SetCurePmax",str);

	str.LoadString(IDS_SetTemperatureCalibration);
	theApp.RegFunction(L"CSetSunPara::SetCureTemp",str);
	str.LoadString(IDS_SetTemperatureCompensation);
	theApp.RegFunction(L"CSetSunPara::SetIVtemp",str);
	str.LoadString(IDS_SetCalibrationYOrN);
	theApp.RegFunction(L"CSetSunPara::SetDiscala",str);


	//禁止所有控件
	if(theApp.IsDisable(L"CSetSunPara::SaveReg"))
		DisableAllSubCtrl(this);
	if (theApp.IsDisable(L"CSetSunPara::SetCurePmax"))
	{
		GetDlgItem(IDC_BUTTON_CALCSCALEPMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRUEPMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CURPMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALEPMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PREPMAX)->EnableWindow(FALSE);
	}
	if (theApp.IsDisable(L"CSetSunPara::SetCureTemp"))
	{
		GetDlgItem(IDC_BUTTON_CALCSCALEIRTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRUEIRTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CURIRTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCALEIRTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PREIRTEMP)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_CALCSCALETEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRUETEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CURTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCALETEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRETEMP)->EnableWindow(TRUE);
	}
	if (theApp.IsDisable(L"CSetSunPara::SetIVtemp"))
	{
		GetDlgItem(IDC_EDIT_TEMPSCALECURR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEMPSCALEVOLT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SOLARCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OBJECTTEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECKTEMPCALI)->EnableWindow(FALSE);
	}
	if (theApp.IsDisable(L"CSetSunPara::SetDiscala"))
		GetDlgItem(IDC_CHECKDISABLECALI)->EnableWindow(FALSE);
	CDialog::OnInitDialog();
//	LoadReg();
	return 0;
}

void CSetSunPara::OnBnClickedCheckReflectScale()
{
	UpdateData(TRUE);
	if (m_swReflectScale && m_fc[REFLECTANCE])
	{
		if (m_reckonIsc && m_reckonVoc)
		{
			GetDlgItem(IDC_BUTTON_CALCSCALEISC)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CALCSCALEVOC)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(TRUE);
		}else
		{
			m_swReflectScale = FALSE;
			AfxMessageBox(_T("请先校准到容差范围!"));
		}

	}else
	{
		GetDlgItem(IDC_BUTTON_CALCSCALEISC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALCSCALEVOC)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);

}

void CSetSunPara::OnBnClickedButtonCalcscaleisc()
{
	UpdateData();
	double m_temppre = 0;
	m_temppre = m_preIsc*0.01;
	m_mirrorIscScale=m_scaleIsc;

	if (m_curIsc < 0 || m_curIsc > 100)
	{
        m_curIsc = 0;
	}
	if(m_curIsc!=0)
	if(m_trueIsc!=0){
		m_mirrorIscScale*=m_trueIsc/m_curIsc;
		if(m_mirrorIscScale>1.5||m_mirrorIscScale<0.5)
			m_mirrorIscScale=1;
		else if (fabs(m_trueIsc - m_curIsc)/m_trueIsc <= m_temppre)
		{

			m_reckonIsc = true;
		}else
		{
			m_reckonIsc = false;
		}
		((CEdit *)GetDlgItem(IDC_EDIT_CURISC))->SetRedraw(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_CURISC))->Invalidate();
		((CEdit *)GetDlgItem(IDC_EDIT_CURISC))->UpdateWindow();
	}else m_mirrorIscScale=1;
	UpdateData(0);
}

void CSetSunPara::OnBnClickedButtonCalcscalevoc()
{
	UpdateData();
	double m_temppre = 0;
	m_temppre = m_preVoc*0.01;
	m_mirrorVocScale=m_scaleVoc;

	if (m_curVoc < 0 || m_curVoc > 100)
	{
		m_curVoc = 0;
	}

	if(m_curVoc!=0)
	if(m_trueVoc!=0){
		m_mirrorVocScale*=m_trueVoc/m_curVoc;
		if(m_mirrorVocScale>1.5||m_mirrorVocScale<0.5)
			m_mirrorVocScale=1;
		else if (fabs(m_trueVoc - m_curVoc)/m_trueVoc <= m_temppre)
		{
			m_reckonVoc = true;		
		}else
		{
			m_reckonVoc = false;
		}
		((CEdit *)GetDlgItem(IDC_EDIT_CURVOC))->SetRedraw(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_CURVOC))->Invalidate();
		((CEdit *)GetDlgItem(IDC_EDIT_CURVOC))->UpdateWindow();
	}else m_mirrorVocScale=1;
	UpdateData(0);
}
void CSetSunPara::OnBnClickedButtonCalcscalePmax()
{
	UpdateData();
	double m_temppre = 0;
	m_temppre = m_prePmax*0.01;
	m_mirrorPmaxScale=m_scalePmax;

	if (m_curPmax < 0 || m_curPmax > 1000)
	{
		m_curPmax = 0;
	}

	if(m_curPmax!=0)
		if(m_truePmax!=0){
			m_mirrorPmaxScale*=m_truePmax/m_curPmax;
			if(m_mirrorPmaxScale>1.5||m_mirrorPmaxScale<0.5)
				m_mirrorPmaxScale=1;
			else if (fabs(m_truePmax - m_curPmax)/m_truePmax <= m_temppre)
			{
				m_reckonPmax = true;
			}else
			{
				m_reckonPmax = false;
			}
			((CEdit *)GetDlgItem(IDC_EDIT_CURPMAX))->SetRedraw(TRUE);
			((CEdit *)GetDlgItem(IDC_EDIT_CURPMAX))->Invalidate();
			((CEdit *)GetDlgItem(IDC_EDIT_CURPMAX))->UpdateWindow();
		}else m_mirrorPmaxScale=1;
	UpdateData(0);
}
void CSetSunPara::OnBnClickedButtonCalcscaletemp()
{
	UpdateData();
	double m_temppre = 0;
	m_temppre = m_preTemp*0.01;
	m_mirrorTempScale=m_scaleTemp;
	//转化为绝对温度后计算修正系数
	if (m_curTemp < -273 || m_curTemp > 300)
	{
		m_curTemp = 0;
	}

	if(m_trueTemp!=-273){
		m_mirrorTempScale*=(273+m_trueTemp)/((m_curTemp+273)/m_mirrorTempScale);
		if(m_mirrorTempScale>1.5||m_mirrorTempScale<0.5)
			m_mirrorTempScale=1;
		else if (fabs(m_trueTemp - m_curTemp)/m_trueTemp <= m_temppre)
		{
			m_reckonTemp = true;
		}else
		{
			m_reckonTemp = false;
		}
		((CEdit *)GetDlgItem(IDC_EDIT_CURTEMP))->SetRedraw(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_CURTEMP))->Invalidate();
		((CEdit *)GetDlgItem(IDC_EDIT_CURTEMP))->UpdateWindow();
	}else m_mirrorTempScale=1;
	UpdateData(0);
}

BOOL CSetSunPara::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	Apply(wParam);    // 判断控件是否更改
	return __super::OnCommand(wParam, lParam);
}

void CSetSunPara::OnBnClickedButtonCalcscaleIRtemp()
{
	UpdateData();
	double m_temppre = 0;
	m_temppre = m_preIRTemp*0.01;
	m_mirrorIRTempScale=m_scaleIRTemp;
	//转化为绝对温度后计算修正系数
	if (m_curIRTemp < -273 || m_curIRTemp > 300)
	{
		m_curIRTemp = 0;
	}
	if(m_trueIRTemp!=-273){
		m_mirrorIRTempScale=(273+m_trueIRTemp)/((m_curIRTemp+273)/m_mirrorIRTempScale);
		if(m_mirrorIRTempScale>1.5||m_mirrorIRTempScale<0.5)
			m_mirrorIRTempScale=1;
		else if (fabs(m_trueIRTemp - m_curIRTemp)/m_trueIRTemp <= m_temppre)
		{
			m_reckonIRTemp = true;
		}else
		{
			m_reckonIRTemp = false;
		}
		((CEdit *)GetDlgItem(IDC_EDIT_CURIRTEMP))->SetRedraw(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_CURIRTEMP))->Invalidate();
		((CEdit *)GetDlgItem(IDC_EDIT_CURIRTEMP))->UpdateWindow();
	}else m_mirrorIRTempScale=1;
	UpdateData(0);
}

void CSetSunPara::LoadReg(void)
{
	CalibrationParaList *calilist = this;

	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo1.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive)){
		file.Read(calilist,sizeof(CalibrationParaList));
		file.Close();
	}
	else
		return;

	m_mirrorVocScale=m_scaleVoc;
	m_mirrorIscScale=m_scaleIsc;
	m_mirrorPmaxScale = m_scalePmax;
	m_mirrorTempScale=m_scaleTemp;
	m_mirrorIRTempScale=m_scaleIRTemp;

	UpdateData(0);
}
void CSetSunPara::SaveReg(void)
{
	UpdateData();
	m_scaleVoc=m_mirrorVocScale;
	m_scaleIsc=m_mirrorIscScale;
	m_scalePmax = m_mirrorPmaxScale;
	m_scaleTemp=m_mirrorTempScale;
	m_scaleIRTemp=m_mirrorIRTempScale;
	CalibrationParaList* calilist=this;

	// 保存配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo1.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		file.Write(calilist, sizeof(CalibrationParaList));
		file.Close();
	}
}

CalibrationParaList* CSetSunPara::GetAppliedConfig(void)
{
	UpdateData();
	m_scaleVoc=m_mirrorVocScale;
	m_scaleIsc=m_mirrorIscScale;
	m_scalePmax = m_mirrorPmaxScale;
	m_scaleTemp=m_mirrorTempScale;
	m_scaleIRTemp=m_mirrorIRTempScale;
	CalibrationParaList* caliList=this;
	return caliList;
}

void CSetSunPara::SetAppliedConfig(CalibrationParaList *caliList)
{
	CalibrationParaList *calilist = this;
	memcpy(calilist, caliList, sizeof(CalibrationParaList));

	m_mirrorVocScale=m_scaleVoc;
	m_mirrorIscScale=m_scaleIsc;
	m_mirrorPmaxScale = m_scalePmax;
	m_mirrorTempScale=m_scaleTemp;
	m_mirrorIRTempScale=m_scaleIRTemp;
	

	GetDlgItem(IDC_CHECK_REFLECT)->ShowWindow(m_fc[REFLECTANCE]);
	GetDlgItem(IDC_CHECK_REFLECT)->EnableWindow(m_fc[REFLECTANCE]);

	GetDlgItem(IDC_EDIT_SCALEISC1)->ShowWindow(m_fc[REFLECTANCE]);
	GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(m_fc[REFLECTANCE]);

	GetDlgItem(IDC_EDIT_SCALEVOC1)->ShowWindow(m_fc[REFLECTANCE]);
	GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(m_fc[REFLECTANCE]);

	if (calilist->m_swReflectScale && m_fc[REFLECTANCE])
	{
		if (m_reckonIsc && m_reckonVoc)
		{
			GetDlgItem(IDC_BUTTON_CALCSCALEISC)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CALCSCALEVOC)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(TRUE);
		}else
		{
			calilist->m_swReflectScale = FALSE;
			GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(FALSE);

		}

	}else
	{
		GetDlgItem(IDC_BUTTON_CALCSCALEISC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALCSCALEVOC)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_SCALEISC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALEVOC1)->EnableWindow(FALSE);
	}

	UpdateData(0);
}

void CSetSunPara::SetSunParaDefault()
{
	CalibrationParaList *default_setsunpara = this;
		default_setsunpara->m_trueIsc=(0);
		default_setsunpara->m_TempType=(1);
		default_setsunpara-> m_curIsc=(0);
		default_setsunpara-> m_scaleIsc=(1);
		default_setsunpara-> m_preIsc=(0.2);
		default_setsunpara-> m_scaleIsc1=(1);
		default_setsunpara->m_trueVoc=(0);
		default_setsunpara->m_curVoc=(0);
		default_setsunpara->m_scaleVoc=(1);
		default_setsunpara-> m_preVoc=(0.2);
		default_setsunpara->m_scaleVoc1=(1);
		default_setsunpara->m_truePmax=(0);
		default_setsunpara->m_curPmax=(0);
		default_setsunpara->m_scalePmax=(1);
		default_setsunpara-> m_prePmax=(0.2);
		default_setsunpara-> m_trueTemp=(0);
		default_setsunpara->m_curTemp=(0);
		default_setsunpara-> m_scaleTemp=(1);
		default_setsunpara-> m_preTemp=(0.2);
		default_setsunpara->m_trueIRTemp=(0);
		default_setsunpara-> m_curIRTemp=(0);
		default_setsunpara->m_scaleIRTemp=(1);
		default_setsunpara-> m_preIRTemp=(0.2);
		default_setsunpara-> m_StdCellIsc=(0);
		default_setsunpara->m_tempScaleCur=(0.02/100);
		default_setsunpara->m_tempScaleVol=(-0.0037);	
		default_setsunpara->m_solarcount=(1);
		default_setsunpara->m_swTempToObj=(FALSE);
		default_setsunpara->m_swIrrToObj=(FALSE);
		default_setsunpara->m_ObjectTemp=(25);
		default_setsunpara->m_swDisableCali=(FALSE);
		default_setsunpara->m_swReflectScale=(FALSE);
		default_setsunpara->m_Vdot=(0);
		default_setsunpara->m_area=(0);
		default_setsunpara-> m_areaEff=(0);
		default_setsunpara->m_reckonIsc = (TRUE);
		default_setsunpara->m_reckonVoc = (TRUE);
		default_setsunpara->m_reckonPmax = (TRUE);
		default_setsunpara->m_reckonTemp = (TRUE);
		default_setsunpara->m_reckonIRTemp = (TRUE);


		m_mirrorVocScale=m_scaleVoc;
		m_mirrorIscScale=m_scaleIsc;
		m_mirrorPmaxScale = m_scalePmax;
		m_mirrorTempScale=m_scaleTemp;
		m_mirrorIRTempScale=m_scaleIRTemp;

	UpdateData(FALSE);
}