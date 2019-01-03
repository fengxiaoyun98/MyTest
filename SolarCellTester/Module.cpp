#include "StdAfx.h"
#include "Module.h"

Module::Module(void)
{
	name=_T("Module");
}

Module::~Module(void)
{
}
void Module::Position(Module main)//register,insert|append|replace a module in list
{
}
void Module::HistoryWafering(CFile *p,bool sw)
{
}
void Module::ApplySetList(std::vector<std::wstring>list)//应用设置表,""表示允许，对应权限表顺序不变
{
}
std::vector<std::wstring> Module::PowerList(void)
{
	std::vector<std::wstring> stra;
	return stra;
}
