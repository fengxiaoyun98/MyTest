/*
�ṩ��CObject���ƵĻ������ͣ���list��Ϊ����
�ô˻���������չ��ϵͳ���������ص㣺
1��������չ���Ϳ��Բ��롢�滻����չ����ϵ�ɾ�̬�Ͷ�̬��չ
2���ṩһЩ���������ԣ�Log����Demo����Ȩ�޷���
ʵ�֣�
1��ȫ�ֱ�������ͨ��theApp���������оֲ�ģ�飬ʵ��ȫ�ֱ���
2��Ȩ�޶��ƣ�ģ������ṩ���Ʊ���˳��ģ����Ʊ������ñ�С�ڶ��Ʊ����ñ��б�ŵĹ��ܱ���ֹ����ʧЧ

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
	//��λģ��λ�ã����Զ�λ��Module���е��κ�λ��
	virtual void Position(Module main);//register,insert|append|replace a module in list
	virtual void HistoryWafering(CFile *p,bool sw);//��ʷ��Ƭ
	virtual void ApplySetList(std::vector<std::wstring>list);//Ӧ�����ñ�,""��ʾ������ӦȨ�ޱ�˳�򲻱�
	virtual std::vector<std::wstring> PowerList(void);
/*
	//Ȩ�޹���
	virtual void LimitedObject();//�޶���ʹ�õĶ���
	virtual void LimitedCustom();//�޶��ɽ��ܵĿͻ�
	//Log
	virtual std::string logger();//���м�¼
	//Demo & Tech
	virtual void DemoTech();
*/
};
