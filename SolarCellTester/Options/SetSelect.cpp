// Select.cpp : implementation file
//

#include "stdafx.h"
#include "Registry.h"
#include "SetSelect.h"
#include "..\resource.h"

// CSetSelect dialog

IMPLEMENT_DYNAMIC(CSetSelect, CDialog)

CSetSelect::CSetSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSelect::IDD, pParent)
{
	m_class = -1;
	m_level = 0.0f;
	m_name = _T("");
	memset(nrge,0,sizeof(nrge));
	m_class=1; //功率
	rn=0;//档位的数量
}

CSetSelect::~CSetSelect()
{
}

void CSetSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RANGRE, m_level);
	DDV_MinMaxFloat(pDX, m_level, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 10);
	DDX_Radio(pDX, IDC_RADIO1, m_class);
	DDX_Control(pDX, IDC_LIST_DISLEVEL, m_disp);
}


BEGIN_MESSAGE_MAP(CSetSelect, CDialog)
	ON_BN_CLICKED(ID_LOAD, &CSetSelect::OnBnClickedLoad)
	ON_BN_CLICKED(ID_SAVE, &CSetSelect::OnBnClickedSave)
	ON_BN_CLICKED(ID_APPEND, &CSetSelect::OnBnClickedAppend)
	ON_LBN_DBLCLK(IDC_LIST_DISLEVEL, &CSetSelect::OnLbnDblclkListDislevel)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CSetSelect message handlers

CString CSetSelect::LvlName(CString name)
{/*
	if(name.IsEmpty())
	{
		return theApp.GetProfileString(_T("Set"),_T("LVLNAME"));
	}
	else
	{
		theApp.WriteProfileString(_T("Set"),_T("LVLNAME"),name);
		
	}*/return name;
}

void CSetSelect::OnBnClickedLoad()
{
	OSLVL(GetFileName(TRUE),TRUE);
}

void CSetSelect::OnBnClickedSave()
{
	OSLVL(GetFileName(FALSE),FALSE);
}

void CSetSelect::OnBnClickedAppend()
{
	UpdateData();
	if(!m_name.IsEmpty ())
	{
		if(rn>=NUM)
		{
			AfxMessageBox(_T("Over area"),MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(rn>0)
		{
			if(nrge[rn-1]>=m_level)
			{
				AfxMessageBox(_T("Input > Prev. Grade!"),MB_OK|MB_ICONINFORMATION);
				return;
			}
			for(UINT n=0;n<rn;n++)
				if(m_name==nstr[n])
				{
				AfxMessageBox(_T("Name dont same!"),MB_OK|MB_ICONINFORMATION);
				return;
				}
		}
		nstr[rn]=m_name;
		nrge[rn]=m_level;
		AddItem(m_name,m_level);
		rn=m_disp.GetCount ();
	}	
	else AfxMessageBox(_T("Please input name!"),MB_OK|MB_ICONINFORMATION);
}

void CSetSelect::DisLevel(CDC* pDC, float I, float Pm, float Eff)
{
		if(nstr[0].IsEmpty ()||rn<2)return;
	
	CString str,cls;
	float eg=0;
	switch(m_class)
	{
	case 0:eg=I;cls.Format(_T("(I:%0.2f)"),I);break;
	case 1:eg=Pm;cls.Format(_T("(Pm:%0.2f)"),Pm);break;
	case 2:eg=Eff;cls.Format(_T("(Eff:%0.2f)"),Eff);break;
	}

	UINT n;
	for(n=1;n<rn;n++)
		if(eg<nrge[n])
			break;

	if(eg<0)str=_T("<<<");
	else
	if(n==rn){
		str=_T(">>>");
	}
	else{
		n--;
		str=nstr[n];//+cls;
	}

	int md=pDC->GetMapMode();
	pDC->SetMapMode(MM_LOENGLISH);//设置映射模式(y向上增加，x向右增加)
	CFont fnt,*oldf;
	fnt.CreateFont(36,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH| FF_SCRIPT,_T("宋体"));
	oldf=pDC->SelectObject(&fnt);
	COLORREF c=pDC->SetTextColor(RGB(0,0,255));
	RECT rt;
	pDC->GetWindow ()->GetClientRect (&rt);
	CSize sz=pDC->GetOutputTextExtent(str);
	pDC->TextOut (rt.right/2-sz.cx/2,0,str);
	//pDC->TextOut (pt.x,pt.y,str);
	pDC->SetTextColor(c);
	pDC->SelectObject(oldf);
	fnt.DeleteObject ();
	pDC->SetMapMode(md);
}

void CSetSelect::OnLbnDblclkListDislevel()
{
	int n=m_disp.GetCurSel ();
	m_disp.DeleteString (n);

	UINT i;
	for(i=n;i<rn-1;i++)
	{
		nstr[i]=nstr[i+1];
		nrge[i]=nrge[i+1];
	}
	nstr[i].Empty ();
	nrge[i]=0;
	rn=m_disp.GetCount ();
}

void CSetSelect::AddItem(CString name, float level)
{
	CString str;
	str.Format (_T("%f"),level);
	str=name+_T(" Min.")+str;
	m_disp.InsertString(m_disp.GetCount (),str.GetBuffer(str.GetLength()));
}

BOOL CSetSelect::OSLVL(CString  fn, bool o)
{
	if(fn.IsEmpty ())
		return false;
	
	CFile myFile;
	if(o){
		if(!myFile.Open (fn, CFile::modeNoTruncate| CFile::modeReadWrite)){
		MessageBox(_T("Open file error\n\r")+fn+_T("\n\rPlease create or select other \"LVL\" file"));
		return false;
		}
	}
	else if(!myFile.Open (fn, CFile::modeCreate |CFile::modeNoTruncate| CFile::modeReadWrite)){
		MessageBox(_T("Save file error\n\r")+fn+_T("\n\rPlease check it"));
		return false;
	}

	myFile.SeekToBegin();
	
	if(o)//load
	{
		if(!GetFileSize(myFile,NULL)){
			MessageBox(_T("The file size is 0 byte\n\r")+fn);
			return false;
		}
		
		CArchive arLoad(&myFile, CArchive::load);
		arLoad>>m_class>>rn;
		if(m_disp)
			m_disp.ResetContent ();
		for(UINT i=0;i<NUM;i++)
		{
			arLoad>>nrge[i]>>nstr[i];
			if(i<rn)if(this->m_hWnd)
				AddItem(nstr[i],nrge[i]);
		}
		m_name=nstr[0];
		m_level=nrge[0];
		if(this->m_hWnd)
			UpdateData(FALSE);
		arLoad.Close();
	}
	else //save
	{
		UpdateData();
		CArchive arStore(&myFile, CArchive::store);
		arStore<<m_class<<rn;
		for(int i=0;i<NUM;i++)
			arStore<<nrge[i]<<nstr[i];
		arStore.Close();
	}
	myFile.Close ();
	m_filename=fn;
	return true;
}

CString CSetSelect::GetFileName(bool o)
{
	TCHAR sCurrentDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),sCurrentDirectory,sizeof(sCurrentDirectory));
	for(int i=_tcslen(sCurrentDirectory)-1;i>0;i--)
	{
		if(sCurrentDirectory[i] == 92)
		{
			sCurrentDirectory[i] = 0;
			break;
		}
	}
	CString str=_T("Grade List(*.LVL)|*.LVL");
	CString str1=_T("LVL");

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

BOOL CSetSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(!(nstr[0].IsEmpty ()||rn<1))
		for(UINT i=0;i<rn;i++)
			AddItem(nstr[i],nrge[i]);
	return TRUE; 
}


