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

//参数计算类
CSunPara::CSunPara()
{
	//初始变量
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
	m_StdCellsun1=0;     //朱峰改动
	m_StdCellsun2=0;     //朱峰改动
	m_Jsc=0;

	SetDateTime();
}
CSunPara::~CSunPara()
{

}

//设置测试环境温度
void CSunPara::SetTemp(double fTemp)
{
	m_Temp=fTemp;
}

//设置测试环境光照度
void CSunPara::SetSun(double fSun)
{
	m_Sun=fSun;
}

void CSunPara::SetSunRef(double fSun)
{
	m_SunRef=fSun;
}
//实际测试功率/输入的光功率=Eff
//1平米单位面积光强1000
//1平米=10000平方厘米
//输入光功率等于1000*（面积（cm2）/10000）
//实测功率Pmax
void CSunPara::EvaluationEff(double area)
{
	//计算Eff
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

//保存参数
//参数：CString strSerial为产品序列号
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
	bopen=file.Open(fileName,CFile::modeWrite);   //判断指定文件是否存在
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
		if(file.Open(fileName,CFile::modeCreate|CFile::modeWrite))//如果文件不存在，创建一个文件
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

	////计算方度
	if((m_Voc*m_Isc)!=0)
	{
		m_FF=(m_Vmp*m_Imp)/(m_Voc*m_Isc);         //改动
	}
}

void CSunPara::SetJsc(double isc,double area)
{
	if (area <= 0)
		return;
	m_Jsc = isc*1000/area;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//进行二次增加曲线点密度计算最大功率点效果不明显，且值减小
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CSunPara::SetPmaxFF(CDoubleXY *p, int n)
{
	if(n==0)n=p->GetDataSize();
	//回零处理，以下参数为类成员变量，需要回零
	m_Pmax=0.0;//最大功率
	m_Vmp=0.0;//对应的电压
	m_Imp=0.0;//对应的电流

	//计算最大功率点和相应的电压电流
	UINT pos=0;
	double m_Pmax_temp = 0;
	m_Pmax_temp=p->GetMaxXY(pos,n);//计算生成点中的最大x*y点     //改动
	m_Vmp=p->x[pos];//对应的电压
	m_Imp=p->y[pos];//对应的电流
	m_Pmax = m_Vmp*m_Imp;
	////计算方度
	if((m_Voc*m_Isc)!=0)
	{
		m_FF=(m_Vmp*m_Imp)/(m_Voc*m_Isc);         //改动
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//数据变换和参数获取
//p为采样点，r为曲线变换结果点集
//其中的ldata为有效数据长度
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CSunPara::TransformerParameter(CDoubleXY *op, CDoubleXY *r)
{
	//避免原数据更改，避免图形数据再次调入产生不一致2k701
	//对原始数据的保护转入上一级程序
	CDoubleXY *p=op ;
	//计算参数
	//预处理数据
	PreTransformer(p);
	//使用de casteljau算法得到曲线上的点
	int times=7;//次数不能太高推荐15-7之间
	if(p->GetDataSize()!=0)
	{
		p->BezierCurve(r,p->GetDataSize(),times,10);//插入点数5-15之间
		SetPmaxFF (r);
	}
}
//使用只拟合Pmax附近一段的办法求Pmax，求其他参数
void CSunPara::TransformerParameter(CDoubleXY *op, UINT rge)
{
	PreTransformer(op);
	double v,i,p;
	op->GetPowMax(v,i,p,rge);
	SetPmax(p,v,i);
}
void CSunPara::PreTransformer(CDoubleXY *p)
{
	//有效点数处理
// 	UINT index;
// 	for(index=0;index<p->size();index++)
// 	{
// 		if(p->y[index]<p->y[0]/20.0)//最后点处理，把小于第一个点的20分之一的数据都舍弃
// 		{
// 			p->SetDataSize(index+1);
// 			p->SetY(0.0,index);
// 			break;
// 		}
// 	}

	UINT index;
	for(index=0;index<p->size();index++)
	{
		if(p->y[index]<0.000001)//最后点处理，把0点的数据都舍弃
		{
			p->SetDataSize(index+1);
			break;
		}
	}
	if(index<50)//如果前面50个点就出现了小于第一个点的20分之一的数据，则认定有效数据点数太少
	{
		p->SetDataSize(0);
		return; 
	}
	if(index==p->size())//直到最后一个点都没出现小于第一个点的20分之一的数据，则舍弃最后一个点
	{
		p->SetDataSize(index);
	}


	double K,B=0;
	int np=30;//用于斜率计算段和结果线性变换段的点数,其他值不变换，使用点数少于400/（4~5）取50点
	int nstart = 10;
	if (theApp.GetSetBase()->GetSetDev841()->GetExternEffCell())
	{
		np = 24;
		nstart = 4;
	}
	K=1.0;
	if(p->TransToLine (K,B,np,nstart))//避开最前边n个点计算
	{
		m_Rsh=fabs(-1.0/K);
		m_Isc=(B);
		p->Liner(K,B,np,0,1);//x线性变换该段y，从0点开始，线性变化np个点
	}

/*
在曲线的最大功率点以后，斜率绝对值逐渐增大，最大时到达开路电压点
以下根据此点，优化开路电电压点附近的曲线
*/
	K=1.0;

	UINT startpos=0;//最大功率点位置
	p->GetMaxXY(startpos);//计算生成点中的最大x*y点位置

	double kmax=0;		//斜率绝对值变化的最大值
	UINT kpos=0;		//斜率绝对值变化的最大值的位置
	double mk=1.0,mb=0; 
	UINT ndot=(p->GetDataSize()-startpos)/7;
	if(ndot<5)
		ndot=5;
	if (theApp.GetSetBase()->GetSetDev841()->GetExternEffCell())
			ndot = 2;
	startpos += (p->GetDataSize()-startpos)/2;	//使起点在最大功率点后面一点可以保留光强不足时效果

	UINT tmpos=0;

	for(UINT n=1;;n++)
	{
		tmpos=startpos+n;
		if(tmpos > (p->GetDataSize()-(ndot)))
			break;
		p->TransToLine (K,B,ndot,tmpos);
		if(kmax*0.999 < fabs(K))//消除少量变异数据对结果的影响
		{
			kmax=fabs(K);
			kpos=tmpos;
			mk=K;mb=B;
		}
	}

	p->SetDataSize(kpos+ndot);
	p->Liner(mk,mb,ndot,kpos,0);//y线性变换该段x
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
	//初始变量
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
	m_StdCellsun1=0;     //朱峰改动
	m_StdCellsun2=0;     //朱峰改动
	m_Jsc=0;
	SetDateTime();

}

	