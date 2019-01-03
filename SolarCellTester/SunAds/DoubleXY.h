// DoubleXY.h: interface for the CDoubleXY class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>

typedef std::vector<double>CDoubleArray;

class CDoubleXY 
{
private:
	void test(void);
	BOOL CalculateCurveParameter(CDoubleArray *X,CDoubleArray *Y,UINT M,UINT N,CDoubleArray *A);
private:
	UINT ldata; //有效数据长度
	CString GetFileName(BOOL o);
public:
	void GetPowMax(double &v,double &i,double &pm,UINT rge=0);
	double GetPowYAtX(UINT power,double atx,UINT rge=0);
	void ToNPowXY(UINT power,UINT p1=0,UINT p2=0);
	double EvaluateX(double they);
	double EvaluateY(double thex);
	BOOL DoSeries(CString filename=_T(""),BOOL o=1);
	void BezierContinue(CDoubleXY *r,UINT n,UINT times,UINT inpd=5);
	void Liner(double k,double b,UINT n,UINT s,BOOL ch);
	void SmoothY(UINT s,UINT n,BOOL d);
	UINT GetYAtX(double &val);
	UINT GetXAtY(double &val);
	void SetZero(void);
	void SetY(double vx,UINT n);
	void SetX(double vy,UINT n);
	void SetX(double base,double step,UINT s=0,UINT n=0);
	double GetY(UINT n);
	double GetX(UINT n);
	BOOL TransToLine(double &k,double &b, UINT n=0,UINT s=0);
	double GetMaxXY(UINT &pos,UINT len=0);
	CDoubleXY(UINT nNewSize);
	CDoubleXY(void);
	void MultiXYToY(void);
	void ReSize(UINT nNewSize, UINT rSize=0);
public:
	void InitialXY();
	void ReturnZeroX(void);
	void MoveBlock(UINT ds,UINT ss,UINT len=0);
	void BezierCurve(CDoubleXY *r, UINT n, UINT times, UINT inpd=5);
	void ForBezierMut(UINT i,double mut);
	UINT GetDataSize(void){return ldata;}
	void SetDataSize(UINT ld){ldata=ld;}
	//UINT l;   //xl=yl可容纳数据长度
	//l被x.size(),x.capacity(),x.maxsize()替代
	UINT size(void){return (UINT)x.size();}
	CDoubleArray x;
	CDoubleArray y;
	void EquB(CDoubleXY *b);
	void EquB(CDoubleXY *b,UINT n);
	void EquB(CDoubleXY *b,UINT an,UINT bn,UINT n=0);
	void EquB(double ix,double iy,UINT i);
	void MUT(UINT i,double b);
	void AND(CDoubleXY *b,UINT ai,UINT bi);
	BOOL Bezier(CDoubleXY *r,UINT n,UINT s,UINT mn,UINT rn=0);
	UINT BPlus(CDoubleXY *r,UINT n,UINT times,UINT inpd=5);
	virtual ~CDoubleXY();
#ifdef _DEBUG
#endif
	UINT MultiXAny(double scale);
	UINT MultiXAny(double Vscale,double Pscale,double TIsc,double TVoc); //改动
	UINT MultiYAny(double scale);
	double GetAverage(bool aix);
	double GetMax(bool aix);
	double GetMin(bool aix);
};

