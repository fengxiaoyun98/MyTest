#pragma once

class ActorPrint
{
	CString wndtitle;//´°¿ÚÃû³Æ±í
	CWnd wnd;
public:
	ActorPrint();
	void SetTitle(CString title);
	~ActorPrint(void);
	void SendCmdMsg(unsigned long id);
	void SendCmdMsg(unsigned long x,unsigned long y);
	void Ctrl_P();
	HWND GetWnd();
	BOOL IsWnd(void);
	void ReWindow(void);
	void Print(void);
};
