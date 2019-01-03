// CInOutTipDlg dialog
#pragma  once

class CInOutTipDlg : public CDialog
{
    DECLARE_DYNAMIC(CInOutTipDlg)
public:
    CInOutTipDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CInOutTipDlg();

    // Dialog Data
    enum { IDD = IDD_MCMSG_DLG };

    void ShowMsgWindow(CWnd* pParent, const CString& strTipInfo);
	void Set_TextColor(COLORREF cr)   
	{   
		m_TextColor = cr;  
	}  

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    //响应关闭消息，删除对象
    virtual void OnCancel();
    virtual void PostNcDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    DECLARE_MESSAGE_MAP()

private:
    void InitDlgPosition();

private:
    CString m_strTipInfo;
	COLORREF m_TextColor;
};