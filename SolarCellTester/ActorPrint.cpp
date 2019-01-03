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
    //窗口是否可视
    if (!IsWindowVisible(hwnd))
        return TRUE;

    //窗口是否可激活
    if (!IsWindowEnabled(hwnd))
        return TRUE;

    //窗口是否 WS_POPUP 与 WS_CAPTION 共存
        //一些可切换的窗体同时具有 WS_POPUP 与 WS_CAPTION，因而有 WS_POPUP 却无 WS_CAPTION 的应被过滤
    //据 Spy++ 观察，符合如 OneNote TrayIcon 等程序可通过此方式过滤
    LONG gwl_style = GetWindowLong(hwnd,GWL_STYLE);
    if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION))
        return TRUE;

    //窗口是否具有父窗口？
    HWND hParent = (HWND)GetWindowLong(hwnd,GWL_HWNDPARENT);
    //父窗口是否可激活？
    //据 Spy++ 观察，如“运行”对话框等被应列入列表的程序有一个隐藏的，具有 WS_DISABLED 的父窗口
    if (IsWindowEnabled(hParent))
        return TRUE;
    //父窗口是否可视？
    if (IsWindowVisible(hParent))
        return TRUE;

    //一个非常奇怪的问题在于，任务栏 Shell_TrayWnd 符合上述过滤条件但是无法被过滤。
        //因而在这里单独列出。
    TCHAR szClassName[50];

	GetWindowText(hwnd,szClassName,50);
	CString str=_T("BarTender");
	CString str1=szClassName;
	str1=str1.Left(str.GetLength());
	int ret=str.Compare(str1);
	if(ret==0){
		gwnd=hwnd;
        return 0;//找着了
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
	SendCmdMsg(6,3);//更新数据
	SendCmdMsg(0x1E108);//直接打印
}