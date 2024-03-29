// Options\SetCurveIV.cpp : implementation file
//
#include "stdafx.h"

#include "..\stdafx.h"
#include "..\SolarCellTester.h"
#include "SetCurveIV.h"
#include "..\dirpath.h"
#include "..\ChartView\ChartView.h"
#include "..\SQLSet.h"

#define  paracnt 29
// CSetCurveIV dialog

IMPLEMENT_DYNAMIC(CSetCurveIV, CDialog)

CSetCurveIV::CSetCurveIV(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetCurveIV::IDD, pParent)
{
	m_state= TRUE;
	m_swloop=0;

	OpReg()->CreateKey(GetThisClassName());
	m_auto=(0)
	,m_area1 = (0)                                          //朱峰改动
	,m_areaeff1 = (0)
	, m_DontClearInput=(FALSE)
	, m_filepath=(_T(""))
	, m_fnstyle=(FALSE)
	, m_eorc = (FALSE)               //改动
	, m_swAuto=(FALSE)
	, m_swSerialBox=(FALSE)
	, m_swSerial=(FALSE)
	, m_swSNLength=(0)
	, m_swSavePara=(FALSE)
	, m_swSaveXLS
	, m_swSaveParaMDB=(FALSE)
	, m_swSerialUnique=(FALSE)
	, m_swSaveInClass=(FALSE)
	, m_swSaveJpg=(FALSE)
	, m_swSaveDaq=(FALSE)
	, m_swSaveCaliDaq=(FALSE)
	, m_swBackInput=(FALSE)
	, m_SQLserver = (FALSE);            //朱峰改动
	m_filepathkey=_T("m_filepathkey");
	m_CurveSetkey=_T("m_CurveSetkey");
	SaveDispKey=_T("SaveDispKey");
	SaveDispCountKey=_T("SaveDispCountKey");

	m_TotalTest=0;

	m_swUpdateFnDate=(FALSE);
	

	memset(m_ParaFileName,0,sizeof(m_ParaFileName));
	memset(m_manuf,0,sizeof(m_manuf));
	memset(m_InputBoxSerial,0,sizeof(m_InputBoxSerial));

	//朱峰改动
	memset(m_username_this,0,sizeof(m_username_this));    
	memset(m_password_this,0,sizeof(m_password_this));   
	memset(m_database_this,0,sizeof(m_database_this));
	memset(m_serverip_this,0,sizeof(m_serverip_this));



	m_usfied = _T("");      //改动
	m_usdata = _T("");      //改动

	//按顺序填写参数名字表
// 	Pair pair;
// 	UINT i=0;
// 	CString str;
// 	for(i=0;;i++)
// 	{
// 		str=GetParaString(0,i);
// 		if(str.IsEmpty())break;
// 		pair.id =i;
// 		pair.name =str;
// 		m_ParaPair.push_back(pair);
// 	}

}

CSetCurveIV::~CSetCurveIV()
{
}

void CSetCurveIV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_CHECK_SWAUTO,m_swAuto);
	DDX_Check(pDX,IDC_CHECK_SWSERIAL,m_swSerial);
	DDX_Check(pDX,IDC_CHECK_SAVEJPG,m_swSaveJpg);
	DDX_Check(pDX,IDC_CHECK_SAVEDAQ,m_swSaveDaq);
	DDX_Check(pDX,IDC_CHECK_SAVEDAQ2,m_swSaveCaliDaq);
	DDX_Check(pDX,IDC_CHECK_SAVEPARA,m_swSavePara);
	DDX_Check(pDX,IDC_CHECK_SAVEPARA2,m_swSaveParaMDB);
	DDX_Check(pDX,IDC_CHECK_SAVEPARAXLS,m_swSaveXLS);
	DDX_Check(pDX,IDC_CHECK_SERIALUNIQUE,m_swSerialUnique);
	DDX_Check(pDX,IDC_CHECK_SAVEINCLASS,m_swSaveInClass);
	DDX_Check(pDX,IDC_CHECK1,m_swSerialBox);
	DDX_Check(pDX,IDC_CHECK2,m_swUpdateFnDate);

	DDX_Check(pDX,IDC_CHECK_SWCLEAR,m_DontClearInput);
	DDX_Check(pDX,IDC_CHECK_SWCLEAR2,m_swBackInput);
	DDX_Check(pDX, IDC_CHECKSQL, m_SQLserver);   //朱峰改动


	DDX_Radio(pDX,IDC_RADIO1,m_fnstyle);
	DDX_Radio(pDX,IDC_RADIO_E,m_eorc);        //改动

	DDX_Text(pDX,IDC_COMBO1,m_usfied);  //改动
	DDX_Text(pDX,IDC_COMBO2,m_usdata);  //改动

	DDX_Control(pDX,IDC_COMBO1,m_usfiedB);  //改动
	DDX_Control(pDX,IDC_COMBO2,m_usdataB);  //改动


	CString str;

	str=m_manuf;
	DDX_Text(pDX,IDC_EDIT3,str);
	DDV_MaxChars(pDX,str,sizeof(m_manuf));
	_tcscpy(m_manuf,str);

	str=m_ParaFileName;
	DDX_Text(pDX,IDC_EDIT5,str);
	DDV_MaxChars(pDX,str,sizeof(m_ParaFileName));
	_tcscpy(m_ParaFileName,str);

	DDX_Text(pDX,IDC_EDIT_FILEPATH,m_filepath);
	DDV_MaxChars(pDX,m_filepath,MAX_PATH);

	DDX_Text(pDX,IDC_EDIT_AUTO,m_auto);
	DDX_Control(pDX, IDC_LIST2, m_SaveList);
	DDX_Control(pDX, IDC_LIST3, m_DispList);
	DDX_Control(pDX, IDC_LIST4, m_ParaList);



	DDX_Text(pDX,IDC_EDIT_SNLENGTH,m_swSNLength);
}


BEGIN_MESSAGE_MAP(CSetCurveIV, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetCurveIV::OnBnClickedButton2)
	ON_LBN_DBLCLK(IDC_LIST2, &CSetCurveIV::OnLbnDblclkList2)
	ON_LBN_DBLCLK(IDC_LIST3, &CSetCurveIV::OnLbnDblclkList3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSetCurveIV::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CSetCurveIV::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CSetCurveIV::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CSetCurveIV::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_SORT,&CSetCurveIV::OnBnClickedButtonSort)                                                 //朱峰改动
	ON_BN_CLICKED(IDC_BUTTON_ADDZ,&CSetCurveIV::OnBnClickedButtonAddUsfied)                                            //朱峰改动
	ON_BN_CLICKED(IDC_BUTTON_DELZ,&CSetCurveIV::OnBnClickedButtonDelUsfied)                                            //朱峰改动
	ON_BN_CLICKED(IDC_CHECKSQL, &CSetCurveIV::OnBnClickedCheckSQL)                       //朱峰改动


	ON_WM_DESTROY()
	ON_COMMAND(IDC_RADIO_C, &CSetCurveIV::OnradioC)   //改动
	ON_COMMAND(IDC_RADIO_E, &CSetCurveIV::OnradioE)   //改动
	ON_CBN_SELCHANGE(IDC_COMBO1,&CSetCurveIV::OnFiedSelchange)       //改动
	ON_CBN_SELCHANGE(IDC_COMBO2,&CSetCurveIV::OnDataSelchange)       //改动


END_MESSAGE_MAP()

void CSetCurveIV::OnradioC()
{
	m_ParaList.ResetContent();
	m_SaveList.ResetContent();
	m_DispList.ResetContent();
	m_ParaPair.clear();
	m_DispPair.clear();
	m_SavePair.clear();
	m_usfiedB.ResetContent();
	m_usdataB.ResetContent();
	AfxMessageBox(_T("Please restart the software to switch！"));
}
void CSetCurveIV::OnradioE()
{
	m_ParaList.ResetContent();
	m_SaveList.ResetContent();
	m_DispList.ResetContent();
	m_ParaPair.clear();
	m_DispPair.clear();
	m_SavePair.clear();
	m_usfiedB.ResetContent();
	m_usdataB.ResetContent();
	AfxMessageBox(_T("Please restart the software to switch！"));
}
// 选择目录或者路径
void CSetCurveIV::OnBnClickedButton2()
{
	BOOL ok=0;
	CDirPath dir;
	ok=dir.SelectFolder();
	UpdateData(1);         //改动
	if(ok!=0)
	{
		m_filepath=dir.GetSelectPath();
		UpdateData(0);
	}
	dir.CreateWorkDir(m_filepath+_T("daq"));
	dir.CreateWorkDir(m_filepath+_T("jpg"));
}

 #include   "shlwapi.h"   
#pragma   comment(lib,   "Shlwapi")

BOOL CSetCurveIV::OnInitDialog()
{
	//登记可管理能容
	CString str(MAKEINTRESOURCE(IDS_CSetCurveIVSaveReg));
	theApp.RegFunction(L"CSetCurveIV::SaveReg",str);

	str.LoadString(IDS_SetSaveData);
	theApp.RegFunction(L"CSetCurveIV::SaveData",str);

	str.LoadString(IDS_SetSerialNumber);
	theApp.RegFunction(L"CSetCurveIV::BuidSerial",str);

	str.LoadString(IDS_SetParameterList);
	theApp.RegFunction(L"CSetCurveIV::ParaList",str);


	if(theApp.IsDisable(L"CSetCurveIV::SaveReg"))
		DisableAllSubCtrl(this);

	if (theApp.IsDisable(L"CSetCurveIV::SaveData"))
	{
		GetDlgItem(IDC_CHECK_SAVEPARA)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVEDAQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVEPARA2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVEDAQ2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVEINCLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVEJPG)->EnableWindow(FALSE);
	}
	if (theApp.IsDisable(L"CSetCurveIV::BuidSerial"))
	{
		GetDlgItem(IDC_CHECK_SWSERIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SWAUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SWCLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SWCLEAR2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SERIALUNIQUE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SNLENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AUTO)->EnableWindow(FALSE);
	}
	if (theApp.IsDisable(L"CSetCurveIV::ParaList"))
	{
		GetDlgItem(IDC_LIST4)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADDZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELZ)->EnableWindow(FALSE);
	}

		//CButton* pBtnxls = (CButton*)GetDlgItem(IDC_CHECK_SAVEPARAXLS);
		SetCurExeDir();
		if(PathFileExists(_T("XLS.gs")))
		{
			GetDlgItem(IDC_CHECK_SAVEPARAXLS)->ShowWindow(TRUE);	
		}
		else
		{
			GetDlgItem(IDC_CHECK_SAVEPARAXLS)->ShowWindow(FALSE);
		}


	CDialog::OnInitDialog();//先于控件操作
	LoadReg();
	return TRUE;
}
void CSetCurveIV::Serialize(CArchive& ar)
{
}
//双击删除SaveList条目
void CSetCurveIV::OnLbnDblclkList2()
{
	UINT index=m_SaveList.GetCurSel();
	m_SaveList.DeleteString(index);
	if(m_SavePair.size()>=index)
		m_SavePair.erase(m_SavePair.begin()+index);
}
//双击删除DispList条目
void CSetCurveIV::OnLbnDblclkList3()
{
	UINT index=m_DispList.GetCurSel();
	m_DispList.DeleteString(index);
	if(m_DispPair.size()>=index)
		m_DispPair.erase(m_DispPair.begin()+index);
}

extern CString SignificanceDigitString(double value,int n);
CString SignificanceDigitStringEx(double value,int n)
{
	CString str;
	if(n>0&&n<20)
	{
		str =  SignificanceDigitString( value, n );
	}
	else
	{
		str.Format(_T("%lf"),value);
	}
	if (str.Left(3) == _T("1.#") || str.Left(4) == _T("-1.#") || str.Right(5) == _T("e+308")  
		|| str.Right(5) == _T("e-308") || str.Right(5) == _T("e+324")  || str.Right(5) == _T("e-324"))        // 变量非正常值则值为0
	{
		str = _T("0");
	}
	return str;
}

CString CSetCurveIV::SignificanceParam(int mdot,double value)
{
	CString str;
	switch(mdot)
	{
	case 1:
			str.Format(_T("%.1f"),value);
			break;
	case 2:
			str.Format(_T("%.2f"),value);
			break;
	case 3:
			str.Format(_T("%.3f"),value);
			break;
	case 4:
			str.Format(_T("%.4f"),value);
			break;
	case 5:
			str.Format(_T("%.5f"),value);
			break;
	case 6:
			str.Format(_T("%.6f"),value);
			break;
	}


	return str;
}
//pList==NULL返回参数名字
//转换参数到字符串，或参数名字
CString CSetCurveIV::GetParaString(CSunPara* pList,UINT id,int n)
{
	CString str;
	if(pList)
	switch(id){
	case 0:str=pList->GetDateTime()->Format(_T("%Y/%m/%d"));break;
	case 1:str=pList->GetSerial();break;
	case 2:str=SignificanceDigitStringEx(pList->GetEff(),n);break;
	case 3:str=SignificanceDigitStringEx(pList->GetRsh(),n);break;
	case 4:str=SignificanceDigitStringEx(pList->GetRs(),n);break;
	case 5:str=SignificanceParam(3,pList->GetFF()*g_FFPrecScale);break;//方度
	case 6:str=SignificanceDigitStringEx(pList->GetIsc(),n);break;//短路电流
	case 7:str=SignificanceDigitStringEx(pList->GetVoc(),n);break;//开路电压
	case 8:str=SignificanceDigitStringEx(pList->GetImax(),n);break;//最大功率点电流
	case 9:str=SignificanceDigitStringEx(pList->GetVmax(),n);break;//最大功率点电压
	case 10:str=SignificanceDigitStringEx(pList->GetPmax(),n);break;//最大功率点
	case 11:str=SignificanceDigitStringEx(pList->GetTemp(),n);break;//参数温度
	case 12:str=SignificanceDigitStringEx(pList->GetEnvTemp(),n);break;//环境温度
	case 13:str=SignificanceDigitStringEx(pList->GetSurfTemp(),n);break;//表面温度
	case 14:str=SignificanceDigitStringEx(pList->GetSun(),n);break;//光照度
	case 15:str=SignificanceDigitStringEx(pList->GetSunRef(),n);break;//参考电池电流
	case 16:str=SignificanceDigitStringEx(pList->GetIrev(),n);break;//漏电流
	case 17:str=SignificanceDigitStringEx(pList->GetIdot(),n);break;//定点值
	case 18:str=SignificanceDigitStringEx(pList->GetVdot(),n);break;
	case 19:str=SignificanceDigitStringEx(pList->GetStdCellIscTesting(),n);break;//标准参考电池的实测Is
	case 20:str=m_manuf;break;
	case 21:str=SignificanceDigitStringEx(pList->GetPdot(),n);break;//定点值
	case 22:str=pList->GetGrade();break;
	case 23:str =SignificanceDigitStringEx(GetArea1(),n);break;                          
	case 24:str=pList->GetDateTime()->Format(_T("%H:%M:%S"));break;
// 	case 25:str=SignificanceDigitStringEx(pList->GetStdCellIscTesting1(),n);break;
// 	case 26:str=SignificanceDigitStringEx(pList->GetStdCellsunTesting1(),n);break;            
// 	case 27:str=SignificanceDigitStringEx(pList->GetStdCellIscTesting2(),n);break;            
// 	case 28:str=SignificanceDigitStringEx(pList->GetStdCellsunTesting2(),n);break;            
	case 25:str=SignificanceDigitStringEx(pList->GetJsc(),n);break;            
	case 26:str=SignificanceDigitStringEx(pList->GetRCTemp(),n);break;
	case 27:str=pList->GetSTime().TrimRight(_T("ms"));break;
	case 28:str=pList->GetSDirection();break;
	case 29:str=pList->GetIrrCali();break;
//	case 24:str=SignificanceDigitStringEx(GetAreaEff1(),n);break;
	default:str=_T("");
	}
	else if(m_eorc)
	{
		switch(id){
		case 0:str=_T("测试日期");break;//datetime与sql关键字类型有冲突
		case 1:str=_T("序列号");break;
		case 2:str=_T("转换效率");break;//转换效率
		case 3:str=_T("并联电阻");break;//并联电阻
		case 4:str=_T("串联电阻");break;//串联电阻
		case 5:str=_T("方度");break;//方度
		case 6:str=_T("短路电流");break;//短路电流
		case 7:str=_T("开路电压");break;//开路电压
		case 8:str=_T("最大功率点电流");break;//最大功率点电流
		case 9:str=_T("最大功率点电压");break;//最大功率点电压
		case 10:str=_T("最大功率");break;//最大功率点
		case 11:str=_T("Temp");break;//参数温度
		case 12:str=_T("环境温度");break;//环境温度
		case 13:str=_T("表面温度");break;//表面温度
		case 14:str=_T("光照度");break;//光照度
		case 15:str=_T("参考电池电流");break;//参考电池电流
		case 16:str=_T("漏电流");break;//漏电流
		case 17:str=_T("定点电流");break;//定点电流
		case 18:str=_T("定点电压");break;//定点电压
		case 19:str=_T("标准参考电池的实测短路电流");break;//标准参考电池的实测Isc
		case 20:str=_T("单位");break;
		case 21:str=_T("定点功率");break;        //定点功率
		case 22:str=_T("档位");break;     //档位
		case 23:str=_T("单片电池面积");break;                  //朱峰改动
		case 24:str=_T("测试时间");break;
// 		case 25:str=_T("StdIsc1");break;
// 		case 26:str=_T("Stdsun1");break;
// 		case 27:str=_T("StdIsc2");break;
// 		case 28:str=_T("Stdsun2");break;
		case 25:str=_T("电流密度");break;
		case 26:str=_T("监测电池温度");break;
		case 27:str=_T("扫描时间");break;
		case 28:str=_T("扫描方向");break;
		case 29:str=_T("辐照度是否修正");break;
//		case 24:str=_T("面积(计算效率)");break;
		default:str=_T("");
		}

	}else
	switch(id){
	case 0:str=_T("Test_Date");break;//datetime与sql关键字类型有冲突
	case 1:str=_T("ID");break;
	case 2:str=_T("ModEff");break;
	case 3:str=_T("Rsh");break;
	case 4:str=_T("Rs");break;
	case 5:str=_T("FF");break;//方度
	case 6:str=_T("Isc");break;//短路电流
	case 7:str=_T("Voc");break;//开路电压
	case 8:str=_T("Ipm");break;//最大功率点电流
	case 9:str=_T("Vpm");break;//最大功率点电压
	case 10:str=_T("Pmax");break;//最大功率点
	case 11:str=_T("Temp");break;//参数温度
	case 12:str=_T("EnvTemp");break;//环境温度
	case 13:str=_T("TMod");break;//表面温度
	case 14:str=_T("Insol");break;//光照度
	case 15:str=_T("SunRef");break;//参考电池电流
	case 16:str=_T("Irev");break;//漏电流
	case 17:str=_T("Ivld");break;//定点值
	case 18:str=_T("Vld");break;
	case 19:str=_T("StdIsc");break;//标准参考电池的实测Isc
	case 20:str=_T("Manuf");break;
	case 21:str=_T("Pvld");break;
	case 22:str=_T("ModType");break;
	case 23:str=_T("Area");break;                  //朱峰改动
	case 24:str=_T("Test_Time");break;                  //朱峰改动
// 	case 25:str=_T("StdIsc1");break;
// 	case 26:str=_T("Stdsun1");break;//改动
// 	case 27:str=_T("StdIsc2");break;//改动
// 	case 28:str=_T("Stdsun2");break;//改动
	case 25:str=_T("Jsc");break;//改动
	case 26:str=_T("RCTemp");break;
	case 27:str=_T("STime");break;
	case 28:str=_T("SDirection");break;
	case 29:str=_T("IrrCali");break;
//	case 24:str=_T("Cell_Eff(%)");break;
	default:str=_T("");
	}
	return str;
}

CString CSetCurveIV::GetParaString(UINT id)
{
/*
* Accesss数据库SQL支持的数据类型
* INTEGER（整型）、LONG（长整型）、 SINGLE（单精度浮点数）、DOUBLE（双精度浮点数）、DATETIME（日期型，也可以写成DATE）、
* BIT（布尔型）、 TEXT（字符串型，最大255个字节）、MEMO（字符串型，最大可达1.2G字节）、 
* COUNTER（自动递增长整型，可确定记录的唯一性）、CURRENCY（货币型，精确到小数点左边15位，右边4位）、
* BINARY（字节型，最大255个）、LONGBINARY（用于OLE对象）、GUID（全局唯一标识符）。
*/
	CString str;
	switch(id){
	case 0:str=_T("TEXT");break;
	case 1:str=_T("TEXT");break;
	case 2:str=_T("FLOAT");break;
	case 3:str=_T("FLOAT");break;
	case 4:str=_T("FLOAT");break;
	case 5:str=_T("FLOAT");break;//方度
	case 6:str=_T("FLOAT");break;//短路电流
	case 7:str=_T("FLOAT");break;//开路电压
	case 8:str=_T("FLOAT");break;//最大功率点电流
	case 9:str=_T("FLOAT");break;//最大功率点电压
	case 10:str=_T("FLOAT");break;//最大功率点
	case 11:str=_T("FLOAT");break;//参数温度
	case 12:str=_T("FLOAT");break;//环境温度
	case 13:str=_T("FLOAT");break;//表面温度
	case 14:str=_T("FLOAT");break;//光照度
	case 15:str=_T("FLOAT");break;//参考电池电流
	case 16:str=_T("FLOAT");break;//漏电流
	case 17:str=_T("FLOAT");break;//定点值
	case 18:str=_T("FLOAT");break;
	case 19:str=_T("FLOAT");break;//标准参考电池的实测Isc
	case 20:str=_T("TEXT");break;
	case 21:str=_T("FLOAT");break;
	case 22:str=_T("TEXT");break;
	case 23:str=_T("FLOAT");break;                   
	case 24:str=_T("TEXT");break;
// 	case 25:str=_T("FLOAT");break;
// 	case 26:str=_T("FLOAT");break;                       
// 	case 27:str=_T("FLOAT");break;                       
// 	case 28:str=_T("FLOAT");break;                       
	case 25:str=_T("FLOAT");break;                      
	case 26:str=_T("FLOAT");break;
	case 27:str=_T("TEXT");break;
	case 28:str=_T("TEXT");break;
	case 29:str=_T("TEXT");break;
	default:str=_T("");
	}
	return str;
}

void CSetCurveIV::LabelCSV(CSunPara* pList,CString fn)       //改动   写汉字的方法
{
	CString str=_T(""),line1,line2;
	if(m_SavePair.empty())
		return;
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	{
		if (it->id > paracnt)
			str = GetTEXTZDY(it->id);                 //改动 自定义
		else
			str=GetParaString(0,it->id);
		if(str.IsEmpty())
			break;
		line1+=str+_T(",");                      
		if(it->id > paracnt)								//改动 自定义
			line2+=GetvalueZDY(it->id)+_T(",");
		else
			line2+=GetParaString(pList,it->id)+_T(",");
	}
	line1+=+_T("\n");
	line2+=+_T("\n");

	CFile file;
	if(file.Open(fn,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		WriteUicodeBOM(file);
		file.Write(line1.GetBuffer(),line1.GetLength()*sizeof(TCHAR));
		file.Write(line2.GetBuffer(),line2.GetLength()*sizeof(TCHAR));
		file.SetLength(file.GetPosition());//改写文件尺寸
		file.Flush();
		file.Close();
	}
}

// 保存TXT格式Label标记
void CSetCurveIV::LabelTXT(CSunPara* pList,CString fn)
{
	CString str=_T(""),line1,line2;
	if(m_SavePair.empty())
		return;
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	{
		if (it->id > paracnt)
			str = GetTEXTZDY(it->id);                 //改动 自定义
		else
			str=GetParaString(0,it->id);
		if(str.IsEmpty())
			break;
		line1+=str+_T(",");
		if(it->id > paracnt)								//改动 自定义
			line2+=GetvalueZDY(it->id)+_T(",");
		else
			line2+=GetParaString(pList,it->id)+_T(",");
	}
	line1+=+_T("\n");
	line2+=+_T("\n");

	CFile file;

	// 写入二进制格式，字符格式为ASCII
	USES_CONVERSION; 

	if(!PathFileExists(fn))
	{
		if(file.Open(fn,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
		{
			CStringA str1(line1);
			file.Write(str1.GetBuffer(),str1.GetLength()-1);
			file.Write(("\r\n"),2);
			//			str += '\0';
			CStringA str2(line2);
			file.Write(str2.GetBuffer(),str2.GetLength()-1);
			file.Write(("\r\n"),2);
		}
	}
	else 
	{
		DeleteFileW(fn);
		if(file.Open(fn,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
		{
			CStringA str1(line1);
			file.Write(str1.GetBuffer(),str1.GetLength()-1);
			file.Write(("\r\n"),2);
			CStringA str2(line2);
			file.Write(str2.GetBuffer(),str2.GetLength()-1);
			file.Write(("\r\n"),2);
		}
	}
	if(file.m_hFile!=INVALID_HANDLE_VALUE)
		file.Close();
}
void CSetCurveIV::OnBnClickedButtonSort()                              //朱峰改动排序函数
{
	m_DispPair.clear();
	m_SavePair.clear();
	Pair pairD, pairS;
	CString strD,strS;
	for(int i=0;i<m_DispList.GetCount();i++)             //改动  自定义
	{
		m_DispList.GetText(i,strD);
		if(strD.IsEmpty())break;
		pairD.id = 0;
		for(int m = 0; ;m++)
		{
			CString strtempD;
			if (m > paracnt)
			{
				strtempD = GetTEXTZDY(m);
			}else
				strtempD = GetParaString(0,m);

			if (strD.CompareNoCase(strtempD) != 0)
			{

			}else
			{
				pairD.id = m;
				break;
			}
		}
		pairD.name =strD;
		m_DispPair.push_back(pairD);
 	}

	for(int z=0;z<m_SaveList.GetCount();z++)
	{
		m_SaveList.GetText(z,strS);
		if(strS.IsEmpty())break;
		pairS.id = 0;
		for(int m = 0; ;m++)
		{
			CString strtempS;
			if (m > paracnt)
			{
				strtempS = GetTEXTZDY(m);
			}else
				strtempS = GetParaString(0,m);

			if (strS.CompareNoCase(strtempS) != 0)
			{

			}else
			{
				pairS.id = m;
				break;
			}
		}
		pairS.name =strS;
		m_SavePair.push_back(pairS);
	}

}
//清除Save列表
void CSetCurveIV::OnBnClickedButton4()
{
	m_SaveList.ResetContent();
	m_SavePair.clear();
}
//清除Disp列表
void CSetCurveIV::OnBnClickedButton6()
{
	m_DispList.ResetContent();
	m_DispPair.clear();
}
CString CSetCurveIV::BuildDisplay(CSunPara* plist)        //改动  数据左移
{
	CString out=_T("");
	if(m_DispPair.empty())
		return out;
	CString str;
	PairList::iterator it;
	it=m_DispPair.begin();
	for(;it<m_DispPair.end();it++)
	{
		int len = 0;
		if (m_eorc)		
		    len = it->name.GetLength()*sizeof(TCHAR);
		else
			len = it->name.GetLength();

		str = _T("");
		if(len>=8)
			str=_T(":");
		else
		{
			int i = 8-len;
			while(i)
			{
				str +=_T(" ");
				i--;
			}
			str += _T(":");

		}
		if (it->id > paracnt)         //改动  自定义
		{
			out+=it->name+str+_T("  ")+GetvalueZDY(it->id)+_T("\n");    //改动 数据左移
		}
		else
			out+=it->name+str+_T("  ")+GetParaString(plist,it->id)+_T("\n");    //改动 数据左移
	}
	return out;
}
CString CSetCurveIV::BuildSaveTitle(CSunPara* plist)
{
	CString out=_T("");
	if(m_SavePair.empty())
		return out;
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
		out+=it->name+_T(",");
	return out+_T("\n");
}
CString CSetCurveIV::BuildSaveItem(CSunPara* plist)
{
	CString out=_T("");
	if(m_SavePair.empty())
		return out;
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
		if (it->id > paracnt)
			out+=GetvalueZDY(it->id)+_T(",");  //自定义 改动
		else
			out+=GetParaString(plist,it->id,0)+_T(",");
	return out+_T("\n");
}
//生成用于SQL的字段信息
CString CSetCurveIV::BuildField(CSunPara* plist,int type)
{
	CString out=_T("(");
	if(m_SavePair.empty())
		return out+_T(")");
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	{
		if (it->id > paracnt)              //改动 自定义
			out+=_T("[")+GetTEXTZDY(it->id)+_T("]");//字段需[]，避免和关键字冲突
		else
			out+=_T("[")+GetParaString(0,it->id)+_T("]");//字段需[]，避免和关键字冲突
		if(type)
		{
			if (it->id > paracnt)
				out+=_T(" ")+GetFormatZDY();//数据类型
			else
				out+=_T(" ")+GetParaString(it->id);//数据类型
		}
		out+=_T(",");
	}
	out.TrimRight(_T(","));
	return out+_T(")");
}
//生成用于SQL的条目信息
CString CSetCurveIV::BuildItem(CSunPara* plist)
{
	CString out=_T("("),str,type;
	if(m_SavePair.empty())
		return out+_T(")");
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	{
		if (it->id > paracnt)           //改动 自定义
		{
			str=GetvalueZDY(it->id);
			type=GetFormatZDY().Left(4);
		}
		else
		{
			str=GetParaString(plist,it->id,0);
			type=GetParaString(it->id).Left(4);
		}
		if(type.CompareNoCase(_T("TEXT"))==0||type.CompareNoCase(_T("DATE"))==0)
			str=_T("'")+str+_T("'");
		out+=str+_T(",");
	}
	out.TrimRight(_T(","));
	return out+_T(")");
}

//参数表存储文件名不是连续更新，每文件中存储多组数据
CString CSetCurveIV::BuildParaFileName(CSunPara* plist)
{
	CString name(m_ParaFileName);
	//每天一个新文件名
	if(m_swUpdateFnDate){
		COleDateTime ole;
		ole=COleDateTime::GetCurrentTime();
		CString newn;
		newn=ole.Format(_T("%Y-%m-%d"));
		if(name!=newn){
			memset(m_ParaFileName,0,sizeof(m_ParaFileName));
			_tcscpy(m_ParaFileName,newn);
			name=newn;
		}
	}
	//全部没有选择，给出缺省文件名
	if(name.IsEmpty())
		name=_T("None Name");
	//如果1、2没有选择，按输入的名字为准
	UpdateData(FALSE);
	return m_filepath+name;
}

//参数表存储文件名不是连续更新，每文件中存储多组数据
CString CSetCurveIV::BuildParaClassFileName(CSunPara* plist)
{
	CString name(L"");
	// 缺省分档文件名称：分档依据_档位名
// 	switch (theApp.m_opt.GetSetParaAlarm()->GetClassType())
// 	{
// 	case 0: name = L"Idot_" + plist->GetGrade(); break;
// 	case 1: name = L"Pmax" + plist->GetGrade(); break;
// 	case 2: name = L"Eff" + plist->GetGrade(); break;
// 	case 3: name = L"Imax"+ plist->GetGrade(); break;
// 	default: name = L""; break;
// 	}
	name = m_filepath + _T("GRADE_") + plist->GetGrade();
	return name;
}

//无后缀，按日期时间和按序号取名
CString CSetCurveIV::BuildFileName(CSunPara* plist)
{
	CString out=_T("");
	////DateTime
	if(m_fnstyle)
	{
		out=plist->GetDateTime()->Format(_T("%Y-%m-%d %H-%M-%S"));
	}
	else
	{//Serial
		out=GetParaString(plist,SERIAL);
	}
	if(out==_T(""))
		return out;
	return out;
}
#include "..\InputBox.h"
void CSetCurveIV::InputSerial(void)
{
	CInputBox in;
	if(m_DontClearInput)
		in.SetString(m_InputBoxSerial);
	in.SetEnableLoop(m_swloop);
	if(m_swSerialBox)
	{
		if(in.DoModal()==IDOK)
		{
			// 序列号长度截取
			if (m_swSNLength != 0)
			{
				_tcscpy(m_InputBoxSerial,in.GetString().Left(m_swSNLength));
			}
			else
				_tcscpy(m_InputBoxSerial,in.GetString().Left(30));
			m_state= TRUE;
			m_swloop=in.GetEnableLoop();
		}
		else
		{
			_tcscpy(m_InputBoxSerial,_T(""));
			m_state= FALSE;
			m_swloop=in.GetEnableLoop();
		}
	}
}
//生成序列号
CString CSetCurveIV::BuildSerial(CSunPara* plist)
{
	CString out=_T("");
	m_TotalTest++;
	if(m_swSerial)
		out=out+m_InputBoxSerial;
	if(m_swAuto){
		m_auto++;
		CString str;
		str.Format(_T("%05d"),m_auto);
		out+=str;
	}
	return out;
}

//表里有相同的项目返回false
bool CSetCurveIV::ExsitPair(PairList* pl,Pair& p)
{
	if(pl->empty())
		return true;
	bool ret=true;
	PairList::iterator it;
	it=pl->begin();
	for(;it<pl->end();it++)
		if(it->id==p.id&&it->name==p.name){
			ret=false;
			break;
		}
	return ret;
}
//添加到存储列表
void CSetCurveIV::OnBnClickedButton5()
{
	if ( -1 == m_ParaList.GetCurSel())
	{
		return;
	}
	for ( int index = 0; index< m_ParaList.GetCount(); index++)
	{
		if (m_ParaList.GetSel(index))
		{
			PairList::iterator it=m_ParaPair.begin()+index;
			if(ExsitPair(&m_SavePair,*it))
			{
				m_SavePair.push_back(*it);
				m_SaveList.AddString(it->name);
			}
		}
	}
}
//添加到显示列表
void CSetCurveIV::OnBnClickedButton3()
{
	if ( -1 == m_ParaList.GetCurSel())
	{
		return;
	}
	for ( int index = 0; index< m_ParaList.GetCount(); index++)
	{
		if (m_ParaList.GetSel(index))
		{
			PairList::iterator it=m_ParaPair.begin()+index;
			if(ExsitPair(&m_DispPair,*it)){
				m_DispPair.push_back(*it);
				m_DispList.AddString(it->name);
			}
		}
	}
}

//有些输入也需要自动保存，例如：输入序列号
void CSetCurveIV::OnDestroy()
{
	CurveSet* set=this;
	OpReg()->Write(m_CurveSetkey,(byte*)set,sizeof(CurveSet));
	__super::OnDestroy();
}

BOOL CSetCurveIV::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
    Apply(wParam);    // 判断控件是否更改
	return __super::OnCommand(wParam, lParam);
}

void CSetCurveIV::LoadReg(void)
{
	/*	CurveSet* set=this;
	OpReg()->Read(m_CurveSetkey,(byte*)set,sizeof(CurveSet));
	CString dir;
	OpReg()->Read(m_filepathkey,dir);
	if(1==PathFileExists(dir)){
	m_filepath=dir;
	}else{
	CDirPath d;
	m_filepath=d.GetExePath();
	}
	size_t count[2];
	OpReg()->Read(SaveDispCountKey,(byte*)count,sizeof(count));
	if(count[0]>m_ParaPair.size()||count[1]>m_ParaPair.size()||(count[0]+count[1])==0){
	UpdateData(0);
	return ; 
	}
	size_t size=(count[0]+count[1]);
	std::vector<UINT>block(size);
	std::vector<UINT>::iterator iter=block.begin();
	OpReg()->Read(SaveDispKey,(byte*)&(*iter),block.size()*sizeof(UINT));
	Pair pair;
	size_t i=0;
	for(;i<count[0];i++)
	if(block[i]<m_ParaPair.size()){
	m_SavePair.push_back(m_ParaPair.at(block[i]));
	m_SaveList.AddString(m_ParaPair.at(block[i]).name);
	}
	for(;i<size;i++)
	if(block[i]<m_ParaPair.size()){
	m_DispPair.push_back(m_ParaPair.at(block[i]));
	m_DispList.AddString(m_ParaPair.at(block[i]).name);
	}

	block.clear();
	*/

	CurveSet* curveSet=this;

	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo5.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive)){
		file.Read(curveSet,sizeof(CurveSet));
		file.Close();
	}
	else
		return;

	m_SavePair.clear();
	m_DispPair.clear();


	for(int i = 0; i < curveSet->m_Tofile[i].name[0] != 0; i++)
	{
		Pair pair;
		pair.id = curveSet->m_Tofile[i].id;
		pair.name = curveSet->m_Tofile[i].name;
		m_SavePair.push_back(pair);
		m_SaveList.AddString(pair.name);
	}

	for(int i = 0; i < curveSet->m_dCount; i++)
	{
		Pair pair;
		pair.id = curveSet->m_ToDisplay[i].id;
		pair.name = curveSet->m_ToDisplay[i].name;
		m_DispPair.push_back(pair);
		m_DispList.AddString(pair.name);
	}

	UpdateData(0);
}
void CSetCurveIV::SaveReg(void)
{
	/*	UpdateData(1);
	CurveSet* set=this;
	OpReg()->Write(m_CurveSetkey,(byte*)set,sizeof(CurveSet));
	OpReg()->Write(m_filepathkey,m_filepath);

	size_t count[2]={m_SavePair.size(),m_DispPair.size()};
	OpReg()->Write(SaveDispCountKey,(byte*)count,sizeof(count));
	if((count[0]+count[1])==0)
	return;

	size_t size=(count[0]+count[1]);
	std::vector<UINT>block(size);
	std::vector<UINT>::iterator iter=block.begin();
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	*iter++=it->id;
	it=m_DispPair.begin();
	for(;it<m_DispPair.end();it++)
	*iter++=it->id;
	iter=block.begin();
	OpReg()->Write(SaveDispKey,(byte*)&(*iter),block.size()*sizeof(UINT));
	block.clear();
	*/
	UpdateData();
	CurveSet* curveSet=this;

	size_t count[2]={m_SavePair.size(),m_DispPair.size()};
	m_fCount = (int)count[0];
	m_dCount = (int)count[1];
	PairList::iterator it;
	it=m_SavePair.begin();
	for(;it<m_SavePair.end();it++)
	{
		curveSet->m_Tofile[it->id].id = it->id;
		_tcscpy(curveSet->m_Tofile[it->id].name, it->name);
	}

	it=m_DispPair.begin();
	for(;it<m_DispPair.end();it++)
	{
		curveSet->m_ToDisplay[it->id].id = it->id;
		_tcscpy(curveSet->m_ToDisplay[it->id].name, it->name);
	}

	// 保存配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo5.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		file.Write(curveSet, sizeof(CurveSet));
		file.Close();
	}

	/*	it=m_DispPair.begin();
	for(;it<m_DispPair.end();it++)
	*iter++=it->id;
	iter=block.begin();
	OpReg()->Write(SaveDispKey,(byte*)&(*iter),block.size()*sizeof(UINT));
	block.clear();
	*/	
}

CurveSet* CSetCurveIV::GetAppliedConfig(void)
{
	UpdateData();
	CurveSet* curveSet=this;

	_tcscpy(curveSet->m_fileSavePath, m_filepath); 

	PairList::iterator it;

	for (int i = 0; i < 50; i++)
	{
		_tcscpy(curveSet->m_Tofile[i].name, L"");
	}
	it=m_SavePair.begin();
	m_fCount = 0;
	for(int n = 0;(n < 50) && (it<m_SavePair.end());n++,it++)                                          //朱峰改动
	{
		m_fCount++;
		curveSet->m_Tofile[n].id = it->id;
		_tcscpy(curveSet->m_Tofile[n].name, it->name);
	}

	for (int i = 0; i < 50; i++)
	{
		_tcscpy(curveSet->m_ToDisplay[i].name, L"");
	}
	it=m_DispPair.begin();
	m_dCount = 0;
	for(int m = 0;(m < 50) && (it<m_DispPair.end());m++,it++)                                                        //朱峰改动
	{
		m_dCount++;
		curveSet->m_ToDisplay[m].id = it->id;
		_tcscpy(curveSet->m_ToDisplay[m].name, it->name);
	}
    ////////////////////////自定义  改动////////////////////////////////
	for(int i = 0;i < 20; i++)
	{
		_tcscpy(curveSet->m_usToDisplay[i].fied ,_T(""));
		_tcscpy(curveSet->m_usToDisplay[i].data ,_T(""));
	}
	for(int m = 0, n = 0; (m < 20) && (n < m_usfiedB.GetCount());m++,n++)
	{
		m_usfiedB.GetLBText(n,curveSet->m_usToDisplay[m].fied);
		m_usdataB.GetLBText(n,curveSet->m_usToDisplay[m].data);
	}
	//////////////////////////////////////////////////////////////////////////
	return curveSet;
}

void CSetCurveIV::SetAppliedConfig(CurveSet* tCurveSet)
{
	CurveSet* curveSet=this;
	memcpy(curveSet, tCurveSet, sizeof(CurveSet));
///////////////////改动///////////////////////////////////////////////////
  //按顺序填写参数名字表
	m_ParaPair.clear();
	m_ParaList.ResetContent();
	Pair pair1;
	UINT i=0;
	CString str;
	for(i=0;;i++)
	{
		str=GetParaString(0,i);
		if(str.IsEmpty())
			break;
		pair1.id =i;
		pair1.name =str;
		m_ParaPair.push_back(pair1);
	}
	PairList::iterator it;
	it=m_ParaPair.begin();
	for(;it<m_ParaPair.end();it++)
		m_ParaList.AddString(it->name);
//////////////////////////////////////////////////////////////////////////
	m_SavePair.clear();
	m_SaveList.ResetContent();
	m_DispPair.clear();
	m_DispList.ResetContent();

	m_filepath = curveSet->m_fileSavePath; 

	Pair pair;
	for(int i = 0; i < 50; i++)
	{
		// 到文件列表
		if (curveSet->m_Tofile[i].name[0] != 0)
		{
			pair.id = curveSet->m_Tofile[i].id;
			pair.name = curveSet->m_Tofile[i].name;
			m_SavePair.push_back(pair);
			m_SaveList.AddString(pair.name);
		}
	}
	for(int i = 0; i < 50; i++)
	{
		// 到显示列表
		if (curveSet->m_ToDisplay[i].name[0] != 0)
		{
			pair.id = curveSet->m_ToDisplay[i].id;
			pair.name = curveSet->m_ToDisplay[i].name;
			m_DispPair.push_back(pair);
			m_DispList.AddString(pair.name);
		}
	}
	////////////////////////////改动  自定义////////////////////////////////
	m_usfiedB.ResetContent();
	m_usdataB.ResetContent();
	for(int i = 0; i < 20; i++)
	{
		if (curveSet->m_usToDisplay[i].fied[0] != 0)
		{
			m_usfiedB.AddString(curveSet->m_usToDisplay[i].fied);
			m_usdataB.AddString(curveSet->m_usToDisplay[i].data);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	UpdateData(0);

}

void CSetCurveIV::SetCurveIVDefault()                               //朱峰改动              恢复默认
{
	CurveSet* default_curveSet=this;
		default_curveSet->m_area1 = (0);                                     
		default_curveSet->m_areaeff1 = (0);
		default_curveSet-> m_swAuto=(FALSE);
		default_curveSet->m_swSerial=(FALSE);
		default_curveSet-> m_swSNLength=(FALSE);
		default_curveSet-> m_swSerialBox=(FALSE);
		default_curveSet-> m_swSaveJpg=(FALSE);
		default_curveSet->m_swSaveDaq=(FALSE);
		default_curveSet->m_swSaveCaliDaq=(FALSE);
		default_curveSet->m_swSavePara=(FALSE);
		default_curveSet->m_swSaveParaMDB=(FALSE);
		default_curveSet-> m_swSerialUnique=(FALSE);
		default_curveSet-> m_swSaveInClass=(FALSE);
		default_curveSet->m_swBackInput=(FALSE);
		default_curveSet->m_fnstyle=(FALSE);
		default_curveSet->m_swUpdateFnDate = (FALSE);
		default_curveSet->m_DontClearInput=(FALSE);
		default_curveSet->m_auto=(0);
		default_curveSet->m_TotalTest = (0);
	memset(default_curveSet->m_fileSavePath,0,sizeof(default_curveSet->m_fileSavePath));
	memset(default_curveSet->m_ParaFileName,0,sizeof(default_curveSet->m_ParaFileName));
	memset(default_curveSet->m_manuf,0,sizeof(default_curveSet->m_manuf));
	memset(default_curveSet->m_InputBoxSerial,0,sizeof(default_curveSet->m_InputBoxSerial));
		default_curveSet->m_fCount = 0;
		default_curveSet->m_dCount = 0;

		m_SavePair.clear();
		m_SaveList.ResetContent();
		m_DispPair.clear();
		m_DispList.ResetContent();	
		m_usfiedB.ResetContent();
		m_usdataB.ResetContent();
	m_filepath = _T(""); 
	UpdateData(FALSE);

}
void CSetCurveIV::OnBnClickedButtonAddUsfied()            //自定义需改动
{
	UpdateData(TRUE);

  if (m_usfied.Compare(_T("")) != 0)
  {
	  int dex = m_usfiedB.FindString(-1,m_usfied);
	  Pair pairD, pairS;
	  PairList::iterator it;
	  if (dex < 0)
	  {
		  m_usfiedB.AddString(m_usfied);
		  m_usdataB.AddString(m_usdata);
		  dex = m_usfiedB.FindString(-1,m_usfied);
	  }else
	  {
		  m_usdataB.DeleteString(dex);
		  m_usdataB.InsertString(dex,m_usdata);
		  m_usdataB.SetCurSel(dex);
	  }

	  int dexD = m_DispList.FindString(-1,m_usfied);
	  if ( dexD < 0)
	  {
		  pairD.id = paracnt+1+dex;
		  pairD.name =m_usfied;
		  m_DispPair.push_back(pairD);
		  m_DispList.AddString(m_usfied);
	  }

	  int dexS = m_SaveList.FindString(-1,m_usfied);
	  if ( dexS < 0)
	  {
		  pairS.id = paracnt+1+dex;
		  pairS.name =m_usfied;
		  m_SavePair.push_back(pairS);
		  m_SaveList.AddString(m_usfied);
	  }

  }else
  {
	  AfxMessageBox(_T("Custom fields are empty!"));
  }

}
void CSetCurveIV::OnBnClickedButtonDelUsfied()
{
	UpdateData(1);
	if(m_usfied.Compare(_T("")) != 0)
	{
		int dex0 = m_usfiedB.FindString(-1,m_usfied);
		if (dex0 < 0)
		{
			AfxMessageBox(_T("This field is not added!"));
		}else
		{
			int dexD = m_DispList.FindString(-1,m_usfied);
			m_DispList.DeleteString(dexD);
			if(m_DispPair.size()>=dexD)
				m_DispPair.erase(m_DispPair.begin()+dexD);


			int dexS = m_SaveList.FindString(-1,m_usfied);
			m_SaveList.DeleteString(dexS);
			if(m_SavePair.size()>=dexS)
				m_SavePair.erase(m_SavePair.begin()+dexS);

			m_usfiedB.DeleteString(dex0);
			m_usdataB.DeleteString(dex0);

			m_usfiedB.SetCurSel(dex0);
			m_usdataB.SetCurSel(dex0);
		}

	}else
	{
		AfxMessageBox(_T("You can not delete an empty field!"));
	}
	OnBnClickedButtonSort();
}
void CSetCurveIV::OnFiedSelchange()
{
	int dex = m_usfiedB.GetCurSel();
	m_usdataB.SetCurSel(dex);

}
void CSetCurveIV::OnDataSelchange()
{
	int dex = m_usdataB.GetCurSel();
	m_usfiedB.SetCurSel(dex);
}

CString CSetCurveIV::GetvalueZDY(int n)         //自定义需改动
{
	CString str;
	m_usdataB.GetLBText(n-paracnt-1,str);
	return str;
}
CString CSetCurveIV::GetTEXTZDY(int n)
{
	CString str;
	m_usfiedB.GetLBText(n-paracnt-1,str);
	return str;
}
CString CSetCurveIV::GetFormatZDY()
{
	return _T("TEXT");

}
void CSetCurveIV::OnBnClickedCheckSQL()                   //朱峰改动
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_SQLserver)
	{
		CSQLSet m_ipinput;
loop:		
		if(m_ipinput.DoModal() == IDOK)
		{
			if ( m_ipinput.Getusername()  == "" )
			{
				AfxMessageBox(_T("请输入数据！"));
				goto loop;
			}else
			{
				_tcscpy(m_serverip_this , m_ipinput.Getserverip());
				_tcscpy(m_database_this,m_ipinput.Getdatabase());
				_tcscpy(m_username_this , m_ipinput.Getusername());
				_tcscpy(m_password_this , m_ipinput.Getpassword());
			}
		}
	}
}