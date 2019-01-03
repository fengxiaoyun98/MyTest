/*
 *
 *	ChartObject.h
 *
 *	Written by C�dric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *
 */
#pragma once

#include <afx.h>			
#include <afxwin.h>

class CChartView;

class CChartObject  
{
	friend CChartView;

public:
	CChartObject(CChartView* pOwner);
	virtual ~CChartObject();

	void SetVisible(bool bVisible);
	bool IsVisible()  const         { return m_bIsVisible; }

	COLORREF GetColor() const			   { return m_ObjectColor; }
	void SetColor(COLORREF NewColor);
	COLORREF GetShadowColor() const		   { return m_ShadowColor; }
	void SetShadowColor(COLORREF NewColor);

	void EnableShadow(bool bEnable);
	void SetShadowDepth(int Depth);

	bool Clip(int& dX, int& dY)  const
	{
		bool bResult = true;

		if (dX>m_ObjectRect.right) 
		{
			dX = m_ObjectRect.right;
			bResult = false;
		}
		if (dX<m_ObjectRect.left)
		{
			dX = m_ObjectRect.left;
			bResult = false;
		}
		if (dY>m_ObjectRect.bottom)
		{
			dY = m_ObjectRect.bottom;
			bResult = false;
		}
		if (dY<m_ObjectRect.top)
		{
			dY = m_ObjectRect.top;
			bResult = false;
		}

		return bResult;
	}



protected:
    void SetRect(CRect NewTect)  {  m_ObjectRect = NewTect; }
    
	CRect		m_ObjectRect;		// Size of the object
	COLORREF	m_ObjectColor;		// Color of the objet

	CChartView*	m_pParent;			// Owner of the object
	bool        m_bIsVisible;
	bool		m_bShadow;			// Specifies if the object generate a shadow.
									// This is not supported for all objects.
	COLORREF	m_ShadowColor;
	int			m_iShadowDepth;

private:
	virtual void Draw(CDC* pDC) = 0;
};

