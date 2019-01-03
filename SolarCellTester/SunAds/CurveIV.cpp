#include "StdAfx.h"
#include "..\stdafx.h"
#include "..\SolarCellTester.h"
#include   <atlimage.h>
#include "CurveIV.h"
#include "..\FastJpegWR\JpegFastOp.h"
#pragma comment(lib, "FastJpegWR\\FastJpegWR.lib")


bool GenerateUniqueFileName(string& strRetName, string& strRetFullPath, const string& strFileFullPath)
{
	strRetFullPath = strFileFullPath;
	string::size_type stPos = strRetFullPath.rfind('\\');
	if (stPos == strRetFullPath.npos)
		return false;
	strRetName = strRetFullPath.substr(stPos+1);
	if (_access(strRetFullPath.c_str(), 0) != 0)
		return true;
	string strDir = strRetFullPath.substr(0, stPos+1);
	string strFileName = strRetFullPath.substr(stPos+1);
	string strExt;
	string::size_type pos = strFileName.rfind('.');
	if (pos != strRetFullPath.npos)
	{
		strExt = strFileName.substr(pos+1);
		strFileName = strFileName.substr(0, pos);
	}
	int i = 1;
	char szBuff[11] = {0};
	do
	{
		sprintf_s(szBuff, 11, "(%d)", i++);
		strRetName = strFileName;
		if (!strExt.empty())
		{
			strRetName += szBuff;
			strRetName += ".";
			strRetName += strExt;     
		}
		else
		{
			strRetName += szBuff;
		}
		strRetFullPath = strDir + strRetName;
	} while (_access(strRetFullPath.c_str(), 0) == 0);

	return true;
}

CString DtoStr(double val)
{
	CString str;
	str.Format(_T("%f"),val);
	return str;
}

CurveIV::CurveIV(int size)
	:CDoubleXY( size)
	,CalibrationFiltering()
{
	
}
CurveIV::CurveIV()
:CDoubleXY()
,CalibrationFiltering()
{

}
CurveIV::~CurveIV(void)
{

}
//给出定制的显示列表
//flag缺省false为输出定制列表，true输出全部列表
CString CurveIV::GetCustomParaListString(bool flag)
{
	if(flag)
		return GetParaListString();
	else
		return m_setiv->BuildDisplay(this);
}
//求给定的一段线中的最大功率点
//专用于两次测计算Pmax
//在CalSunPara(void)中间进行功率修正
void CurveIV::EnvalPmax(CDoubleXY* maxiv,UINT rge)
{
//	SaveDaq();//保存原始数据
	CalFilter(this);//修正，校准

	TransformerParameter(this,30);
/*
	EvaluationEff(m_setcal->GetAreaEff());//求Eff
	//在两次测量Pmax模式下这两个值无效
	SetVdot(m_setcal->GetVdot());//保存Vdot
	SetIdot(EvaluateY(GetVdot()));//求Idot
*/	
	//对第二次测试数据进行修正
	if(!m_setcal->GetDisableCaliSw()){
		FilteringElec(maxiv);
		FilteringTemp(maxiv);
		FilteringSun(maxiv);
	}
	double i,v,p;
	maxiv->GetPowMax(v,i,p,rge);
	SetPmax(p,v,i);
	EvaluationEff(m_setcal->GetAreaEff());//求Eff
	//对第二次测试数据进行修正

	m_setcal->UpdataFromSunPara(this);//更新数据提供给标定校准使用
// 	BOOL ret=FilteringParaAlarm();//分档报警       改动  保存两次的解决
// 	if(!(ret&&m_setbase->GetSetParaAlarm()->GetErrorNoSave()))
// 			SavePara();//存参数表

}

//计算电池参数，数据处理
void CurveIV::CalSunPara(void)
{
//	SaveDaq();//保存原始数据
	CalFilter(this);//修正，校准

	if(m_setbase->GetSetDev841()->GetExternEffCell())
		TransformerParameter(this,2);
	else
		TransformerParameter(this,30);

	EvaluationEff(m_setcal->GetAreaEff());//求Eff
	SetVdot(m_setcal->GetVdot());//保存Vdot
	SetIdot(EvaluateY(GetVdot()));//求Idot
	SetJsc(GetIsc(),m_setcal->GetArea());  //算jsc

	m_setcal->UpdataFromSunPara(this);//更新数据提供给标定校准使用

//	SaveCalibratedDaq();//保存校准后数据
}
void CurveIV::SaveParaRun(void)
{
	BOOL ret=FilteringParaAlarm();//分档报警
	if(!(ret&&m_setbase->GetSetParaAlarm()->GetErrorNoSave()))
			SavePara();//存参数表
	PrtLabel();
}
//保存参数列表
#include   "Shlwapi.h"   
void CurveIV::SavePara()
{
	if(m_setiv->GetSwSavePara())
		SaveParaCSV();

	if (m_setiv->GetSwSaveXLS())
		SaveParaXLS();

	if(m_setiv->GetSwSaveMDB())
		SaveParaMDB();

	if(m_setiv->GetSwSaveInClass())
		SaveParaInClass();

	if (m_setiv->GetSwSaveSQLserver())
		SaveParaSQLServer();
}
void WriteUicodeBOM(CFile &f)
{
	f.SeekToBegin();
	byte BOM[]={0xff,0xfe};
	f.Write(BOM,sizeof(BOM));
}
void CurveIV::SaveParaCSV()
{
	USES_CONVERSION;
    //写入二进制格式，开头写入的是Uicode文件头
	CFile file;
	CString str;
	CString fn=m_setiv->BuildParaFileName(this)+_T(".csv");
	if(!PathFileExists(fn))
	{
		if(file.Open(fn,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)){
			WriteUicodeBOM(file);      //改动
			str=m_setiv->BuildSaveTitle(this);
			file.Write(str.GetBuffer(),str.GetLength()*sizeof(TCHAR));
			str=m_setiv->BuildSaveItem(this);
			file.Write(str.GetBuffer(),str.GetLength()*sizeof(TCHAR));
		}
	}
	else if(file.Open(fn,CFile::modeReadWrite|CFile::shareExclusive))
	{
		WriteUicodeBOM(file);          //改动
		// 检测“保存到文件”参数列表是否更新 //////////////////////
		CString sNewTitle, NewTitle[50], sOldTitle, OldTitle[50], sTitle, Title[50];  // 新的“到文件”参数表、文件参数表、更新后的参数表
		sNewTitle = L"";
		sOldTitle = L"";
		sTitle = L"";
		for(int i = 0; i < 50; i++)
		{
			NewTitle[i] = L"";
			OldTitle[i] = L"";
			Title[i] = L"";
		}
		int nCountNew, nCountOld, nCount;
		nCountNew = nCountOld = nCount = 0;

        // 获得新的“到文件”参数序列 //////////////////////
		sNewTitle = m_setiv->BuildSaveTitle(this);
		
		// 得到.CSV文件中的第一行，即已存参数 //////////////////////
		CString ch;
		ch = " ";
		int retRead;
		retRead = file.Read(ch.GetBuffer(), 2);
		while (ch != "\n")
		{
			sOldTitle = sOldTitle + ch;
			ch.ReleaseBuffer();
			ch = " ";
			retRead = file.Read(ch.GetBuffer(), 2);
		}
		sOldTitle += L"\n";

		if (strcmp(T2A(sNewTitle), T2A(sOldTitle)) == 0)         // 标题未变动
		{
			nCountOld = GetArrayFromString(sOldTitle, OldTitle);

			// 建立新的参数列表
			int nSerialID;           // 序列号所在位置
			nSerialID = -1;
			sTitle = sOldTitle;
			for (int i = 0; i < nCountOld; i++)
			{
				Title[i] = OldTitle[i];
				if (m_setiv->m_eorc)
				{
					if (OldTitle[i] == L"序列号")
					{
						nSerialID = i;
					}
				}else
				{
					if (OldTitle[i] == L"ID")
					{
						nSerialID = i;
					}
				}

			}

			// 删除之前所有和当前序列号相同的记录
			if(m_setiv->GetSwSerialUnique() && nSerialID != -1)      // 序列号不重复，且之前保存有“序列号”这一列
			{
				// 读取第一行的参数名行，查询序列号所在的参数列
				CStringArray sArray;
				sArray.Add(sOldTitle);

				CString sSerial = GetSerial();           // 获取当前序列号
				CString strLine;
				strLine = "";
				CString ch;
				ch = " ";
				int retRead;
				retRead = file.Read(ch.GetBuffer(), 2);
				while (retRead != 0)
				{
					strLine = strLine + ch;
					ch.ReleaseBuffer();
					ch = " ";
					retRead = file.Read(ch.GetBuffer(), 2);
					if (ch == "\n")
					{
						strLine = strLine + ch;
						int id = 0;
						CString str;
						str = "";
						for (int i = 0; i < strLine.GetLength(); i++)
						{
							if (strLine[i] != ',')
							{
								str += strLine[i];
							}
							else
							{
								if (id == nSerialID)
								{
									if (str != sSerial)
									{
										sArray.Add(strLine);
									}
									else
									{
										id = 0;
										AfxMessageBox(L"The ID number is repeated.");      //改动
									}
									break;
								}
								else
								{
									id++;
								}
								str = "";
							}
						}
						strLine = "";
						retRead = file.Read(ch.GetBuffer(), 2);
					}
				}

				file.SetLength(0);    // 清空文件
				// 重新写入无重复的数据
				WriteUicodeBOM(file);          //改动
				for (int i = 0; i < sArray.GetSize(); i++)
				{
					file.Write(sArray[i].GetBuffer(),sArray[i].GetLength()*sizeof(TCHAR));
				}
				strLine = m_setiv->BuildSaveItem(this);
				file.Write(strLine.GetBuffer(),strLine.GetLength()*sizeof(TCHAR));
			}
			else
			{
				file.SeekToEnd();
				CString strLine;
				strLine = "";
				strLine = m_setiv->BuildSaveItem(this);
				file.Write(strLine.GetBuffer(),strLine.GetLength()*sizeof(TCHAR));
			}
		}
		else
		{
			// 将参数名保存到NewTitle数组中
			nCountNew = GetArrayFromString(sNewTitle, NewTitle);
			nCountOld = GetArrayFromString(sOldTitle, OldTitle);

			// 建立新的参数列表
			int nSerialID;           // 序列号所在位置
			nSerialID = -1;
			sTitle = sOldTitle;
			nCount = nCountOld;
			for (int i = 0; i < nCountOld; i++)
			{
				Title[i] = OldTitle[i];
				if (m_setiv->m_eorc)
				{
					if (OldTitle[i] == L"序列号")
					{
						nSerialID = i;
					}
				}else
				{
					if (OldTitle[i] == L"ID")
					{
						nSerialID = i;
					}
				}
			}
			sTitle.Remove('\n');
			for (int i = 0; i < nCountNew; i++)
			{
				if (sOldTitle.Find(NewTitle[i]) == -1)
				{
					Title[nCount++] = NewTitle[i];
					sTitle = sTitle + NewTitle[i] + L",";
				}
			}
			sTitle += L"\n";

			// 将参数列表存储于sArray中
			CStringArray sArray;
			sArray.Add(sTitle);

			CString sItem, sNewItem, Item[50], NewItem[50];
			sItem = L"";
			sNewItem = L"";
			for(int i = 0; i < 50; i++)
			{
				NewItem[i] = L"";
				Item[i] = L"";
			}
			sNewItem = m_setiv->BuildSaveItem(this);
			GetArrayFromString(sNewItem, NewItem);
            
			for (int i = 0; i < nCount; i++)
			{
				int num = 0;
				while(num < nCountNew)
				{
					if (NewTitle[num] == Title[i])
					{
						sItem += NewItem[num] + L",";
						break;
					}
					num++;
				}
				if (num == nCountNew)
				{
					sItem += L",";
				}
			}
			sItem += L"\n";

			// 删除之前所有和当前序列号相同的记录
			if(m_setiv->GetSwSerialUnique() && nSerialID != -1)      // 序列号不重复，且之前保存有序列号
			{
				CString sSerial = GetSerial();           // 获取当前序列号
				CString sLine;
				sLine = "";
				CString ch;
				ch = " ";
				int retRead;
				retRead = file.Read(ch.GetBuffer(), 2);
				while (retRead != 0)
				{
					sLine = sLine + ch;
					ch.ReleaseBuffer();
					ch = " ";
					retRead = file.Read(ch.GetBuffer(), 2);
					if (ch == "\n")
					{
						sLine = sLine + ch;
						int id = 0;
						CString str;
						str = "";
						for (int i = 0; i < sLine.GetLength(); i++)
						{
							if (sLine[i] != ',')
							{
								str += sLine[i];
							}
							else
							{
								if (id == nSerialID)
								{
									if (str != sSerial)
									{
										sArray.Add(sLine);
									}
									else
									{
										id = 0;
										AfxMessageBox(L"The ID number is repeated.");      //改动
									}
									break;
								}
								else
								{
									id++;
								}
								str = "";
							}
						}
						sLine = "";
						retRead = file.Read(ch.GetBuffer(), 2);
					}
				}

				file.SetLength(0);    // 清空文件
				WriteUicodeBOM(file);          //改动

				// 重新写入无重复的数据
				for (int i = 0; i < sArray.GetSize(); i++)
				{
					file.Write(sArray[i].GetBuffer(),sArray[i].GetLength()*sizeof(TCHAR));
				}
                // 写入新的记录
				file.Write(sItem.GetBuffer(),sItem.GetLength()*sizeof(TCHAR));
			}
			else
			{
				CString sLine;
				sLine = "";
				CString ch;
				ch = " ";
				int retRead;
				retRead = file.Read(ch.GetBuffer(), 2);
				while (retRead != 0)
				{
					sLine = sLine + ch;
					ch.ReleaseBuffer();
					ch = " ";
					retRead = file.Read(ch.GetBuffer(), 2);
					if (ch == "\n")
					{
						sLine = sLine + ch;
						sArray.Add(sLine);
						sLine = "";
						retRead = file.Read(ch.GetBuffer(), 2);
					}
				}

				file.SetLength(0);    // 清空文件
				WriteUicodeBOM(file);          //改动

				// 重新写入无重复的数据
				for (int i = 0; i < sArray.GetSize(); i++)
				{
					file.Write(sArray[i].GetBuffer(),sArray[i].GetLength()*sizeof(TCHAR));
				}
				// 写入新的记录
				file.Write(sItem.GetBuffer(),sItem.GetLength()*sizeof(TCHAR));
			}

		}
	}
	if(file.m_hFile!=INVALID_HANDLE_VALUE)
		file.Close();
}
// 将字符串中的各项提取出来存到数组里
int CurveIV::GetArrayFromString(CString str, CString *pArray)
{
	int nCount = 0;
	int pos = 0;     // 参数名序列中的字符位置
	int i;
	for(i = 0; i < 50; i++)
	{	
		pos = str.FindOneOf(_T(","));
		if (pos != -1)
		{
			nCount++;
			pArray[i] = str.Left(pos);
			str.Delete(0,pos+1);
		}else break;


	}
	return nCount;
}


#include "..\DB.h"
//用SQL方式保存数据参数
void CurveIV::SaveParaMDB()
{
	// 写入记录
	DB db;
	CString pass=theApp.GetDataPassword();
	CString sql,str1,str2,str3;
	str2=m_setiv->BuildItem(this);
	str3=m_setiv->BuildField(this);
	if(str2.GetLength()<=2||str3.GetLength()<=2)
		return;
	CString fn=m_setiv->BuildParaFileName(this)+_T(".mdb");
	if(!PathFileExists(fn)){
		if(db.CreateDatabase(fn,pass))
		{
			str1=m_setiv->BuildField(this,1);
			if(str1.GetLength()<=2)
				return;
			db.ExeSql(fn,_T("CREATE TABLE SunData")+str1,pass);
			sql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;
			db.ExeSql(fn,sql,pass);
		}
	}
	else
	{
		// 检测并更新数据库列名
		AlterMDB();
		CString sqlcmd = L"SELECT * FROM SunData";
		CString Field[50];
		for(int i = 0; i < 50; i++)
		{
			Field[i] = L"";
		}
		CString sFieldsofFile = db.GetFields(fn, sqlcmd, pass, Field);
		CString str = L"";
		if (m_setiv->m_eorc)
		{
			str = L"序列号";
		}else
		{
			str = L"ID";
		}

		if(m_setiv->GetSwSerialUnique() && -1 != sFieldsofFile.Find(str))   // 删除之前所有和当前序列号相同的记录
		{
			if (m_setiv->m_eorc)
			{
				sql=L"DELETE * FROM SunData WHERE 序列号 = \'" + GetSerial() + L"\'";
			}else
			{
				sql=L"DELETE * FROM SunData WHERE ID = \'" + GetSerial() + L"\'";
			}
			
			db.ExeSql(fn,sql,pass);
		}
		sql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;
		db.ExeSql(fn,sql,pass);
	}
}


CString CurveIV::GetExcelDriver()
{
	TCHAR szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	TCHAR *pszBuf = szBuf;
	CString sDriver;
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return _T("");
	do
	{
		if(wcsstr( pszBuf, L"Excel" ) != NULL ) 
		{
			sDriver.Format(_T("%s"),pszBuf);  
			break;
		}
		pszBuf = wcschr( pszBuf, '\0' ) + 1;
	}
	while( pszBuf[1] != '\0' );

	return sDriver;
}


void CurveIV::SaveParaXLS()
{
	// 写入记录
	CDatabase   database;   
	CString   sDriver;										//   Excel安装驱动   
	CString   sExcelFile;                                   //   要建立的Excel文件   
	CString   sSql;   

	CString str1,str2,str3;
	sDriver = _T("");

	str2=m_setiv->BuildItem(this);
	str3=m_setiv->BuildField(this);
	if(str2.GetLength()<=2||str3.GetLength()<=2)
		return;
	sExcelFile=m_setiv->BuildParaFileName(this)+_T(".xls");

	sDriver = GetExcelDriver();

	if (sDriver.IsEmpty())
	{
		AfxMessageBox(_T("NO Excel Driver!"));
		return;
	}

	if(!PathFileExists(sExcelFile))
	{
		try   
		{
			sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=/%s/;DBQ=%s"),sDriver, sExcelFile, sExcelFile);
			//   创建数据库   (既Excel表格文件)   
			if(database.OpenEx(sSql,CDatabase::noOdbcDialog)   )   
			{   
				//   创建表结构  
				str1=m_setiv->BuildField(this,1);
				if(str1.GetLength()<=2)
					return;
				sSql = _T("CREATE TABLE SunData")+str1;//demo   
				database.ExecuteSQL(sSql); 

				sSql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;

				database.ExecuteSQL(sSql);   

			}               
			//   关闭数据库   
			database.Close(); 
		}
		catch(CDBException *e)   
		{   
			AfxMessageBox(e->m_strError);  
		}  
	}else
	{
		sSql.Format(_T("ODBC;DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%s"), sDriver, sExcelFile);

		try
		{
			database.Open(NULL, false, false, sSql);

			sSql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;

			database.ExecuteSQL(sSql);   
			database.Close();
		}
		catch(CDBException *e)   
		{   
			AfxMessageBox(e->m_strError);  
		} 
	}
}

void CurveIV::SaveParaInClass()
{ 
	// 写入记录
	DB db;
	CString pass=theApp.GetDataPassword();
	CString sql,str1,str2,str3;
	str2=m_setiv->BuildItem(this);
	str3=m_setiv->BuildField(this);
	if(str2.GetLength()<=2||str3.GetLength()<=2)
		return;
	CString fn=m_setiv->BuildParaClassFileName(this)+_T(".mdb");
	if(!PathFileExists(fn))
	{
		if(db.CreateDatabase(fn,pass))
		{
			str1=m_setiv->BuildField(this,1);
			if(str1.GetLength()<=2)
				return;
			db.ExeSql(fn,_T("CREATE TABLE SunData")+str1,pass);
			sql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;
			db.ExeSql(fn,sql,pass);
		}
	}
	else
	{
		// 检测并更新数据库列名
		AlterMDBCLass();
		CString sqlcmd = L"SELECT * FROM SunData";
		CString Field[50];
		for(int i = 0; i < 50; i++)
		{
			Field[i] = L"";
		}
		CString sFieldsofFile = db.GetFields(fn, sqlcmd, pass, Field);
		CString str = L"";
		if (m_setiv->m_eorc)
		{
			str = L"序列号";
		}else
		{
			str = L"ID";
		}

		if(m_setiv->GetSwSerialUnique() && -1 != sFieldsofFile.Find(str))   // 删除之前所有和当前序列号相同的记录
		{
			if (m_setiv->m_eorc)
			{
				sql=L"DELETE * FROM SunData WHERE 序列号 = \'" + GetSerial() + L"\'";
			}else
			{
				sql=L"DELETE * FROM SunData WHERE ID = \'" + GetSerial() + L"\'";
			}
			db.ExeSql(fn,sql,pass);
		}
		sql=_T("INSERT INTO SunData")+str3+_T("VALUES")+str2;
		db.ExeSql(fn,sql,pass);
	}
}

void CurveIV::SaveParaSQLServer()             //改动
{
	CString strsql,strtable,strfield,stritem;
	m_db.ConnectDatabase(m_setiv->GetSwSQLserverip(),m_setiv->GetSwSQLdatabase(),m_setiv->GetSwSQLusername(),m_setiv->GetSwSQLpassword());

	stritem=m_setiv->BuildItem(this);
	strfield=m_setiv->BuildField(this);
	if(stritem.GetLength()<=2||strfield.GetLength()<=2)
		return;

	strsql = _T("select * from Results");
	if (m_db.ExecuteSQL(strsql))
	{
		// 检测并更新数据库列名
		AlterSQL();
		CString sqlcmd = L"SELECT * FROM Results";
		CString sFieldsofFile = m_db.GetFields(sqlcmd);
		CString str = L"";
		if (m_setiv->m_eorc)
		{
			str = L"序列号";
		}else
		{
			str = L"ID";
		}

		if(m_setiv->GetSwSerialUnique() && -1 != sFieldsofFile.Find(str))   // 删除之前所有和当前序列号相同的记录
		{
			if (m_setiv->m_eorc)
			{
				strsql=L"DELETE * FROM Results WHERE 序列号 = \'" + GetSerial() + L"\'";
			}else
			{
				strsql=L"DELETE * FROM Results WHERE ID = \'" + GetSerial() + L"\'";
			}

			m_db.ExecuteSQL(strsql);
		}
		strsql=_T("INSERT INTO Results")+strfield+_T("VALUES")+stritem;
		m_db.ExecuteSQL(strsql);
	}else
	{
// 		strtable=m_setiv->BuildField(this,1);
// 		if(strtable.GetLength()<=2)
// 			return;
// 		m_db.ExecuteSQL(_T("CREATE TABLE SunData")+strtable);
// 		strsql=_T("INSERT INTO SunData")+strfield+_T("VALUES")+stritem;
// 		m_db.ExecuteSQL(strsql);
	}
}

void CurveIV::AlterMDB()
{
	DB db;
	CString pass=theApp.GetDataPassword();
	CString sql;   
	CString sFields, sNewFields, Field[50], Field2[50],strtemp;
	sFields = L"";
    sNewFields = L"";
	strtemp = L"";
	for(int i = 0; i < 50; i++)
	{
		Field[i] = L"";
		Field2[i] = L"";
	}
	// 得到新的字段序列
	sNewFields = m_setiv->BuildField(this);
	strtemp = sNewFields;
// 	strtemp.Remove('(');
// 	strtemp.Remove(')');
	int posK = 0;
	posK = strtemp.FindOneOf(_T("("));
	if (posK != -1)
	{
		strtemp.Delete(posK,1);
	}
	posK = strtemp.ReverseFind(')');
	if (posK != -1)
	{
		strtemp.Delete(posK,1);
	}
	strtemp += L",";
	// 将字段保存到Field数组中
	int pos = 0;     // 字段序列中的字符位置
	for(int i = 0; ; i++)
	{
		pos = strtemp.FindOneOf(_T(","));
		if (pos != -1)
		{
			Field[i] = strtemp.Left(pos);
			strtemp.Delete(0,pos+1);
		}else break;
	}

	if(sNewFields.GetLength()<=2)
		return;
	CString fn = m_setiv->BuildParaFileName(this)+_T(".mdb");

	if(PathFileExists(fn))
	{
		sql = L"SELECT * FROM SunData";

		sFields = db.GetFields(fn, sql, pass, Field2);

		int i = 0;
		while(!(Field[i].IsEmpty()))
		{
			if(-1 ==sFields.Find(Field[i]))
			{
				sql = L"";
				sql = L"ALTER TABLE SunData ADD " + Field[i] + GetParaFormat(Field[i]);
				db.ExeSql(fn,sql,pass);
			}
			i++;
		}
	}
}

void CurveIV::AlterSQL()
{
	CString sql;   
	CString sFields, sNewFields, Field[50],strtemp;
	sFields = L"";
	sNewFields = L"";
	strtemp = L"";
	for(int i = 0; i < 50; i++)
	{
		Field[i] = L"";
	}
	// 得到新的字段序列
	sNewFields = m_setiv->BuildField(this);
	strtemp = sNewFields;
	int posK = 0;
	posK = strtemp.FindOneOf(_T("("));
	if (posK != -1)
	{
		strtemp.Delete(posK,1);
	}
	posK = strtemp.ReverseFind(')');
	if (posK != -1)
	{
		strtemp.Delete(posK,1);
	}
	strtemp += L",";
	// 将字段保存到Field数组中
	int pos = 0;     // 字段序列中的字符位置
	for(int i = 0; ; i++)
	{
		pos = strtemp.FindOneOf(_T(","));
		if (pos != -1)
		{
			Field[i] = strtemp.Left(pos);
			strtemp.Delete(0,pos+1);
		}else break;
	}
	if(sNewFields.GetLength()<=2)
		return;

	sql = L"SELECT * FROM Results";
	sFields = m_db.GetFields(sql);

	int i = 0;
	while(!(Field[i].IsEmpty()))
	{
		if(-1 ==sFields.Find(Field[i]))
		{
			sql = L"";
			sql = L"ALTER TABLE Results ADD " + Field[i] + GetParaFormat(Field[i]);
			m_db.ExecuteSQL(sql);
		}
		i++;
	}
}

void CurveIV::AlterMDBCLass()
{
	DB db;
	CString pass=theApp.GetDataPassword();
	CString sql;   
	CString sFields, sNewFields, Field[50], Field2[50],strtemp;
	sFields = L"";
	sNewFields = L"";
	strtemp = L"";
	for(int i = 0; i < 50; i++)
	{
		Field[i] = L"";
		Field2[i] = L"";
	}
	// 得到新的字段序列
	sNewFields = m_setiv->BuildField(this);
	strtemp = sNewFields;
	strtemp.Remove('(');
	strtemp.Remove(')');
	strtemp += L",";
	// 将字段保存到Field数组中
	int pos = 0;     // 字段序列中的字符位置
	for(int i = 0; i < 50; i++)
	{
		pos = strtemp.FindOneOf(_T(","));
		if (pos != -1)
		{
			Field[i] = strtemp.Left(pos);
			strtemp.Delete(0,pos+1);
		}else break;
	}

	if(sNewFields.GetLength()<=2)
		return;
	CString fn = m_setiv->BuildParaClassFileName(this)+_T(".mdb");

	if(PathFileExists(fn))
	{
		sql = L"SELECT * FROM SunData";

		sFields = db.GetFields(fn, sql, pass, Field2);

		int i = 0;
		while(!(Field[i].IsEmpty()))
		{
			if(-1 ==sFields.Find(Field[i]))
			{
				sql = L"";
				sql = L"ALTER TABLE SunData ADD " + Field[i] + GetParaFormat(Field[i]);
				db.ExeSql(fn,sql,pass);
			}
			i++;
		}
	}
}

CString CurveIV::GetParaFormat(CString sPara)    //改动
{
	CString sFormat;
	sPara.Remove('[');
	sPara.Remove(']');
	if (m_setiv->m_eorc)
	{
		if(sPara == L"时间")
			sFormat=_T("TEXT");
		else if(sPara == L"序列号")
			sFormat=_T("TEXT");
		else if(sPara == L"扫描时间")
			sFormat=_T("TEXT");
		else if(sPara == L"扫描方向")
			sFormat=_T("TEXT");
		else if(sPara == L"辐照度是否修正")
			sFormat=_T("TEXT");
		else if(sPara == L"转换效率")
			sFormat=_T("FLOAT");
		else if(sPara == L"并联电阻")
			sFormat=_T("FLOAT");
		else if(sPara == L"串联电阻")
			sFormat=_T("FLOAT");
		else if(sPara == L"方度")
			sFormat=_T("FLOAT");
		else if(sPara == L"短路电流")
			sFormat=_T("FLOAT");
		else if(sPara == L"开路电压")
			sFormat=_T("FLOAT");
		else if(sPara == L"最大功率点电流")
			sFormat=_T("FLOAT");
		else if(sPara == L"最大功率点电压")
			sFormat=_T("FLOAT");
		else if(sPara == L"最大功率")
			sFormat=_T("FLOAT");
		else if(sPara == L"Temp")
			sFormat=_T("FLOAT");
		else if(sPara == L"环境温度")
			sFormat=_T("FLOAT");
		else if(sPara == L"表面温度")
			sFormat=_T("FLOAT");
		else if(sPara == L"监测电池温度")
			sFormat=_T("FLOAT");
		else if(sPara == L"光照度")
			sFormat=_T("FLOAT");
		else if(sPara == L"参考电池电流")
			sFormat=_T("FLOAT");
		else if(sPara == L"漏电流")
			sFormat=_T("FLOAT");
		else if(sPara == L"定点电流")
			sFormat=_T("FLOAT");
		else if(sPara == L"定点电压")
			sFormat=_T("FLOAT");
		else if(sPara == L"标准参考电池的实测短路电流")
			sFormat=_T("FLOAT");
// 		else if(sPara == L"StdIsc1")                                //朱峰改动
// 			sFormat=_T("FLOAT");
// 		else if(sPara == L"Stdsun1")                                //朱峰改动
// 			sFormat=_T("FLOAT");
// 		else if(sPara == L"Stdsun2")                                //朱峰改动
// 			sFormat=_T("FLOAT");		
// 		else if(sPara == L"StdIsc2")                                //朱峰改动
// 			sFormat=_T("FLOAT");
		else if(sPara == L"电流密度")                                //朱峰改动
			sFormat=_T("FLOAT");
		else if(sPara == L"单位")
			sFormat=_T("TEXT");
		else if(sPara == L"定点功率")
			sFormat=_T("FLOAT");
		else if(sPara == L"档位")
			sFormat=_T("TEXT");
		else if(sPara == L"单片电池面积")                          //改动
			sFormat = _T("FLOAT");
		else if(sPara==L"测试时间")
			sFormat = _T("TEXT");
		else
			sFormat=_T("TEXT");

	}else
	{
		if(sPara == L"Test_Date")
			sFormat=_T("TEXT");
		else if(sPara == L"ID")
			sFormat=_T("TEXT");

		else if(sPara == L"STime")
			sFormat=_T("TEXT");
		else if(sPara == L"SDirection")
			sFormat=_T("TEXT");
		else if(sPara == L"IrrCali")
			sFormat=_T("TEXT");

		else if(sPara == L"ModEff")
			sFormat=_T("FLOAT");
		else if(sPara == L"Rsh")
			sFormat=_T("FLOAT");
		else if(sPara == L"Rs")
			sFormat=_T("FLOAT");
		else if(sPara == L"FF")
			sFormat=_T("FLOAT");
		else if(sPara == L"Isc")
			sFormat=_T("FLOAT");
		else if(sPara == L"Voc")
			sFormat=_T("FLOAT");
		else if(sPara == L"Ipm")
			sFormat=_T("FLOAT");
		else if(sPara == L"Vpm")
			sFormat=_T("FLOAT");
		else if(sPara == L"Pmax")
			sFormat=_T("FLOAT");
		else if(sPara == L"Temp")
			sFormat=_T("FLOAT");
		else if(sPara == L"EnvTemp")
			sFormat=_T("FLOAT");
		else if(sPara == L"RCTemp")
			sFormat=_T("FLOAT");
		else if(sPara == L"TMod")
			sFormat=_T("FLOAT");
		else if(sPara == L"Insol")
			sFormat=_T("FLOAT");
		else if(sPara == L"SunRef")
			sFormat=_T("FLOAT");
		else if(sPara == L"Irev")
			sFormat=_T("FLOAT");
		else if(sPara == L"Ivld")
			sFormat=_T("FLOAT");
		else if(sPara == L"Vld")
			sFormat=_T("FLOAT");
		else if(sPara == L"StdIsc")
			sFormat=_T("FLOAT");
// 		else if(sPara == L"StdIsc1")
// 			sFormat=_T("FLOAT");
// 		else if(sPara == L"Stdsun1")                                //朱峰改动
// 			sFormat=_T("FLOAT");
// 		else if(sPara == L"StdIsc2")
// 			sFormat=_T("FLOAT");
// 		else if(sPara == L"Stdsun2")                                //朱峰改动
// 			sFormat=_T("FLOAT");
		else if(sPara == L"Jsc")                                //朱峰改动
			sFormat=_T("FLOAT");
		else if(sPara == L"Manuf")
			sFormat=_T("TEXT");
		else if(sPara == L"Pvld")
			sFormat=_T("FLOAT");
		else if(sPara == L"ModType")
			sFormat=_T("TEXT");
		else if(sPara == L"Area")                          //改动
			sFormat = _T("FLOAT");
		else if(sPara==L"Test_Time")
			sFormat = _T("TEXT");
		else
			sFormat=_T("TEXT");   //改动 自定义

	}


	return sFormat;
}

//用SQL方式分档保存数据参数

void CurveIV::PrtLabel()
{
	//打印标签
	if(m_setbase->GetSetPrtLabel()->GetEnPrt())
	{
		CString sLabelPathName;
		if (m_setbase->GetSetPrtLabel()->m_saveCSV)
		{
			sLabelPathName = m_setbase->GetSetPrtLabel()->GetFullFilePath() + L".csv";
			m_setiv->LabelCSV(this,sLabelPathName);
		}
		if (m_setbase->GetSetPrtLabel()->m_saveTXT)
		{
			sLabelPathName = m_setbase->GetSetPrtLabel()->GetFullFilePath() + L".txt";
			m_setiv->LabelTXT(this,sLabelPathName);		
		}
		m_setbase->GetSetPrtLabel()->Print();
	}
}


//保存采集进来的原始值
//没有经过修正和校准的结果
void CurveIV::SaveDaq(CString filename)
{
	USES_CONVERSION;
	CStdioFile file;
	CString fn=m_setiv->m_filepath+_T("\\daq\\")+m_setiv->BuildFileName(this);
	if(fn==_T(""))//文件名为空不保存文件
		return;
	fn=fn+filename;

	string strRetName,strRetFullPath,strFileFullPath;
	strFileFullPath = W2A(fn);
	GenerateUniqueFileName(strRetName,strRetFullPath,strFileFullPath);
	fn = strRetFullPath.c_str();

	if(!PathFileExists(fn)){
		if(!file.Open(fn,CFile::modeCreate|CFile::modeWrite))
			return;
	}else	
	{
		DeleteFile(fn);
		if(!file.Open(fn,CFile::modeCreate|CFile::modeWrite))
			return;
	}
	file.SeekToBegin();
	std::vector<double>::iterator itx,ity;
	itx=x.begin();
	ity=y.begin();

	for(;itx<x.end();itx++,ity++)
		file.WriteString(DtoStr(*itx)+_T(",")+DtoStr(*ity)+_T("\n"));
	file.Flush();
	file.Close();
}
//转色深的函数
HBITMAP ColorBitmap(HDC hdc,HBITMAP hBmpSrc,UINT nColor)
{
	HBITMAP hCreate = NULL;
	char * lpHeader = NULL;
	char * lpBits = NULL;
	__try{
		__try{
			BITMAP bmp;
			if(GetObject(hBmpSrc,sizeof(BITMAP),&bmp) == 0){
				__leave;
			}
			BITMAPINFOHEADER bmpInfo;
			memset(&bmpInfo,0,sizeof(BITMAPINFOHEADER));
			bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
			bmpInfo.biWidth = bmp.bmWidth;
			bmpInfo.biHeight = bmp.bmHeight;
			bmpInfo.biPlanes = 1;
			//设置新的位图的颜色数
			//可以设置的数值有
			//1 - 2色
			//4 - 16色
			//8 - 256色
			//16 - 16位色
			//24 - 24位色
			bmpInfo.biBitCount = nColor;
			//256色(包含256)以下的位图需要调色版,
			//下面是计算所需调色版空间大小
			int nHeadSize = nColor> 8?0:1<<nColor;
			nHeadSize *= sizeof(RGBQUAD);
			//加上位图信息头的空间大小
			nHeadSize += sizeof(BITMAPINFOHEADER);
			//为信息头和调色版分配空间
			lpHeader = new char[nHeadSize];
			BITMAPINFOHEADER * lpBmpInfo = (BITMAPINFOHEADER *)lpHeader;
			* lpBmpInfo = bmpInfo;
			//GetBIBits 取得存储位图数据所需空间
			if(::GetDIBits(hdc,
				           hBmpSrc, 
				           0, //数据开始的扫描线,从0开始
				           bmp.bmHeight,//数据结束的扫描线,位图高
				           NULL,   //数据存储缓冲区,= NULL则会返需求量
				           (LPBITMAPINFO)lpBmpInfo,
				           DIB_RGB_COLORS) == 0)
			{
				__leave;
			}
			//为位图数据分配空间
			lpBits = new char[lpBmpInfo->biSizeImage];
			//GetBIBits 取得存储位图数据
			if(::GetDIBits(hdc,hBmpSrc,0,bmp.bmHeight,lpBits,(LPBITMAPINFO)lpBmpInfo,DIB_RGB_COLORS) == 0) {
				__leave;
			}
			//生成新位图
			hCreate = ::CreateCompatibleBitmap(hdc   ,   bmp.bmWidth,   bmp.bmHeight);
			if(hCreate == NULL){
				__leave;
			}
			//将位图数据写入新位图中
			::SetDIBits(hdc,hCreate, 0,bmp.bmHeight,lpBits,(LPBITMAPINFO)lpBmpInfo,DIB_RGB_COLORS);

		}__except(EXCEPTION_EXECUTE_HANDLER){
			__leave;
		}
	}__finally{
		if(lpHeader != NULL){
			delete [] lpHeader;
		}
		if(lpBits != NULL){
			delete [] lpBits;
		}
	}
	return hCreate;
}


void CurveIV::SaveJpg(CWnd* pWnd)
{
	if(!m_setiv->GetSwSaveJpg())
	{
		return;
	}
	USES_CONVERSION;
	CString fn=m_setiv->m_filepath+_T("\\jpg\\")+m_setiv->BuildFileName(this);
	if(fn==_T(""))//文件名为空不保存文件
	{
		return;
	}
	fn=fn+_T(".jpg");

	string strRetName,strRetFullPath,strFileFullPath;
	strFileFullPath = W2A(fn);
	GenerateUniqueFileName(strRetName,strRetFullPath,strFileFullPath);
	fn = strRetFullPath.c_str();


	if(pWnd==NULL){
		pWnd=AfxGetMainWnd();
		if(!pWnd)
			return;
	}
	pWnd->UpdateWindow();

	CRect m_rect_view;
	pWnd->GetClientRect(&m_rect_view);
	CWindowDC bmemDC(pWnd);
	CImage img;
	img.Create(m_rect_view.Width(),m_rect_view.Height(),24);
	if (!img.IsNull())
	{
		CImageDC imageDCtl(img);
		::BitBlt(imageDCtl, 0, 0, m_rect_view.Width(), m_rect_view.Height(), bmemDC, 0, 0, SRCCOPY);
	}
	int height=0;
	int width=0;

	if (!img.IsNull())
	{
		height=img.GetHeight();
		width=img.GetWidth();
	}


	//int TopHeight=(height1>height2)?height1:height2;

	unsigned char *pData=new unsigned char[height*width*3];
	int WLength=(int) width*3*sizeof(unsigned char);
	int wlength1=0;


	if (!img.IsNull())
	{
		int step=img.GetPitch();

		unsigned char *pImg=(unsigned char *) img.GetBits();
		int k = 0, i = 0, j = 0;
		// 彩色图像

		wlength1 = (int) width*3*sizeof(unsigned char);
		//#pragma omp parallel for
		for (i=0; i<height; i++)
		{
			memcpy(&pData[i*WLength],pImg+i*step , wlength1);
		}
	}

	//声明标识符  
	char *pbuffer = T2A(fn); 

	write_jpeg_file_continue (pbuffer, 98, height, width,JCS_EXT_BGR, pData);

	delete [] pData;
}
int CurveIV::InputBoxSerialRun(int pos)
{
	//调用位置和设定位置不同时不出现输入框
	//实现对输入框出现时机的控制
	if(pos!=m_setiv->GetSwBackInput())
		return -1;
	else
	{
		m_setiv->InputSerial();
		SetSerial(m_setiv->BuildSerial(this));
		CString str = GetSerial();
		if (!GetSerial().IsEmpty())
			return 1;
		else
			return 0;
	}
/*	m_setiv->InputSerial();
	SetSerial(m_setiv->BuildSerial(this));
	CString str = GetSerial();
	if (!GetSerial().IsEmpty())
		return 1;
	else
		return 0;*/
}
BOOL CurveIV::GetEnableLoop(void)
{
	return m_setiv->GetEnableLoop();
}

//校准和修正
void CurveIV::CalFilter(CDoubleXY*const obj)
{
	if(m_setcal->GetDisableCaliSw())
		return;
	FilteringSun(obj);                         //光强修正函数
	CDoubleXY tmpcur(1100);
	TransformerParameter(this,&tmpcur);        // 计算出参数的函数
	FilteringTemp(obj);                        //温度修正函数
	FilteringElec(obj);						   // 根据Isc和Voc校准系数对测试的原始I\V数据进行校准，实际为每个实测点乘以校准系数
	if (m_setcal->GetReflectScaleSw() && m_fc[REFLECTANCE])
	{
		FilteringElec1(obj);
	}
	ReturnZeroX();
}
//温度修正IEC6125
void CurveIV::FilteringTemp(CDoubleXY*const obj)
{
	if(!m_setcal->GetTempToObjSw() && !m_setcal->GetIrrToObjSw())
		return;
	CString logtest;
	SetEnvTemp((GetEnvTemp()+273)*m_setcal->GetScaleTemp()-273);//应用温度传感器修正系数
    SetSurfTemp((GetSurfTemp()+273)*m_setcal->GetScaleIRTemp()-273);//应用红外温度探头修正系数

	const double K=0.0000125;//曲线因子
	double uT=m_setcal->GetScaleVolTemp();
	double iT=m_setcal->GetScaleCurTemp();
	double n = m_setcal->GetSolarCount();

	double Tobj=m_setcal->GetObjectTemp();
	double tarea=0;
	if (m_setcal->GetTempToObjSw())
	{
		SetTemp(Tobj);
		if(m_setcal->GetTempType())//环境温度和表面温度切换
			tarea=Tobj-GetEnvTemp();
		else 
			tarea=Tobj-GetSurfTemp();
	}else
		tarea=0;

	double Rs=GetRs();
	double Isc=GetIsc();
	double Voc=GetVoc();
	double a = 0.06;
	double G2_G1 ;
	if (m_setcal->GetIrrToObjSw())
	{
		SetIrrCali(_T("yes"));
		if ((m_setcal->GetStdCellIsc() > -0.000001) && (m_setcal->GetStdCellIsc() < 0.000001))
		{
			G2_G1 = 1;
			logtest = _T("The irradiance calibration is invalid because the standisc's value is 0!");
			m_LogTrace.WriteLine(logtest);
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.m_szStatusString = logtest;
			((CSolarCellTesterMainFrame*)AfxGetMainWnd())->m_StatusBar.GetStatusBarCtrl().SetText(_T(""), 0, SBT_OWNERDRAW); 
		}else
			G2_G1= (m_setcal->GetStdCellIsc()*GetSun())/(GetStdCellIscTesting()*1000);
	}else
	{
		SetIrrCali(_T("no"));
		G2_G1 = 1;
	}

	UINT i=0;
	UINT len=0;
	len=obj->GetDataSize();
	double I1,I2;
	for( i=0;i<len;i++)
	{//结果转换
		I1=obj->y[i];
		I2=obj->y[i]=I1*(1+iT*tarea)*G2_G1;
		obj->x[i]=obj->x[i]-Rs*(I2-I1)-K*I2*n*tarea+Voc*(uT*tarea+a*log(G2_G1));
	}
//	SetTemp(Tobj);
}
//光强修正
void CurveIV::FilteringSun(CDoubleXY*const obj)
{		
	//计算实测光强,有标准电池Isc值才能进行光强调整
	double stds=m_setcal->GetStdCellIsc();
	if(stds>0.001)
	{
		double stdt=GetStdCellIscTesting();
		SetSunRef(1000*stdt/stds);

	}else
	{
		SetSunRef(0);
	}
}
//电参数校准
void CurveIV::FilteringElec(CDoubleXY*const obj)
{
	if (theApp.IsDisable(L"CSetSunPara::SetCurePmax"))
	{
		obj->MultiYAny (m_setcal->GetScaleIsc());
		obj->MultiXAny (m_setcal->GetScaleVoc());
	}else
	{
		if (m_setcal->GetTrueVoc() != 0 )
		{
			obj->MultiYAny (m_setcal->GetScaleIsc());
			obj->MultiXAny (m_setcal->GetScaleVoc(),m_setcal->GetScalePmax(),m_setcal->GetTrueIsc(),m_setcal->GetTrueVoc());
		}else
			AfxMessageBox(_T("Please input the value of true voc!"));
	}

}

//电参数校准1
void CurveIV::FilteringElec1(CDoubleXY*const obj)
{
	obj->MultiYAny (m_setcal->GetScaleIsc1());
	obj->MultiXAny (m_setcal->GetScaleVoc1());
}
//对计算所得参数分档和报警
//有报警返回TRUE
BOOL CurveIV::FilteringParaAlarm(void)
{
	return m_setbase->GetSetParaAlarm()->FilterSeter(*this);
}
BOOL CurveIV::FilteringParaAlarmTmod(void)
{
	return m_setbase->GetSetParaAlarm()->FilterSeterTmod(*this);
}
MultiCurveIV::~MultiCurveIV(void)
{
}
MultiCurveIV::MultiCurveIV(void)
{
}
#include "math.h"
//双光强测量Ｒｓ
void CurveIV::EnvalRs(CurveIV* iv800)
{
	UINT pos=0;
	double dlti,v1k,v8k;
	double isc=GetIsc();
	double i8=iv800->GetIsc();

	iv800->GetMaxXY(pos);
	dlti=i8-iv800->y[pos];
	GetMaxXY(pos);
	dlti+=isc-y[pos];
	dlti/=2;
	v8k=i8-dlti;
	pos=iv800->GetXAtY(v8k);
	v1k=isc-dlti;
	pos=GetXAtY(v1k);
	SetRs(fabs((v8k-v1k)/(isc-i8)));	
}

void CurveIV::InitialIV()
{
	InitialSunPara();
	//InitialXY();
	x.clear();
	y.clear();
}