#include "StdAfx.h"
#include "Daq841.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  f(x)  (x/32768)
Daq841::Daq841(void)
{
	//����m_ModeList�����ò��Թ���
	//�������Թ���
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
//ARM�ɼ�����
INT_PTR Daq841::ARMTestingRun(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short gq,unsigned short cmd)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else 
		sun=m_set->GetSun();
	iv->SetSun(sun);//��ǿ
	sun=m_set->GetMcuSun(sun);
	double m_gqv = sun*1.8/1000;
	USHORT  m_int_gqv = (1+m_gqv/2.5)*32768;

	
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
// 	if (dwvol == 0x77)
// 		dwcur = 0x00;
	USHORT m_ncmd = cmd;

	byte delay=m_set->GetDelay();//���Ըı��������ʱ

	USHORT gqcnt = 100;
	USHORT testcnt = 400;// m_set->GetPointCnt();

	return ARMTestSingle(m_ncmd,dwcur+dwvol,m_int_gqv,gqcnt,iv,/*suns_voc,*/irradiance,delay,testcnt);
}
//
//ȱʡģʽ0���Բ����������
//���Թ��ֶܷΣ�
//��8λΪһ�����
//
INT_PTR Daq841::TestingRun(CurveIV*const iv,unsigned short gq)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else 
		sun=m_set->GetSun();
	byte delay=m_set->GetDelay();//���Ըı��������ʱ
	iv->SetSun(sun);//��ǿ
	sun=m_set->GetMcuSun(sun);
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODSET,iv,0,0,400,delay);
}
//����ѹȫ�β���
//��Ч���Է�Χ�ڵĹ�ǿ���߶���ѹ�����
INT_PTR Daq841::TestingRun(CurveIV *const iv,unsigned short gq,double vol,UINT step)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else sun=m_set->GetSun();
	iv->SetSun(sun);//��ǿ

	//volת����MCUֵ
	double vscale=1.0;
	double dw_vol;
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;

	//���ɲ����б�
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
	//��ʱ��ÿ�β��Լ�����255us��
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODCUS,iv,st,min,cnt,delay);
}
//����ѹ�����β���
//���β�������ѹ�����Σ�VOLΪ���ĵ�ѹ������С��ѹ��
INT_PTR Daq841::TestingRun(CurveIV *const iv,unsigned short gq,double vol)
{
	UINT sun;
	if(!(gq<200||gq>50000))
		sun=gq;	
	else sun=m_set->GetSun();
	iv->SetSun(sun);//��ǿ

	//volת����MCUֵ
	double vscale=1.0;
	double dw_vol;
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;

	//���ɲ����б�
	USHORT mv,min,cnt,rge,pluslen;
	byte delay,step,ivtime;
	mv=(USHORT)(vol/vscale);
	if(mv>4095)
		return 0;
	pluslen=5000;//400��ɼ���ʱus,�Զ���ʵ��ʱ��
	pluslen+=m_set->GetDelay();//��ʱ��
	ivtime=pluslen/400;//�ɼ�һ��IV���ʱ��us
	rge=240;//�������γ���
	step=3;
	cnt=rge/step;
	delay=(pluslen-cnt*ivtime)/(cnt);
	//�趨���������м���߰���
	if(mv<rge/3)
		min=0;
	else
		min=mv-rge/3;
	//��ʱ��ÿ�β��Լ�����256us��
	byte dwcur=(byte)m_set->GetDwCur();
	byte dwvol=(byte)m_set->GetDwVol();
	return TestSingle(dwcur,dwvol,sun,TMODCUS,iv,step,min,cnt,delay);
}
//��Ƭ���������ʵ�
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
//�ۺϲ��Թ���
//�ֶΣ���λ��V��I������ǿ�����򣬲�������ʼ��ѹ���ɼ���������ʱ��iv&iv��i&v�䣩
//��ȣ���1+1��2��1��1��2��2��1+1��
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
	/////////////////////������Ȩ����/////////////////////////////
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
	//////////////////////////////������Ȩ����//////////////////////////////*/
	//�趨��̼��
	UINT interval=m_set->GetInterval();
	while(!m_PlusCtrl.IsLost(interval*pow(10.0,6)));
	m_PlusCtrl.Reset();

	iv->SetDateTime();

	//��������������
	BOOL exSwitch=m_set->GetExternSwitch();
	m_set->SetExternSwitch(FALSE);
	m_set->Increment();


	//������ԡ�������
	unsigned char notnx;
	if(m_set->GetCloseNx())
		notnx=1;
	else
		notnx=0;
	if(daqcnt==0)//���ݲɼ�����
		daqcnt=400;
	gq*=3;//3*X to mcu DAC register
	double list[20];
	//����ת����
	double vscale=1.0;
	double iscale=10.0;
	double nxscale=10.0;
	double m_isc_scale = m_setcal->GetStdCellIsc();;  //�Ķ�
	double sunscale=2.5/4096/17.16;// I*3.9*4.4/�ɼ�ֵ= 2.5/4096;//�ο���ص�������ϵ��=2.5/�Ŵ���/ȡ������/(����4096)
	double dw_cur,dw_vol;
	dw_cur=m_set->GetDwCurValue ();
	dw_vol=m_set->GetDwVolValue ();
	vscale=vscale*dw_vol/4096.0;
	iscale=iscale*dw_cur/4096.0;
	nxscale=nxscale/4096.0;//�̶�10A������

//�����ۺϲ��Թ����Ƿ�����  
//��λ	��ǿ	����	����	��ʼ��ѹ	�ɼ�����	��ʱ
//1+1	2	1	1	2	2	1+1
	byte para[16];
	memset(para,0,sizeof(para));
	para[0]=dwi;
	para[1]=dwv;
	para[2]=gq/256;
	para[3]=gq%256;
	para[4]=mode;
	para[5]=step;//7;//����
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
			/*/mcu �洢˳��*/
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
			list[0]*=1.0/16;//�¶Ȼ����ʵ���¶�
			list[1]*=vscale;
			list[2]*=vscale;
			list[3]*=nxscale;
			list[5]*=sunscale;
			list[6]*=sunscale;
			break;
		}else{//6�γ�ʱ�ȴ�ʱ�����Ч��3��̫���п�����©�����ĵ�һ������
			if(errct<6)	errct++;
			else return 0;
		}
	}while(1);
	iv->SetEnvTemp(list[0]);//���û����¶�ֵ
	iv->SetIrev(list[3]);//©����
	iv->SetStdCellIscTesting1(list[5]);//�ο����Isc
	iv->SetStdCellsunTesting1(list[5]*1000/m_isc_scale);                         //���Ķ�
	iv->SetStdCellIscTesting2(list[6]);//�ο����Isc
	iv->SetStdCellsunTesting2(list[6]*1000/m_isc_scale);                         //���Ķ�
	// ��IV����
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
			for(int k=0;k<4;k++){//16byte,4��(y,x)
				//Y Ϊ������XΪ��ѹ�ɼ����
				//n&0xF delete ADC Channel NO.
				*itery=(*p&0xF)*256+*(p+1);p+=2;
				*itery*=iscale;
				*iterx=(*p&0xF)*256+*(p+1);p+=2;
				*iterx*=vscale;
				//if(ret>350)//����ĩβ����Ч����
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
	//����������������Ϊ16�ı���
	if(daqcnt<ret)
		ret=daqcnt;
	iv->SetDataSize(ret);

	//�ָ������������
	m_set->SetExternSwitch(exSwitch);
	m_set->UpdateData(FALSE);     //�Ķ�

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
	/////////////////////������Ȩ����/////////////////////////////
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

	//�жϷ��͵�������Ƿ�����λ������
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
	//////////////////////////////������Ȩ����//////////////////////////////*/
	//�趨��̼��

	iv->SetDateTime();

	double m_cI,m_cV;
	double dataV,dataI,dataIsc;                //�����Ժͷ��ն�������
	DWORD dwBytesRead = 0;
	INT_PTR ret=0;

	m_cI = m_set->GetDwCurValue();
	m_cV = m_set->GetDwVolValue();

	byte m_extersuns;
	m_extersuns = 0x00;

	byte sendbuf[21];
	///////////////////����λ��д��λ������////////////////////////////////
	memset(sendbuf, 0,(sizeof(byte)*21) );
	//֡ͷ
	sendbuf[0] = 0xA5;
	sendbuf[1] = 0xA5;
	sendbuf[2] = 0xA5;
	sendbuf[3] = 0x5A;
	//���ݳ���
	sendbuf[4] = 0x0F;
	sendbuf[5] = 0x00;

	sendbuf[6] = 0xE1;                //������
	sendbuf[7] = 0x01;                //������ ��������

	sendbuf[8] = 0xFF;                //ִ�в���ʱ���Ӹ��ص���ʼDA���Ƶ�ѹ���ֵ
	sendbuf[9] = 0xFF;
	sendbuf[10] = testcnt & 0xFF;     //����ȡ������
	sendbuf[11] = testcnt >> 8;
	sendbuf[12] = dw;                //��λ
	sendbuf[13] = gqv & 0xFF;        //���նȵ�ѹ
	sendbuf[14] = gqv >> 8;
	sendbuf[15] = gqcnt & 0xFF;      //suns-voc����
	sendbuf[16] = gqcnt >> 8;
	sendbuf[17] = dly;               //���ʱ��
	sendbuf[18] = m_extersuns;         //sunsʹ��λ

	sendbuf[19] = LOBYTE (m_com.crc16(0,sendbuf,19));
	sendbuf[20] = HIBYTE (m_com.crc16(0,sendbuf,19));
	m_com.ClearALL();
	m_com.Write( sendbuf,21);
	SingleSleep(20);

	///////////////////����������////////////////////////////////
	bool m_resend = false;
	byte m_ccmd = 0x01;
	UINT  m_recnt = 0;
loop:
	m_com.ClearALL();
	m_testdata.clear();
	memset(sendbuf, 0,(sizeof(byte)*21) );
	double m_env_temp(0.0);  //�����¶�
	double m_surf_temp(0.0); //�����¶�
	double m_rc_r(0.0);      //����صĵ���
	double m_rc_temp(0.0);   //������¶�
	UINT m_cnt = 0;           //�������
	UINT m_datasize = 0;      //���ݳ���

	double m_irev_10(0.0);      //-10V������
	double m_irev_12(0.0);      //-12V������

	USHORT j = 0;            //��ȡ���ַ�����

	if(m_resend)
		m_ccmd = 0x10;
	else
		m_ccmd = 0x01;

	//֡ͷ
	sendbuf[0] = 0xA5;
	sendbuf[1] = 0xA5;
	sendbuf[2] = 0xA5;
	sendbuf[3] = 0x5A;
	//���ݳ���
	sendbuf[4] = 0x08;
	sendbuf[5] = 0x00;

	sendbuf[6] = 0xD1;                //������
	sendbuf[7] = m_ccmd;                //������ ��������

	sendbuf[8] = cmd & 0xFF;          //��ϲ�������
	sendbuf[9] = cmd >> 8;

	sendbuf[10] = 0xEC;               //�����˵����ר��ʶ����

	sendbuf[11] = 0x00;               //��Ч��ص�ģʽ����ͨ��Ч��0�����Ż���Ч��1��

	sendbuf[12] = LOBYTE (m_com.crc16(0,sendbuf,12));
	sendbuf[13] = HIBYTE (m_com.crc16(0,sendbuf,12));
	m_com.ClearALL();
	m_com.Write( sendbuf,14);

	m_PlusCtrl.Reset();
	while(1)//m_com.DataWaiting(5000)) //(1)    ��ЩӢ��ϵͳ����DataWaiting
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
		/*������ӽ�����AA BB CC DD*/
		if (m_testdata[m_testdata.size()-1] == 0xDD)
			if (m_testdata[m_testdata.size()-2] == 0xCC)
				if (m_testdata[m_testdata.size()-3] == 0xBB)
					if (m_testdata[m_testdata.size()-4] == 0xAA)
						break;
	}

	//�������ݳ��Ⱥ�У��ֵ���ж�
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
			j = MAKEWORD(m_testdata[i+4],m_testdata[i+5]); //J�ĳ����������+����+����+У�飨ע�⣬��������������

			if ((m_testdata[i+6] == 0xD1)             //�ж�������01�Ƿ񷵻���ͬ���������
				&& (m_testdata[i+7] == 0x01))
			{
				USHORT m_read_cmd = MAKEWORD(m_testdata[i+8],m_testdata[i+9]);
				if (m_read_cmd != cmd)
					return 0;
			}

			if ((m_testdata[i+6] == 0xD1)             
				&& ((m_testdata[i+7] == 0x66)||(m_testdata[i+7] == 0x67)))       // ��IV����
			{
				//���ݳ���
				/*�����44����
				�����2byte(D1 66)

				�����¶�(18b20   2byte)
				������¶�(pt1000  2byte)
				�������(4byte)
				�����¶�(2byte)
				-10V������(2byte)
				-12V������(2byte)
				��������λ(2byte)

				����(2byte*12)

				У��(2byte)		
				*///////
				m_datasize = (j-44)/(2*3);  
				
				//�����¶�
				m_env_temp = MAKEWORD(m_testdata[i+8],m_testdata[i+9]);
				if (m_env_temp > 64656)
					m_env_temp = 0;

				m_env_temp *= 0.0625;

				if (m_env_temp > 80)
					m_env_temp = 25;

				iv->SetEnvTemp(m_env_temp);

				//������¶�
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

				//�����¶�
				m_surf_temp = MAKEWORD(m_testdata[i+12],m_testdata[i+13]);
				if (m_surf_temp > 64656)
					m_surf_temp = 0;

				m_surf_temp /= 1000.0;

				if (m_surf_temp > 80)
					m_surf_temp = 25;

				//iv->SetSurfTemp(m_surf_temp);

				//�������
				WORD l_lo_cnt = MAKEWORD(m_testdata[i+14],m_testdata[i+15]);
				WORD l_hi_cnt = MAKEWORD(m_testdata[i+16],m_testdata[i+17]);

				m_cnt = MAKELONG(l_lo_cnt,l_hi_cnt);

				m_set->m_cnt3 = m_set->m_cnt2 = m_set->m_cnt1 = m_cnt;
				m_set->Increment();

				//-10V������
				m_irev_10 = MAKEWORD(m_testdata[i+18],m_testdata[i+19]);
				//-12V������
				m_irev_12 = MAKEWORD(m_testdata[i+20],m_testdata[i+21]);
				
				//������ϵ��
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

				//����12���������ã�ÿ��2byte

				//����,���������У��λ����ȡ,���������ݼ��㹫ʽ
				//��3�����ݣ�����Ϊ�����ص�ѹ�������صĵ�����������صĶ�·������ʵ�ʵõ����ǵ�ѹ����
				//Ȼ����   ��ѹ*2.5/20=����   �����20�Ƿ�����صĵ���
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
					irradiance->y.push_back(f(dataIsc)*2.5/10);    //�˴�����ԭʼ�������ת���Ļ�*m_yscale/2;
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
	m_com.ClearALL();       //������У��λû��ȡ  �����д洢��
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
//���Ե�Ƭ����Ӧ��������
	CSerial *com=&m_com;
	com->SendFrm (CSerial::ONLINE ,0,0);
	UINT cmd=com->RecvFrm ();
	if(CSerial::ONLINE==cmd)
		MessageBox(0,_T("Online Ok"),_T("Infomation"),IDOK);
//�����ۺϲ��Թ����Ƿ�����
//��λ	��ǿ	����	����	��ʼ��ѹ	�ɼ�����	��ʱ
//1+1	2	1	1	2	1	1+1
	byte para[]={1,1,0,0,1,30,0,0,10,0,0};
	//*(unsigned short*)(&para[2])=3000;//˳�򲻶�
	para[2]=3000/256;
	para[3]=3000%256;
	//*(unsigned short*)(&para[6])=1000;
	para[6]=1000/256;
	para[7]=1000%256;
	com->SendFrm (CSerial::TESTSTART,para,sizeof(para));
	 cmd=com->RecvFrm ();
	if(CSerial::READPARA==cmd)
		MessageBox(0,_T("TESTSTART Ok"),_T("Infomation"),IDOK);
//���Զ�XRAM,CODE,IRAM���ܣ�2�ֽ���ʼ��ַ��1�ֽڳ���
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
//���԰汾��Ϣ��ȡ
	com->SendFrm (CSerial::VERSION,para,sizeof(para));
	cmd=com->RecvFrm ();
	if(CSerial::VERSION==cmd){
		memcpy(buf,com->recvbuf+2,0x10);
		MessageBox(0,_T("VERSION Ok"),_T("Infomation"),IDOK);
	}
//���Բο���ѹ����
	return 1;
}
//�����λ���¼���������Ӧ
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
//��ⵥƬ�������¼�
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

		if ((recvbuf[6] == 0xB1)             //�ж�������01�Ƿ񷵻���ͬ���������
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