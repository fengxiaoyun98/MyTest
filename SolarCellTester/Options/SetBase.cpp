// SetBase.cpp : implementation file
//

#include "stdafx.h"
#include "SetBase.h"


// CSetBase dialog

IMPLEMENT_DYNAMIC(CSetBase, CDialog)

CString GetStrAfterDot(CString str)
{
	int dotPos;
	CString sValName;
	dotPos = str.ReverseFind('.');
	sValName = str.Right(str.GetLength() - dotPos -1);
	sValName.Replace(L"m_", L"");

	return sValName;
}


CSetBase::CSetBase(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetBase::IDD, pParent)
{
	OpReg()->CreateKey(GetThisClassName());

	m_sunpara=new CSetSunPara();
	m_841=new CSetDev841();
	m_alarm=new CSetParaAlarm();
	m_chart=new CSetChartView();
	m_curve=new CSetCurveIV();
	m_PrtLabel=new CSetPrtLabel();

	m_sConfigFileName = L"";
}
void CSetBase::Destroy(CDialog* dlg)
{
	if(dlg){
		delete dlg;
		dlg=NULL;
	}
}
CSetBase::~CSetBase()
{
	Destroy(m_841);
	Destroy(m_sunpara);
	Destroy(m_alarm);
	Destroy(m_chart);
	Destroy(m_curve);
	Destroy(m_PrtLabel);
}

void CSetBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_sheet);
	DDX_Text(pDX,IDC_EDIT_CONFIGFILE, m_sConfigFileName);
}


BEGIN_MESSAGE_MAP(CSetBase, CDialog)
	ON_BN_CLICKED(ID_SETBASE_APPLY, &CSetBase::OnBnClickedSetbaseApply)
	ON_BN_CLICKED(IDC_SETBASE_DEFAULT, &CSetBase::OnBnClickedSetbaseDefault)                                          //朱峰改动
	ON_BN_CLICKED(ID_SETBASE_SAVEUSERCONFIG, &CSetBase::OnBnClickedApplyConfigFile)
	ON_BN_CLICKED(ID_SETBASE_BROWSE, &CSetBase::OnBnClickedBrowseConfigFile)
	ON_EN_CHANGE(IDC_EDIT_CONFIGFILE, &CSetBase::OnEnChangeEditConfigfile)
END_MESSAGE_MAP()


// CSetBase message handlers

BOOL CSetBase::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetBaseSaveReg));
	theApp.RegFunction(L"CSetBase::SaveReg",str);

	str.LoadString(IDS_Restore_default);
	theApp.RegFunction(L"CSetBase::DefaultApply",str);

	if (theApp.IsDisable(L"CSetBase::DefaultApply"))
		GetDlgItem(IDC_SETBASE_DEFAULT)->EnableWindow(FALSE);

	//禁止所有控件
	if(theApp.IsDisable(L"CSetBase::SaveReg"))
	{
//		DisableAllSubCtrl(this);
		GetDlgItem(ID_SETBASE_BROWSE)->EnableWindow(0);             // 浏览按钮不可用
        GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(0);    // 应用配置文件按钮不可用
		GetDlgItem(IDOK)->EnableWindow(1);                          // 退出按钮可用
        GetDlgItem(IDC_EDIT_CONFIGFILE)->ShowWindow(0);             // 配置文件信息不可见
	}
	CDialog::OnInitDialog();

	int nCntTotal = 0;

	if (!OpReg()->Read(m_sConfigFileKey,m_sConfigFileName))
	{
		m_sConfigFileName = L"";
	}
	else if (m_sConfigFileName == L"NoConfigFile" || !PathFileExists(m_sConfigFileName))
	{
		m_sConfigFileName = L"";
	}

	CString title;
	title.LoadString(IDS_SETBASE_CALI);
	// TODO:  Add extra initialization here
	m_sheet.AddPage(title,m_sunpara,CSetSunPara::IDD);
	title.LoadString(IDS_SETBASE_ACQU);
	m_sheet.AddPage(title,m_841,CSetDev841::IDD);
	title.LoadString(IDS_SETBASE_FILT);
	m_sheet.AddPage(title,m_alarm,CSetParaAlarm::IDD);
	title.LoadString(IDS_SETBASE_AXIS);
	m_sheet.AddPage(title,m_chart,CSetChartView::IDD);
	title.LoadString(IDS_SETBASE_SAVE);
	m_sheet.AddPage(title,m_curve,CSetCurveIV::IDD);
	title.LoadString(IDS_SETBASE_LABEL);
	m_sheet.AddPage(title,m_PrtLabel,CSetPrtLabel::IDD);

	m_sheet.Show();

	GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(0);

	// 读取并应用配置文件
//	CString str;
	str = m_sConfigFileName;
	if (!str.IsEmpty())          // 改動
	{
		if(ReadUserConfig(m_sConfigFileName))
		{
			m_sunpara->SetAppliedConfig(&(theApp.m_userConfig.Calibration));
			m_841->SetAppliedConfig(&(theApp.m_userConfig.Acquisition));
			m_alarm->SetAppliedConfig(&(theApp.m_userConfig.DataFilter));
			m_chart->SetAppliedConfig(&(theApp.m_userConfig.ChartAxis));
			m_curve->SetAppliedConfig(&(theApp.m_userConfig.SaveData));
			m_PrtLabel->SetAppliedConfig(&(theApp.m_userConfig.PrintLabel));
		}
	}
	else
	{
		m_841->LoadCntReg();
	}

	UpdateData(FALSE);
	GetDlgItem(ID_SETBASE_APPLY)->EnableWindow(0);           // 应用按钮缺省为不可用

/*
//	CString str(MAKEINTRESOURCE(IDS_CSetBaseSaveReg));
	str = MAKEINTRESOURCE(IDS_CSetBaseSaveReg);
	theApp.RegFunction(L"CSetBase::SaveReg",str);

	//禁止所有控件
	if(theApp.IsDisable(L"CSetBase::SaveReg"))
	{
		DisableAllSubCtrl(this);
		GetDlgItem(IDOK)->EnableWindow(1);
		GetDlgItem(ID_SETBASE_APPLY)->EnableWindow(0);
	}
*/
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSetBase::OnBnClickedSetbaseDefault()                                             //恢复默认配置函数，朱峰改动
{
	m_sunpara->SetSunParaDefault();
	m_841->SetDev841Default();
	m_alarm->SetParaAlarmDefault();
	m_chart->SetChartViewDefault();
	m_curve->SetCurveIVDefault();
	m_PrtLabel->SetPrtLabelDefault();
	GetDlgItem(ID_SETBASE_APPLY)->EnableWindow(1);

}

void CSetBase::OnBnClickedSetbaseApply()
{
	
    UpdateData(TRUE);
	
	theApp.m_userConfig.Calibration = *(m_sunpara->GetAppliedConfig());
	theApp.m_userConfig.Acquisition = *(m_841->GetAppliedConfig());
	theApp.m_userConfig.DataFilter = (m_alarm->GetAppliedConfig());
	theApp.m_userConfig.ChartAxis = *(m_chart->GetAppliedConfig());
	theApp.m_userConfig.SaveData = *(m_curve->GetAppliedConfig());
	theApp.m_userConfig.PrintLabel = (m_PrtLabel->GetAppliedConfig());

	CString str;
	str = m_sConfigFileName;
	if (!str.IsEmpty())
	{
		SaveUserConfig(str);
		(theApp.m_opt.GetDlgItem(ID_SETBASE_APPLY))->EnableWindow(0);
	}
	else
	{
		CString szFilter= _T("ini文件 (*.ini)|*.ini|");  
		CFileDialog FileDlg(FALSE, _T("*.ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

		if (FileDlg.DoModal() == IDOK)
		{
			m_sConfigFileName = FileDlg.GetPathName();
		}
		CString str;
		str = m_sConfigFileName;
		if (!str.IsEmpty())
		{
			SaveUserConfig(str);
			OpReg()->Write(m_sConfigFileKey,m_sConfigFileName);
			GetDlgItem(ID_SETBASE_APPLY)->EnableWindow(0);
			GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(1);
		}
	}
	UpdateData(FALSE);
	m_curve->SetArea1(m_sunpara->GetArea());                             //改动
	m_curve->SetAreaEff1(m_sunpara->GetAreaEff());
}

void CSetBase::OnBnClickedBrowseConfigFile()
{
	// 读取配置文件
	CString szFilter= _T("ini文件 (*.ini)|*.ini|");
	CFileDialog FileDlg(TRUE, _T("*.ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		m_sConfigFileName = FileDlg.GetPathName();
		GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(1);
	}
   
	UpdateData(FALSE);
}

void CSetBase::OnBnClickedApplyConfigFile()
{
	// 读取并应用配置文件
	UpdateData(TRUE);
	CString str;
	str = m_sConfigFileName;
	if (str.IsEmpty())
	{
		m_sConfigFileName = L"NoConfigFile";
		OpReg()->Write(m_sConfigFileKey,m_sConfigFileName);
		GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(0);
		return;
	}
	if(ReadUserConfig(m_sConfigFileName))
	{
		theApp.m_userConfig.Calibration.m_scaleIsc = 1.0;
		theApp.m_userConfig.Calibration.m_scaleVoc = 1.0;
		theApp.m_userConfig.Calibration.m_scalePmax = 1.0;
		theApp.m_userConfig.Calibration.m_scaleTemp = 1.0;
		theApp.m_userConfig.Calibration.m_scaleIRTemp = 1.0;

		OpReg()->Write(m_sConfigFileKey,m_sConfigFileName);
		m_sunpara->SetAppliedConfig(&(theApp.m_userConfig.Calibration));
		m_841->SetAppliedConfig(&(theApp.m_userConfig.Acquisition));
		m_alarm->SetAppliedConfig(&(theApp.m_userConfig.DataFilter));
		m_chart->SetAppliedConfig(&(theApp.m_userConfig.ChartAxis));
		m_curve->SetAppliedConfig(&(theApp.m_userConfig.SaveData));
		m_PrtLabel->SetAppliedConfig(&(theApp.m_userConfig.PrintLabel));

		GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(0);
	}

	else
		return;
}

void CSetBase::SaveUserConfig(CString filename)
{
	USES_CONVERSION;
	DeleteFile(filename);
	CString str = L"";
	//校准和修正
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_area);
	WritePrivateProfileString(L"SunPara",L"Area",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_trueIsc);
	WritePrivateProfileString(L"SunPara",L"TrueIsc",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_curIsc);
	WritePrivateProfileString(L"SunPara",L"CurrentIsc",str,filename);	
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleIsc);
	WritePrivateProfileString(L"SunPara",L"IscScale",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_preIsc);
	WritePrivateProfileString(L"SunPara",L"IscPre",str,filename);

	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleIsc1);
	WritePrivateProfileString(L"SunPara",L"IscScale1",str,filename);

	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_trueVoc);
	WritePrivateProfileString(L"SunPara",L"TureVoc",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_curVoc);
	WritePrivateProfileString(L"SunPara",L"CurrentVoc",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleVoc);
	WritePrivateProfileString(L"SunPara",L"VocScale",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_preVoc);
	WritePrivateProfileString(L"SunPara",L"VocPre",str,filename);

	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleVoc1);
	WritePrivateProfileString(L"SunPara",L"VocScale1",str,filename);

	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_truePmax);
	WritePrivateProfileString(L"SunPara",L"TurePmax",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_curPmax);
	WritePrivateProfileString(L"SunPara",L"CurrentPmax",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scalePmax);
	WritePrivateProfileString(L"SunPara",L"PmaxScale",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_prePmax);
	WritePrivateProfileString(L"SunPara",L"PmaxPre",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_trueTemp);
	WritePrivateProfileString(L"SunPara",L"TureTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_curTemp);
	WritePrivateProfileString(L"SunPara",L"CurrentTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleTemp);
	WritePrivateProfileString(L"SunPara",L"TempScalse",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_preTemp);
	WritePrivateProfileString(L"SunPara",L"TempPre",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_trueIRTemp);
	WritePrivateProfileString(L"SunPara",L"TureIRTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_curIRTemp);
	WritePrivateProfileString(L"SunPara",L"CurrentIRTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_scaleIRTemp);
	WritePrivateProfileString(L"SunPara",L"IRTempScalse",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_preIRTemp);
	WritePrivateProfileString(L"SunPara",L"IRTempPre",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Calibration.m_reckonIsc);
	WritePrivateProfileString(L"SunPara",L"ReckIsc",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_reckonVoc);
	WritePrivateProfileString(L"SunPara",L"ReckVoc",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_reckonPmax);
	WritePrivateProfileString(L"SunPara",L"ReckPmax",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_reckonTemp);
	WritePrivateProfileString(L"SunPara",L"ReckTemp",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_reckonIRTemp);
	WritePrivateProfileString(L"SunPara",L"ReckIRTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_StdCellIsc);
	WritePrivateProfileString(L"SunPara",L"StdCellIsc",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_tempScaleCur);
	WritePrivateProfileString(L"SunPara",L"CurrentTempScalse",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_tempScaleVol);
	WritePrivateProfileString(L"SunPara",L"CurrentVolScalse",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_solarcount);
	WritePrivateProfileString(L"SunPara",L"Solarcount",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_ObjectTemp);
	WritePrivateProfileString(L"SunPara",L"ObjectTemp",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_Vdot);
	WritePrivateProfileString(L"SunPara",L"Vdot",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_swTempToObj);
	WritePrivateProfileString(L"SunPara",L"TempToObj",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Calibration.m_swIrrToObj);
	WritePrivateProfileString(L"SunPara",L"IrrToObj",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Calibration.m_swReflectScale);
	WritePrivateProfileString(L"SunPara",L"ReflectScale",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Calibration.m_swDisableCali);
	WritePrivateProfileString(L"SunPara",L"DisableCali",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Calibration.m_TempType);
	WritePrivateProfileString(L"SunPara",L"TempType",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.Calibration.m_areaEff);
	WritePrivateProfileString(L"SunPara",L"AreaEff",str,filename);

	// 数据采集
	// 总计数存在注册表中，不可更改
	str.Format(L"%d",theApp.m_userConfig.Acquisition.m_externSwitch);
	WritePrivateProfileString(L"DAQ",L"externSwitch",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Acquisition.m_exterV_I);
	WritePrivateProfileString(L"DAQ",L"exterV_I",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Acquisition.m_exterV_I_Sec);
	WritePrivateProfileString(L"DAQ",L"exterV_I_Sec",str,filename);

	str.Format(L"%d",theApp.m_userConfig.Acquisition.m_eff_cell);
	WritePrivateProfileString(L"DAQ",L"eff_cell",str,filename);

	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_nocur);
	WritePrivateProfileString(L"DAQ",L"nocur",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_novol);
	WritePrivateProfileString(L"DAQ",L"novol",str,filename);	
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_nslen);
	WritePrivateProfileString(L"DAQ",L"nslen",str,filename);

	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_nsweepdirection);
	WritePrivateProfileString(L"DAQ",L"nsweepdirection",str,filename);

	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_set841sun);
	WritePrivateProfileString(L"DAQ",L"set841sun",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_Interval);
	WritePrivateProfileString(L"DAQ",L"Interval",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_CloseNx);
	WritePrivateProfileString(L"DAQ",L"CloseNx",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_Lmt1);
	WritePrivateProfileString(L"DAQ",L"Lmt1",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_Lmt2);
	WritePrivateProfileString(L"DAQ",L"Lmt2",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_Lmt3);
	WritePrivateProfileString(L"DAQ",L"Lmt3",str,filename);
	str.Format(L"%d",(int)theApp.m_userConfig.Acquisition.m_model);
	WritePrivateProfileString(L"DAQ",L"model",str,filename);
	str.Format(L"%d",theApp.m_userConfig.Acquisition.m_DisCntAlarm);
	WritePrivateProfileString(L"DAQ",L"DisCntAlarm",str,filename);

	// 分档报警
	str.Format(L"%d",theApp.m_userConfig.DataFilter.m_number);
	WritePrivateProfileString(L"GradeAlarm",L"number",str,filename);
	for (int i = 0; i < theApp.m_userConfig.DataFilter.m_number; i++)
	{
		CString strW;
		strW.Format(L"File%d", i+1);
		WritePrivateProfileString(L"GradeAlarm",strW,theApp.m_userConfig.DataFilter.m_GAList[i],filename);
	}

	// 坐标设置
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Imin);
	WritePrivateProfileString(L"ChartAxis",L"Imin",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Imax);
	WritePrivateProfileString(L"ChartAxis",L"Imax",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Vmin);
	WritePrivateProfileString(L"ChartAxis",L"Vmin",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Vmax);
	WritePrivateProfileString(L"ChartAxis",L"Vmax",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Pmin);
	WritePrivateProfileString(L"ChartAxis",L"Pmin",str,filename);
	str.Format(L"%lf",theApp.m_userConfig.ChartAxis.m_Pmax);
	WritePrivateProfileString(L"ChartAxis",L"Pmax",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_EnableXAuto);
	WritePrivateProfileString(L"ChartAxis",L"EnableXAuto",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_EnableYAuto);
	WritePrivateProfileString(L"ChartAxis",L"EnableYAuto",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideTitle);
	WritePrivateProfileString(L"ChartAxis",L"hideTitle",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideXTitle);
	WritePrivateProfileString(L"ChartAxis",L"hideXitle",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideYTitle);
	WritePrivateProfileString(L"ChartAxis",L"hideYitle",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideZTitle);
	WritePrivateProfileString(L"ChartAxis",L"hideZitle",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideXGrid);
	WritePrivateProfileString(L"ChartAxis",L"hideXGrid",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideYGrid);
	WritePrivateProfileString(L"ChartAxis",L"hideYGrid",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hidePV);
	WritePrivateProfileString(L"ChartAxis",L"hidePV",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideIRR);
	WritePrivateProfileString(L"ChartAxis",L"hideIRR",str,filename);
	str.Format(L"%d",theApp.m_userConfig.ChartAxis.m_hideComm);
	WritePrivateProfileString(L"ChartAxis",L"hideComm",str,filename);

	// 保存和显示
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swAuto);
	WritePrivateProfileString(L"SaveData",L"swAuto",str,filename);

	str.Format(L"%d",theApp.m_userConfig.SaveData.m_SQLserver);  //改动
	WritePrivateProfileString(L"SaveData",L"SQLserver",str,filename);

	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSerial);
	WritePrivateProfileString(L"SaveData",L"swSerial",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSNLength);
	WritePrivateProfileString(L"SaveData",L"swSNLength",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSerialBox);
	WritePrivateProfileString(L"SaveData",L"swSerialBox",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSaveJpg);
	WritePrivateProfileString(L"SaveData",L"swSaveJpg",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSaveDaq);
	WritePrivateProfileString(L"SaveData",L"swSaveDaq",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSaveCaliDaq);
	WritePrivateProfileString(L"SaveData",L"swSaveCaliDaq",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSavePara);
	WritePrivateProfileString(L"SaveData",L"swSavePara",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSaveParaMDB);
	WritePrivateProfileString(L"SaveData",L"swSaveParaMDB",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSerialUnique);
	WritePrivateProfileString(L"SaveData",L"swSerialUnique",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swSaveInClass);
	WritePrivateProfileString(L"SaveData",L"swSaveInClass",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swBackInput);
	WritePrivateProfileString(L"SaveData",L"swBackInput",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_fnstyle);
	WritePrivateProfileString(L"SaveData",L"fnstyle",str,filename);
 	str.Format(L"%d",theApp.m_userConfig.SaveData.m_eorc);
 	WritePrivateProfileString(L"SaveData",L"paraE",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_swUpdateFnDate);
	WritePrivateProfileString(L"SaveData",L"swUpdateFnDate",str,filename);
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_DontClearInput);
	WritePrivateProfileString(L"SaveData",L"DontClearInput",str,filename);
	str.Format(L"%ld",theApp.m_userConfig.SaveData.m_auto);
	WritePrivateProfileString(L"SaveData",L"auto",str,filename);
	str.Format(L"%ld",theApp.m_userConfig.SaveData.m_TotalTest);
	WritePrivateProfileString(L"SaveData",L"TotalTest",str,filename);
	WritePrivateProfileString(L"SaveData",L"fileSavePath",theApp.m_userConfig.SaveData.m_fileSavePath,filename);
	WritePrivateProfileString(L"SaveData",L"manuf",theApp.m_userConfig.SaveData.m_manuf,filename);
	WritePrivateProfileString(L"SaveData",L"ParaFileName",theApp.m_userConfig.SaveData.m_ParaFileName,filename);
	WritePrivateProfileString(L"SaveData",L"InputBoxSerial",theApp.m_userConfig.SaveData.m_InputBoxSerial,filename);

    //朱峰改动
	WritePrivateProfileString(L"SaveData",L"Serverip",theApp.m_userConfig.SaveData.m_serverip_this,filename);
	WritePrivateProfileString(L"SaveData",L"Database",theApp.m_userConfig.SaveData.m_database_this,filename);
	WritePrivateProfileString(L"SaveData",L"Username",theApp.m_userConfig.SaveData.m_username_this,filename);
	WritePrivateProfileString(L"SaveData",L"Password",theApp.m_userConfig.SaveData.m_password_this,filename);

	// 工作目录文件夹不存在则创建
	if(!PathFileExists(theApp.m_userConfig.SaveData.m_fileSavePath))
	{
		CString m_PathDataStr;
		char* m_PathDataChar;
		m_PathDataStr = theApp.m_userConfig.SaveData.m_fileSavePath;
		m_PathDataChar = W2A(m_PathDataStr);
		MakeSureDirectoryPathExists(m_PathDataChar);
	}
	////////////////////////////改动///////////////////////////////////////////
	TCHAR m_Pathdaq[255] , m_Pathjpg[255];
	CString m_Pathdaqstr , m_Pathjpgstr;
	m_Pathdaqstr = theApp.m_userConfig.SaveData.m_fileSavePath;
	m_Pathjpgstr = theApp.m_userConfig.SaveData.m_fileSavePath;

	m_Pathdaqstr = m_Pathdaqstr+_T("daq");
	m_Pathjpgstr = m_Pathjpgstr+_T("jpg");

	_tcscpy(m_Pathdaq,m_Pathdaqstr);
	_tcscpy(m_Pathjpg,m_Pathjpgstr);

	CreateDirectoryW(m_Pathdaq, 0);
	CreateDirectoryW(m_Pathjpg, 0);
	//////////////////////////////////////////////////////////////////////////
	str.Format(L"%d",theApp.m_userConfig.SaveData.m_fCount);
	WritePrivateProfileString(L"SaveData",L"fCount",str,filename);
	// 到文件列表
	for (int i = 0; i < 50; i++)
	{
		if (theApp.m_userConfig.SaveData.m_Tofile[i].name[0] != 0)
		{
			CString strF = L"";
			strF.Format(L"fNum%d", i);
			str.Format(L"%d",theApp.m_userConfig.SaveData.m_Tofile[i].id);
			WritePrivateProfileString(L"SaveData",strF,str,filename);
			strF.Format(L"fVar%d", i);
			WritePrivateProfileString(L"SaveData",strF, theApp.m_userConfig.SaveData.m_Tofile[i].name,filename);
		}
	}

	str.Format(L"%d",theApp.m_userConfig.SaveData.m_dCount);
	WritePrivateProfileString(L"SaveData",L"dCount",str,filename);
	// 到显示列表
	for (int i = 0; i < 50; i++)
	{
		if (theApp.m_userConfig.SaveData.m_ToDisplay[i].name[0] != 0)
		{
			CString strD = L"";
			strD.Format(L"dNum%d", i);
			str.Format(L"%d",theApp.m_userConfig.SaveData.m_ToDisplay[i].id);
			WritePrivateProfileString(L"SaveData",strD,str,filename);
			strD.Format(L"dVar%d", i);
			WritePrivateProfileString(L"SaveData",strD, theApp.m_userConfig.SaveData.m_ToDisplay[i].name,filename);
		}
	}
	//////////////////////////////自定义 改动//////////////////////////////////
	for (int i = 0; i < 20; i++)
	{
		if (theApp.m_userConfig.SaveData.m_usToDisplay[i].fied[0] != 0)
		{
			CString strD = L"";
			strD.Format(L"dUSFied%d", i);
			WritePrivateProfileString(L"SaveData",strD,theApp.m_userConfig.SaveData.m_usToDisplay[i].fied,filename);
			strD.Format(L"dUSData%d", i);
			WritePrivateProfileString(L"SaveData",strD, theApp.m_userConfig.SaveData.m_usToDisplay[i].data,filename);
		}
	}
	/////////////////////////////////////////////////////////////////////////*/
	// 标签打印
	str.Format(L"%d",theApp.m_userConfig.PrintLabel.m_EnPrtLabel);
	WritePrivateProfileString(L"PrintLabel",L"EnPrtLabel",str,filename);
	WritePrivateProfileString(L"PrintLabel",L"sFilePath",theApp.m_userConfig.PrintLabel.m_sFilePath,filename);
	WritePrivateProfileString(L"PrintLabel",L"sFileName",theApp.m_userConfig.PrintLabel.m_sFileName,filename);
	str.Format(L"%d",theApp.m_userConfig.PrintLabel.m_saveCSV);
	WritePrivateProfileString(L"PrintLabel",L"saveCSV",str,filename);
	str.Format(L"%d",theApp.m_userConfig.PrintLabel.m_saveTXT);
	WritePrivateProfileString(L"PrintLabel",L"saveTXT",str,filename);
	/////////////////////////////////////////////////////////////////////////*/
}

BOOL CSetBase::ReadUserConfig(CString filename)
{
	USES_CONVERSION;

	CString str = L"";

	// 校准和修正 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"SunPara",L"Area",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_area = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"TrueIsc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_trueIsc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentIsc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_curIsc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"IscScale",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleIsc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"IscPre",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_preIsc = atof(T2A(str));

	GetPrivateProfileString(L"SunPara",L"IscScale1",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleIsc1 = atof(T2A(str));

	GetPrivateProfileString(L"SunPara",L"TureVoc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_trueVoc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentVoc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_curVoc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"VocScale",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleVoc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"VocPre",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_preVoc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"VocScale1",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleVoc1 = atof(T2A(str));

	GetPrivateProfileString(L"SunPara",L"TurePmax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_truePmax = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentPmax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_curPmax = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"PmaxScale",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scalePmax = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"PmaxPre",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_prePmax = atof(T2A(str));

	GetPrivateProfileString(L"SunPara",L"TureTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_trueTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_curTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"TempScalse",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"TempPre",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_preTemp = atof(T2A(str));

	GetPrivateProfileString(L"SunPara",L"TureIRTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_trueIRTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentIRTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_curIRTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"IRTempScalse",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_scaleIRTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"IRTempPre",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_preIRTemp = atof(T2A(str));
	
	GetPrivateProfileString(L"SunPara",L"ReckIsc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_reckonIsc = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"ReckVoc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_reckonVoc = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"ReckPmax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_reckonPmax = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"ReckTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_reckonTemp = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"ReckIRTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_reckonIRTemp = atoi(T2A(str));

	GetPrivateProfileString(L"SunPara",L"StdCellIsc",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_StdCellIsc = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentTempScalse",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_tempScaleCur = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"CurrentVolScalse",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_tempScaleVol = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"Solarcount",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_solarcount = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"ObjectTemp",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_ObjectTemp = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"Vdot",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_Vdot = atof(T2A(str));
	GetPrivateProfileString(L"SunPara",L"TempToObj",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_swTempToObj = atoi(T2A(str));

	GetPrivateProfileString(L"SunPara",L"ReflectScale",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_swReflectScale = atoi(T2A(str));

	GetPrivateProfileString(L"SunPara",L"IrrToObj",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_swIrrToObj = atoi(T2A(str));

	GetPrivateProfileString(L"SunPara",L"DisableCali",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_swDisableCali = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"TempType",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_TempType = atoi(T2A(str));
	GetPrivateProfileString(L"SunPara",L"AreaEff",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Calibration.m_areaEff = atof(T2A(str));

	// 数据采集 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"DAQ",L"externSwitch",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_externSwitch = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"exterV_I",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_exterV_I = atoi(T2A(str));

	GetPrivateProfileString(L"DAQ",L"exterV_I_Sec",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_exterV_I_Sec = atoi(T2A(str));

	GetPrivateProfileString(L"DAQ",L"eff_cell",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_eff_cell = atoi(T2A(str));

	GetPrivateProfileString(L"DAQ",L"nocur",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
    theApp.m_userConfig.Acquisition.m_nocur = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"novol",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_novol = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"nslen",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_nslen = atoi(T2A(str));

	GetPrivateProfileString(L"DAQ",L"nsweepdirection",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_nsweepdirection = atoi(T2A(str));

	GetPrivateProfileString(L"DAQ",L"set841sun",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_set841sun = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"Interval",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_Interval = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"CloseNx",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_CloseNx = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"Lmt1",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_Lmt1 = atol(T2A(str));
	GetPrivateProfileString(L"DAQ",L"Lmt2",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_Lmt2 = atol(T2A(str));
	GetPrivateProfileString(L"DAQ",L"Lmt3",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_Lmt3 = atol(T2A(str));
	GetPrivateProfileString(L"DAQ",L"model",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_model = atoi(T2A(str));
	GetPrivateProfileString(L"DAQ",L"DisCntAlarm",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.Acquisition.m_DisCntAlarm = atoi(T2A(str));

	// 分档报警 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"GradeAlarm",L"number",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.DataFilter.m_number = atoi(T2A(str));
	for (int i = 0; i < theApp.m_userConfig.DataFilter.m_number; i++)
	{
		CString strW;
		strW.Format(L"File%d", i+1);
		GetPrivateProfileString(L"GradeAlarm",strW,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
		_tcscpy(theApp.m_userConfig.DataFilter.m_GAList[i], str);
	}

	// 坐标设置 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"ChartAxis",L"Imin",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Imin = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"Imax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Imax = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"Vmin",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Vmin = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"Vmax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Vmax = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"Pmin",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Pmin = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"Pmax",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_Pmax = atof(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"EnableXAuto",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_EnableXAuto = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"EnableYAuto",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_EnableYAuto = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideTitle",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideTitle = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideXTitle",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideXTitle = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideYTitle",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideYTitle = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideZTitle",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideZTitle = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideXGrid",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideXGrid = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideYGrid",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideYGrid = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hidePV",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hidePV = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideIRR",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideIRR = atoi(T2A(str));
	GetPrivateProfileString(L"ChartAxis",L"hideComm",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.ChartAxis.m_hideComm = atoi(T2A(str));

	// 保存和显示 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"SaveData",L"swAuto",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swAuto = atoi(T2A(str));

	GetPrivateProfileString(L"SaveData",L"SQLserver",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename); //改动
	theApp.m_userConfig.SaveData.m_SQLserver = atoi(T2A(str));

	GetPrivateProfileString(L"SaveData",L"swSerial",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSerial = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSNLength",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSNLength = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSerialBox",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSerialBox = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSaveJpg",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSaveJpg = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSaveDaq",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSaveDaq = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSaveCaliDaq",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSaveCaliDaq = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSavePara",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSavePara = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSaveParaMDB",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSaveParaMDB = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSerialUnique",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSerialUnique = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swSaveInClass",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swSaveInClass = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swBackInput",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swBackInput = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"fnstyle",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_fnstyle = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"paraE",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
 	theApp.m_userConfig.SaveData.m_eorc = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"swUpdateFnDate",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_swUpdateFnDate = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"DontClearInput",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_DontClearInput = atoi(T2A(str));
	GetPrivateProfileString(L"SaveData",L"auto",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_auto = atol(T2A(str));
	GetPrivateProfileString(L"SaveData",L"TotalTest",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_TotalTest = atol(T2A(str));
	GetPrivateProfileString(L"SaveData",L"fileSavePath",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_fileSavePath, str);
	GetPrivateProfileString(L"SaveData",L"manuf",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_manuf, str);
	GetPrivateProfileString(L"SaveData",L"ParaFileName",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_ParaFileName, str);
	GetPrivateProfileString(L"SaveData",L"InputBoxSerial",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_InputBoxSerial, str);

	//改动
	GetPrivateProfileString(L"SaveData",L"Serverip",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_serverip_this, str);	
	GetPrivateProfileString(L"SaveData",L"Database",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_database_this, str);	
	GetPrivateProfileString(L"SaveData",L"Username",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_username_this, str);	
	GetPrivateProfileString(L"SaveData",L"Password",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.SaveData.m_password_this, str);
	
  	theApp.m_userConfig.SaveData.m_area1 = theApp.m_userConfig.Calibration.m_area;          //朱峰改动
	theApp.m_userConfig.SaveData.m_areaeff1 = theApp.m_userConfig.Calibration.m_areaEff;

	// 工作目录文件夹不存在则创建
	if(!PathFileExists(theApp.m_userConfig.SaveData.m_fileSavePath))
	{
		CString m_PathDataStr;
		char* m_PathDataChar;
		m_PathDataStr = theApp.m_userConfig.SaveData.m_fileSavePath;
		m_PathDataChar = W2A(m_PathDataStr);
		MakeSureDirectoryPathExists(m_PathDataChar);
	}
	////////////////////////////改动///////////////////////////////////////////
	TCHAR m_Pathdaq[255] , m_Pathjpg[255];
	CString m_Pathdaqstr , m_Pathjpgstr;
	m_Pathdaqstr = theApp.m_userConfig.SaveData.m_fileSavePath;
	m_Pathjpgstr = theApp.m_userConfig.SaveData.m_fileSavePath;

	m_Pathdaqstr = m_Pathdaqstr+_T("daq");
	m_Pathjpgstr = m_Pathjpgstr+_T("jpg");

	_tcscpy(m_Pathdaq,m_Pathdaqstr);
	_tcscpy(m_Pathjpg,m_Pathjpgstr);

	CreateDirectoryW(m_Pathdaq, 0);
	CreateDirectoryW(m_Pathjpg, 0);
	//////////////////////////////////////////////////////////////////////////

	// 到文件列表
	GetPrivateProfileString(L"SaveData",L"fCount",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_fCount = atoi(T2A(str));

		for (int i = 0; i < 50; i++)
		{
			CString tStr = L"";
			tStr.Format(L"fNum%d", i);
			GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
			if (str != L"")
			{
				theApp.m_userConfig.SaveData.m_Tofile[i].id = atoi(T2A(str));
				tStr.Format(L"fVar%d", i);
				GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
				_tcscpy(theApp.m_userConfig.SaveData.m_Tofile[i].name, str);
			}
		}
	
	// 到显示列表
	GetPrivateProfileString(L"SaveData",L"dCount",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.SaveData.m_dCount = atoi(T2A(str));

		for (int i = 0; i < 50; i++)
		{
			CString tStr = L"";
			tStr.Format(L"dNum%d", i);
			GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
			if (str != L"")
			{
				theApp.m_userConfig.SaveData.m_ToDisplay[i].id = atoi(T2A(str));
				tStr.Format(L"dVar%d", i);
				GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
				_tcscpy(theApp.m_userConfig.SaveData.m_ToDisplay[i].name, str);
			}
		}
	
	///////////////////////////////改动  自定义////////////////////////////
		for (int i = 0; i < 20; i++)
		{
			CString tStr = L"";
			tStr.Format(L"dUSFied%d", i);
			GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
			if (str != L"")
			{
				_tcscpy(theApp.m_userConfig.SaveData.m_usToDisplay[i].fied, str);
				tStr.Format(L"dUSData%d", i);
				GetPrivateProfileString(L"SaveData",tStr,L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
				_tcscpy(theApp.m_userConfig.SaveData.m_usToDisplay[i].data, str);
			}
		}
	//////////////////////////////////////////////////////////////////////////
	// 标签打印 ///////////////////////////////////////////////////
	GetPrivateProfileString(L"PrintLabel",L"EnPrtLabel",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.PrintLabel.m_EnPrtLabel = atoi(T2A(str));
	GetPrivateProfileString(L"PrintLabel",L"sFilePath",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.PrintLabel.m_sFilePath, str);
	GetPrivateProfileString(L"PrintLabel",L"sFileName",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	_tcscpy(theApp.m_userConfig.PrintLabel.m_sFileName, str);
	GetPrivateProfileString(L"PrintLabel",L"saveCSV",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.PrintLabel.m_saveCSV = atoi(T2A(str));
	GetPrivateProfileString(L"PrintLabel",L"saveTXT",L"",str.GetBuffer(MAX_PATH),MAX_PATH,filename);
	theApp.m_userConfig.PrintLabel.m_saveTXT = atoi(T2A(str));

	return TRUE;
}


void CSetBase::OnEnChangeEditConfigfile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CSetInterFace::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	GetDlgItem(ID_SETBASE_APPLYUSERCONFIG)->EnableWindow(1);
	// TODO:  Add your control notification handler code here
}
