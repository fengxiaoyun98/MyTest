// Options\SetParaAlarm.cpp : implementation file
//
#include "stdafx.h"
#include "..\SolarCellTester.h"

#include "..\stdafx.h"
#include "SetParaAlarm.h"


// CSetParaAlarm dialog

IMPLEMENT_DYNAMIC(CSetParaAlarm, CDialog)

CString SignificanceDigitString(double value,int n);

CSetParaAlarm::CSetParaAlarm(CWnd* pParent /*=NULL*/)
	: CSetInterFace(CSetParaAlarm::IDD, pParent)
{
	OpReg()->CreateKey(GetThisClassName());

	m_filenamekey=_T("m_filename");
	m_filecountkey=_T("m_filecount");

	m_pSet=new _Seter;
	memset(m_pSet,0,sizeof(_Seter));
	m_pList=&m_pSet->list;
	memset(&m_item.name,0,sizeof(m_item.name));
	memset(&m_item.area,0,sizeof(m_item.area));
	memset(&m_pSet->item,0,sizeof(m_pSet->item));
	m_pSet->item[SUN][0] = 700;
	m_pSet->item[SUN][1] = 1200;
	for(int i=0; i<4;i++)
	{
		m_item.m_class[i] = _T("");
		m_item.m_Gname[i] = _T("");
	}
}

CSetParaAlarm::~CSetParaAlarm()
{
	if(m_pSet->list.size())
		m_pSet->list.clear();
	if(m_pSet)
		delete m_pSet;
	m_pSet=NULL;
}

void CSetParaAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ITEMLIST, m_listbox);


	DDX_Radio(pDX,IDC_RADIO1,m_pSet->type);
	DDX_Check(pDX,IDC_CHECK1,m_pSet->sys[SWALARM]);
	DDX_Check(pDX,IDC_CHECK2,m_pSet->sys[SWDISPLAY]);
	DDX_Check(pDX,IDC_CHECK3,m_pSet->sys[SWRING]);
	DDX_Check(pDX,IDC_CHECK4,m_pSet->sys[SWNOSAVE]);
	DDX_Check(pDX,IDC_CHECK5,m_pSet->sys[SWTMODNOFLASH]);


/////////////////////////改动 分档////////////////////////////////////
	DDX_Text(pDX, IDC_EDIT_1MIN, m_item.area[0][MIN]);
	DDV_MinMaxDouble(pDX, m_item.area[0][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT_1MAX, m_item.area[0][MAX]);
	DDV_MinMaxDouble(pDX, m_item.area[0][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT_2MIN, m_item.area[1][MIN]);
	DDV_MinMaxDouble(pDX, m_item.area[1][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT_2MAX, m_item.area[1][MAX]);
	DDV_MinMaxDouble(pDX, m_item.area[1][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT_3MIN, m_item.area[2][MIN]);
	DDV_MinMaxDouble(pDX, m_item.area[2][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT_3MAX, m_item.area[2][MAX]);
	DDV_MinMaxDouble(pDX, m_item.area[2][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT_4MIN, m_item.area[3][MIN]);
	DDV_MinMaxDouble(pDX, m_item.area[3][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT_4MAX, m_item.area[3][MAX]);
	DDV_MinMaxDouble(pDX, m_item.area[3][MAX], 0, 1000);
//////////////////////////////////////////////////////////////////////////
	DDX_Text(pDX, IDC_EDIT1A, m_pSet->item[ISC][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[ISC][MIN], 0, 100);
	DDX_Text(pDX, IDC_EDIT1B, m_pSet->item[ISC][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[ISC][MAX], 0, 100);

	DDX_Text(pDX, IDC_EDIT2A, m_pSet->item[VOC][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[VOC][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT2B, m_pSet->item[VOC][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[VOC][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT3A, m_pSet->item[PM][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[PM][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT3B, m_pSet->item[PM][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[PM][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT4A, m_pSet->item[FF][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[FF][MIN], 0, 100);
	DDX_Text(pDX, IDC_EDIT4B, m_pSet->item[FF][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[FF][MAX], 0, 100);

	DDX_Text(pDX, IDC_EDIT5A, m_pSet->item[I1][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[I1][MIN], 0, 100);
	DDX_Text(pDX, IDC_EDIT5B, m_pSet->item[I1][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[I1][MAX], 0, 100);

	DDX_Text(pDX, IDC_EDIT6A, m_pSet->item[EFF][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[EFF][MIN], 0, 100);
	DDX_Text(pDX, IDC_EDIT6B, m_pSet->item[EFF][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[EFF][MAX], 0, 100);

	DDX_Text(pDX, IDC_EDIT7A, m_pSet->item[IM][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[IM][MIN], 0, 100);
	DDX_Text(pDX, IDC_EDIT7B, m_pSet->item[IM][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[IM][MAX], 0, 100);

	DDX_Text(pDX, IDC_EDIT8A, m_pSet->item[VM][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[VM][MIN], 0, 1000);
	DDX_Text(pDX, IDC_EDIT8B, m_pSet->item[VM][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[VM][MAX], 0, 1000);

	DDX_Text(pDX, IDC_EDIT9A, m_pSet->item[RS][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[RS][MIN], 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT9B, m_pSet->item[RS][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[RS][MAX], 0, 1000000000);

	DDX_Text(pDX, IDC_EDITAA, m_pSet->item[RSH][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[RSH][MIN], 0, 1000000000);
	DDX_Text(pDX, IDC_EDITAB, m_pSet->item[RSH][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[RSH][MAX], 0, 1000000000);

	DDX_Text(pDX, IDC_EDITBA, m_pSet->item[IREV][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[IREV][MIN], 0, 1000000000);
	DDX_Text(pDX, IDC_EDITBB, m_pSet->item[IREV][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[IREV][MAX], 0, 1000000000);

	DDX_Text(pDX, IDC_EDITCA, m_pSet->item[ENVTEMP][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[ENVTEMP][MIN], 0, 1000000000);
	DDX_Text(pDX, IDC_EDITCB, m_pSet->item[ENVTEMP][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[ENVTEMP][MAX], 0, 1000000000);

	DDX_Text(pDX, IDC_EDITDA, m_pSet->item[SURFTEMP][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[SURFTEMP][MIN], 0, 1000000000);
	DDX_Text(pDX, IDC_EDITDB, m_pSet->item[SURFTEMP][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[SURFTEMP][MAX], 0, 1000000000);

	DDX_Text(pDX, IDC_EDITEA, m_pSet->item[SUN][MIN]);
	DDV_MinMaxDouble(pDX, m_pSet->item[SUN][MIN], 700, 1200);
	DDX_Text(pDX, IDC_EDITEB, m_pSet->item[SUN][MAX]);
	DDV_MinMaxDouble(pDX, m_pSet->item[SUN][MAX], 700, 1200);

	DDX_Control(pDX, IDC_FILELIST, m_filelist);
/////////////////////////改动  分档//////////////////////////////////////
	DDX_Control(pDX, IDC_LIST_TYPE, m_TypeList);         
	DDX_Text(pDX, IDC_EDIT_1, m_item.m_class[0]);
	DDX_Text(pDX, IDC_EDIT_2, m_item.m_class[1]);
	DDX_Text(pDX, IDC_EDIT_3, m_item.m_class[2]);
	DDX_Text(pDX, IDC_EDIT_4, m_item.m_class[3]);

	DDX_Text(pDX, IDC_EDIT_1G, m_item.m_Gname[0]);
	DDX_Text(pDX, IDC_EDIT_2G, m_item.m_Gname[1]);
	DDX_Text(pDX, IDC_EDIT_3G, m_item.m_Gname[2]);
	DDX_Text(pDX, IDC_EDIT_4G, m_item.m_Gname[3]);

	CString strname(m_item.name);
	DDX_Text(pDX, IDC_EDIT_NAME,strname);
	DDV_MaxChars(pDX,strname, NAMELENGTH);
	strname=strname.Left(NAMELENGTH);
	memset(m_item.name,0,sizeof(m_item.name));
	_tcsncpy_s(m_item.name,strname,strname.GetLength());
//////////////////////////////////////////////////////////////////////////
}


BEGIN_MESSAGE_MAP(CSetParaAlarm, CDialog)
	ON_BN_CLICKED(ID_APPEND, &CSetParaAlarm::OnBnClickedAppend)
	ON_BN_CLICKED(ID_UPDATA, &CSetParaAlarm::OnBnClickedUpdata)
	ON_LBN_SELCHANGE(IDC_ITEMLIST, &CSetParaAlarm::OnLbnSelchangeItemlist)
	ON_LBN_DBLCLK(IDC_ITEMLIST, &CSetParaAlarm::OnLbnDblclkItemlist)
	ON_BN_CLICKED(ID_INSERT, &CSetParaAlarm::OnBnClickedInsert)
	ON_BN_CLICKED(ID_DELETE, &CSetParaAlarm::OnBnClickedDelete)
	ON_BN_CLICKED(ID_SAVE, &CSetParaAlarm::OnBnClickedSave)
	ON_BN_CLICKED(ID_LOAD, &CSetParaAlarm::OnBnClickedLoad)
	ON_LBN_DBLCLK(IDC_FILELIST, &CSetParaAlarm::OnLbnDblclkFilelist)
	ON_BN_CLICKED(ID_MODIFY, &CSetParaAlarm::OnBnClickedModify)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO1, &CSetParaAlarm::OnBnClickedRadio1)      //改动分档
	ON_BN_CLICKED(IDC_RADIO2, &CSetParaAlarm::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CSetParaAlarm::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CSetParaAlarm::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON_GSTYLE, &CSetParaAlarm::OnBnClickedGStyle)

END_MESSAGE_MAP()


// CSetParaAlarm message handlers

//调入历史设置
BOOL CSetParaAlarm::OnInitDialog()
{
	CString str(MAKEINTRESOURCE(IDS_CSetParaAlarmSaveReg));
	theApp.RegFunction(L"CSetParaAlarm::SaveReg",str);

	if(theApp.IsDisable(L"CSetParaAlarm::SaveReg"))
		DisableAllSubCtrl(this);
	CDialog::OnInitDialog();

	m_filelist.ShowScrollBar(SB_HORZ, TRUE);
	m_filelist.SetHorizontalExtent(1000);

////////////////////////改动  档位///////////////////////////////////
	m_TypeList.ResetContent();
	m_TypeList.AddString(_T("Ivld"));
	m_TypeList.AddString(_T("Pmax"));
	m_TypeList.AddString(_T("Eff"));
	m_TypeList.AddString(_T("Imax"));

	GetDlgItem(IDC_EDIT_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2G)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3G)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
//////////////////////////////////////////////////////////////////////////

    
//	LoadReg();

	return TRUE;
}
//将当前的修改结果追加为一个新的条目，追加一个条目
void CSetParaAlarm::OnBnClickedAppend()
{
// 	if(IsNameEmpty())
// 		return;
	UpdateData();
	if(!AlarmItemValidityCheck(m_item))
		return;
	m_listbox.AddString(m_item.name);
	m_pList->push_back(m_item);
}
//选为当前条目
void CSetParaAlarm::OnLbnSelchangeItemlist()
{
	m_iItem=m_pList->begin()+m_listbox.GetCurSel();
	m_item=*m_iItem;
	UpdateData(0);
}
//删除双击条目
void CSetParaAlarm::OnLbnDblclkItemlist()
{
	int pos=m_listbox.GetCurSel();
	m_iItem=m_pList->begin()+pos;
	m_pList->erase(m_iItem);
	m_listbox.DeleteString(pos);
}
//此调用用于修改当前条目
//更新、修改当前的设置
void CSetParaAlarm::OnBnClickedUpdata()
{
// 	if(IsNameEmpty())
// 		return;
	UpdateData();
	if(!AlarmItemValidityCheck(m_item))
		return;
	int pos=m_listbox.GetCurSel();
	if(pos<0)
	{
		return;
	}
	else
	{
		m_listbox.DeleteString(pos);
		m_listbox.InsertString(pos,m_item.name);
		m_pList->at(pos)=m_item;
	}
}

//在当前位置插入一条目
void CSetParaAlarm::OnBnClickedInsert()
{
// 	if(IsNameEmpty())
// 		return;
	UpdateData();
	if(!AlarmItemValidityCheck(m_item))
		return;
	int pos=m_listbox.GetCurSel();
	if(pos<0){
		return;
	}else{
		m_listbox.InsertString(pos,m_item.name);
		m_iItem=m_pList->begin()+pos;
		m_pList->insert(m_iItem,m_item);
	}
}
//删除一个条目
void CSetParaAlarm::OnBnClickedDelete()
{
	int pos=m_listbox.GetCurSel();
	if(pos<0){
		if(m_listbox.GetCount())
			m_listbox.DeleteString(m_listbox.GetCount()-1);
		if(!m_pList->empty())
			m_pList->pop_back();
	}else{
		m_iItem=m_pList->begin()+pos;
		m_pList->erase(m_iItem);
		m_listbox.DeleteString(pos);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//Config file Operation
//
//保存当前的配置到文件
void CSetParaAlarm::OnBnClickedSave()
{
	/*/////////////////////////////////////////////////////////////////////////
	UpdateData();
	if (!m_pSet->list.empty())            // 分档报警列表为空
	{
		m_pSet->list.pop_back();
	}
	m_pSet->list.push_back(m_item);       // 写入当前分档报警列表
	if (!SeterValidityCheck(m_pSet->list))
	{
		return;
	}
	/////////////////////////////////////////////////////////////////////////*/
	ConfigFileOperation(GetFileName(SAVE),SAVE);
}
//从配置文件中读取
void CSetParaAlarm::OnBnClickedLoad()
{
	ConfigFileOperation(GetFileName(LOAD),LOAD);
}
//选择当前的配置，从配置文件表中选入修改区域
void CSetParaAlarm::OnBnClickedModify()
{
	int pos=m_filelist.GetCurSel();
	if(pos<0)
		return;
	Seter::iterator it=m_appset.begin()+pos;
	m_pSet->ver=it->ver ;
	m_pSet->size=it->size;
	m_pSet->type=it->type;
	for(int i = 0; i < ALARMNUM; i++)         //改动
	{
		m_pSet->item[i][MIN]=it->item[i][MIN];
		m_pSet->item[i][MAX]=it->item[i][MAX];
	}
	///////////////////////////改动 分档////////////////////////////////////
	switch(m_pSet->type)
	{
	case 0:
		{
			GetDlgItem(IDC_EDIT_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_2G)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3G)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);

		}
		break;
	case 1:
		{
			GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_3G)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
		}
		break;
	case 2:
		{
			GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3G)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
		}
		break;
	case 3:
		{
			GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_3G)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_4G)->EnableWindow(TRUE);
		}
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	memcpy(m_pSet->sys,it->sys,sizeof(BOOL)*CLASSNUM);
	m_pSet->list=AlarmList(it->list);

	if(m_pSet->list.size())
		m_item=m_pSet->list.front();
	AlarmList::iterator iter=m_pSet->list.begin();

	m_listbox.ResetContent();//不能重复添加
	for(;iter<m_pSet->list.end();iter++)                    // 此处有问题
		m_listbox.AddString(iter->name);
	m_listbox.SetCurSel(0);
	UpdateData(0);

}

//从配置文件表中删除一条，并删除相应的应用数据
void CSetParaAlarm::OnLbnDblclkFilelist()
{
	int pos=m_filelist.GetCurSel();
	if(pos<0)
		return;
	if(!m_appset.empty())
	{
		Seter::iterator iter;
		iter=m_appset.begin()+pos;
		m_appset.erase(iter);
	}
	m_filelist.DeleteString(pos);
}
CString CSetParaAlarm::GetFileName(BOOL o)
{
	TCHAR sCurrentDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),sCurrentDirectory,sizeof(sCurrentDirectory));
	for(size_t i=_tcslen(sCurrentDirectory)-1;i>0;i--)
	{
		if(sCurrentDirectory[i] == 92)
		{
			sCurrentDirectory[i] = 0;
			break;
		}
	}
	CString str=_T("Grade&Alarm List(*.PGA)|*.PGA|");
	CString str1=_T("PGA");

	CFileDialog of(TRUE,str1,NULL,NULL,str);
	CFileDialog sf(FALSE,str1,NULL,NULL,str);
	CFileDialog *p;
	if(o)
		p=&of;
	else
		p=&sf;
	
	p->m_ofn.lpstrInitialDir=sCurrentDirectory;

	if(p->DoModal()==IDOK )
	{
		str=p->m_ofn.lpstrFile ;
	}
	else str.Empty ();
	return str;
}
// 此操作不影响当前交互操作区_Seter的数据内容
// 把相应的_Seter数据加入到m_appset中，用于对参数数据的实际处理
BOOL CSetParaAlarm::ConfigFileOperation(CString  fn, BOOL o)
{
	if(fn.IsEmpty ())
		return false;
	
	CFile myFile;
	if(o)
	{
		int nIndex = m_filelist.FindString(0,fn);
		if (-1 != nIndex)                              // 文件列表已存在则不打开
		{
			return false;
		}

		if(!myFile.Open (fn, CFile::modeNoTruncate| CFile::modeReadWrite)){
		MessageBox(_T("Open file error\n\r")+fn+_T("\n\rPlease create or select other \"PGA\" file"));
		return false;
		}
	}
	else if(!myFile.Open (fn, CFile::modeCreate |CFile::modeNoTruncate| CFile::modeReadWrite))
	{
		MessageBox(_T("Save file error\n\r")+fn+_T("\n\rPlease check it"));
		return false;
	}

	myFile.SeekToBegin();
	
	if(o)//load
	{
		if(!GetFileSize(myFile,NULL))
		{
			MessageBox(_T("The file size is 0 byte\n\r")+fn);
			return false;
		}
		CArchive arLoad(&myFile, CArchive::load);
		Serialize(arLoad);                             //与单_Seter文件交互并选入应用列表，用于实际的测试数据处里
		arLoad.Close();
		m_filelist.AddString(fn);
	}
	else //save Present operater area to file
	{
		UpdateData();//更新存储变量
		if(SeterValidityCheck(m_pSet->list) || AlarmItemValidityCheckNoBeep_arm())
		{
			int nIndex = m_filelist.FindString(0,fn);
			if (-1 != nIndex)                              // 文件列表已存在则不打开
			{
				m_filelist.DeleteString(nIndex);
			}
			CArchive arStore(&myFile, CArchive::store);
			Serialize(arStore);                        //与单_Seter文件交互并选入应用列表，用于实际的测试数据处里
			m_filelist.AddString(fn);                  //把当前文件名存入列表
			arStore.Close();
		}
	}
	myFile.Close ();
	return true;
}
BOOL CheckMinMax(double min,double max)
{
	if((max<0||min<0)||(min>max))
		return FALSE;
	return TRUE;
}
//AlarmItem的有效性检查:
//1 CheckMinMax返回全真则返回TRUE
//2 名字？非空
//3 area（？CheckMinMax=TRUE）
BOOL CSetParaAlarm::AlarmItemValidityCheck( AlarmItem& item)
{
	BOOL ret=AlarmItemValidityCheckNoBeep(item);
	if(!ret)
		Beep(1000,50);
	return ret;
}

BOOL CSetParaAlarm::AlarmItemValidityCheckNoBeep_arm()
{
	CString errorItem[ALARMNUM] = {L"Isc",L"Voc",L"Pm",L"FF",L"I_Id",L"Eff",L"Im",L"Vm",L"Rs",L"Rsh",L"Irev",L"EnvTemp",L"TMod",L"Sun"};
	for(int i=0;i<ALARMNUM;i++)
	if(!CheckMinMax(m_pSet->item[i][MIN],m_pSet->item[i][MAX]))  //改动
	{
	//	AfxMessageBox(errorItem[i]);
		return FALSE;
	}
	return TRUE;
}
BOOL CSetParaAlarm::AlarmItemValidityCheckNoBeep( AlarmItem& Item)
{
	CString str(Item.name);
	if((str!=_T(""))&&(!CheckMinMax(Item.area[0][MIN],Item.area[0][MAX])))       // 非空则检验上下限值
	{
	//	AfxMessageBox(L"Error: "+str);
		return FALSE;
	}
// 	CString errorItem[ALARMNUM] = {L"Isc",L"Voc",L"Pm",L"FF",L"Idot",L"Eff",L"Im",L"Vm",L"Rs",L"Rsh",L"Irev",L"EnvTemp",L"SurfTemp"};
// 	for(int i=0;i<ALARMNUM;i++)
// 	if(!CheckMinMax(item[i][MIN],item[i][MAX]))
// 	{
// 	//	AfxMessageBox(errorItem[i]);
// 		return FALSE;
// 	}
	return TRUE;
}
//AlarmItem列表的有效性检查
//名字非空
//档位MIN>=0;MIN<=MAX,IN type(MIN,MAX)
//ALARM,MIN>=0;MIN<=MAX,IN type(MIN,MAX)
BOOL CSetParaAlarm::SeterValidityCheck( AlarmList& list)
{
	if(list.empty())
		return FALSE;
	BOOL ret=TRUE;
	AlarmList::iterator it;
	it=list.begin();
	for(it;it<list.end();it++)
		if(!AlarmItemValidityCheck(*it))
			return FALSE;
	return TRUE;
}
void CSetParaAlarm::Serialize(CArchive &ar)
{
	if(ar.IsLoading())
	{//Load one _Seter from file
		_Seter st;
		ar>>st.ver>>st.size>>st.type;
		switch(st.ver)
		{//版本差异处理
			case VERFIRST:break;
			case VERFIRST+1:break;
			default:break;
		}
		BOOL* iBOOL;
		iBOOL=&st.sys[0];

		int i=0;
		for(i=0;i<CLASSNUM;i++)
			ar>>*iBOOL++;

		for(i=0;i<ALARMNUM;i++)    //改动
		{
			ar>>st.item[i][MIN]>>st.item[i][MAX];
		}

		AlarmItem it;
		UINT k=0;
		
		for(;k<st.size;k++)
		{
			for(i=0;i<NAMELENGTH;i++)               //改动 分档
			{
				ar>>it.name[i];
			}
			for(i = 0;i< 4;i++)
			{
				ar>>it.m_class[i]>>it.area[i][MIN]>>it.area[i][MAX]>>it.m_Gname[i];
			}
			st.list.push_back(it);
		}
		m_appset.push_back(st);

	}else
	{//save one _Seter to file
		if(!m_pSet)
			return;
		m_pSet->size=(UINT)m_pSet->list.size();
		m_pSet->ver=VERFIRST;//可升级版本
		ar<<m_pSet->ver<<m_pSet->size<<m_pSet->type;
		BOOL* iBOOL;
		iBOOL=&m_pSet->sys[0];

		int i=0;
		for(i=0;i<CLASSNUM;i++)
			ar<<*iBOOL++;

		for(i=0;i<ALARMNUM;i++)
		{
			ar<<m_pSet->item[i][MIN]<<m_pSet->item[i][MAX];				
		}
		int k=0;
		AlarmList::iterator iter=m_pSet->list.begin();		
		for(;iter<m_pSet->list.end();iter++)
		{
			for(i=0;i<NAMELENGTH;i++)
			{
				ar<<iter->name[i];
			}
			for(i=0;i<4;i++)                                //改动 分档
			{
				ar<<iter->m_class[i]<<iter->area[i][MIN]<<iter->area[i][MAX]<<iter->m_Gname[i];
			}
			
		}
		m_appset.push_back(*m_pSet);

	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//将m_appset应用于CSunPara中，返回是否警告，分级和警报信息
//
//有报警返回TRUE
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//转换double为n为有效数字的字符串
//不带有四舍五入，舍去法处理
//在CSetCurveIV中引用
//可以处理带符号值
CString SignificanceDigitString(double value,int n)
{
	if(value==0||(n<=0))
		return CString(_T("0"));

	CString num;
	num.Format(_T("%.20lf"),value);
	num.TrimRight(_T('0'));
	int len=num.GetLength();
	int trimlen=0,pdot=0,p9=0;//有效位;
	int i=0;
	TCHAR ch=0;
	//找到第一个有效位
	for(;i<len;i++){
		ch=num.GetAt(i);
		if(ch!=_T('0'))
			if(ch!=_T('+'))
			if(ch!=_T('-'))
			if(ch!=_T('.'))
				break;
			else pdot=i;
	}
	if(i==len)
		return CString(_T("0"));
	//找到全部有效位
	for(;i<len;i++){
		ch=num.GetAt(i);
		if(ch!=_T('.'))
			 p9++;
		else pdot=i;
		if(p9==n)
			break;
	}
	if(p9<n)//有效位不够
		return num.TrimRight(_T('0'));
	else{//==n
		trimlen=++i;
		if(pdot!=0)//足位有点不需写0
			return 	(num.Left(trimlen)).TrimRight(_T('0'));
	}
	//找到.
	for(;i<len;i++){
		ch=num.GetAt(i);
		if(ch==_T('.'))
			break;
		num.SetAt(i,_T('0'));
		trimlen++;
	}
	return 	num.Left(trimlen); 
}
BOOL InArea(const double& obj,const double& min,const double& max)
{
	if((max<0)||(min<0)||(min>max)||min==max)
		return FALSE;
	return (obj<min)||(obj>=max);//大于等于max报警，小于min报警
}
BOOL CSetParaAlarm::AlarmDeciding( CSunPara& sc,_Seter& set,CStringArray& inf)      //显示改动  报警
{
	// 	if(Item==NULL)
	// 	{
	// 	//	AfxMessageBox(L"档位无效！");
	// 		return FALSE;
	// 	}

	for(int i = 0; i < ALARMNUM; i++)         //改动
	{
		m_pSet->item[i][MIN] = set.item[i][MIN];
		m_pSet->item[i][MAX] = set.item[i][MAX];
	}
	double obj=0;

	UINT i=0;
	BOOL ret=FALSE;
	CString name;
	for(i=0;i<ALARMNUM;i++){
		switch(i){
			case (CSetParaAlarm::SURFTEMP):name=TEXT("!TMod:");obj=sc.GetSurfTemp();break;
			case (CSetParaAlarm::ISC):name=TEXT("!Isc:");obj=sc.GetIsc();break;
			case (CSetParaAlarm::VOC):name=TEXT("!Voc:");obj=sc.GetVoc();break;
			case (CSetParaAlarm::PM):name=TEXT("!Pmax:");obj=sc.GetPmax();break;
			case (CSetParaAlarm::I1):name=TEXT("!I_Id:");obj=sc.GetIdot();break;
			case (CSetParaAlarm::EFF):name=TEXT("!Eff:");obj=sc.GetEff();break;
			case (CSetParaAlarm::FF):name=TEXT("!FF:");obj=sc.GetFF();break;
			case (CSetParaAlarm::RS):name=TEXT("!Rs:");obj=sc.GetRs();break;
			case (CSetParaAlarm::RSH):name=TEXT("!Rsh:");obj=sc.GetRsh();break;
			case (CSetParaAlarm::IM):name=TEXT("!Imax:");obj=sc.GetImax();break;
			case (CSetParaAlarm::VM):name=TEXT("!Vmax:");obj=sc.GetVmax();break;
			case (CSetParaAlarm::IREV):name=TEXT("!Irev:");obj=sc.GetIrev();break;
			case (CSetParaAlarm::ENVTEMP):name=TEXT("!EnvTemp:");obj=sc.GetEnvTemp();break;		
			case (CSetParaAlarm::SUN):name=TEXT("!SUN:");obj=sc.GetSun();break;
		}
		if(InArea(obj,m_pSet->item[i][MIN],m_pSet->item[i][MAX]))
		{
			ret=TRUE;		
			inf.Add(name+SignificanceDigitString(obj,4));         //保留4位有效数字
		}
	}
	return ret;
}
BOOL CSetParaAlarm::AlarmDecidingTmod( CSunPara& sc,_Seter& set,CStringArray& inf)      //显示改动  报警
{
// 	if(Item==NULL)
// 	{
// 	//	AfxMessageBox(L"档位无效！");
// 		return FALSE;
// 	}

	//for(int i = 0; i < ALARMNUM; i++)         //改动
	{
		m_pSet->item[SURFTEMP][MIN] = set.item[SURFTEMP][MIN];
		m_pSet->item[SURFTEMP][MAX] = set.item[SURFTEMP][MAX];
	}
	double obj=0;
	
	UINT i=0;
	BOOL ret=FALSE;
	CString name;

	name=TEXT("!TMod:");
	obj=sc.GetSurfTemp();
	if(InArea(obj,m_pSet->item[SURFTEMP][MIN],m_pSet->item[SURFTEMP][MAX]))
	{
		ret=TRUE;		
		inf.Add(name+SignificanceDigitString(obj,4));         //保留4位有效数字
		inf.Add(_T("out of range!"));
	}

	return ret;
}
CSetParaAlarm::AlarmItem* CSetParaAlarm::GradeDeciding( CSunPara& sc,_Seter& set,CStringArray& inf)     //改动 分档 难点
{
	if(set.list.empty())
		return NULL;
	CString gs;            
	double obj[4];
	memset(&obj,0,sizeof(obj));

	AlarmItem* pIt=NULL;
	AlarmList::iterator iter;
	iter=set.list.begin();
	switch(set.type)
	{
		case 0:
			for(iter;iter<set.list.end();iter++)//确定档位
			{
				obj[0] = ReGetWhichData(iter->m_class[0],sc);
				if((obj[0]>=iter->area[0][MIN])&&(obj[0]<iter->area[0][MAX]))
				{
					pIt=&(*iter);
					gs=iter->name;       //改动  可以实现 只显示分档的名字  不显示数据等
					break;
				}
			}
			break;
		case 1:
			for(iter;iter<set.list.end();iter++)//确定档位
			{
				obj[0] = ReGetWhichData(iter->m_class[0],sc);
				obj[1] = ReGetWhichData(iter->m_class[1],sc);
				if((obj[0]>=iter->area[0][MIN])&&(obj[0]<iter->area[0][MAX]))
					if((obj[1]>=iter->area[1][MIN])&&(obj[1]<iter->area[1][MAX]))
					{
						pIt=&(*iter);
						gs=iter->name;       //改动  可以实现 只显示分档的名字  不显示数据等
						break;
					}
			}

			break;
		case 2:
			for(iter;iter<set.list.end();iter++)//确定档位
			{
				obj[0] = ReGetWhichData(iter->m_class[0],sc);
				obj[1] = ReGetWhichData(iter->m_class[1],sc);
				obj[2] = ReGetWhichData(iter->m_class[2],sc);
				if((obj[0]>=iter->area[0][MIN])&&(obj[0]<iter->area[0][MAX]))
					if((obj[1]>=iter->area[1][MIN])&&(obj[1]<iter->area[1][MAX]))
						if((obj[2]>=iter->area[2][MIN])&&(obj[2]<iter->area[2][MAX]))
						{
							pIt=&(*iter);
							gs=iter->name;       //改动  可以实现 只显示分档的名字  不显示数据等
							break;
						}
			}
			break;
		case 3:
			for(iter;iter<set.list.end();iter++)//确定档位
			{
				obj[0] = ReGetWhichData(iter->m_class[0],sc);
				obj[1] = ReGetWhichData(iter->m_class[1],sc);
				obj[2] = ReGetWhichData(iter->m_class[2],sc);
				obj[3] = ReGetWhichData(iter->m_class[3],sc);
				if((obj[0]>=iter->area[0][MIN])&&(obj[0]<iter->area[0][MAX]))
					if((obj[1]>=iter->area[1][MIN])&&(obj[1]<iter->area[1][MAX]))
						if((obj[2]>=iter->area[2][MIN])&&(obj[2]<iter->area[2][MAX]))
							if((obj[3]>=iter->area[3][MIN])&&(obj[3]<iter->area[3][MAX]))
							{
								pIt=&(*iter);
								gs=iter->name;       //改动  可以实现 只显示分档的名字  不显示数据等
								break;
							}
			}
			break;
	}
	if(iter==set.list.end())
	{
		gs=TEXT("NONAME");
		pIt=NULL;
	}
	sc.SetGrade(gs);//设置档位
	inf.Add(gs);
	return pIt;
	//////////////////////////////////////////////////////////////////////////
}
//改动  分档  获得数据
double CSetParaAlarm::ReGetWhichData(CString Bname,CSunPara& sc)
{
	int ret;
	double dataBuf = 0;
	if (Bname.CompareNoCase(_T("Ivld")) == 0)
	{
		ret = 0;
	}else if (Bname.CompareNoCase(_T("Pmax")) == 0)
	{
		ret = 1;
	}else if (Bname.CompareNoCase(_T("Eff")) == 0)
	{
		ret = 2;
	}else if (Bname.CompareNoCase(_T("Imax")) == 0)
	{
		ret = 3;
	}else
	{
		return 0;
	}
	switch(ret)
	{
	case 0:dataBuf = sc.GetIdot();break;
	case 1:dataBuf = sc.GetPmax();break;
	case 2:dataBuf = sc.GetEff(); break;
	case 3:dataBuf = sc.GetImax();break;
	}
	return dataBuf;
}

//由一个Seter产生分档报警信息
BOOL CSetParaAlarm::ApplayFilterSeter( CSunPara& sc,_Seter& set,CStringArray& inf)
{
	//第一个串区别分档和报警，分档@,报警!开头
	//分档信息格式：@+档位名称+数值
	//报警信息格式：!+项名+数值

	AlarmItem* item;
	item=GradeDeciding(sc,set,inf);
	if(set.sys[SWALARM])//报警禁止针对单个文件处理
		return FALSE;
	return AlarmDeciding(sc,set,inf);      //改动
}

//由一个Seter产生分档报警信息
BOOL CSetParaAlarm::ApplayFilterSeterTmod(CSunPara& sc,_Seter& set,CStringArray& inf)
{

	if(set.sys[SWALARM])//报警禁止针对单个文件处理
		return FALSE;
	return AlarmDecidingTmod(sc,set,inf);      //改动
}
#include "..\SolarCellTesterMainFrm.h"
#include "..\DisplayInfo.h"
void CSetParaAlarm::DisplayInfo(CStringArray& gaInfo)
{
	CDisplayInfo* pBar=((CSolarCellTesterMainFrame*)AfxGetMainWnd())->GetInfoBox();
	
	if(GetNoDisplay())
		pBar->ShowWindow(SW_HIDE);
	else
		pBar->ShowWindow(SW_SHOW);

	pBar->ClearBoard();
	pBar->DisplayMessage(&gaInfo);
}
//对参数表使用多组_Seter进行判定，保存判定表，可以通过CStringArray* inf传回报告
//返回值告知有没有报警信息，报警返回TRUE
BOOL CSetParaAlarm::FilterSeter( CSunPara& sc,CStringArray* inf)
{
	if(m_appset.empty())
	{
		m_gaInfo.RemoveAll();           // 清空分档报警显示列表
		DisplayInfo(m_gaInfo);
		return FALSE;
	}
	
	BOOL ret=FALSE;
	m_gaInfo.RemoveAll();//清空报告
	int pos=m_filelist.GetCurSel();
	while(pos<0)
	{
		m_filelist.SetCurSel(0);
	    pos=m_filelist.GetCurSel();
	}
	Seter::iterator iter;
	iter=m_appset.begin()+pos;
	
// 	for(iter;iter<m_appset.end();iter++)         //改动 分档 
// 	{
	if(ApplayFilterSeter(sc,*iter,m_gaInfo))
	{
		ret=TRUE;                     //有报警
		if(!iter->sys[SWRING])        //disable ring
			Beep(1000,100);
	}
//	}
	
	DisplayInfo(m_gaInfo);
	if(inf)
	{
		inf->RemoveAll();
		inf->Append(m_gaInfo);
	}
	return ret;
}

BOOL CSetParaAlarm::FilterSeterTmod( CSunPara& sc,CStringArray* inf)
{
	if(m_appset.empty())
	{
		m_gaInfo.RemoveAll();           // 清空分档报警显示列表
		DisplayInfo(m_gaInfo);
		return FALSE;
	}

	BOOL ret=FALSE;
	m_gaInfo.RemoveAll();//清空报告
	int pos=m_filelist.GetCurSel();
	while(pos<0)
	{
		m_filelist.SetCurSel(0);
		pos=m_filelist.GetCurSel();
	}
	Seter::iterator iter;
	iter=m_appset.begin()+pos;

	// 	for(iter;iter<m_appset.end();iter++)         //改动 分档 
	// 	{
	if(ApplayFilterSeterTmod(sc,*iter,m_gaInfo))
	{
		ret=TRUE;                     //有报警
		if(!iter->sys[SWRING])        //disable ring
			Beep(1000,100);
	}
	//	}

	DisplayInfo(m_gaInfo);
	if(inf)
	{
		inf->RemoveAll();
		inf->Append(m_gaInfo);
	}
	return ret;
}
//TRUE则不保存有报警的参数组
BOOL CSetParaAlarm::GetErrorNoSave()
{
	if(m_appset.size()<=0)
		return FALSE;
	BOOL ret=FALSE;
	Seter::iterator it=m_appset.begin();
	for(;it<m_appset.end();it++)
		if(it->sys[SWNOSAVE])
			ret=TRUE;

	return ret;
}
//Tmod超过范围不闪光
BOOL CSetParaAlarm::GetTModNOFlash()
{
	if(m_appset.size()<=0)
		return FALSE;
	BOOL ret=FALSE;
	Seter::iterator it=m_appset.begin();
	for(;it<m_appset.end();it++)
		if(it->sys[SWTMODNOFLASH])
			ret=TRUE;

	return ret;
}
//返回报警和分档显示开关设置
BOOL CSetParaAlarm::GetNoDisplay(void)
{
	if(m_appset.size()<=0)
		return FALSE;
	BOOL ret=FALSE;
	Seter::iterator it=m_appset.begin();
	for(;it<m_appset.end();it++)
		if(it->sys[SWDISPLAY])
			ret=TRUE;
	return ret;


}
//分档和报警设置队列中有无允许报警开关
BOOL CSetParaAlarm::GetDisableAlarm(void)
{
	if(m_appset.size()<=0)
		return FALSE;
	BOOL ret=FALSE;
	Seter::iterator it=m_appset.begin();
	for(;it<m_appset.end();it++)
		if(it->sys[SWALARM])
			ret=TRUE;
	return ret;
}
BOOL CSetParaAlarm::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
    Apply(wParam);    // 判断控件是否更改
	return CSetInterFace::OnCommand(wParam, lParam);
}

void CSetParaAlarm::LoadReg(void)
{
	/*	int cnt=0;
	OpReg()->Read(m_filecountkey,&cnt);
	if(cnt<=0)
	return;
	CString str,name;
	m_filelist.ResetContent();
	for(int i=0;i<cnt;i++){
	str.Format(_T("%d"),i);
	OpReg()->Read(m_filenamekey+str,name);
	if ( 0== PathFileExists(name))
	break;
	ConfigFileOperation(name,LOAD);
	}
	*/

	GradeAlarm gradeAlarm;

	// 读取配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo3.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeRead|CFile::shareExclusive))
	{
		file.Read(&gradeAlarm,sizeof(GradeAlarm));
		file.Close();
	}
	else
		return;

	int cnt=gradeAlarm.m_number;
	for(int i=0;i<cnt;i++)
	{
		if ( 0== PathFileExists(gradeAlarm.m_GAList[i]))
			break;
		ConfigFileOperation(gradeAlarm.m_GAList[i],LOAD);
	}

	UpdateData(0);


}
//由apply调用，保存文件列表
void CSetParaAlarm::SaveReg(void)
{
	/*	int cnt=m_filelist.GetCount();
	if(cnt<=0)
	return;
	OpReg()->Write(m_filecountkey,cnt);
	CString name,str;
	for(int i=0;i<cnt;i++){
	m_filelist.GetText(i,name);
	str.Format(_T("%d"),i);
	OpReg()->Write(m_filenamekey+str,name);
	name.ReleaseBuffer();
	}*/

	UpdateData();
	GradeAlarm gradeAlarm;
	int cnt=m_filelist.GetCount();
	gradeAlarm.m_number = cnt;
	CString filename;
	for(int i=0;i<cnt;i++)
	{
		m_filelist.GetText(i,filename);
		_tcscpy(gradeAlarm.m_GAList[i],filename);
		filename.ReleaseBuffer();
	}

	// 保存配置文件
	CString configFileName = L"C:\\Documents and Settings\\Administrator\\桌面\\新建文件夹\\UserInfo3.Dat";

	CFile file;
	if(file.Open(configFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareExclusive)){
		file.Write(&gradeAlarm, sizeof(GradeAlarm));
		file.Close();
	}
}

GradeAlarm CSetParaAlarm::GetAppliedConfig(void)
{
	UpdateData();
	GradeAlarm gradeAlarm;
	gradeAlarm.m_number = 0;
	memset(&gradeAlarm, 0, 10*255*sizeof(TCHAR));
	int cnt=m_filelist.GetCount();
	gradeAlarm.m_number = cnt;
	CString filename;
	for(int i=0;i<cnt;i++)
	{
		m_filelist.GetText(i,filename);
		_tcscpy(gradeAlarm.m_GAList[i],filename);
	}
	
/*	gradeAlarm.m_number = cnt;
	CString filename;
	for(int i=0;i<cnt;i++)
	{
		m_filelist.GetText(i,filename);
		_tcscpy(gradeAlarm.m_GAList[i],filename);
	}*/

	return gradeAlarm;

}

void CSetParaAlarm::SetAppliedConfig(GradeAlarm* gradeAlarm)
{
	int cnt=gradeAlarm->m_number;
	m_filelist.ResetContent();
	for(int i=0;i<cnt;i++)
	{
		if ( 0== PathFileExists(gradeAlarm->m_GAList[i]))
			break;
		ConfigFileOperation(gradeAlarm->m_GAList[i],LOAD);
	}

	UpdateData(0);
}

void CSetParaAlarm::SetParaAlarmDefault()
{

}
void CSetParaAlarm::OnBnClickedRadio1()
{
	UpdateData(1);
	for(int i = 0;i < 4; i++)
	{
		m_item.area[i][MIN] = 0;
		m_item.area[i][MAX] = 0;
		m_item.m_class[i] = _T("");
		m_item.m_Gname[i] = _T("");

	}
	m_pList->clear();
	m_listbox.ResetContent();
	
	GetDlgItem(IDC_EDIT_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_2G)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3G)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
	UpdateData(0);
}
void CSetParaAlarm::OnBnClickedRadio2()
{
	UpdateData(1);

	for(int i = 0;i < 4; i++)
	{
		m_item.area[i][MIN] = 0;
		m_item.area[i][MAX] = 0;
		m_item.m_class[i] = _T("");
		m_item.m_Gname[i] = _T("");
	}
	m_pList->clear();
	m_listbox.ResetContent();

	GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_3G)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
	UpdateData(0);

}

void CSetParaAlarm::OnBnClickedRadio3()
{
	UpdateData(1);

	for(int i = 0;i < 4; i++)
	{
		m_item.area[i][MIN] = 0;
		m_item.area[i][MAX] = 0;
		m_item.m_class[i] = _T("");
		m_item.m_Gname[i] = _T("");
	}
	m_pList->clear();
	m_listbox.ResetContent();

	GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3G)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_4G)->EnableWindow(FALSE);
	UpdateData(0);

}
void CSetParaAlarm::OnBnClickedRadio4()
{
	UpdateData(1);

	for(int i = 0;i < 4; i++)
	{
		m_item.area[i][MIN] = 0;
		m_item.area[i][MAX] = 0;
		m_item.m_class[i] = _T("");
		m_item.m_Gname[i] = _T("");
	}
	m_pList->clear();
	m_listbox.ResetContent();

	GetDlgItem(IDC_EDIT_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_2G)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_3G)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_4MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_4MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_4G)->EnableWindow(TRUE);
	UpdateData(0);

}

void CSetParaAlarm::OnBnClickedGStyle()
{
	switch (m_pSet->type)
	{
	case 0:
		{
			m_TypeList.GetText(0,m_item.m_class[0]);
		}
		break;
	case 1:
		{
			m_TypeList.GetText(0,m_item.m_class[0]);
			m_TypeList.GetText(1,m_item.m_class[1]);
		}
		break;
	case 2:
		{
			m_TypeList.GetText(0,m_item.m_class[0]);
			m_TypeList.GetText(1,m_item.m_class[1]);
			m_TypeList.GetText(2,m_item.m_class[2]);
		}
		break;
	case 3:
		{
			m_TypeList.GetText(0,m_item.m_class[0]);
			m_TypeList.GetText(1,m_item.m_class[1]);
			m_TypeList.GetText(2,m_item.m_class[2]);
			m_TypeList.GetText(3,m_item.m_class[3]);
		}
		break;

	}

	UpdateData(0);
}
