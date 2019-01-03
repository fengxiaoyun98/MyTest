// SunPara.cpp: implementation of the CSunPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunPara.h"
#include "math.h"
#include "DoubleXY.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//����������
CSunPara::CSunPara()
{
	//��ʼ����
	m_FF=0.0;
	m_Isc=0.0;
	m_Voc=0.0;
	m_Imp=0.0;
	m_Vmp=0.0;
	m_Pmax=0.0;
	m_Sun=0.0;
	m_Eff=0.0;	
	m_Rs=0;
	m_Rsh=0;
	m_Serial=_T("");
	m_grade=_T("");
	m_Stime=_T("");
	m_SDirection=_T("");
	m_IrrCali=_T("");
	m_Temp=0.0;	
	m_SurfTemp=0;
	m_EnvTemp=0;
	m_RCTemp=0;
	m_SunRef=0;
	m_Irev=0;
	m_Idot=0;
	m_Pdot=0;
	m_Vdot=0;
	m_StdCellIsc=0;
	m_StdCellIsc1=0;
	m_StdCellIsc2=0;
	m_StdCellsun1=0;     //���Ķ�
	m_StdCellsun2=0;     //���Ķ�
	m_Jsc=0;

	SetDateTime();
}
CSunPara::~CSunPara()
{

}

//���ò��Ի����¶�
void CSunPara::SetTemp(double fTemp)
{
	m_Temp=fTemp;
}

//���ò��Ի������ն�
void CSunPara::SetSun(double fSun)
{
	m_Sun=fSun;
}

void CSunPara::SetSunRef(double fSun)
{
	m_SunRef=fSun;
}
//ʵ�ʲ��Թ���/����Ĺ⹦��=Eff
//1ƽ�׵�λ�����ǿ1000
//1ƽ��=10000ƽ������
//����⹦�ʵ���1000*�������cm2��/10000��
//ʵ�⹦��Pmax
void CSunPara::EvaluationEff(double area)
{
	//����Eff
	if(area>0&&m_Sun>0)
		SetEff(m_Pmax/(m_Sun*area/10000));
	else
		SetEff(0);
}
void CSunPara::SetEff(double fEff)
{
	if(fEff<100&&fEff>0)m_Eff=fEff;
	else m_Eff=0;
}

BOOL CSunPara::SaveSunParam(CString fileName)
{
	return SaveSunParam(m_Serial,fileName);
}

//�������
//������CString strSerialΪ��Ʒ���к�
BOOL CSunPara::SaveSunParam(CString strSerial,CString fileName)
{
	CString m_time;
	m_Serial=strSerial;
	m_oledata=m_oledata.GetCurrentTime();
	m_time=m_oledata.Format(_T("%Y/%m/%d %X"));

	CString Voc,Isc,Vmp,Imp,Pmax,Eff,FF,Rs,Rsh,Sun,Temp;

	Voc.Format(_T("%f,"),m_Voc);
	Isc.Format(_T("%f,"),m_Isc);
	Vmp.Format(_T("%f,"),m_Vmp);
	Imp.Format(_T("%f,"),m_Imp);
	Pmax.Format(_T("%f,"),m_Pmax);
	Eff.Format(_T("%f,"),m_Eff);
	FF.Format(_T("%f,"),m_FF);
	Rs.Format(_T("%f,"),m_Rs);
	Rsh.Format(_T("%f,"),m_Rsh);
	Sun.Format(_T("%f,"),m_Sun);
	Temp.Format(_T("%f,"),m_Temp);

	CString result=m_Serial+_T(',')+Isc+Voc+Pmax+Vmp+Imp+Eff+FF+Rs+Rsh+Sun+Temp+fstr(m_EnvTemp)+fstr(m_SurfTemp)+fstr(m_Irev)+m_time+_T('\n');

	CStdioFile file;
	BOOL bopen=false;
	bopen=file.Open(fileName,CFile::modeWrite);   //�ж�ָ���ļ��Ƿ����
	if(bopen)
	{
		if(file.GetLength()<2)return FALSE; 

		file.SeekToEnd();
		file.WriteString(result);
		file.Close();
		return TRUE;
	}
	else
	{
		if(file.Open(fileName,CFile::modeCreate|CFile::modeWrite))//����ļ������ڣ�����һ���ļ�
		{
			CString s=_T("SerialNo,Isc,Voc,Vmp,Pmax,Imp,Eff,FF,Rs,Rsh,Sun,Temp,m_envTemp,SurfTemp,Irev,Timer");
			
			file.WriteString(s);
			file.SeekToEnd();
			file.WriteString(result);
			file.Close();
			return TRUE;
		}
		else
		{
			MessageBox(0,_T("Store Data failed!"),_T("Can't Create files"),IDOK);
			return FALSE;
		}
	}
	return TRUE;
}

void CSunPara::SetPmax(double pm,double vm,double im)
{
	m_Pmax=pm;
	m_Vmp=vm;
	m_Imp=im;

	////���㷽��
	if((m_Voc*m_Isc)!=0)
	{
		m_FF=(m_Vmp*m_Imp)/(m_Voc*m_Isc);         //�Ķ�
	}
}

void CSunPara::SetJsc(double isc,double area)
{
	if (area <= 0)
		return;
	m_Jsc = isc*1000/area;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���ж����������ߵ��ܶȼ�������ʵ�Ч�������ԣ���ֵ��С
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CSunPara::SetPmaxFF(CDoubleXY *p, int n)
{
	if(n==0)n=p->GetDataSize();
	//���㴦�����²���Ϊ���Ա��������Ҫ����
	m_Pmax=0.0;//�����
	m_Vmp=0.0;//��Ӧ�ĵ�ѹ
	m_Imp=0.0;//��Ӧ�ĵ���

	//��������ʵ����Ӧ�ĵ�ѹ����
	UINT pos=0;
	double m_Pmax_temp = 0;
	m_Pmax_temp=p->GetMaxXY(pos,n);//�������ɵ��е����x*y��     //�Ķ�
	m_Vmp=p->x[pos];//��Ӧ�ĵ�ѹ
	m_Imp=p->y[pos];//��Ӧ�ĵ���
	m_Pmax = m_Vmp*m_Imp;
	////���㷽��
	if((m_Voc*m_Isc)!=0)
	{
		m_FF=(m_Vmp*m_Imp)/(m_Voc*m_Isc);         //�Ķ�
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���ݱ任�Ͳ�����ȡ
//pΪ�����㣬rΪ���߱任����㼯
//���е�ldataΪ��Ч���ݳ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CSunPara::TransformerParameter(CDoubleXY *op, CDoubleXY *r)
{
	//����ԭ���ݸ��ģ�����ͼ�������ٴε��������һ��2k701
	//��ԭʼ���ݵı���ת����һ������
	CDoubleXY *p=op ;
	//�������
	//Ԥ��������
	PreTransformer(p);
	//ʹ��de casteljau�㷨�õ������ϵĵ�
	int times=7;//��������̫���Ƽ�15-7֮��
	if(p->GetDataSize()!=0)
	{
		p->BezierCurve(r,p->GetDataSize(),times,10);//�������5-15֮��
		SetPmaxFF (r);
	}
}
//ʹ��ֻ���Pmax����һ�εİ취��Pmax������������
void CSunPara::TransformerParameter(CDoubleXY *op, UINT rge)
{
	PreTransformer(op);
	double v,i,p;
	op->GetPowMax(v,i,p,rge);
	SetPmax(p,v,i);
}
void CSunPara::PreTransformer(CDoubleXY *p)
{
	//��Ч��������
// 	UINT index;
// 	for(index=0;index<p->size();index++)
// 	{
// 		if(p->y[index]<p->y[0]/20.0)//���㴦����С�ڵ�һ�����20��֮һ�����ݶ�����
// 		{
// 			p->SetDataSize(index+1);
// 			p->SetY(0.0,index);
// 			break;
// 		}
// 	}

	UINT index;
	for(index=0;index<p->size();index++)
	{
		if(p->y[index]<0.000001)//���㴦����0������ݶ�����
		{
			p->SetDataSize(index+1);
			break;
		}
	}
	if(index<50)//���ǰ��50����ͳ�����С�ڵ�һ�����20��֮һ�����ݣ����϶���Ч���ݵ���̫��
	{
		p->SetDataSize(0);
		return; 
	}
	if(index==p->size())//ֱ�����һ���㶼û����С�ڵ�һ�����20��֮һ�����ݣ����������һ����
	{
		p->SetDataSize(index);
	}


	double K,B=0;
	int np=30;//����б�ʼ���κͽ�����Ա任�εĵ���,����ֵ���任��ʹ�õ�������400/��4~5��ȡ50��
	int nstart = 10;
	if (theApp.GetSetBase()->GetSetDev841()->GetExternEffCell())
	{
		np = 24;
		nstart = 4;
	}
	K=1.0;
	if(p->TransToLine (K,B,np,nstart))//�ܿ���ǰ��n�������
	{
		m_Rsh=fabs(-1.0/K);
		m_Isc=(B);
		p->Liner(K,B,np,0,1);//x���Ա任�ö�y����0�㿪ʼ�����Ա仯np����
	}

/*
�����ߵ�����ʵ��Ժ�б�ʾ���ֵ���������ʱ���￪·��ѹ��
���¸��ݴ˵㣬�Ż���·���ѹ�㸽��������
*/
	K=1.0;

	UINT startpos=0;//����ʵ�λ��
	p->GetMaxXY(startpos);//�������ɵ��е����x*y��λ��

	double kmax=0;		//б�ʾ���ֵ�仯�����ֵ
	UINT kpos=0;		//б�ʾ���ֵ�仯�����ֵ��λ��
	double mk=1.0,mb=0; 
	UINT ndot=(p->GetDataSize()-startpos)/7;
	if(ndot<5)
		ndot=5;
	if (theApp.GetSetBase()->GetSetDev841()->GetExternEffCell())
			ndot = 2;
	startpos += (p->GetDataSize()-startpos)/2;	//ʹ���������ʵ����һ����Ա�����ǿ����ʱЧ��

	UINT tmpos=0;

	for(UINT n=1;;n++)
	{
		tmpos=startpos+n;
		if(tmpos > (p->GetDataSize()-(ndot)))
			break;
		p->TransToLine (K,B,ndot,tmpos);
		if(kmax*0.999 < fabs(K))//���������������ݶԽ����Ӱ��
		{
			kmax=fabs(K);
			kpos=tmpos;
			mk=K;mb=B;
		}
	}

	p->SetDataSize(kpos+ndot);
	p->Liner(mk,mb,ndot,kpos,0);//y���Ա任�ö�x
	p->SmoothY(kpos-2,kpos+ndot,0);

	m_Rs=fabs(-1.0/mk);
	m_Voc=fabs(-mb/mk);
}

CString CSunPara::GetParaListString(void)
{
	CString str=_T("");
	CString val=_T("");

	val.Format(	_T("Isc :%3.3f\n"),m_Isc);
	str+=val;
	val.Format(	_T("Voc :%3.4f\n"),m_Voc);
	str+=val;
	val.Format(	_T("Pmax:%3.3f\n"),m_Pmax);
	str+=val;
	val.Format(	_T("Imax:%3.3f\n"),m_Imp);
	str+=val;
	val.Format(	_T("Vmax:%3.3f\n"),m_Vmp);
	str+=val;
	val.Format(	_T("FF  :%3.3f\n"),m_FF);
	str+=val;
	val.Format(	_T("Eff :%3.3f\n"),m_Eff);
	str+=val;
	val.Format(	_T("Irev :%3.3f\n"),m_Irev);
	str+=val;
	val.Format(	_T("Rs  :%3.3f\n"),m_Rs);
	str+=val;
	val.Format(	_T("Rsh :%3.3f\n"),m_Rsh);
	str+=val;
	val.Format(	_T("EnviTemp:%3.1f\n"),m_EnvTemp);
	str+=val;
	val.Format(	_T("SurfTemp:%3.1f\n"),m_SurfTemp);
	str+=val;
	val.Format(	_T("ParaTemp:%3.1f\n"),m_Temp);
	str+=val;
	val.Format(	_T("Sun :%3.1f\n"),m_Sun);
	str+=val;
	val.Format(	_T("SunRef :%3.1f\n"),m_SunRef);
	str+=val;
	val.Format(	_T("Serial:%s\n"),m_Serial);
	str+=val;
	m_oledata=m_oledata.GetCurrentTime();
	CString time=m_oledata.Format(_T("%Y-%m-%d"));
	val.Format(	_T("Date:%s\n"),time);
	str+=val;
	time=m_oledata.Format(_T("%X"));
	val.Format(	_T("Times:%s\n"),time);
	str+=val;
	
	return str;
}

void CSunPara::InitialSunPara()
{
	//��ʼ����
	m_FF=0.0;
	m_Isc=0.0;
	m_Voc=0.0;
	m_Imp=0.0;
	m_Vmp=0.0;
	m_Pmax=0.0;
	m_Sun=0.0;
	m_Eff=0.0;	
	m_Rs=0;
	m_Rsh=0;
	m_Serial=_T("");
	m_grade=_T("");
	m_Stime=_T("");
	m_SDirection=_T("");
	m_IrrCali=_T("");
	m_Temp=0.0;	
	m_SurfTemp=0;
	m_EnvTemp=0;
	m_RCTemp=0;
	m_SunRef=0;
	m_Irev=0;
	m_Idot=0;
	m_Pdot=0;
	m_Vdot=0;
	m_StdCellIsc=0;
	m_StdCellIsc1=0;
	m_StdCellIsc2=0;
	m_StdCellsun1=0;     //���Ķ�
	m_StdCellsun2=0;     //���Ķ�
	m_Jsc=0;
	SetDateTime();

}

	