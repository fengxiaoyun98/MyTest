/*
提供与CObject类似的基本类型，以list作为基础
用此基本类型扩展的系统具有以下特点：
1、各个扩展类型可以插入、替换、扩展，体系可静态和动态扩展
2、提供一些基本的特性：Log服务，Demo服务，权限服务
实现：
1、全局变量，或通过theApp来连接所有局部模块，实现全局遍历
2、权限定制：模块可以提供定制表（按顺序的：名称表），设置表小于定制表，设置表有编号的功能被禁止或者失效

*/

#pragma once
#include <list>
#include <vector>
#include <string>

using namespace std;

class Module
{
public:
	Module(void);
	virtual ~Module(void);
private:
	std::wstring name;
public:
	std::list<Module*> p;//ip list
	void setname(std::wstring newname){name=newname;};
	std::wstring getname(void){return name;};
public:
	//定位模块位置，可以定位于Module链中的任何位置
	virtual void Position(Module main);//register,insert|append|replace a module in list
	virtual void HistoryWafering(CFile *p,bool sw);//历史切片
	virtual void ApplySetList(std::vector<std::wstring>list);//应用设置表,""表示允许，对应权限表顺序不变
	virtual std::vector<std::wstring> PowerList(void);
/*
	//权限管理
	virtual void LimitedObject();//限定可使用的对象
	virtual void LimitedCustom();//限定可接受的客户
	//Log
	virtual std::string logger();//运行记录
	//Demo & Tech
	virtual void DemoTech();
*/
};
