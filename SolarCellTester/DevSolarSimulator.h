/*
设备管理，为系统提供测试服务，提供测试数据
包括红外温度等设备
*/
#pragma once
#include "device\serial.h"
#include "sunads\CurveIV.h"
#include "device\IR485.h"
#include <list>
#include <vector>
#include "DaqBase.h"

typedef	std::list <DaqBase*> __DaqList;
class MultiDAQ:public __DaqList //数据采集器管理
{
public:
	iterator m_iter;
	//CSerial m_com;
	enum DEVTYPE{ M841A,M005A,PCI6250,USB6250,DIODEMODEL};
public:
	MultiDAQ(void);
	virtual ~MultiDAQ(void);
	void AddDev(DaqBase* pdev);
	void DeletDev(CString name);
	void ConfigDev(void);//配置设备，初始化，加入设备列表，设定当前设备
};

class DevSolarSimulator : 
	public MultiDAQ
{
public:
	void SurfTemp(CurveIV*const iv);
	IR485 m_irTemp;
	//发往外部事件定义
	enum ENVENT{
		PUSHSTARTKEY=1
	};
public:
	INT_PTR GetIV(CurveIV*const iv);//获取IV序列，带Ire
	INT_PTR GetIVAtSun(CurveIV*const iv,unsigned short sun);
	INT_PTR GetIVAtPmax(CurveIV*const iv,unsigned short sun,double vol);
	INT_PTR GetISetV(CurveIV*const iv,unsigned short sun,double vol);//定电压测试电流

	INT_PTR GetARMDATA(CurveIV*const iv,CurveIV*const irradiance,/*CurveIV*const suns_voc,*/unsigned short sun,unsigned short cmd);

	DevSolarSimulator(void);
	~DevSolarSimulator(void);
//定点测试，定点段测试

/*
多曲线测试，弱光，和多次曲线叠加，在保存数据（IV和Para）上有些不同
RS两曲线测试，800光强和1000光强测试，重新计算RS
Pmax测试第一次标准光强，第二次聚焦最大功率点测试，重新计算Pmax，Im，Vm
*/
	bool DevKeyTriggleCheck(void);
};
