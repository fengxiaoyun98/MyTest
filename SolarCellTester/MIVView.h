#pragma once
#include "..\ChartView\ChartView.h"

class CMIVView :
	public CChartView
{
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	void AddIV(float *x,float *y,int cnt);
	CMIVView(void);
	~CMIVView(void);
};
