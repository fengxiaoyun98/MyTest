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
void Module::ApplySetList(std::vector<std::wstring>list)//Ӧ�����ñ�,""��ʾ������ӦȨ�ޱ�˳�򲻱�
{
}
std::vector<std::wstring> Module::PowerList(void)
{
	std::vector<std::wstring> stra;
	return stra;
}
