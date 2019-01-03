#include "StdAfx.h"
#include "ActorPrint.h"
HWND gwnd=0;
ActorPrint::ActorPrint( )
{
}
BOOL ActorPrint::IsWnd(void)
{
	return ::IsWindow(gwnd);
}
void ActorPrint::ReWindow(void)
{
	::PostMessage(gwnd,WM_SYSCOMMAND,SC_RESTORE,0);
}
void ActorPrint::SetTitle(CString title)
{
	wndtitle=title;
}

ActorPrint::~ActorPrint(void)
{
}
void ActorPrint::SendCmdMsg(unsigned long id)
{
	::PostMessage(gwnd,WM_COMMAND,id,0);
//	::SendMessage(gwnd,WM_COMMAND,id,0);
}
void ActorPrint::SendCmdMsg(unsigned long x,unsigned long y)
{
     HMENU m=GetMenu(gwnd);
	 HMENU n=GetSubMenu(m,x);
	 MENUITEMINFO info;
	 ::GetMenuItemInfo(n,1,1,&info);
     unsigned long id=0;
	 id=GetMenuItemID(n,y);
	 SendCmdMsg(id);
}
void ActorPrint::Ctrl_P()
{
	SetForegroundWindow(gwnd);
	keybd_event(17,MapVirtualKey(17,0),0,0);
	keybd_event(80,MapVirtualKey(80,0),0,0);
	keybd_event(17,MapVirtualKey(17,0),KEYEVENTF_KEYUP,0);
	keybd_event(80,MapVirtualKey(80,0),KEYEVENTF_KEYUP,0);
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
    //�����Ƿ����
    if (!IsWindowVisible(hwnd))
        return TRUE;

    //�����Ƿ�ɼ���
    if (!IsWindowEnabled(hwnd))
        return TRUE;

    //�����Ƿ� WS_POPUP �� WS_CAPTION ����
        //һЩ���л��Ĵ���ͬʱ���� WS_POPUP �� WS_CAPTION������� WS_POPUP ȴ�� WS_CAPTION ��Ӧ������
    //�� Spy++ �۲죬������ OneNote TrayIcon �ȳ����ͨ���˷�ʽ����
    LONG gwl_style = GetWindowLong(hwnd,GWL_STYLE);
    if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION))
        return TRUE;

    //�����Ƿ���и����ڣ�
    HWND hParent = (HWND)GetWindowLong(hwnd,GWL_HWNDPARENT);
    //�������Ƿ�ɼ��
    //�� Spy++ �۲죬�硰���С��Ի���ȱ�Ӧ�����б�ĳ�����һ�����صģ����� WS_DISABLED �ĸ�����
    if (IsWindowEnabled(hParent))
        return TRUE;
    //�������Ƿ���ӣ�
    if (IsWindowVisible(hParent))
        return TRUE;

    //һ���ǳ���ֵ��������ڣ������� Shell_TrayWnd ���������������������޷������ˡ�
        //��������ﵥ���г���
    TCHAR szClassName[50];

	GetWindowText(hwnd,szClassName,50);
	CString str=_T("BarTender");
	CString str1=szClassName;
	str1=str1.Left(str.GetLength());
	int ret=str.Compare(str1);
	if(ret==0){
		gwnd=hwnd;
        return 0;//������
	}
	return TRUE;
}

HWND ActorPrint::GetWnd()
{
	gwnd=NULL;
	::EnumWindows(EnumWindowsProc,0);
	return gwnd;
}
void ActorPrint::Print(void)
{
	SendCmdMsg(6,3);//��������
	SendCmdMsg(0x1E108);//ֱ�Ӵ�ӡ
}