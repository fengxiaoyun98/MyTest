#pragma once
#include <vector>
#include <math.h>
#include "float.h"
//�����飬����MultiMap
//����Ӳ����ѹ��������λ����
class GradePair{
	double m_scale;
	CString m_title;
	byte m_grade;
public:
	GradePair(double sc,CString str,byte gd){Set(sc,str,gd);}
	void Set(double sc,CString str,byte gd){m_scale=sc;m_title=str;m_grade=gd;}
	CString GetTitle(void)const{return m_title;}
	double GetScale(void)const{return m_scale;}
	byte GetGrade(void)const{return m_grade;}
};

typedef std::vector<GradePair>__GradePairList;

class GradePairList:public __GradePairList
{
public:
	CString TitleAt(UINT pos){return at(pos).GetTitle();}
	double ScaleAt(UINT pos){return at(pos).GetScale();}
	byte GradeAt(UINT pos){return at(pos).GetGrade();}
};

//����ӳ����
//���ڹ�ǿ����ʾ�������κ�Ӳ����������ӳ��
//�����趨ȱʡ����
class AreaMapping
{
	double m_bmax;
	double m_bmin;
	double m_amax;
	double m_amin;
	double m_scale;
	double m_defalut;
public:
	double getdefalut(void)const{return m_defalut;}
	double getamin(void)const{return m_amin;}
	double getamax(void)const{return m_amax;}
	double getbmin(void)const{return m_bmin;}
	double getbmax(void)const{return m_bmax;}
	AreaMapping(void);
	AreaMapping(double a0,double a1,double b0,double b1,double defalut);
	bool SetPair(double a0,double a1,double b0,double b1,double defalut);
	bool atob(double& a);
	bool btoa(double& b);
	bool equ(double a,double b){return fabs(a-b)<DBL_EPSILON;}
	~AreaMapping();
};
//���ں㶨��ǿʱ�����趨
class SunLenPair{
	CString m_title;
	byte m_delay;
public:
	SunLenPair(CString str,byte gd){Set(str,gd);}
	void Set(CString str,byte gd){m_title=str;m_delay=gd;}
	CString GetTitle(void)const{return m_title;}
	byte GetDelay(void)const{return m_delay;}
};

typedef std::vector<SunLenPair>__SunLenPairList;

class SunLenPairList:public __SunLenPairList
{
public:
	CString TitleAt(UINT pos){return at(pos).GetTitle();}
	byte DelayAt(UINT pos){return at(pos).GetDelay();}
};
