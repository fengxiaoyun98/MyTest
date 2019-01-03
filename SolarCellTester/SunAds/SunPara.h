// SunPara.h: interface for the CSunPara class.
//
//////////////////////////////////////////////////////////////////////

#include "DoubleXY.h"
#pragma once

///////////////////////////////////////////
//时间：2002.2.13
//功能：太阳能电池参数计算类
//
//太阳电池电参数表和一些附加属性
///////////////////////////////////////////

class CSunPara  
{
public:
	CSunPara();
	virtual ~CSunPara();
	BOOL SaveSunParam(CString fileName);
	BOOL SaveSunParam(CString strSerial,CString fileName);
	void PreTransformer(CDoubleXY *p);
	void TransformerParameter(CDoubleXY *p,UINT rge=0);
	void TransformerParameter(CDoubleXY *p,CDoubleXY *r);
	void EvaluationEff(double area);
public://Set
	void InitialSunPara();
	void SetVmax(double vm){m_Vmp=vm;}
	void SetSerial(CString ser){m_Serial=ser;}
	void SetPmax(double pm,double vm,double im);
	void SetPmaxFF(CDoubleXY *p,int n=0);
	void SetTemp(double fTemp);
	void SetSunRef(double fSun);//设置测试环境光照度
	void SetSun(double fSun);//设置测试环境光照度

	void SetStdCellIscTesting(double si){m_StdCellIsc=si;}

	void SetStdCellIscTesting1(double si1){m_StdCellIsc1=si1;}
	void SetStdCellIscTesting2(double si2){m_StdCellIsc2=si2;}

	void SetStdCellsunTesting1(double si1){m_StdCellsun1 =si1;}            //朱峰改动
	void SetStdCellsunTesting2(double si2){m_StdCellsun2 =si2;}            //朱峰改动


	void SetSurfTemp(double t){m_SurfTemp=t;}//表面温度
	void SetEnvTemp(double t){m_EnvTemp=t;}//设置测试环境温度
	void SetRCTemp(double t){m_RCTemp=t;}

	void SetSTime(CString st){m_Stime=st;}
	void SetSDirection(CString SDt){m_SDirection=SDt;}
	void SetIrrCali(CString Sirr){m_IrrCali=Sirr;}

	void SetIrev(double t){m_Irev=t;}
	void SetEff(double fEff);
	void SetRs(double rs){m_Rs=rs;};
	void SetVdot(double dot){m_Vdot=dot;};
	void SetPdot(double dot){m_Pdot=dot;};
	void SetIdot(double dot){m_Idot=dot;SetPdot(m_Idot*m_Vdot);};
	void SetDateTime(void){m_oledata=COleDateTime::GetCurrentTime();}
	void SetGrade(CString gs){m_grade=gs;}
	void SetJsc(double isc,double area);
public://Get
	CString GetGrade(void){return m_grade;}
	COleDateTime* GetDateTime(void){return &m_oledata;}
	CString GetSerial(void)const{return m_Serial;}
	double GetPdot(void)const{return m_Pdot;}
	double GetVdot(void)const{return m_Vdot;}
	double GetIdot(void)const{return m_Idot;}
	double GetEff(void)const{return m_Eff;}
	double GetFF(void)const{return m_FF;}
	double GetRs(void)const{return m_Rs;}
	double GetRsh(void)const{return m_Rsh;}
	double GetImax(void)const{return m_Imp;}
	double GetVmax(void)const{return m_Vmp;}
	double GetPmax(void)const{return m_Pmax;}
	double GetIsc(void)const{return m_Isc;}
	double GetVoc(void)const{return m_Voc;}
	double GetSun(void)const{return m_Sun;}
	double GetSunRef(void)const{return m_SunRef;}

	double GetStdCellIscTesting(void)const{return m_StdCellIsc;}

	double GetStdCellIscTesting1(void)const{return m_StdCellIsc1;}
	double GetStdCellIscTesting2(void)const{return m_StdCellIsc2;}

	double GetStdCellsunTesting1(void)const{return m_StdCellsun1;}                                  //朱峰改动
	double GetStdCellsunTesting2(void)const{return m_StdCellsun2;}                                  //朱峰改动


	double GetEnvTemp(void)const{return m_EnvTemp;}
	double GetRCTemp(void)const{return m_RCTemp;}

	CString GetSTime(void)const{return m_Stime;}
	CString GetSDirection(void)const{return m_SDirection;}
	CString GetIrrCali(void)const{return m_IrrCali;}

	double GetSurfTemp(void)const{return m_SurfTemp;}
	double GetIrev(void)const{return m_Irev;}
	double GetTemp(void)const{return m_Temp;}
	double GetJsc(void)const{return m_Jsc;}
	CString GetParaListString(void);   
private:
	CString m_grade;
	COleDateTime m_oledata;
	CString m_Serial;
	CString m_Stime;
	CString m_SDirection;
	CString m_IrrCali;
	double m_Eff;
	double m_Rsh;
	double m_Rs;
	double m_FF;//方度=(m_Vmp*m_Imp)/(m_Voc*m_Isc)
	double m_Isc;//短路电流
	double m_Voc;//开路电压
	double m_Imp;//最大功率点电流
	double m_Vmp;//最大功率点电压
	double m_Pmax;//最大功率点
	double m_Temp;//参数温度
	double m_EnvTemp;//环境温度
	double m_SurfTemp;//表面温度
	double m_RCTemp;
	double m_Sun;//光照度
	double m_SunRef;//参考电池电流
	double m_Irev;//反向漏电流
	double m_Idot;//定点值
	double m_Vdot;
	double m_Pdot;
	double m_StdCellIsc;//标准参考电池的实测Isc
	double m_StdCellIsc1;
	double m_StdCellIsc2;
	double m_StdCellsun1;     //朱峰改动
	double m_StdCellsun2;     //朱峰改动
	double m_Jsc;             //电流密度
private:
	CString fstr(double val){CString str;str.Format(_T("%f,"),val);return str;};
};

