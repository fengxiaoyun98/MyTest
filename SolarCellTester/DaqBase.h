#pragma once

#include "SunAds\CurveIV.h"

//class CDevLamp;//模拟器的控制，包含CDevRefCell参考电池] 
//class CDevLoad;//电子负载的控制] 
//class CDevTemp;用来测量表面温度测温装置] 
//硬件抽象层
//多功能数据采集卡的硬件接口
//可以是DAQ卡、单片机(F005/841)、其他通信方式的采集器等

class DaqBase
{
public:
	DaqBase(void);
	virtual ~DaqBase(void);
public:
	//exmsg是要检测的内容定义，返回相应码
	enum EXTERNEVENT{KEYACTOR=0,ONLINE,LOGIN};

	virtual UINT CheckExternEvent(UINT exmsg)=0;
	//单次测试IV曲线，给出(I,V)序列
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq=0)=0;
	//单次测试IV曲线，曲线段由vol给定
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol)=0;
	//单次测试IV曲线，曲线段由vol给定
	virtual INT_PTR TestingRun(CurveIV *const iv,unsigned short gq,double vol,UINT step)=0;
	//其他可用的独立测试功能
	//ARM采集函数

	virtual INT_PTR ARMTestingRun(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short gq,unsigned short cmd) = 0;
public:
	CString GetModel(){return m_model;}
	CString GetName(){return m_name;};
	void SetName(CString n){ m_name=n;};
private:
	void SetModel(CString m){m_model=m;}
	//设备信息：设备类型//生产年月//设备编号//设备功能表
	COleDateTime m_date;
	CString m_model;
	CString m_name;
};

