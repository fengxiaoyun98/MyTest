#include "StdAfx.h"
#include "MIVView.h"


CMIVView::CMIVView(void)
{
}

CMIVView::~CMIVView(void)
{
}

void CMIVView::OnDraw(CDC* pDC)      // overridden to draw this view
{
	CChartView::OnDraw(pDC);
}
void CMIVView::AddIV(float *x,float *y,int cnt)
{
	if(cnt<=0)return;

	CChartLineSerie* pIV = (CChartLineSerie*)AddSerie(CChartSerie::stLineSerie);
	pIV->SetSeriesOrdering(CChartSerie::soNoOrdering);
	GetBottomAxis()->SetMinMax(0,0.7);
	GetLeftAxis()->SetMinMax(0,11);
	for(;cnt--;){
		pIV->AddPoint(*x,*y);
		x++,y++;
	}
}
