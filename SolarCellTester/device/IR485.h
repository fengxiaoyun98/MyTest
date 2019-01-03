/*
读取红外温度传感器温度
设备符合工业操作标准的符号语言
*/
#pragma once

class IR485
{
public:
	IR485(void);
	~IR485(void);
private:
	HANDLE hCom;//串口
	BYTE buf[50];
	BYTE order[5];
	double m_temp;
	static const int exist=1;//传感器是否存在
	enum ENUMVALUE{ZERO=-273,DOWNLMT=0,UPLMT=50};
public:
	// 从已打开的串口中读取温度值
	BOOL ReqTemp();
	double GetTemp(void);
	CString GetTempStr(void);
	void SetTemp(double val);
	//返回一个可通信的串口
	BOOL open(int no);
	DWORD ReadSync(LPVOID Buffer, size_t dwBufferLength);
	DWORD WriteSync(LPVOID Buffer, size_t dwBufferLength);
	// 关闭串口
	void close(void);
private:
	//温度值温度是否有效
	BOOL IsRight(double val){return (DOWNLMT<val)&&(val<UPLMT);};
	BOOL IsOpen() const { return hCom != INVALID_HANDLE_VALUE; };
	//连接IRTP300L，从串口中查寻是否存在，如果存在则打开
	void ConIR(void);
	//设置通信参数
	BOOL SetPara(void);
	//验证温度传器是否再此串口
	BOOL IsIrTemp(void);
	BOOL Encode(void);
};
