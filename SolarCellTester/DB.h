#pragma once
//使用 ADO
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//使用ADOX 
#import "C:\Program Files\Common Files\System\ado\msadox.dll" 
/*
#ifdef __DEBUG
#import "C:\Program Files\Common Files\System\ado\msadox.dll" 
#else
#import "msadox.dll" 
#endif
*/
/*
TINYINT 1 字节 0 和 255 之间的整数值。 
MONEY 8 字节 介于– 922,337,203,685,477.5808 和 922,337,203,685,477.5807 之间的小数。 
DATETIME
FLOAT 8 字节 双精度浮点值，其范围为 –1.79769313486232E308 到 –4.94065645841247E-324（负值）、4.94065645841247E-324 到 1.79769313486232E308（正值）和 0。 
INTEGER 4 字节 –2,147,483,648 和 2,147,483,647 之间的长整数。 
TEXT 每个字符 2 字节（请参阅“注释”） 零到最大 2.14 GB。 
CHARACTER 每个字符 2 字节（请参阅“注释”） 零到 255 个字符。 
*/
//直接执行SQL语句,除了要用到结果集其余的大部分功能都可以直接用SQL语言实现 
class DB
{
public:
	DB(void);
	//建立数据库
	BOOL CreateDatabase(CString dbPath,CString pass);
	//增加表	//增加字段
	void CreateTable(CString dbPath,CString sqlcmd);
	void AppendItem(CString dbPath,CString sqlcmd);
	//执行SQL语句
	void ExeSql(CString dbPath,CString sqlcmd,CString pass);
    CString GetFields(CString dbPath,CString sqlcmd,CString pass, CString *spField);
	//写入一条记录
	~DB(void);
};
