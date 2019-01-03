#include "StdAfx.h"
#include "Daq841.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  f(x)  (x/32768)
Daq841::Daq841(void)
{
	//配置m_ModeList，配置测试过程
	//独立测试功能
	m_setbase=&theApp.m_opt;
	m_set=m_setbase->GetSetDev841();
	m_setcal=m_setbase->GetSetSunPara();
	m_PlusCtrl.Reset();

	strvalue = _T("");

	SetName(_T("Daq841"));
	memset(m_ModeList,0,sizeof(m_ModeList));
}

Daq841::~Daq841(void)
{
}

INT_PTR Daq841::TestSingle(CurveIV *const iv,ParaList* pl)
{
	return TestSingle(pl->dwi,pl->dwv,pl->gq,pl->mode,iv,pl->step,pl->lowvol,pl->daqcnt,pl->dly);
}
//ARM采集函数
INT_PTR Daq841::ARMTestingRun(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short gq,unsigned short cmd)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else 
		sun=m_set->GetSun();
	iv->SetSun(sun);//光强
	sun=m_set->GetMcuSun(sun);
	double m_gqv = sun*1.8/1000;
	USHORT  m_int_gqv = (1+m_gqv/2.5)*32768;

	
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
// 	if (dwvol == 0x77)
// 		dwcur = 0x00;
	USHORT m_ncmd = cmd;

	byte delay=m_set->GetDelay();//可以改变测试总用时

	USHORT gqcnt = 100;
	USHORT testcnt = 400;// m_set->GetPointCnt();

	return ARMTestSingle(m_ncmd,dwcur+dwvol,m_int_gqv,gqcnt,iv,/*suns_voc,*/irradiance,delay,testcnt);
}
//
//缺省模式0可以测量逆向电流
//测试功能分段：
//第8位为一般测试
//
INT_PTR Daq841::TestingRun(CurveIV*const iv,unsigned short gq)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else 
		sun=m_set->GetSun();
	byte delay=m_set->GetDelay();//可以改变测试总用时
	iv->SetSun(sun);//光强
	sun=m_set->GetMcuSun(sun);
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODSET,iv,0,0,400,delay);
}
//定电压全段测试
//有效测试范围内的光强或者定电压测电流
INT_PTR Daq841::TestingRun(CurveIV *const iv,unsigned short gq,double vol,UINT step)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else sun=m_set->GetSun();
	iv->SetSun(sun);//光强

	//vol转换成MCU值
	double vscale=1.0;
	double dw_vol;
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;

	//生成参数列表
	USHORT mv,min,cnt;
	byte delay,st;
	mv=(USHORT)(vol/vscale);
	if(mv>4095)
		mv=4000;

	switch(step){
		case 0:st=0,min=mv,cnt=400,delay=1;
			break;//
		default:st=0,min=mv,cnt=400,delay=1;
			break;
	}
	delay=m_set->GetDelay();
	//延时，每次测试间隔最大255us；
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODCUS,iv,st,min,cnt,delay);
}
//定电压点区段测试
//单次测量定电压点区段（VOL为中心电压或者最小电压）
INT_PTR Daq841::TestingRun(CurveIV *const iv,unsigned short gq,double vol)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else sun=m_set->GetSun();
	iv->SetSun(sun);//光强

	//vol转换成MCU值
	double vscale=1.0;
	double dw_vol;
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;

	//生成参数列表
	USHORT mv,min,cnt,rge,pluslen;
	byte delay,step,ivtime;
	mv=(USHORT)(vol/vscale);
	if(mv>4095)
		return 0;
	pluslen=5000;//400点采集用时us,稍短于实际时长
	pluslen+=m_set->GetDelay();//总时长
	ivtime=pluslen/400;//采集一组IV点的时间us
	rge=240;//测试区段长度
	step=3;
	cnt=rge/step;
	delay=(pluslen-cnt*ivtime)/(cnt);
	//设定点在区段中间或者包容
	if(mv<rge/3)
		min=0;
	else
		min=mv-rge/3;
	//延时，每次测试间隔最大256us；
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODCUS,iv,step,min,cnt,delay);
}
//单片机求解最大功率点
INT_PTR Daq841::GetPmaxPoint(double &i,double &v,double prevol)
{
	if(!m_com.IsOpen())
		return 0;
	unsigned short val=(unsigned short )prevol;
	byte para[16];
	memset(para,0,sizeof(para));
	para[0]=val/256;
	para[1]=val%256;
	para[2]=1;
	m_com.SendFrm(CSerial::GETPMAX,para,sizeof(para));
	Sleep(150);
	int errct=0;
	unsigned int cmd;
	cmd=m_com.RecvFrm ();
	if(CSerial::GETPMAX==cmd){
		byte *pb=&m_com.recvbuf[2];
		i=*pb;i=i*256+*(pb+1);pb+=2;
		v=*pb;v=v*256+*(pb+1);
		return 1;
	}
	return 0;
}

//*************************************************************************//
//综合测试功能
//字段：档位（V，I），光强，逆向，步长，起始电压，采集次数，延时（iv&iv，i&v间）
//宽度：（1+1，2，1，1，2，2，1+1）
//*************************************************************************//
INT_PTR Daq841::TestSingle(unsigned char dwi,unsigned char dwv,unsigned short gq,unsigned char mode,CurveIV *const iv,unsigned char step,unsigned short lowvol,unsigned short daqcnt,unsigned char dly)
{
/*
#ifdef _DEBUG 
	iv->SetDateTime();
	return 400;
#endif
*/
// 	if(!m_com.AutoOpenPort())
// 	{
// 		m_set->SetDevState(0);
// 		return 0;
// 	}else{
// 		m_set->SetDevState(1);
// 	}
	/////////////////////加密授权部分/////////////////////////////
	m_ifneedactive = m_encr.NeedActive();
	if (m_ifneedactive == 0)    
	{
		return FALSE;
	}
	if (m_ifneedactive == 1)
	{
		m_encr_edit.SetCpuId(m_encr.GetCPUID());
		if (m_encr_edit.DoModal() == IDOK)
		{
			if (m_encr_edit.m_authcode.IsEmpty())
			{
				CString str;
				str.LoadString(IDS_STRAUTHMESSAGE_170);
				AfxMessageBox(str);
				return FALSE;
			}
			if (!m_encr.WriteRegitInfToBoard(m_encr_edit.m_authcode))
				return FALSE;
		}else
			return FALSE;
	}
	if (m_ifneedactive == 2)
	{
		m_set->m_atype = _T("Temporary");
		m_set->m_atime = m_encr.GetSurplustime()+_T(" days");
	}
	if (m_ifneedactive == 3)
	{
		m_set->m_atype = _T("Permanent");
		m_set->m_atime = _T("Permanent");
	}
	//////////////////////////////加密授权部分//////////////////////////////*/
	//设定最短间隔
	UINT interval=m_set->GetInterval();
	while(!m_PlusCtrl.IsLost(interval*pow(10.0,6)));
	m_PlusCtrl.Reset();

	iv->SetDateTime();

	//清除按键检测设置
	BOOL exSwitch=m_set->GetExternSwitch();
	m_set->SetExternSwitch(FALSE);
	m_set->Increment();


	//逆向测试——开关
	unsigned char notnx;
	if(m_set->GetCloseNx())
		notnx=1;
	else
		notnx=0;
	if(daqcnt==0)//数据采集次数
		daqcnt=400;
	gq*=3;//3*X to mcu DAC register
	double list[20];
	//计算转换率
	double vscale=1.0;
	double iscale=10.0;
	double nxscale=10.0;
	double m_isc_scale = m_setcal->GetStdCellIsc();;  //改动
	double sunscale=2.5/4096/17.16;// I*3.9*4.4/采集值= 2.5/4096;//参考电池电流换算系数=2.5/放大倍数/取样电阻/(满度4096)
	double dw_cur,dw_vol;
	dw_cur=m_set->GetDwCurValue ();
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;
	iscale=iscale*dw_cur/4096.0;
	nxscale=nxscale/4096.0;//固定10A档测量

//测试综合测试功能是否正常  
//档位	光强	逆向	步长	起始电压	采集次数	延时
//1+1	2	1	1	2	2	1+1
	byte para[16];
	memset(para,0,sizeof(para));
	para[0]=dwi;
	para[1]=dwv;
	para[2]=gq/256;
	para[3]=gq%256;
	para[4]=mode;
	para[5]=step;//7;//步长
	para[6]=lowvol/256;
	para[7]=lowvol%256;
	para[8]=daqcnt/256;
	para[9]=daqcnt%256;
	para[10]=dly;
	para[11]=notnx;
	para[12]=0x0A;
	unsigned int cmd;

	m_com.SendFrm (CSerial::TESTSTART,para,sizeof(para));
	SingleSleep(10);
	int errct=0;
	do{
		cmd=m_com.RecvFrm ();
		if(CSerial::READPARA==cmd){
			/*/mcu 存储顺序*/
			byte *p=m_com.recvbuf+2;
			double *lt=list,ref;
			*lt++=*p*256+*(p+1);p+=2;//envTemp
			*lt++=*p*256+*(p+1);p+=2;//voc1
			*lt++=*p*256+*(p+1);p+=2;//voc2
			*lt++=*p*256+*(p+1);p+=2;//nxcur;
			ref=*lt++=*p*256+*(p+1);p+=2;//ref25;
			*lt++=*p*256+*(p+1);p+=2;//SunRef1;
			*lt++=*p*256+*(p+1);p+=2;//SunRef2;
			*lt++=*p*256+*(p+1);p+=2;//cusdaqcnt;
			if((ref>2600)||(ref<2400))
				ref=2500;
			vscale*=2.5/(ref/1000.0);
			iscale*=2.5/(ref/1000.0);
			nxscale*=2.5/(ref/1000.0);
			list[0]*=1.0/16;//温度换算成实际温度
			list[1]*=vscale;
			list[2]*=vscale;
			list[3]*=nxscale;
			list[5]*=sunscale;
			list[6]*=sunscale;
			break;
		}else{//6次超时等待时间更有效，3次太短有可能遗漏正常的第一段数据
			if(errct<6)	errct++;
			else return 0;
		}
	}while(1);
	iv->SetEnvTemp(list[0]);//设置环境温度值
	iv->SetIrev(list[3]);//漏电流
	iv->SetStdCellIscTesting1(list[5]);//参考电池Isc
	iv->SetStdCellsunTesting1(list[5]*1000/m_isc_scale);                         //朱峰改动
	iv->SetStdCellIscTesting2(list[6]);//参考电池Isc
	iv->SetStdCellsunTesting2(list[6]*1000/m_isc_scale);                         //朱峰改动
	// 读IV数据
	INT_PTR ret=0;
	std::vector<double>::iterator itery,iterx;
	iterx=iv->x.begin ();
	itery=iv->y.begin ();
	for(int i=0;i<daqcnt*2*2/16+1;i++)
	{
		para[0]=i*0x10/256;
		para[1]=i*0x10%256;
		para[2]=0x10;
		m_com.SendFrm(CSerial::READXRAM,para,3);
		cmd=m_com.RecvFrm ();
		if(CSerial::READXRAM==cmd){
			byte *p=m_com.recvbuf+2;
			for(int k=0;k<4;k++){//16byte,4组(y,x)
				//Y 为电流、X为电压采集结果
				//n&0xF delete ADC Channel NO.
				*itery=(*p&0xF)*256+*(p+1);p+=2;
				*itery*=iscale;
				*iterx=(*p&0xF)*256+*(p+1);p+=2;
				*iterx*=vscale;
				//if(ret>350)//过滤末尾的无效坐标
				if(*iterx==0.0)
					if(*itery==0.0)
						break;
				itery++;iterx++;
				ret++;
			}
			errct=0;
		}else{
			if(errct<3)	errct++;
			else return 0;
		}
	}
	//读回来的数据数量为16的倍数
	if(daqcnt<ret)
		ret=daqcnt;
	iv->SetDataSize(ret);

	//恢复按键检测设置
	m_set->SetExternSwitch(exSwitch);
	m_set->UpdateData(FALSE);     //改动

	return ret;
}
INT_PTR Daq841::ARMTestSingle(USHORT cmd,byte dw,USHORT gqv,USHORT gqcnt,CurveIV *const iv,/*CDoubleXY *const suns_voc,*/CDoubleXY *const irradiance,byte dly,USHORT testcnt)
{
	//	m_com.Open( 2,115200,m_com.NoParity,8,m_com.OneStopBit );
	if( !m_com.AutoOpenPort(g_strvalue))
	{
		m_set->SetDevState(0,g_strvalue);
		m_LogTrace.WriteLine(_T("The motherboard is not detected!"));
		AfxMessageBox(_T("serialport is not opened!"));
		return 0;
	}else
	{
		m_set->SetDevState(1,g_strvalue);

		if(g_strvalue.Mid(0,2) != _T("EC"))
		{
			AfxMessageBox(_T("E00B3:Lower machine recognition code is incorrect!"));
			return 0;	
		}
	}
	/////////////////////加密授权部分/////////////////////////////
	m_ifneedactive = m_encr.NeedActive();
	if (m_ifneedactive == 0)    
	{
		return FALSE;
	}
	if (m_ifneedactive == 1)
	{
		m_encr_edit.SetCpuId(m_encr.GetCPUID());
		if (m_encr_edit.DoModal() == IDOK)
		{
			if (m_encr_edit.m_authcode.IsEmpty())
			{
				CString str;
				str.LoadString(IDS_STRAUTHMESSAGE_170);
				AfxMessageBox(str);
				return FALSE;
			}
			if (!m_encr.WriteRegitInfToBoard(m_encr_edit.m_authcode))
				return FALSE;
		}else
			return FALSE;
	}
	if (m_ifneedactive == 2)
	{
		m_set->m_atype = _T("Temporary");
		m_set->m_atime = m_encr.GetSurplustime()+_T(" days");
	}
	if (m_ifneedactive == 3)
	{
		m_set->m_atype = _T("Permanent");
		m_set->m_atime = _T("Permanent");
	}

	//判断发送的命令功能是否在下位机中有
	UCHAR L_cmd_fc[16];
	memset(L_cmd_fc,0,sizeof(L_cmd_fc));

	for (int i = 0; i < 16; i++)
	{
		L_cmd_fc[i] = cmd >> i & 1;
		if (L_cmd_fc[i] == 1)
		{
			if (m_fc[i] == 1)
				break;
			else
				return FALSE;
		}
		if (i == 15)
			return FALSE;
	}
	//////////////////////////////加密授权部分//////////////////////////////*/
	//设定最短间隔

	iv->SetDateTime();

	double m_cI,m_cV;
	double dataV,dataI,dataIsc;                //明特性和辐照度数据组
	DWORD dwBytesRead = 0;
	INT_PTR ret=0;

	m_cI = m_set->GetDwCurValue();
	m_cV = m_set->GetDwVolValue();

	byte m_extersuns;
	m_extersuns = 0x00;

	byte sendbuf[21];
	///////////////////给下位机写档位等配置////////////////////////////////
	memset(sendbuf, 0,(sizeof(byte)*21) );
	//帧头
	sendbuf[0] = 0xA5;
	sendbuf[1] = 0xA5;
	sendbuf[2] = 0xA5;
	sendbuf[3] = 0x5A;
	//数据长度
	sendbuf[4] = 0x0F;
	sendbuf[5] = 0x00;

	sendbuf[6] = 0xE1;                //主命令
	sendbuf[7] = 0x01;                //子命令 参数设置

	sendbuf[8] = 0xFF;                //执行测试时电子负载的起始DA控制电压输出值
	sendbuf[9] = 0xFF;
	sendbuf[10] = testcnt & 0xFF;     //测试取样点数
	sendbuf[11] = testcnt >> 8;
	sendbuf[12] = dw;                //档位
	sendbuf[13] = gqv & 0xFF;        //辐照度电压
	sendbuf[14] = gqv >> 8;
	sendbuf[15] = gqcnt & 0xFF;      //suns-voc点数
	sendbuf[16] = gqcnt >> 8;
	sendbuf[17] = dly;               //恒光时间
	sendbuf[18] = m_extersuns;         //suns使能位

	sendbuf[19] = LOBYTE (m_com.crc16(0,sendbuf,19));
	sendbuf[20] = HIBYTE (m_com.crc16(0,sendbuf,19));
	m_com.ClearALL();
	m_com.Write( sendbuf,21);
	SingleSleep(20);

	///////////////////主测试命令////////////////////////////////
	bool m_resend = false;
	byte m_ccmd = 0x01;
	UINT  m_recnt = 0;
loop:
	m_com.ClearALL();
	m_testdata.clear();
	memset(sendbuf, 0,(sizeof(byte)*21) );
	double m_env_temp(0.0);  //环境温度
	double m_surf_temp(0.0); //表面温度
	double m_rc_r(0.0);      //检测电池的电阻
	double m_rc_temp(0.0);   //检测电池温度
	UINT m_cnt = 0;           //闪光次数
	UINT m_datasize = 0;      //数据长度

	double m_irev_10(0.0);      //-10V暗电流
	double m_irev_12(0.0);      //-12V暗电流

	USHORT j = 0;            //读取的字符长度

	if(m_resend)
		m_ccmd = 0x10;
	else
		m_ccmd = 0x01;

	//帧头
	sendbuf[0] = 0xA5;
	sendbuf[1] = 0xA5;
	sendbuf[2] = 0xA5;
	sendbuf[3] = 0x5A;
	//数据长度
	sendbuf[4] = 0x08;
	sendbuf[5] = 0x00;

	sendbuf[6] = 0xD1;                //主命令
	sendbuf[7] = m_ccmd;                //子命令 参数设置

	sendbuf[8] = cmd & 0xFF;          //组合测试命令
	sendbuf[9] = cmd >> 8;

	sendbuf[10] = 0xEC;               //加密了的软件专用识别码

	sendbuf[11] = 0x00;               //高效电池的模式：普通高效（0），优化高效（1）

	sendbuf[12] = LOBYTE (m_com.crc16(0,sendbuf,12));
	sendbuf[13] = HIBYTE (m_com.crc16(0,sendbuf,12));
	m_com.ClearALL();
	m_com.Write( sendbuf,14);

	m_PlusCtrl.Reset();
	while(1)//m_com.DataWaiting(5000)) //(1)    有些英文系统不认DataWaiting
	{
		dwBytesRead = m_com.BytesWaiting();
		if (dwBytesRead == 0)
			if (m_PlusCtrl.GetLost() > 5.0)
				return 0;
			else
				continue;
		unsigned char* databuffer = new unsigned char[dwBytesRead];
		unsigned char* backbuffer = databuffer;
		m_com.Read(databuffer,dwBytesRead);
		for(int i = 0; i < dwBytesRead; i++)
		{
			m_testdata.push_back(*databuffer);
			if (i == (dwBytesRead-1))
				break;
			databuffer = databuffer+1;
		}
		delete [] backbuffer;
		/*在这里加结束符AA BB CC DD*/
		if (m_testdata[m_testdata.size()-1] == 0xDD)
			if (m_testdata[m_testdata.size()-2] == 0xCC)
				if (m_testdata[m_testdata.size()-3] == 0xBB)
					if (m_testdata[m_testdata.size()-4] == 0xAA)
						break;
	}

	//增加数据长度和校验值的判断
	if(m_testdata.size() > 12)
	{
		byte m_crclo,m_crchi;
		m_crclo = LOBYTE(m_com.crc16vector(0,m_testdata,m_testdata.size()-6));
		m_crchi = HIBYTE(m_com.crc16vector(0,m_testdata,m_testdata.size()-6));

		if(!(m_crclo == m_testdata[m_testdata.size()-6] && m_crchi == m_testdata[m_testdata.size()-5]))
		{
			m_LogTrace.WriteLine(_T("crc wrong!"));
			if(m_recnt < 3)
			{
				m_resend = true;
				m_recnt++;
				goto loop;
			}else
				return 0;
		}
	}


	int i = 0;
	while(i < m_testdata.size())
	{
		if ((m_testdata[i+0] == 0xA5) 
			&& (m_testdata[i+1] == 0xA5) 
			&& (m_testdata[i+2] == 0xA5)
			&& (m_testdata[i+3] == 0x5A))
		{
			j = MAKEWORD(m_testdata[i+4],m_testdata[i+5]); //J的长度是命令符+参数+数据+校验（注意，不包含结束符）

			if ((m_testdata[i+6] == 0xD1)             //判断子命令01是否返回相同的命令组合
				&& (m_testdata[i+7] == 0x01))
			{
				USHORT m_read_cmd = MAKEWORD(m_testdata[i+8],m_testdata[i+9]);
				if (m_read_cmd != cmd)
					return 0;
			}

			if ((m_testdata[i+6] == 0xD1)             
				&& ((m_testdata[i+7] == 0x66)||(m_testdata[i+7] == 0x67)))       // 读IV数据
			{
				//数据长度
				/*这里的44包括
				命令符2byte(D1 66)

				环境温度(18b20   2byte)
				检测电池温度(pt1000  2byte)
				闪光次数(4byte)
				红外温度(2byte)
				-10V暗电流(2byte)
				-12V暗电流(2byte)
				暗电流档位(2byte)

				保留(2byte*12)

				校验(2byte)		
				*///////
				m_datasize = (j-44)/(2*3);  
				
				//环境温度
				m_env_temp = MAKEWORD(m_testdata[i+8],m_testdata[i+9]);
				if (m_env_temp > 64656)
					m_env_temp = 0;

				m_env_temp *= 0.0625;

				if (m_env_temp > 80)
					m_env_temp = 25;

				iv->SetEnvTemp(m_env_temp);

				//检测电池温度
				double m_rc_r_ad;
				double R0 = 1000.0;
				double a = 3.9038;
				double b = 5.7750;
				m_rc_r_ad = MAKEWORD(m_testdata[i+10],m_testdata[i+11]);
				m_rc_r = f(m_rc_r_ad)*4000;
				if (m_rc_r > 1000 && m_rc_r < 1200)
				{
					m_rc_temp = (sqrt(R0*R0*a*pow(10.0,-3)*a*pow(10.0,-3)-4*R0*b*pow(10.0,-7)*(R0-m_rc_r))-R0*a*pow(10.0,-3))/(2*R0*b*pow(10.0,-7));
				}else
					m_rc_temp = 0.0;

				iv->SetRCTemp(m_rc_temp);

				//表面温度
				m_surf_temp = MAKEWORD(m_testdata[i+12],m_testdata[i+13]);
				if (m_surf_temp > 64656)
					m_surf_temp = 0;

				m_surf_temp /= 1000.0;

				if (m_surf_temp > 80)
					m_surf_temp = 25;

				//iv->SetSurfTemp(m_surf_temp);

				//闪光次数
				WORD l_lo_cnt = MAKEWORD(m_testdata[i+14],m_testdata[i+15]);
				WORD l_hi_cnt = MAKEWORD(m_testdata[i+16],m_testdata[i+17]);

				m_cnt = MAKELONG(l_lo_cnt,l_hi_cnt);

				m_set->m_cnt3 = m_set->m_cnt2 = m_set->m_cnt1 = m_cnt;
				m_set->Increment();

				//-10V暗电流
				m_irev_10 = MAKEWORD(m_testdata[i+18],m_testdata[i+19]);
				//-12V暗电流
				m_irev_12 = MAKEWORD(m_testdata[i+20],m_testdata[i+21]);
				
				//暗电流系数
				double  m_cI_an = MAKEWORD(m_testdata[i+22],m_testdata[i+23]);
				if (m_cI_an == 11)
				{
					m_cI_an = 0.2;
				}else if (m_cI_an == 12)
				{
					m_cI_an =2;
				}else if (m_cI_an == 13)
				{
					m_cI_an = 20;
				}

// 				iv->SetIrev_10(f(m_irev_10)*m_cI_an);
// 				iv->SetIrev_12(f(m_irev_12)*m_cI_an);

				//保留12个参数备用，每个2byte

				//数据,最后有两个校验位不读取,明特性数据计算公式
				//共3种数据（依次为待测电池电压、待测电池的电流、反馈电池的短路电流（实际得到的是电压））
				//然后用   电压*2.5/20=电流   这里的20是反馈电池的电阻
				for( int k = 0; k < m_datasize;k++ )             
				{
					dataV = MAKEWORD(m_testdata[i+48+k*2*3],m_testdata[i+49+k*2*3]);
					if (f(dataV) > 1)
					{
						dataV = 0;
					}

					dataI = MAKEWORD(m_testdata[i+50+k*2*3],m_testdata[i+51+k*2*3]);
					if (f(dataI) > 1)
					{
						dataI = 0;
					}

					dataIsc = MAKEWORD(m_testdata[i+52+k*2*3],m_testdata[i+53+k*2*3]);

					iv->x.push_back(f(dataV)*m_cV);
					iv->y.push_back(f(dataI)*m_cI);
					irradiance->x.push_back(f(dataV)*m_cV);
					irradiance->y.push_back(f(dataIsc)*2.5/10);    //此处保留原始数据如果转换的话*m_yscale/2;
				}
			}
			i = i+6+j;
		}else
		{
			i++;
		}
	}

	iv->SetDataSize(iv->x.size());
	irradiance->SetDataSize(irradiance->x.size());
	ret = iv->x.size();
	m_set->UpdateData(FALSE);
	m_com.ClearALL();       //有两个校验位没读取  导致有存储。
	memset(sendbuf, 0,(sizeof(byte)*21) );
	return ret;
}
INT_PTR Daq841::portstate(unsigned char* p)
{
	if(!m_com.IsOpen ())
		return 0;
	m_com.SendFrm (CSerial::SWOPERATE,p,16);
	unsigned int cmd;
	cmd=m_com.RecvFrm ();
	if(CSerial::SWOPERATE==cmd){
		unsigned char* r=m_com.recvbuf+2;
		memcpy(p,r,4);
		return 4;
	}else return 0;
}
//?
INT_PTR Daq841::__Test(void)
{
	if(m_com.IsOpen ())
		return 0;
//测试单片机回应，和在线
	CSerial *com=&m_com;
	com->SendFrm (CSerial::ONLINE ,0,0);
	UINT cmd=com->RecvFrm ();
	if(CSerial::ONLINE==cmd)
		MessageBox(0,_T("Online Ok"),_T("Infomation"),IDOK);
//测试综合测试功能是否正常
//档位	光强	逆向	步长	起始电压	采集次数	延时
//1+1	2	1	1	2	1	1+1
	byte para[]={1,1,0,0,1,30,0,0,10,0,0};
	//*(unsigned short*)(&para[2])=3000;//顺序不对
	para[2]=3000/256;
	para[3]=3000%256;
	//*(unsigned short*)(&para[6])=1000;
	para[6]=1000/256;
	para[7]=1000%256;
	com->SendFrm (CSerial::TESTSTART,para,sizeof(para));
	 cmd=com->RecvFrm ();
	if(CSerial::READPARA==cmd)
		MessageBox(0,_T("TESTSTART Ok"),_T("Infomation"),IDOK);
//测试读XRAM,CODE,IRAM功能，2字节起始地址，1字节长度
	byte buf[60000];
	memset(buf,0xaa,sizeof(buf));
	for(int i=0;i<1000;i++){
		para[0]=i*0x10/256;
		para[1]=i*0x10%256;
		para[2]=0x10;
//	com->SendFrm (CSerial::READXRAM,para,3);
	com->SendFrm (CSerial::READCODE,para,3);
//	com->SendFrm (CSerial::READIRAM,para,3);
	cmd=com->RecvFrm ();
//	if(CSerial::READXRAM==cmd)
	if(CSerial::READCODE==cmd)
//	if(CSerial::READIRAM==cmd)
		memcpy(buf+i*0x10,com->recvbuf+2,0x10);
		//MessageBox(0,_T("READXRAM Ok"),_T("Infomation"),IDOK);
	}
//测试版本信息读取
	com->SendFrm (CSerial::VERSION,para,sizeof(para));
	cmd=com->RecvFrm ();
	if(CSerial::VERSION==cmd){
		memcpy(buf,com->recvbuf+2,0x10);
		MessageBox(0,_T("VERSION Ok"),_T("Infomation"),IDOK);
	}
//测试参考电压设置
	return 1;
}
//检测下位机事件，给出回应
//exmsg
UINT Daq841::CheckExternEvent(UINT exmsg)
{
	UINT ret=0;
	switch(exmsg){
		case DaqBase::KEYACTOR:
			ret=checkmcukeystate();
			break;
		case DaqBase::LOGIN:
			break;
		case DaqBase::ONLINE:
			break;
	}
	return ret;
}
//检测单片机按键事件
UINT Daq841::checkmcukeystate(void)
{
	if (!m_set->GetExternSwitch() && m_com.IsOpen())
	{
		m_com.ClearALL();
	}
	if(!m_set->GetExternSwitch()||!m_com.IsOpen())
		return 0;
	UINT ret=0;
	byte recvbuf[10];
	memset(recvbuf, 0,(sizeof(byte)*10) );
	SingleSleep(10);
	m_com.Read(recvbuf,10);
	if ((recvbuf[0] == 0xA5) 
		&& (recvbuf[1] == 0xA5) 
		&& (recvbuf[2] == 0xA5)
		&& (recvbuf[3] == 0x5A))
	{
		USHORT j = MAKEWORD(recvbuf[4],recvbuf[5]);

		if ((recvbuf[6] == 0xB1)             //判断子命令01是否返回相同的命令组合
			&& (recvbuf[7] == 0x01)
			&& j == 4)
		{
			m_com.ClearALL();
			memset(recvbuf, 0,(sizeof(byte)*10) );
			ret = 1;
		}
	}

	return ret;
}