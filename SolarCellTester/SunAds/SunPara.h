// SunPara.h: interface for the CSunPara class.
//
//////////////////////////////////////////////////////////////////////

#include "DoubleXY.h"
#pragma once

///////////////////////////////////////////
//ʱ�䣺2002.2.13
//���ܣ�̫���ܵ�ز���������
//
//̫����ص�������һЩ��������
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
	void SetSunRef(double fSun);//���ò��Ի������ն�
	void SetSun(double fSun);//���ò��Ի������ն�

	void SetStdCellIscTesting(double si){m_StdCellIsc=si;}

	void SetStdCellIscTesting1(double si1){m_StdCellIsc1=si1;}
	void SetStdCellIscTesting2(double si2){m_StdCellIsc2=si2;}

	void SetStdCellsunTesting1(double si1){m_StdCellsun1 =si1;}            //���Ķ�
	void SetStdCellsunTesting2(double si2){m_StdCellsun2 =si2;}            //���Ķ�


	void SetSurfTemp(double t){m_SurfTemp=t;}//�����¶�
	void SetEnvTemp(double t){m_EnvTemp=t;}//���ò��Ի����¶�
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

	double GetStdCellsunTesting1(void)const{return m_StdCellsun1;}                                  //���Ķ�
	double GetStdCellsunTesting2(void)const{return m_StdCellsun2;}                                  //���Ķ�


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
	double m_FF;//����=(m_Vmp*m_Imp)/(m_Voc*m_Isc)
	double m_Isc;//��·����
	double m_Voc;//��·��ѹ
	double m_Imp;//����ʵ����
	double m_Vmp;//����ʵ��ѹ
	double m_Pmax;//����ʵ�
	double m_Temp;//�����¶�
	double m_EnvTemp;//�����¶�
	double m_SurfTemp;//�����¶�
	double m_RCTemp;
	double m_Sun;//���ն�
	double m_SunRef;//�ο���ص���
	double m_Irev;//����©����
	double m_Idot;//����ֵ
	double m_Vdot;
	double m_Pdot;
	double m_StdCellIsc;//��׼�ο���ص�ʵ��Isc
	double m_StdCellIsc1;
	double m_StdCellIsc2;
	double m_StdCellsun1;     //���Ķ�
	double m_StdCellsun2;     //���Ķ�
	double m_Jsc;             //�����ܶ�
private:
	CString fstr(double val){CString str;str.Format(_T("%f,"),val);return str;};
};

