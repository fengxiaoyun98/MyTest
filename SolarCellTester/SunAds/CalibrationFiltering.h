//
//�����ṩIV���ߵ�������У׼
//m_set�ṩ��������ȡ��һЩ������ϵ��
//
#pragma once
#include "..\Options\Setbase.h"
#include "..\Options\SetSunPara.h"
#include "DoubleXY.h"
#include "..\Options\SetCurveIV.h"
class CalibrationFiltering
{
public:
	CSetBase* m_setbase;//����ҳ
	CSetSunPara* m_setcal;//У׼����
	CSetCurveIV* m_setiv;//���ݴ���
public:
	virtual void CalFilter(CDoubleXY*const obj);
	CalibrationFiltering(void);
	virtual ~CalibrationFiltering(void);
};
