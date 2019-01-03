// Options\SetDev841.cpp : implementation file
//
#include "stdafx.h"
#include "..\stdafx.h"
#include "..\SolarCellTester.h"

#include "SetDev841.h"

// CSetDev841 dialog

IMPLEMENT_DYNAMIC(CSetDev841, CDialog)
CSetDev841::CSetDev841(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetDev841::IDD, pParent)
{
	OpReg()->CreateKey(GetThisClassName());
	Reg841ParaKey=_T("Reg841Para");

	m_sTestCountKey=_T("TestCountKey");

	Reg841Para::Reg841Para();
	m_set841sun=(1000);
	m_testcnt = 1;
	m_pointcnt=400;
	m_atype = _T("");
	m_atime = _T("");

	m_slen.push_back(SunLenPair(L"10ms",	0x05));
	m_slen.push_back(SunLenPair(L"20ms",	0x0A));
	m_slen.push_back(SunLenPair(L"30ms",	0x0F));
	m_slen.push_back(SunLenPair(L"40ms",	0x14));
	m_slen.push_back(SunLenPair(L"50ms",	0x19));
	m_slen.push_back(SunLenPair(L"60ms",	0x1E));
	m_slen.push_back(SunLenPair(L"70ms",	0x23));
	m_slen.push_back(SunLenPair(L"80ms",	0x28));
	m_slen.push_back(SunLenPair(L"90ms",	0x2D));
	m_slen.push_back(SunLenPair(L"100ms",	0x32));
	m_slen.push_back(SunLenPair(L"110ms",	0x37));
	m_slen.push_back(SunLenPair(L"120ms",	0x3C));
	m_slen.push_back(SunLenPair(L"130ms",	0x41));
	m_slen.push_back(SunLenPair(L"140ms",	0x46));
	m_slen.push_back(SunLenPair(L"150ms",	0x4B));
// 	m_slen.push_back(SunLenPair(L"160ms",	0x50));
// 	m_slen.push_back(SunLenPair(L"170ms",	0x55));
// 	m_slen.push_back(SunLenPair(L"180ms",	0x5A));
// 	m_slen.push_back(SunLenPair(L"190ms",	0x5F));
// 	m_slen.push_back(SunLenPair(L"200ms",	0x64));
// 	m_slen.push_back(SunLenPair(L"210ms",	0x69));
// 	m_slen.push_back(SunLenPair(L"220ms",	0x6E));
// 	m_slen.push_back(SunLenPair(L"230ms",	0x73));
// 	m_slen.push_back(SunLenPair(L"240ms",	0x78));

	UINT no=1000;
	m_mlist.push_back(Model(L"XJCM-9000 Model",no++));
	m_mlist.push_back(Model(L"XJCM-9000 Cell",no++));
	m_mlist.push_back(Model(L"XJCM-9000 Sun",no++));
	m_mlist.push_back(Model(L"XJCM-9000 Model 20A",no++));

	m_sdirection.push_back(SunLenPair(L"Isc→Voc",	0));
	m_sdirection.push_back(SunLenPair(L"Voc→Isc",	1));

	m_cnt1=0;
	m_cnt2=0;
	m_cnt3=0;
//光强的区段映射
	m_ss.SetPair(700,1200,700,1200,1000);   
/*
电流电压档位比例
GradePair（比例，名称，硬件档位）
*/
#if defined(研究20A)
//#elif defined(研究20A)
	m_gpi.push_back(GradePair(2,	_T("20 A"),	0));
	m_gpi.push_back(GradePair(0.1,	_T("1 A"),1));
	m_gpi.push_back(GradePair(0.2,	_T("2 A"),	2));
	m_gpi.push_back(GradePair(0.01,_T("0.1 A"),3));
	m_gpv.push_back(GradePair(1,	_T("1 V"),	0));
	m_gpv.push_back(GradePair(10,	_T("10 V"),	1));
	m_gpv.push_back(GradePair(50,	_T("50 V"),	2));
	m_gpv.push_back(GradePair(100,	_T("100 V"),3));
#else//常规配置
// 	m_gpi.push_back(GradePair(1,	_T("10 A"),	0));
// 	m_gpi.push_back(GradePair(0.25,	_T("2.5 A"),1));
// 	m_gpi.push_back(GradePair(0.1,	_T("1 A"),	2));
// 	m_gpi.push_back(GradePair(0.025,_T("0.25 A"),3));
// 
// 	m_gpv.push_back(GradePair(1,	_T("1 V"),	0));
// 	m_gpv.push_back(GradePair(10,	_T("10 V"),	1));
// 	m_gpv.push_back(GradePair(50,	_T("50 V"),	2));
// 	m_gpv.push_back(GradePair(100,	_T("100 V"),3));
//	m_gpv.push_back(GradePair(200,	_T("200 V"),4));


	m_gpi.push_back(GradePair(0.285,_T("0.25 A"),0x01));
	m_gpi.push_back(GradePair(1,   _T("1 A"),   0x02));
	m_gpi.push_back(GradePair(2000.0/351.0,   _T("5 A"),	0x03));
	m_gpi.push_back(GradePair((1820.0/141.0),  _T("12.5 A"),  0x04));
	m_gpi.push_back(GradePair(20,  _T("20 A"),  0x05));

	m_gpv.push_back(GradePair(1,	_T("1 V"),	0x10));
	m_gpv.push_back(GradePair(10,	_T("10 V"),	0x20));
	m_gpv.push_back(GradePair(50,	_T("50 V"),	0x30));
	m_gpv.push_back(GradePair(100,	_T("100 V"),0x40));
	m_gpv.push_back(GradePair(200,	_T("200 V"),0x50));
#endif
}

UINT CSetDev841::GetMcuSun(UINT sun)
{
	double rs;
	if(sun==0)
		rs=m_set841sun;
	else
		rs=sun;
	m_ss.btoa(rs);
	return (UINT)rs;
}

CSetDev841::~CSetDev841()
{
	delete pDXToVar;
	delete pDXToCtrl;

}

void CSetDev841::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CURR, m_cbbcur);
	DDX_Control(pDX, IDC_COMBO_VOLO, m_cbbvol);
	DDX_Control(pDX, IDC_COMBO_DESITY, m_cbbslen);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_SweepDirection);

	DDX_Control(pDX,IDC_COMBO2,m_modlist);
	DDX_Control(pDX, IDC_EDIT1, m_sunarea);
	DDX_Control(pDX, IDC_EDIT2, m_devstate);


	DDX_Text(pDX,IDC_EDIT_SET841PERI,m_Interval);
	DDV_MinMaxUInt(pDX,m_Interval,5,120);
	DDX_Text(pDX,IDC_EDIT_SET841SUN,m_set841sun);
	DDV_MinMaxUInt(pDX,m_set841sun,(UINT)m_ss.getbmin(),(UINT)m_ss.getbmax());
	DDX_Control(pDX, IDC_EDIT_SET841SUN, m_EC841sun);
	DDX_Check(pDX,IDC_CHECK1,m_externSwitch);
	DDX_Check(pDX,IDC_VOC_ISC,m_exterV_I);
	DDX_Check(pDX,IDC_VOC_ISC_SEC,m_exterV_I_Sec);
	DDX_Check(pDX,IDC_EFF_CELL,m_eff_cell);
	DDX_Check(pDX,IDC_CHECK6,m_CloseNx);
	DDX_Check(pDX,IDC_CHECK7,m_DisCntAlarm);

	DDX_Text(pDX,IDC_EDIT_COUNTERNAME1,m_sCName1);
	DDX_Text(pDX,IDC_EDIT_COUNTERNAME2,m_sCName2);
	DDX_Text(pDX,IDC_EDIT_COUNTERNAME3,m_sCName3);
	DDX_Text(pDX,IDC_EDIT3,m_pointcnt);
	DDX_Text(pDX,IDC_EDIT5,m_testcnt);
		
	DDX_Text(pDX,IDC_STATIC_ATYPE,m_atype);
	DDX_Text(pDX,IDC_STATIC_ATIME,m_atime);



	DDX_Text(pDX,IDC_EDIT_SET841PERI2,m_cnt1);
	DDX_Text(pDX,IDC_EDIT_SET841PERI3,m_cnt2);
	DDX_Text(pDX,IDC_EDIT_SET841PERI4,m_cnt3);
	DDX_Text(pDX,IDC_EDIT_SET841PERI8,m_cntTotal);
	DDX_Text(pDX,IDC_EDIT_SET841PERI5,m_Lmt1);
	DDX_Text(pDX,IDC_EDIT_SET841PERI6,m_Lmt2);
	DDX_Text(pDX,IDC_EDIT_SET841PERI7,m_Lmt3);
}


BEGIN_MESSAGE_MAP(CSetDev841, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DESITY, &CSetDev841::OnCbnSelchangeComboSLen)
	ON_CBN_SELCHANGE(IDC_COMBO_CURR, &CSetDev841::OnCbnSelchangeComboCurr)
	ON_CBN_SELCHANGE(IDC_COMBO_VOLO, &CSetDev841::OnCbnSelchangeComboVolo)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, &CSetDev841::OnCbnSelchangeComboSweepDirection)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetDev841::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CSetDev841::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSetDev841::OnBnClickedButton8)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CSetDev841::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_VOC_ISC, &CSetDev841::OnBnClickedVocIsc)
	ON_BN_CLICKED(IDC_EFF_CELL, &CSetDev841::OnBnClickedEffCell)
	ON_BN_CLICKED(IDC_VOC_ISC_SEC, &CSetDev841::OnBnClickedVocIscSec)
END_MESSAGE_MAP()


// CSetDev841 message handlers
BOOL CSetDev841::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetDev841SaveReg));
	theApp.RegFunction(L"CSetDev841::SaveReg",str);

// 	str.LoadString(IDS_SetCurAndVolGrade);
// 	theApp.RegFunction(L"CSetDev841::CSetvoc_isc_class",str);

	str.LoadString(IDS_SetIrradianceAndTestTime);
	theApp.RegFunction(L"CSetDev841::CSetsun_testlong",str);

	str.LoadString(IDS_SetCounterAndAlarm);
	theApp.RegFunction(L"CSetDev841::CSetRsetCnt",str);

	if(theApp.IsDisable(L"CSetDev841::SaveReg"))
		DisableAllSubCtrl(this);

// 	if (theApp.IsDisable(L"CSetDev841::CSetvoc_isc_class"))
// 	{
// 		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(FALSE);
// 	}
	if (theApp.IsDisable(L"CSetDev841::CSetsun_testlong"))
	{
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841PERI)->EnableWindow(FALSE);
	}
	if (theApp.IsDisable(L"CSetDev841::CSetRsetCnt"))
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SET841PERI5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841PERI6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841PERI7)->EnableWindow(FALSE);
	}

//设定硬件缺省档位
	CDialog::OnInitDialog();

	CComboBox *bb=&m_cbbcur;
	bb->ResetContent();
	GradePairList::iterator it;
	UINT ic=0;//档个数
	for(it=m_gpi.begin();it<m_gpi.end();it++,ic++)
		bb->AddString (it->GetTitle());
//设定缺省电压档位
	bb=&m_cbbvol;
	bb->ResetContent();
	UINT iv=0;//档个数
	for(it=m_gpv.begin();it<m_gpv.end();it++,iv++)
		bb->AddString (it->GetTitle());
//填充恒光时长表
	bb=&m_cbbslen;
	bb->ResetContent();
	UINT is=0;//档个数
	SunLenPairList::iterator itr;
	for(itr=m_slen.begin();itr<m_slen.end();itr++,is++)
		bb->AddString (itr->GetTitle());
//填充型号表
	bb=&m_modlist;
	bb->ResetContent();
	UINT im=0;
	vector<Model>::iterator itm=m_mlist.begin();
	for(itm;itm<m_mlist.end();itm++,im++)
		bb->AddString(itm->first);
//填充扫描方向
	bb=&m_SweepDirection;
	bb->ResetContent();
	UINT iswd=0;
	SunLenPairList::iterator itswd;
	for(itswd=m_sdirection.begin();itswd<m_sdirection.end();itswd++,iswd++)
		bb->AddString (itswd->GetTitle());
	
	// 初始化计数器名称
	CString str1;
	str1.LoadStringW(IDS_DEV841_LAMPCOUNTER);
	_tcscpy(m_sCntName1,str1);
	str1.LoadStringW(IDS_DEV841_PROBECOUNTER);
	_tcscpy(m_sCntName2,str1);

//	LoadReg();
	if(m_nocur>=ic)m_nocur=0;
	if(m_novol>=iv)m_novol=0;
	if(m_nslen>=is)m_nslen=0;
	if(m_model>=im)m_model=0;
	if (m_nsweepdirection>=iswd)m_nsweepdirection=0;

	m_cbbvol.SetCurSel(m_novol);
	m_cbbcur.SetCurSel(m_nocur);
	m_cbbslen.SetCurSel(m_nslen);
	m_modlist.SetCurSel(m_model);
	m_SweepDirection.SetCurSel(m_nsweepdirection);
//光强设置
	CString str2;
	str2.Format(_T("%.0f-%.0f"),m_ss.getbmin(),m_ss.getbmax());
	m_sunarea.SetWindowText(str2);
//可见性设置,权限定制，不同设备也可以看作不同权限的客户
	m_EC841sun.ShowWindow(SW_SHOW);
//处理计数器
	m_sCName1 = m_sCntName1;
	m_sCName2 = m_sCntName2;
	m_sCName3 = m_sCntName3;
// 	str.Format(L"%d",m_cnt1);
// 	m_cnt1.SetWindowText(str);
// 	str.Format(L"%d",m_cnt2);
// 	m_cnt2.SetWindowText(str);
// 	str.Format(L"%d",m_cnt3);
// 	m_cnt3.SetWindowText(str);
// 	str.Format(L"%d",m_cntTotal);
// 	m_cntTotal.SetWindowText(str);
	CheckLimit();
//设置通信状态
	SetDevState(0,_T(""));
	pDXToVar=new CDataExchange(this, TRUE);
	pDXToCtrl=new CDataExchange(this, FALSE);

	UpdateData(0);
	return 0;
}
void CSetDev841::SetDevState(int st,CString str)
{
	CString str2=L"";
	if(st==0)
		str2=L"Device off line";
	else if(st==1)
		str2=L"Device on line;"+str;
	else
		str2=L"";
	m_devstate.SetWindowText(str2);
}

void CSetDev841::OnCbnSelchangeComboCurr()
{
	m_nocur=m_cbbcur.GetCurSel();
}
void CSetDev841::OnCbnSelchangeComboVolo()
{
	m_novol=m_cbbvol.GetCurSel();
}
void CSetDev841::OnCbnSelchangeComboSLen()
{
	m_nslen=m_cbbslen.GetCurSel();

//	UpdateData(FALSE);
}
void CSetDev841::OnCbnSelchangeComboSweepDirection()
{
	m_nsweepdirection=m_SweepDirection.GetCurSel();
}

void CSetDev841::OnCbnSelchangeCombo2()
{
	m_model=m_modlist.GetCurSel();
}

void CSetDev841::resetcnt(int no)
{
	CString str=L"";
	switch(no)
	{
	case 1:str.Format(L"Current Value:%d \nWant To Reset!",m_cnt1);break;
	case 2:str.Format(L"Current Value:%d \nWant To Reset!",m_cnt2);break;
	case 3:str.Format(L"Current Value:%d \nWant To Reset!",m_cnt3);break;
	}
	if(MessageBox(str,L"Alarm",IDOK)==IDOK)
	switch(no){
	case 1:m_cnt1=0;break;
	case 2:m_cnt2=0;break;
	case 3:m_cnt3=0;break;
	}
	UpdateData(0);
	if (m_com.IsOpen())
	{
		byte sendbuf[11];
		byte recvbuf[20];
		memset(sendbuf, 0,(sizeof(byte)*11) );
		memset(recvbuf, 0,(sizeof(byte)*20) );
		//帧头
		sendbuf[0] = 0xA5;
		sendbuf[1] = 0xA5;
		sendbuf[2] = 0xA5;
		sendbuf[3] = 0x5A;
		//数据长度
		sendbuf[4] = 0x05;
		sendbuf[5] = 0x00;

		sendbuf[6] = 0xA1;                //主命令
		sendbuf[7] = 0x02;                //子命令 参数设置

		sendbuf[8] = 0x20; 

		sendbuf[9] = LOBYTE (m_com.crc16(0,sendbuf,9));               
		sendbuf[10] = HIBYTE (m_com.crc16(0,sendbuf,9));

		m_com.ClearALL();
		m_com.Write(sendbuf,11);
		SingleSleep(20);
		m_com.Read(recvbuf,20);
	}
}
void CSetDev841::OnBnClickedButton1()
{
	resetcnt(1);
}

void CSetDev841::OnBnClickedButton7()
{
	resetcnt(2);
}

void CSetDev841::OnBnClickedButton8()
{
	resetcnt(3);
}

void CSetDev841::OnBnClickedVocIsc()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_exterV_I)
	{
		m_novol = 2;
		m_nocur = 3;
		m_nsweepdirection = 1;
		m_cbbvol.SetCurSel(m_novol);
		m_cbbcur.SetCurSel(m_nocur);
		m_SweepDirection.SetCurSel(m_nsweepdirection);
		m_set841sun = 1000;
		m_eff_cell = FALSE;
		m_exterV_I_Sec = FALSE;
		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(FALSE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);

//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_HIDE);
	}else
	{
		m_nsweepdirection = 0;
		m_SweepDirection.SetCurSel(m_nsweepdirection);

		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(TRUE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(TRUE);

		if (m_fc[VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}

//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_NORMAL);
	}
	UpdateData(FALSE);
}
void CSetDev841::OnBnClickedVocIscSec()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_exterV_I_Sec)
	{
		m_novol = 2;
		m_nocur = 3;
		m_nsweepdirection = 1;
		m_cbbvol.SetCurSel(m_novol);
		m_cbbcur.SetCurSel(m_nocur);
		m_SweepDirection.SetCurSel(m_nsweepdirection);
		m_set841sun = 1000;
		m_eff_cell = FALSE;
		m_exterV_I = FALSE;
		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(FALSE);
		GetDlgItem(IDC_VOC_ISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);

//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_HIDE);
	}else
	{

		m_nsweepdirection = 0;
		m_SweepDirection.SetCurSel(m_nsweepdirection);

		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(TRUE);
		GetDlgItem(IDC_VOC_ISC)->EnableWindow(TRUE);

		if (m_fc[VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}

//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_NORMAL);
	}
	UpdateData(FALSE);
}

void CSetDev841::OnBnClickedEffCell()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_eff_cell && (m_fc[EFF_IV_FC] || m_fc[EFF_VI_FC]))
	{
		m_exterV_I = FALSE;
		m_exterV_I_Sec = FALSE;

		GetDlgItem(IDC_VOC_ISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(FALSE);

		if (m_fc[EFF_VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			m_nsweepdirection = 0;
			m_SweepDirection.SetCurSel(m_nsweepdirection);
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}
	}else
	{	
		GetDlgItem(IDC_VOC_ISC)->EnableWindow(TRUE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(TRUE);

		if (m_fc[VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			m_nsweepdirection = 0;
			m_SweepDirection.SetCurSel(m_nsweepdirection);
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}
	}
	UpdateData(FALSE);

	if(MessageBox(_T("Do you want to reset the coefficient of Isc,Voc and Pmax?")
		,_T("Warning")
		,MB_ICONEXCLAMATION|MB_OKCANCEL) == IDOK)
	{
		theApp.GetSetBase()->GetSetSunPara()->SetScaleIsc(1.0);
		theApp.GetSetBase()->GetSetSunPara()->SetScaleVoc(1.0);
		theApp.GetSetBase()->GetSetSunPara()->SetScalePmax(1.0);
		theApp.GetSetBase()->GetSetSunPara()->SetScaleIRTemp(1.0);
		theApp.GetSetBase()->GetSetSunPara()->UpdateData(FALSE);
	}
}
void CSetDev841::Increment(void)
{
// 	m_cnt1++;
// 	m_cnt2++;
// 	m_cnt3++;
 	//m_cntTotal++;

	//m_cnt1 = m_cnt2 =  m_cnt3 =m_cntTotal;
	m_cntTotal = m_cnt1;

	UpdateData(0);
// 	CString str;
// 	str.Format(L"%d",m_cnt1);
// 	m_cnt1.SetWindowText(str);
// 	str.Format(L"%d",m_cnt2);
// 	m_cnt2.SetWindowText(str);
// 	str.Format(L"%d",m_cnt3);
// 	m_cnt3.SetWindowText(str);
// 	str.Format(L"%d",m_cntTotal);
// 	m_cntTotal.SetWindowText(str);
	// 保存计数
    SaveCntReg();
	CheckLimit();
}
void CSetDev841::CheckLimit(void)
{
	if(m_DisCntAlarm)
		return;
	if(m_Lmt1==0&&m_Lmt2==0&&m_Lmt3==0)
		return;
	BOOL err=FALSE;
	CString str1=L"";
	CString str2=L"";
	CString str3=L"";
	str1.Format(L"1 Limit:%d,Counter:%d",m_Lmt1,m_cnt1);
	if(m_Lmt1 > 0 && m_cnt1>m_Lmt1){
		str1+=L" Over!\n";err=TRUE;
	}else str1+=L"\n";
	str2.Format(L"2 Limit:%d,Counter:%d",m_Lmt2,m_cnt2);
	if(m_Lmt2 > 0 && m_cnt2>m_Lmt2){
		str2+=L" Over!\n";err=TRUE;
	}else str2+=L"\n";
	str3.Format(L"3 Limit:%d,Counter:%d",m_Lmt3,m_cnt3);
	if(m_Lmt3 > 0 && m_cnt3>m_Lmt3){
		str3+=L" Over!\n";err=TRUE;
	}else str3+=L"\n";
	if(err==TRUE)
		MessageBox(str1+str2+str3,L"Counter Alarm",IDOK);
}

BOOL CSetDev841::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	Apply(wParam);    // 判断控件是否更改
	return __super::OnCommand(wParam, lParam);
}

void CSetDev841::LoadReg(void)
{
	Reg841Para *reg841Para=this;
	//	OpReg()->Read(Reg841ParaKey,(byte*)set,sizeof(Reg841Para));


	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo2.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive)){
		file.Read(reg841Para,sizeof(CalibrationParaList));
		file.Close();
	}
	else
		return;

	m_sCName1=m_sCntName1;
	m_sCName2=m_sCntName2;
	m_sCName3=m_sCntName3;

	UpdateData(0);
}

void CSetDev841::SaveReg(void)
{
	UpdateData();

	_tcscpy(m_sCntName1,m_sCName1);
	_tcscpy(m_sCntName2,m_sCName2);
	_tcscpy(m_sCntName3,m_sCName3);

	Reg841Para *reg841Para = this;
	OpReg()->Write(Reg841ParaKey,(byte*)reg841Para,sizeof(Reg841Para));

// 	// 保存配置文件
// 	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo2.Dat";
// 
// 	CFile file;
// 	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
// 		file.Write(reg841Para, sizeof(CalibrationParaList));
// 		file.Close();
// 	}
}

void CSetDev841::LoadCntReg(void)
{
	CTestCount *cTestCount=this;
    OpReg()->Read(m_sTestCountKey,(byte*)cTestCount,sizeof(CTestCount));

	m_sCName1=m_sCntName1;
	m_sCName2=m_sCntName2;
	m_sCName3=m_sCntName3;

	UpdateData(0);
}

void CSetDev841::SaveCntReg(void)
{
	UpdateData();

	_tcscpy(m_sCntName1,m_sCName1);
	_tcscpy(m_sCntName2,m_sCName2);
	_tcscpy(m_sCntName3,m_sCName3);

	CTestCount *cTestCount;
	cTestCount = this;

	OpReg()->Write(m_sTestCountKey,(byte*)(cTestCount),sizeof(CTestCount));
}

Reg841Para* CSetDev841::GetAppliedConfig(void)
{
	UpdateData();

	Reg841Para *reg841Para = this;
    SaveCntReg();

	for (int i = 0; i < 12; i++)
	{
		if (m_nslen == i)
		{
			if (m_Interval < (5+2*i))
			{
				m_Interval = 5+2*i;
			}
		}
	}
	if (m_nslen == 12)
	{
		if (m_Interval < 28)
			m_Interval = 28;
	}
	if (m_nslen == 13)
	{
		if (m_Interval < 29)
			m_Interval = 29;
	}
	if (m_nslen == 14)
	{
		if (m_Interval < 30)
			m_Interval = 30;
	}
	if (m_nslen > 9)
	{
		if (m_set841sun > 1000)
		{
			m_set841sun = 1000;
		}

	}
	if (m_exterV_I || m_exterV_I_Sec)
	{
		if (m_nslen >9)
		{
			m_nslen = 9;
			m_cbbslen.SetCurSel(m_nslen);
		}
	}

	UpdateData(FALSE);
	return reg841Para;
}

void CSetDev841::SetAppliedConfig(Reg841Para* tReg841Para)
{
	Reg841Para *reg841Para=this;
	memcpy(reg841Para, tReg841Para, sizeof(Reg841Para));

	LoadCntReg();
	m_sCName1=m_sCntName1;
	m_sCName2=m_sCntName2;
	m_sCName3=m_sCntName3;

	m_cbbvol.SetCurSel(m_novol);
	m_cbbcur.SetCurSel(m_nocur);
	m_cbbslen.SetCurSel(m_nslen);
	m_SweepDirection.SetCurSel(m_nsweepdirection);

	UpdateData(TRUE);
	//先判断哪些功能是开放的
	GetDlgItem(IDC_VOC_ISC_SEC)->ShowWindow(m_fc[VI_FC]);
	GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(m_fc[VI_FC]);

	GetDlgItem(IDC_EFF_CELL)->ShowWindow(m_fc[EFF_IV_FC]);
	GetDlgItem(IDC_EFF_CELL)->EnableWindow(m_fc[EFF_IV_FC]);

	GetDlgItem(IDC_VOC_ISC)->ShowWindow(m_fc[SMART_VI_FC]);
	GetDlgItem(IDC_VOC_ISC)->EnableWindow(m_fc[SMART_VI_FC]);

	if (m_fc[PULSE_TO150])
	{
		GetDlgItem(IDC_COMBO_DESITY)->EnableWindow(TRUE);
	}else
	{
		m_nslen = 0;
		m_cbbslen.SetCurSel(m_nslen);
		GetDlgItem(IDC_COMBO_DESITY)->EnableWindow(FALSE);
	}

	if ((m_exterV_I && (m_fc[SMART_VI_FC])) || (m_exterV_I_Sec && (m_fc[VI_FC])))
	{
		m_novol = 2;
		m_nocur = 3;
		m_nsweepdirection = 1;
		m_cbbvol.SetCurSel(m_novol);
		m_cbbcur.SetCurSel(m_nocur);
		m_SweepDirection.SetCurSel(m_nsweepdirection);
		if ((m_nslen > 9) && m_fc[PULSE_TO150])
		{
			m_nslen = 9;
			m_cbbslen.SetCurSel(m_nslen);
		}
		m_set841sun = 1000;
		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(FALSE);
//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_HIDE);
	}else
	{
		GetDlgItem(IDC_COMBO_CURR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_VOLO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SET841SUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EFF_CELL)->EnableWindow(TRUE);

		if (m_fc[VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			m_nsweepdirection = 0;
			m_SweepDirection.SetCurSel(m_nsweepdirection);
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}

//		((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_wndSolarCellTesterCtrlBar.GetDlgItem(IDC_BUTTON_OPTIONS5)->ShowWindow(SW_NORMAL);
	}

	if (m_eff_cell && (m_fc[EFF_IV_FC] || m_fc[EFF_VI_FC]))
	{
		GetDlgItem(IDC_VOC_ISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(FALSE);

		if (m_fc[EFF_VI_FC])
		{
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);
		}else
		{
			m_nsweepdirection = 0;
			m_SweepDirection.SetCurSel(m_nsweepdirection);
			GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);
		}

	}else
	{
		GetDlgItem(IDC_VOC_ISC)->EnableWindow(TRUE);
		GetDlgItem(IDC_VOC_ISC_SEC)->EnableWindow(TRUE);
	}


	UpdateData(0);
}

void CSetDev841::SetDev841Default()                                          
{
	Reg841Para *default_reg841Para=this;
	default_reg841Para->m_set841sun=(1000);
	default_reg841Para->m_externSwitch=FALSE;
	default_reg841Para->m_exterV_I=FALSE;
	default_reg841Para->m_exterV_I_Sec=FALSE;
	default_reg841Para->m_eff_cell=FALSE;
	default_reg841Para->m_nocur=0;
	default_reg841Para->m_novol=0;
	default_reg841Para->m_nslen=0;
	default_reg841Para->m_nsweepdirection=0;
	default_reg841Para->m_Lmt1 = 0;
	default_reg841Para->m_Lmt2 = 0;
	default_reg841Para->m_Lmt3 = 0;
	default_reg841Para->m_Interval=5;//默认测试间隔0秒
	default_reg841Para->m_CloseNx=FALSE;
	default_reg841Para->m_model=0;
	default_reg841Para->m_DisCntAlarm=FALSE;


	LoadCntReg();
	m_sCName1=m_sCntName1;
	m_sCName2=m_sCntName2;
	m_sCName3=m_sCntName3;

	m_cbbvol.SetCurSel(m_novol);
	m_cbbcur.SetCurSel(m_nocur);
	m_cbbslen.SetCurSel(m_nslen);
	m_SweepDirection.SetCurSel(m_nsweepdirection);

	UpdateData(FALSE);

}
