#pragma once
//ʹ�� ADO
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//ʹ��ADOX 
#import "C:\Program Files\Common Files\System\ado\msadox.dll" 
/*
#ifdef __DEBUG
#import "C:\Program Files\Common Files\System\ado\msadox.dll" 
#else
#import "msadox.dll" 
#endif
*/
/*
TINYINT 1 �ֽ� 0 �� 255 ֮�������ֵ�� 
MONEY 8 �ֽ� ���ڨC 922,337,203,685,477.5808 �� 922,337,203,685,477.5807 ֮���С���� 
DATETIME
FLOAT 8 �ֽ� ˫���ȸ���ֵ���䷶ΧΪ �C1.79769313486232E308 �� �C4.94065645841247E-324����ֵ����4.94065645841247E-324 �� 1.79769313486232E308����ֵ���� 0�� 
INTEGER 4 �ֽ� �C2,147,483,648 �� 2,147,483,647 ֮��ĳ������� 
TEXT ÿ���ַ� 2 �ֽڣ�����ġ�ע�͡��� �㵽��� 2.14 GB�� 
CHARACTER ÿ���ַ� 2 �ֽڣ�����ġ�ע�͡��� �㵽 255 ���ַ��� 
*/
//ֱ��ִ��SQL���,����Ҫ�õ����������Ĵ󲿷ֹ��ܶ�����ֱ����SQL����ʵ�� 
class DB
{
public:
	DB(void);
	//�������ݿ�
	BOOL CreateDatabase(CString dbPath,CString pass);
	//���ӱ�	//�����ֶ�
	void CreateTable(CString dbPath,CString sqlcmd);
	void AppendItem(CString dbPath,CString sqlcmd);
	//ִ��SQL���
	void ExeSql(CString dbPath,CString sqlcmd,CString pass);
    CString GetFields(CString dbPath,CString sqlcmd,CString pass, CString *spField);
	//д��һ����¼
	~DB(void);
};
