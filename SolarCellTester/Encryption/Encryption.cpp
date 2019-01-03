#include "stdafx.h"
#include "Encryption.h"
#include "CppSQLite3U.h"
#include "zip_encrypt.h"


#define Dictfilename _T("Gsola.zip")
#define Dictname     _T("Gsola.db")
#define Dictpwd      _T("ppnn13%dkstfeb.1")
#define MONTH        37
#define QUARTER      97
#define YEAR         372


CEncryption::CEncryption(void)
{
	m_assigner = _T("");
	m_Surplustime = _T("");

	m_Auth_flag_temp=0;
	m_Reg_time_temp=0;
	m_LastTest_time_temp=0;
	m_authcnt_temp=0;
}

CEncryption::~CEncryption(void)
{

}

//获取CPU ID    //MAC ID
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
CString CEncryption::GetCPUID()
{
	////获得mac地址函数///*/
	char *mac=new char[32];
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	CString str_return = _T("");
	int flag = 0;
	if (ERROR_SUCCESS == nRel)
	{
		USES_CONVERSION;
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			if ((pIpAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)&&(flag == 0))
			{
				cout<<"网卡MAC地址：";
				for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
				{
					sprintf(mac,"%02X",pIpAdapterInfo->Address[i]);
					str_return += A2T(mac);
				}
				flag = 1;
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo != NULL)
	{
		delete pIpAdapterInfo;
	}
	delete mac;
	return str_return;
}


BOOL CEncryption::GetDataBaseDict(CString filezip,CString pwd)
{
	CString fn;
	Getpath(fn);
	CString defilenameW,newfilenameW;
	defilenameW = fn+_T("temp.zip");
	newfilenameW = fn+Dictname;

	PCHAR defilezip,depassword,defilename,newfilename; 

	USES_CONVERSION;
	defilezip = W2A(filezip);
	defilename = W2A(defilenameW);
	depassword = W2A(pwd);
	newfilename = W2A(newfilenameW);

	if(defilezip[strlen(defilezip)-1] == 'n')
		defilezip[strlen(defilezip)-1] = ' ';

	if(defilename[strlen(defilename)-1] == 'n')
		defilename[strlen(defilename)-1] = ' ';

	zip_encrypt  m_zip;
	if(m_zip.DecryptFile(defilezip, defilename, depassword))
	{
		if (m_zip.OnExtract(defilename,newfilename))
		{
			DeleteFile(defilenameW);
			return TRUE;
		}
	}
	DeleteFile(defilenameW);
	return FALSE;
}

BOOL CEncryption::ZipDict(CString dictforzip,CString pwd)
{
	CString fn;
	Getpath(fn);

	CString filezipW,szDestinationW;
	filezipW = fn+_T("temp.zip");
	szDestinationW = fn+Dictfilename;
	
	PCHAR fileforzip,filezip,szDestination,szPassword;

	USES_CONVERSION;
	fileforzip = W2A(dictforzip);
	filezip = W2A(filezipW);
	szPassword = W2A(pwd);
	szDestination = W2A(szDestinationW);

	zip_encrypt  m_zip;
	if (m_zip.OnZipit(fileforzip,filezip))
	{
		if(filezip[strlen(filezip)-1] == 'n')
			filezip[strlen(filezip)-1] = ' ';

		if(szDestination[strlen(szDestination)-1] == 'n')
			szDestination[strlen(szDestination)-1] = ' ';
		if (m_zip.EncryptFile(filezip, szDestination, szPassword))
		{
			DeleteFile(filezipW);
			return TRUE;
		}
	}
	DeleteFile(filezipW);
	return FALSE;
}

int CEncryption::CheckNumberFromDataBase(CString number)
{

	int RetV;
	CString fn;
	Getpath(fn);
	CString m_filezip = fn+Dictfilename;
	CString m_dictforzip = fn+Dictname;
	CString m_tempzip = fn+_T("temp.zip");
	RetV = 0;

	CppSQLite3DB  m_db;
	CppSQLite3Query m_query;
	
	USES_CONVERSION;
	char* password = T2A(Dictpwd);

	CString str;
	if(!PathFileExists(m_filezip))
	{
// 		str.LoadString(_T("E00D1:The Gsola.zip package was not found!"));
// 		AfxMessageBox(str);
	}else
	{
		if(!GetDataBaseDict(m_filezip,Dictpwd))
			DeleteFile(m_tempzip);
		else
		{
			m_db.open(m_dictforzip);
			sqlite3_rekey(m_db.mpDB,password,strlen(password));
			m_db.close();
			DeleteFile(m_filezip);
		}

	}

	if(!PathFileExists(m_dictforzip))
	{
		AfxMessageBox(_T("E00D1:The Gsola.db package was not found in the directory of the software!"));
		RetV = 0;
	}else
	{

		try 
		{
			m_db.open(m_dictforzip);
			
			sqlite3_key(m_db.mpDB,password,strlen(password));
			sqlite3_rekey(m_db.mpDB,0,0);

			if(!m_db.tableExists(_T("AccountTable")))   
			{   
				AfxMessageBox(_T("E00D2:The Gsola.db has been damaged!"));
				RetV = 0;
			}else
			{
				CString strtemp(_T(""));
				CString m_cpuid = GetCPUID();
				m_query = m_db.execQuery(_T("SELECT * FROM AccountTable WHERE CPUSerialNumber = \'") + m_cpuid + _T("\';"));
				strtemp = m_query.fieldValue(4);
				if (strtemp.IsEmpty())
				{
					m_query.finalize();
					AfxMessageBox(_T("E00D3:The machine code does not match,\n\
									 please use the matched machine code or \n\
									 request new authorization package by the new machine code!"));
					RetV = 0;
				}else
				{
					m_query.finalize();
					m_query = m_db.execQuery(_T("SELECT * FROM AccountTable WHERE TMaccount = \'") + number + _T("\';"));
					strtemp = m_query.fieldValue(0);
					if (strtemp.IsEmpty())
					{
						m_query.finalize();
						m_query = m_db.execQuery(_T("SELECT * FROM AccountTable WHERE T3Maccount = \'") + number + _T("\';"));
						strtemp = m_query.fieldValue(1);
						if (strtemp.IsEmpty())
						{
							m_query.finalize();
							m_query = m_db.execQuery(_T("SELECT * FROM AccountTable WHERE TYaccount = \'") + number + _T("\';"));
							strtemp = m_query.fieldValue(2);
							if (strtemp.IsEmpty())
							{
								m_query.finalize();
								m_query = m_db.execQuery(_T("SELECT * FROM AccountTable WHERE Paccount = \'") + number + _T("\';"));
								strtemp = m_query.fieldValue(3);
								if (strtemp.IsEmpty())
								{
									AfxMessageBox(_T("E00D4:This authorization code may be wrong or has been used!"));
									RetV = 0;
								}else
								{
									m_query.finalize();
									m_db.execDML(_T("update AccountTable set Paccount='' where Paccount='")+number+_T("';"));

									m_query = m_db.execQuery(_T("select * from AccountTable"));
									m_assigner = m_query.fieldValue(6);
									RetV = 4;

								}
							}else
							{
								m_query.finalize();
								m_db.execDML(_T("update AccountTable set TYaccount='' where TYaccount='")+number+_T("';"));
								m_query = m_db.execQuery(_T("select * from AccountTable"));
								m_assigner = m_query.fieldValue(6);
								RetV = 3;
							}
						}else
						{
							m_query.finalize();
							m_db.execDML(_T("update AccountTable set T3Maccount='' where T3Maccount='")+number+_T("';"));

							m_query = m_db.execQuery(_T("select * from AccountTable"));
							m_assigner = m_query.fieldValue(6);
							RetV = 2;
						}
					}else
					{
						m_query.finalize();
						m_db.execDML(_T("update AccountTable set TMaccount='' where TMaccount='")+number+_T("';"));

						m_query = m_db.execQuery(_T("select * from AccountTable"));
						m_assigner = m_query.fieldValue(6);
						RetV = 1;
					}
				}
			}

			m_query.finalize();
			sqlite3_rekey(m_db.mpDB,password,strlen(password));
			m_db.close();
			DeleteFile(m_filezip);
		}	
		catch(CppSQLite3Exception ex)  
		{  
			m_query.finalize();
			sqlite3_rekey(m_db.mpDB,password,strlen(password));
			m_db.close();

			DeleteFile(m_filezip);

			AfxMessageBox(ex.errorMessage());  
			RetV = 0;
		}
	}


	DeleteFile(m_tempzip);

	return RetV;
}
int CEncryption::Random()
{
	srand( (unsigned)time( NULL ) );
	int n = 1 + rand();
	return n;
}

int CEncryption::ReadSign()
{
	UCHAR L_sign = 0;
	CString str;

	m_head_address = 0;
	m_length = 1;

	if(ReadEEPROM(m_head_address,m_length,m_ReadData))
	{
		if(m_ReadData.size() == m_length)
		{
			L_sign = m_ReadData[0];
		}else
		{
			AfxMessageBox(_T("E0004:The lower machine returns the wrong data length"));
		}
	}
	else
	{
		AfxMessageBox(_T("E00A1:Read flag bit error!"));
	}

	return L_sign;
}

void CEncryption::Read2time(UINT& lasttime,UINT& regtime)
{
	CString str;

	m_head_address = 1;
	m_length = 8;

	if(ReadEEPROM(m_head_address,m_length,m_ReadData))
	{
		if(m_ReadData.size() == m_length)
		{
			WORD m_lo_lasttime = MAKEWORD(m_ReadData[0],m_ReadData[1]);
			WORD m_hi_lasttime = MAKEWORD(m_ReadData[2],m_ReadData[3]);
			lasttime = MAKELONG(m_lo_lasttime,m_hi_lasttime);

			WORD m_lo_regtime = MAKEWORD(m_ReadData[4],m_ReadData[5]);
			WORD m_hi_regtime = MAKEWORD(m_ReadData[6],m_ReadData[7]);
			regtime = MAKELONG(m_lo_regtime,m_hi_regtime);
		}else
		{
			AfxMessageBox(_T("E0004:The lower machine returns the wrong data length"));
		}
	}
	else
	{
		AfxMessageBox(_T("E00A2:Read storage time error!"));
	}

}
USHORT CEncryption::ReadFC()
{
	USHORT L_FC = 0;

	CString str;

	m_head_address = 9;
	m_length = 2;
	if(ReadEEPROM(m_head_address,m_length,m_ReadData))
	{
		if(m_ReadData.size() == m_length)
		{
			L_FC = MAKEWORD(m_ReadData[0],m_ReadData[1]);
		}else	
		{
			AfxMessageBox(_T("E0004:The lower machine returns the wrong data length"));
		}
	}
	else
	{
		AfxMessageBox(_T("E00A4:Read function code error!"));
	}

	return L_FC;
}
int CEncryption::ReadRegCnt()
{
	UCHAR L_cnt = 0;

	CString str;

	m_head_address = 11;
	m_length = 1;
	if(ReadEEPROM(m_head_address,m_length,m_ReadData))
	{
		if(m_ReadData.size() == m_length)
		{
			L_cnt = m_ReadData[0];
		}else	
		{
			AfxMessageBox(_T("E0004:The lower machine returns the wrong data length"));
		}
	}
	else
	{
		AfxMessageBox(_T("E00A5:Read the number of authorization errors!"));
	}

	return L_cnt;
}
int CEncryption::ReadRegitNumberCRC(std::vector<USHORT> &regCRC)
{
	CString str;

    UCHAR L_RegCnt = 0;
	L_RegCnt = ReadRegCnt();
	regCRC.clear();

	m_head_address = 12;
	m_length = 2;

	for (int i = 0; i < L_RegCnt; i++)
	{
		if(ReadEEPROM(m_head_address+2*i,m_length,m_ReadData))
		{
			if(m_ReadData.size() == m_length)
			{
				regCRC.push_back(MAKEWORD(m_ReadData[0],m_ReadData[1]));
			}else
			{
				AfxMessageBox(_T("E0004:The lower machine returns the wrong data length"));
			}
		}
		else
		{
			AfxMessageBox(_T("E00A6:Read authorization code checksum error!"));
		}
	}
	return L_RegCnt;
}
BOOL CEncryption::WriteNowTime(UINT nowtime)
{
	CString str;

	byte parabuf[4];

	m_head_address = 1;
	m_length = 4;
	
	memset(parabuf,0,sizeof(parabuf));

	parabuf[0] = (nowtime & 0xFFFF) & 0xFF;
	parabuf[1] = (nowtime & 0xFFFF) >> 8;
	parabuf[2] = (nowtime >> 16) & 0xFF;
	parabuf[3] = (nowtime >> 16) >> 8;

	if (WriteEEPROM(m_head_address,parabuf,4))
		return TRUE;
	else
	{
		AfxMessageBox(_T("E00A3:Write time error!"));
	}
	return FALSE;
}
BOOL CEncryption::WriteRegitInfToBoard(CString RegitNumber)
{
	CString str;

	if(RegitNumber.GetLength() != 14)
	{
		AfxMessageBox(_T("E00B2:The length of the authorization code is not 14!"));
		return FALSE;
	}

	CString str_tempFC(_T("")),str_regitnumber(_T(""));
	
	str_tempFC = RegitNumber.Mid(1,1)+RegitNumber.Mid(3,1)
					+RegitNumber.Mid(5,1)+RegitNumber.Mid(7,1);

	str_regitnumber = RegitNumber.Mid(0,1)+RegitNumber.Mid(2,1)
						+RegitNumber.Mid(4,1)+RegitNumber.Mid(6,1)+RegitNumber.Mid(8,6);
	
	int L_write_flag = 0;
	L_write_flag = CheckNumberFromDataBase(str_regitnumber);    //标志位
	if (L_write_flag == 0)
	{
		return FALSE;
	}

	int L_regcnt = 0;
	std::vector<USHORT> L_tempRegCRC;
	L_regcnt = ReadRegitNumberCRC(L_tempRegCRC);      //授权次数
	
	if (L_regcnt != L_tempRegCRC.size())
	{
		AfxMessageBox(_T("E00A7:The number of authorized times read is wrong!"));
		return FALSE;
	}

	///////////////////使用序列号生成的CRC值作为保存的数/////////////////////////
	UCHAR L_regnumber_buf[5];
	memset(L_regnumber_buf, 0,sizeof(L_regnumber_buf) );

	for(int i = 0; i < 5; i++)
	{
		L_regnumber_buf[i] = wcstol(str_regitnumber.Mid(i*2,2),NULL,16);
	}

	USHORT L_NewRegnumberCRC;
	L_NewRegnumberCRC = m_com.crc16(0,L_regnumber_buf,5);

	for (int i = 0; i < L_tempRegCRC.size(); i++)
	{
		if (L_NewRegnumberCRC == L_tempRegCRC[i])      //如果存的有这个码，则退出
		{
			AfxMessageBox(_T("E00A8:This authorization code has been used!"));
			return FALSE;
		}
	}

// 	if (L_regcnt == 44)                 //要不要对注册次数满的情况进行处理
// 	{
// 		L_regcnt = 0;
// 	}

	UINT L_timenow;
	L_timenow = (unsigned)time( NULL );           //lasttime && regtime


	UCHAR L_FC_buf[2];                            //功能码
	memset(L_FC_buf, 0,sizeof(L_FC_buf) );
	
	for(int i = 0; i < 2; i++)
	{
		L_FC_buf[i] = wcstol(str_tempFC.Mid(i*2,2),NULL,16);
	}

	byte parabuf[12];

	memset(parabuf,0,sizeof(parabuf));

	parabuf[0] = (byte)L_write_flag;                //标志位

	parabuf[1] = (L_timenow & 0xFFFF) & 0xFF;       //lasttime    
	parabuf[2] = (L_timenow & 0xFFFF) >> 8;
	parabuf[3] = (L_timenow >> 16) & 0xFF;
	parabuf[4] = (L_timenow >> 16) >> 8;

	parabuf[5] = (L_timenow & 0xFFFF) & 0xFF;         //regtime
	parabuf[6] = (L_timenow & 0xFFFF) >> 8;
	parabuf[7] = (L_timenow >> 16) & 0xFF;
	parabuf[8] = (L_timenow >> 16) >> 8;

	parabuf[9] = L_FC_buf[0];                        //功能码
	parabuf[10] = L_FC_buf[1]; 

	parabuf[11] = (byte)(L_regcnt+1);               //授权次数

	m_head_address = 0;                             //首地址和长度
	m_length = 12;
	
	if(WriteEEPROM(m_head_address,parabuf,m_length))
	{
		byte parabuf_1[2];
		memset(parabuf_1,0,sizeof(parabuf_1));

		parabuf_1[0] = LOBYTE(L_NewRegnumberCRC);
		parabuf_1[1] = HIBYTE(L_NewRegnumberCRC);

		m_head_address = 12+L_regcnt*2;        //首地址和长度
		m_length = 2;

		Sleep(1000);

		if(WriteEEPROM(m_head_address,parabuf_1,m_length))
		{
			str.LoadString(IDS_STRENCRMESSAGE_206);
			AfxMessageBox(str);
			exit(0);
			return TRUE;
		}
		else
		{
			AfxMessageBox(_T("E00A9:Write authorization check code error!"));
		}
	}
	else
	{
		AfxMessageBox(_T("E00AA:Write authorization information error!"));
	}
	
	return FALSE;
}



#include "../InOutTip.h"
/*/
返回值意义
0:电脑最后一次测试时间大于现在电脑时间；标志位是0（未授权），1,2,3（临时授权），4（永久授权）之外的数。
1:需要授权了
2:是临时授权类型，授权期限还没到
3:是永久授权类型
/*/
int CEncryption::NeedActive()
{
	//
	int m_read_flag = 0;
	CString str;
	m_read_flag = ReadSign();
	

	if (m_read_flag == 0)    
	{
		return 1;
	}

	if (m_read_flag == 1 || m_read_flag == 2 || m_read_flag == 3)
	{
		UINT m_lasttime = 0;
		UINT m_regtime = 0;
		UINT m_nowtime = 0;

		Read2time(m_lasttime,m_regtime);

		m_nowtime = (unsigned)time( NULL );

		if (m_lasttime > m_nowtime)
		{
			AfxMessageBox(_T("E00B1:Computer time error, please adjust the computer time correctly!"));
			return 0;
		}
		double times = (m_nowtime-m_lasttime)/3600.0;
		if(times > 24.0)
			WriteNowTime(m_nowtime);

		int days = (m_nowtime-m_regtime)/(3600*24);

		if (m_read_flag == 1)
			if (days >= MONTH)
				return 1;
			else
			{
				m_Surplustime.Format(_T("%d"),(MONTH-days-7));

				if (((MONTH-days) <= 17) && ((MONTH-days) >7))
				{
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(0,0,255));
					str.LoadString(IDS_STRENCRMESSAGE_212);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				
				}
				if (((MONTH-days) <= 7) && ((MONTH-days) >= 1))
				{
					m_Surplustime.Format(_T("%d"),(MONTH-days));
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(255,0,0));
					str.LoadString(IDS_STRENCRMESSAGE_213);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				}
			}

		if (m_read_flag == 2)
			if (days >= QUARTER)
				return 1;
			else
			{
				m_Surplustime.Format(_T("%d"),(QUARTER-days-7));

				if (((QUARTER-days) <= 17) && ((QUARTER-days) > 7))
				{
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(0,0,255));
					str.LoadString(IDS_STRENCRMESSAGE_212);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				}
				if (((QUARTER-days) <= 7) && ((QUARTER-days) >= 1))
				{
					m_Surplustime.Format(_T("%d"),(QUARTER-days));
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(255,0,0));
					str.LoadString(IDS_STRENCRMESSAGE_213);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				}
			}

		if (m_read_flag == 3)
			if (days >= YEAR)
				return 1;
			else
			{
				m_Surplustime.Format(_T("%d"),(YEAR-days-7));

				if (((YEAR-days) <= 17) && ((YEAR-days) > 7))
				{
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(0,0,255));
					str.LoadString(IDS_STRENCRMESSAGE_212);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				}
				if (((YEAR-days) <= 7) && ((YEAR-days) >= 1))
				{
					m_Surplustime.Format(_T("%d"),(YEAR-days));
					CInOutTipDlg* pMsgWindow = new CInOutTipDlg();
					pMsgWindow->Set_TextColor(RGB(255,0,0));
					str.LoadString(IDS_STRENCRMESSAGE_213);
					pMsgWindow->ShowMsgWindow(AfxGetMainWnd(), m_Surplustime+str);
				}
			}

		return 2;
	}

	if (m_read_flag == 4)
	{
		return 3;
	}
	
	AfxMessageBox(_T("E00C2:This motherboard may not reset!"));
	return 0;
	//*/
}

BOOL CEncryption::GetAuthInf()
{
	CString str;
	if(!m_com.IsOpen())
	{
		str.LoadString(IDS_STRENCRMESSAGE_205);
		AfxMessageBox(str);
		return FALSE;
	}

	m_Auth_flag_temp = ReadSign();
	if (m_Auth_flag_temp == 255 || m_Auth_flag_temp == 0)
	{
		str.LoadString(IDS_STRENCRMESSAGE_210);
		AfxMessageBox(str);
		m_com.ClearALL();
		m_com.Close();
		return FALSE;
	}

	Read2time(m_Reg_time_temp,m_LastTest_time_temp);

	m_authcnt_temp=ReadRegCnt();

	if (m_authcnt_temp == 0)
	{
		str.LoadString(IDS_STRENCRMESSAGE_211);
		AfxMessageBox(str);
		m_com.ClearALL();
		m_com.Close();
		return FALSE;
	}
	m_com.ClearALL();
	m_com.Close();
	return TRUE;
}

int CEncryption::ReSet()
{
	CString str;
	if(!m_com.IsOpen())
	{
		str.LoadString(IDS_STRENCRMESSAGE_205);
		AfxMessageBox(str);
		return FALSE;
	}

	m_com.ClearALL();
	memset(m_com.sendbuf, 0,(sizeof(byte)*21) );
	memset(m_com.recvbuf, 0,(sizeof(byte)*21) );

	int m_Random_number = Random();
	unsigned char m_rand_buf[2];
	m_rand_buf[0] = LOBYTE(m_Random_number);
	m_rand_buf[1] = HIBYTE(m_Random_number);

	m_com.sendbuf[0] = 0xEF;
	m_com.sendbuf[1] = 0x97;
	m_com.sendbuf[2] = m_rand_buf[0];
	m_com.sendbuf[3] = m_rand_buf[1];
	m_com.sendbuf[19] = LOBYTE(m_com.CRC16(m_com.sendbuf,19));
	m_com.sendbuf[20] = HIBYTE(m_com.CRC16(m_com.sendbuf,19));	

	m_com.ClearALL();
	m_com.Write(m_com.sendbuf,21);
	SingleSleep(10);
	m_com.Read(m_com.recvbuf,21);
	if (m_com.recvbuf[1] == 0x97 && m_com.recvbuf[2] == 0x00)
	{
		m_com.ClearALL();
		memset(m_com.sendbuf, 0,(sizeof(byte)*21));
		memset(m_com.recvbuf, 0,(sizeof(byte)*21));

		m_com.sendbuf[0] = 0xEF;
		m_com.sendbuf[1] = 0xED;
		m_com.sendbuf[2] = LOBYTE(m_com.CRC16(m_rand_buf,2));
		m_com.sendbuf[3] = HIBYTE(m_com.CRC16(m_rand_buf,2));
		m_com.sendbuf[4] = 0xC4;
		m_com.sendbuf[5] = 0xD2;
		m_com.sendbuf[6] = 0xA8;
		m_com.sendbuf[7] = 0xB6;
		m_com.sendbuf[19] = LOBYTE(m_com.CRC16(m_com.sendbuf,19));
		m_com.sendbuf[20] = HIBYTE(m_com.CRC16(m_com.sendbuf,19));

		m_com.ClearALL();
		m_com.Write(m_com.sendbuf,21);
		SingleSleep(1000);
		m_com.Read(m_com.recvbuf,21);

		if (m_com.recvbuf[1] == 0xED && m_com.recvbuf[2] == 0x00)
		{
			m_com.ClearALL();
			m_com.Close();
			return 1;
		}
		if (m_com.recvbuf[1] == 0xED && m_com.recvbuf[2] == 0xC4)
		{
			m_com.ClearALL();
			m_com.Close();
			return 0;
		}
	}

}

BOOL CEncryption::SetAuthInf()
{
	CString str;
	if(!m_com.IsOpen())
	{
		str.LoadString(IDS_STRENCRMESSAGE_205);
		AfxMessageBox(str);
		return FALSE;
	}
	int m_cnt_count = 0;
	for (int i=0; i<m_authcnt_temp; i++)
	{
		m_com.ClearALL();
		memset(m_com.sendbuf, 0,(sizeof(byte)*21) );
		memset(m_com.recvbuf, 0,(sizeof(byte)*21) );

		USES_CONVERSION;
		unsigned char m_rand_buf[2];
		int m_rand_buf_number;
		m_rand_buf_number = atoi(T2A(m_ranumber_temp.GetAt(i)));
		m_rand_buf[0] = LOBYTE(m_rand_buf_number);
		m_rand_buf[1] = HIBYTE(m_rand_buf_number);



		m_com.sendbuf[0] = 0xEF;
		m_com.sendbuf[1] = 0x97;
		m_com.sendbuf[2] = m_rand_buf[0];
		m_com.sendbuf[3] = m_rand_buf[1];

		m_com.sendbuf[19] = HIBYTE(m_com.DataCRC(m_com.sendbuf,19));
		m_com.sendbuf[20] = LOBYTE(m_com.DataCRC(m_com.sendbuf,19));	
		m_com.ClearALL();
		m_com.Write(m_com.sendbuf,21);
		SingleSleep(10);
		m_com.Read(m_com.recvbuf,21);
		if (m_com.recvbuf[1] == 0x97 && m_com.recvbuf[2] == 0x00)
		{
			m_com.ClearALL();
			memset(m_com.sendbuf, 0,(sizeof(byte)*21) );
			memset(m_com.recvbuf, 0,(sizeof(byte)*21) );

			int m_buf_assigner;
			m_buf_assigner = atoi(T2A(m_assigner_temp.GetAt(i)));

			WORD m_hiregtime = HIWORD(m_Reg_time_temp);
			WORD m_loregtime = LOWORD(m_Reg_time_temp);

			WORD m_hitesttime = HIWORD(m_LastTest_time_temp);
			WORD m_lotesttime = LOWORD(m_LastTest_time_temp);

			m_com.sendbuf[0] = 0xEF;
			m_com.sendbuf[1] = 0xEF;
			m_com.sendbuf[2] = HIBYTE(m_com.DataCRC(m_rand_buf,2));
			m_com.sendbuf[3] = LOBYTE(m_com.DataCRC(m_rand_buf,2));
			m_com.sendbuf[4] = (byte)m_Auth_flag_temp;
			m_com.sendbuf[5] = LOBYTE(m_buf_assigner);
			m_com.sendbuf[6] = HIBYTE(m_buf_assigner);

			m_com.sendbuf[7] = LOBYTE(m_loregtime);    //注册时间
			m_com.sendbuf[8] = HIBYTE(m_loregtime);
			m_com.sendbuf[9] = LOBYTE(m_hiregtime);
			m_com.sendbuf[10] = HIBYTE(m_hiregtime);

			m_com.sendbuf[11] = LOBYTE(m_lotesttime);    //电脑当前时间
			m_com.sendbuf[12] = HIBYTE(m_lotesttime);
			m_com.sendbuf[13] = LOBYTE(m_hitesttime);
			m_com.sendbuf[14] = HIBYTE(m_hitesttime);


			m_com.sendbuf[19] = HIBYTE(m_com.DataCRC(m_com.sendbuf,19));
			m_com.sendbuf[20] = LOBYTE(m_com.DataCRC(m_com.sendbuf,19));

			m_com.ClearALL();

			m_com.Write(m_com.sendbuf,21);
			SingleSleep(10);
			m_com.Read(m_com.recvbuf,21);

			if (m_com.recvbuf[1] == 0xEF && m_com.recvbuf[2] == 0x00)
			{
				m_cnt_count++;
			}
			if (m_com.recvbuf[1] == 0xEF && m_com.recvbuf[2] == 0xC4)
			{
				
				m_cnt_count--;
			}
		}
		if (m_com.recvbuf[1] == 0x97 && m_com.recvbuf[2] == 0xC4)
		{
			m_cnt_count--;
		}
	}

	if ((m_cnt_count >= 1) && (m_cnt_count <= m_authcnt_temp))
	{
		m_com.ClearALL();
		m_com.Close();
		return TRUE;
	}else
	{
		m_com.ClearALL();
		m_com.Close();
		return FALSE;
	}

}
BOOL CEncryption::ReadEEPROM(UCHAR head_address,UCHAR length,std::vector<UCHAR> &read_data)
{
	if (m_com.IsOpen())
	{
		SingleSleep(30);
		byte sendbuf[50];
		DWORD dwBytesRead = 0;
		std::vector<UCHAR> temp_data;

		temp_data.clear();
		read_data.clear();

		memset(sendbuf,0,sizeof(sendbuf));
		//帧头
		sendbuf[0] = 0xA5;
		sendbuf[1] = 0xA5;
		sendbuf[2] = 0xA5;
		sendbuf[3] = 0x5A;
		//数据长度
		sendbuf[4] = 0x0C;
		sendbuf[5] = 0x00;

		sendbuf[6] = 0xA1;                //主命令
		sendbuf[7] = 0x00;                //子命令 读

		sendbuf[8] = 0x00;               
		sendbuf[9] = 0x00;
		sendbuf[10] = 0x00;    
		sendbuf[11] = 0x00;
		sendbuf[12] = 0x00;                
		sendbuf[13] = 0x00;   

		sendbuf[14] = head_address;
		sendbuf[15] = length;            
		sendbuf[16] = LOBYTE (m_com.crc16(0,sendbuf,16));
		sendbuf[17] = HIBYTE (m_com.crc16(0,sendbuf,16));
		m_com.ClearALL();
		m_com.Write(sendbuf,18);

		m_TimeOut.Reset();
		while(1)
		{
			dwBytesRead = m_com.BytesWaiting();
			if (dwBytesRead == 0)
				if (m_TimeOut.GetLost() > 3.0)
				{
					AfxMessageBox(_T("E0002:Timeout,the lower machine does not return any data in 2 seconds!"));
					return FALSE;
				}
				else
					continue;
			UCHAR* databuffer = new UCHAR[dwBytesRead];
			UCHAR* backbuffer = databuffer;
			m_com.Read(databuffer,dwBytesRead);
			for(int i = 0; i < dwBytesRead; i++)
			{
				temp_data.push_back(*databuffer);
				if (i == (dwBytesRead-1))
					break;
				databuffer = databuffer+1;
			}
			delete [] backbuffer;
			if (temp_data[temp_data.size()-1] == 0xDD)
				if (temp_data[temp_data.size()-2] == 0xCC)
					if (temp_data[temp_data.size()-3] == 0xBB)
						if (temp_data[temp_data.size()-4] == 0xAA)
							break;
		}
		
		int n=0;
		USHORT len=0;
		while(n < temp_data.size())
		{
			if ((temp_data[n+0] == 0xA5) 
				&& (temp_data[n+1] == 0xA5) 
				&& (temp_data[n+2] == 0xA5)
				&& (temp_data[n+3] == 0x5A))
			{
				len = MAKEWORD(temp_data[n+4],temp_data[n+5]);

				if ((temp_data[n+6] == 0xA1)             
					&& (temp_data[n+7] == 0x00))
				{
					if ((temp_data[n+14] == head_address)             
						&& (temp_data[n+15] == length))
						for (int m=0;m<length;m++)
							read_data.push_back(temp_data[n+15+m+1]);

				}

				if ((temp_data[n+6] == 0xA1)            
					&& (temp_data[n+7] == 0x02))
				{
					AfxMessageBox(_T("E0003:The lower machine returned a mistake.\n \
									 The reason for the mistake may be that you changed the time of the computer.\n \
									 Please correct the computer time and restart the device, then proceed with subsequent operation."));
					return FALSE;
				}

				n = n+len+6;
			}else
			{
				n++;
			}
		}
		
	}else
	{
		AfxMessageBox(_T("E0001:Serialport is not opened!"));
		return FALSE;
	}
	return TRUE;
}
BOOL CEncryption::WriteEEPROM(UCHAR head_address,UCHAR *data,UCHAR length)
{
	if (m_com.IsOpen())
	{
		byte sendbuf[50],tempbuf[6];
		DWORD dwBytesRead = 0;
		std::vector<UCHAR> temp_data;
		temp_data.clear();

		memset(sendbuf,0,sizeof(sendbuf));
		memset(tempbuf,0,sizeof(tempbuf));


		USHORT L_len;
		int L_timenow;

		L_len = 12+length;
		L_timenow = (unsigned)time( NULL );

		tempbuf[0] = (L_timenow & 0xFFFF) & 0xFF;
		tempbuf[1] = (L_timenow & 0xFFFF) >> 8;
		tempbuf[2] = (L_timenow >> 16) & 0xFF;
		tempbuf[3] = (L_timenow >> 16) >> 8;	
		tempbuf[4] = LOBYTE (m_com.crc16(0,tempbuf,4));
		tempbuf[5] = HIBYTE (m_com.crc16(0,tempbuf,4));

		//帧头
		sendbuf[0] = 0xA5;
		sendbuf[1] = 0xA5;
		sendbuf[2] = 0xA5;
		sendbuf[3] = 0x5A;
		//数据长度
		sendbuf[4] = L_len & 0xFF;
		sendbuf[5] = L_len >> 8;

		sendbuf[6] = 0xA1;                //主命令
		sendbuf[7] = 0x01;                //子命令 写

		memcpy(&sendbuf[8],tempbuf,6);

		sendbuf[14] = head_address;
		sendbuf[15] = length; 

		memcpy(&sendbuf[16],data,length);


		sendbuf[16+length] = LOBYTE (m_com.crc16(0,sendbuf,16+length));
		sendbuf[17+length] = HIBYTE (m_com.crc16(0,sendbuf,16+length));
		m_com.ClearALL();
		m_com.Write(sendbuf,18+length);

		m_TimeOut.Reset();
		while(1)
		{
			dwBytesRead = m_com.BytesWaiting();
			if (dwBytesRead == 0)
				if (m_TimeOut.GetLost() > 2.0)
				{
					AfxMessageBox(_T("E0002:Timeout,the lower machine does not return any data in 2 seconds!"));
					return FALSE;
				}
				else
					continue;
			UCHAR* databuffer = new UCHAR[dwBytesRead];
			UCHAR* backbuffer = databuffer;
			m_com.Read(databuffer,dwBytesRead);
			for(int i = 0; i < dwBytesRead; i++)
			{
				temp_data.push_back(*databuffer);
				if (i == (dwBytesRead-1))
					break;
				databuffer = databuffer+1;
			}
			delete [] backbuffer;
			if (temp_data[temp_data.size()-1] == 0xDD)
				if (temp_data[temp_data.size()-2] == 0xCC)
					if (temp_data[temp_data.size()-3] == 0xBB)
						if (temp_data[temp_data.size()-4] == 0xAA)
							break;
		}

		int n=0;
		USHORT len=0;
		while(n < temp_data.size())
		{
			if ((temp_data[n+0] == 0xA5) 
				&& (temp_data[n+1] == 0xA5) 
				&& (temp_data[n+2] == 0xA5)
				&& (temp_data[n+3] == 0x5A))
			{
				len = MAKEWORD(temp_data[n+4],temp_data[n+5]);

				if ((temp_data[n+6] == 0xA1)             
					&& (temp_data[n+7] == 0x01))
				{
					if ((temp_data[n+14] == head_address)             
						&& (temp_data[n+15] == length))
							return TRUE;
				}

				if ((temp_data[n+6] == 0xA1)            
					&& (temp_data[n+7] == 0x02))
				{
					AfxMessageBox(_T("E0003:The lower machine returned a mistake.\n \
									 The reason for the mistake may be that you changed the time of the computer.\n \
									 Please correct the computer time and restart the device, then proceed with subsequent operation."));
					return FALSE;
				}
				n = n+len+6;
			}else
			{
				n++;
			}			
		}
	}else
	{
		AfxMessageBox(_T("E0001:Serialport is not opened!"));
		return FALSE;
	}
	return TRUE;
}