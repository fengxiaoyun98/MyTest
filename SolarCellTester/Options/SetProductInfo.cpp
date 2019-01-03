// Serial.cpp : implementation file
//

#include "stdafx.h"
#include "SetProductInfo.h"
#include "SetRunPara.h"
#include "..\resource.h"
#include "Registry.h"

// CSetProductInfo dialog

IMPLEMENT_DYNAMIC(CSetProductInfo, CDialog)

CSetProductInfo::CSetProductInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CSetProductInfo::IDD, pParent)
	, m_no(_T(""))
	, m_unit(_T(""))
	, m_size(_T(""))
	, m_name(_T(""))
	, m_model(_T(""))
	, m_save_on(FALSE)
	, m_SwDlg(FALSE)
{
	m_area=0.001f;
}

CSetProductInfo::~CSetProductInfo()
{
}

void CSetProductInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_no);
	DDV_MaxChars(pDX, m_no, 20);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDV_MaxChars(pDX, m_unit, 30);
	DDX_Text(pDX, IDC_EDIT_AREA, m_area);
	DDV_MinMaxFloat(pDX, m_area, 1.e-003f, 1.e+006f);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_size);
	DDV_MaxChars(pDX, m_size, 20);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 10);
	DDX_Text(pDX, IDC_EDIT_MODEL, m_model);
	DDV_MaxChars(pDX, m_model, 16);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_save_on);
	DDX_Check(pDX, IDC_CHECK_SWDLG, m_SwDlg);
}


BEGIN_MESSAGE_MAP(CSetProductInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CSetProductInfo::OnBnClickedOk)
END_MESSAGE_MAP()


void CSetProductInfo::OnBnClickedOk()
{
	SaveSet();
}
void CSetProductInfo::Save(CString name,CString value)
{
	CString strData;
	CRegistry reg;
	if(reg.Open(_T("Software\0")))
	{
		reg.CreateKey(_T("ProductInfo"));
		strData.Format(_T("%ud"),value);
		reg.Write(name,strData);
	}
}
CString  CSetProductInfo::Get(CString name)
{
	CString strData;
	CRegistry reg;
	if(reg.Open(_T("Software\\ProductInfo\0")))
	{
		reg.Read(name,strData);
		return strData;
	}
	reg.Close();
}
void CSetProductInfo::SaveSet()
{
	UpdateData(TRUE);
	Save(_T("Size"),m_size );
	Save(_T("Model"),m_model );
	Save(_T("Name"),m_name );
	Save(_T("SerialNO"),m_no );
	//Save(_T("Date"),str );
	Save(_T("Unit"),m_unit );
}
void CSetProductInfo::GetSet()
{
	m_size = Get(_T("Size"));
	m_model = Get(_T("Model"));
	m_name = Get(_T("Name"));
	m_no = Get(_T("SerialNO"));
	//Get(_T("Date"));
	m_unit = Get(_T("Unit"));
	UpdateData(false);
}
BOOL CSetProductInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	SaveSet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
