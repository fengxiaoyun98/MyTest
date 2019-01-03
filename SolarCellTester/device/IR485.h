/*
��ȡ�����¶ȴ������¶�
�豸���Ϲ�ҵ������׼�ķ�������
*/
#pragma once

class IR485
{
public:
	IR485(void);
	~IR485(void);
private:
	HANDLE hCom;//����
	BYTE buf[50];
	BYTE order[5];
	double m_temp;
	static const int exist=1;//�������Ƿ����
	enum ENUMVALUE{ZERO=-273,DOWNLMT=0,UPLMT=50};
public:
	// ���Ѵ򿪵Ĵ����ж�ȡ�¶�ֵ
	BOOL ReqTemp();
	double GetTemp(void);
	CString GetTempStr(void);
	void SetTemp(double val);
	//����һ����ͨ�ŵĴ���
	BOOL open(int no);
	DWORD ReadSync(LPVOID Buffer, size_t dwBufferLength);
	DWORD WriteSync(LPVOID Buffer, size_t dwBufferLength);
	// �رմ���
	void close(void);
private:
	//�¶�ֵ�¶��Ƿ���Ч
	BOOL IsRight(double val){return (DOWNLMT<val)&&(val<UPLMT);};
	BOOL IsOpen() const { return hCom != INVALID_HANDLE_VALUE; };
	//����IRTP300L���Ӵ����в�Ѱ�Ƿ���ڣ�����������
	void ConIR(void);
	//����ͨ�Ų���
	BOOL SetPara(void);
	//��֤�¶ȴ����Ƿ��ٴ˴���
	BOOL IsIrTemp(void);
	BOOL Encode(void);
};
