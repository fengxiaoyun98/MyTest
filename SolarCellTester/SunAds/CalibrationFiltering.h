//
//本例提供IV曲线的修正和校准
//m_set提供交互设置取得一些常量和系数
//
#pragma once
#include "..\Options\Setbase.h"
#include "..\Options\SetSunPara.h"
#include "DoubleXY.h"
#include "..\Options\SetCurveIV.h"
class CalibrationFiltering
{
public:
	CSetBase* m_setbase;//设置页
	CSetSunPara* m_setcal;//校准修正
	CSetCurveIV* m_setiv;//数据处理
public:
	virtual void CalFilter(CDoubleXY*const obj);
	CalibrationFiltering(void);
	virtual ~CalibrationFiltering(void);
};
