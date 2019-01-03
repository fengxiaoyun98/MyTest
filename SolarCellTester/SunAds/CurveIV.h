/*
�̳���CSunPara��CDoubleXY��CalibrationFiltering
ʵ�ֹ������ݵ��м䴦����̣���֯������������ݹ�������
������CurveIV������������MultiCurveIV
*/
#pragma once

#include <vector>
#include "DoubleXY.h"
#include "SunPara.h"
#include "CalibrationFiltering.h"
#include "..\Sqlsave.h"    //�Ķ�


class CurveIV:public CDoubleXY,public CalibrationFiltering,public CSunPara
{
public:
	void InitialIV();
	BOOL GetEnableLoop(void);
	CString GetCustomParaListString(bool flag=false);
	int InputBoxSerialRun(int pos=0);
	void SaveJpg(CWnd* pWnd=NULL);
	CurveIV(int size);
	CurveIV(void);
	~CurveIV(void);
	void CalSunPara(void);
	void SaveParaRun(void);
	void EnvalRs(CurveIV* iv800);
	void EnvalPmax(CDoubleXY* maxiv,UINT rge);

   
public:
	void PrtLabel(void);
	void SavePara(void);
	void SaveParaCSV(void);
	void SaveParaMDB(void);
	void AlterMDB(void);
	void AlterMDBCLass(void);
	void AlterSQL(void);
	void SaveParaXLS(void);
	void SaveParaSQLServer(void);                       //�Ķ�

	CString GetExcelDriver(void);

	CString GetParaFormat(CString sPara);
	void SaveParaInClass(void);
	void SaveDaq(CString filename);

	int GetArrayFromString(CString str, CString *pArray);
	
	virtual void CalFilter(CDoubleXY*const obj);
	void FilteringTemp(CDoubleXY*const obj);
	void FilteringSun(CDoubleXY*const obj);
	void FilteringElec(CDoubleXY*const obj);
	void FilteringElec1(CDoubleXY*const obj);
	BOOL FilteringParaAlarm(void);
	BOOL FilteringParaAlarmTmod(void);
public:
	Sqlsave m_db;
};
//allocator���ڱ����Զ��ڴ����ʱ��ʹ�ã�һ��ʹ��Ĭ�Ϸ���������
typedef std::vector <CurveIV> __MultiCurveIV;

class MultiCurveIV :public __MultiCurveIV
{
public:
	MultiCurveIV(void);
	~MultiCurveIV(void);
private:
	MultiCurveIV::iterator iter;
};
