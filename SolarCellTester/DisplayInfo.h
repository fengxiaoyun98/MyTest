//
	//�����     �Ķ�   ͸������
	//m_dispBar.SetMsgFont(36);�߶Ⱥ���������
	//m_dispBar.SetTransparency(int scale);͸����
	//m_dispBar.SetStyle��ʽ���ƶ��ı�ߴ磬�̶��ޱ߿�
//
#pragma once
class CRegistry;
// CDisplayInfo dialog

class CDisplayInfo : public CDialog
{
	DECLARE_DYNAMIC(CDisplayInfo)

public:
	void SetBoardColor(CDC* pDC,COLORREF bcolor);
	void DrawTextRgn(CDC* pDC,CRect& rect,CStringArray* sta);
	void DisplayMessage(CStringArray* msglist);
	void AddString(CString item);
	void ClearBoard(int at=-1);
	void SetMsgFont( int high,CString fontname=_T(""));
	CDisplayInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayInfo();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_FLOATDISPLAY };

protected:
	CString m_classname;
	CString m_wndstatestr;
	CRegistry reg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CFont m_fnt;
	CStringArray m_list;

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnInitDialog(UINT wParam,LONG lParam) ;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);


};
