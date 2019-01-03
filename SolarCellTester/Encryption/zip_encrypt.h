#pragma once
#include <wincrypt.h> 
class zip_encrypt
{
public:
	zip_encrypt(void);
public:
	~zip_encrypt(void);
public:
	///// �ļ�ѹ����zip��(��������Ҫѹ�����ļ���ѹ������ļ�)
	 BOOL OnZipit(PCHAR fileforzip, PCHAR filezip);
	///// �ļ���ѹ 
	 BOOL OnExtract(PCHAR defilename, PCHAR newfilename);

	///// ���ܺ����������������ļ������ܺ��ļ����������룩
	BOOL EncryptFile(PCHAR szSource, PCHAR szDestination, PCHAR szPassword);

	///// ��ȡ���ܾ��
	HCRYPTPROV GetCryptProv();

	///// �������룬��д����Ҫ���ܵ��ļ�
	HCRYPTKEY GenKeyByRandom(HCRYPTPROV hCryptProv, FILE* hDestination);

	///// ����д���ϣ���ݱ�����ȡ
	HCRYPTKEY GenKeyByPassword(HCRYPTPROV hCryptProv,PCHAR szPassword);

	///// ���ܺ����������������ļ������ܺ��ļ��������ļ����룩
	BOOL DecryptFile(PCHAR szSource,PCHAR szDestination,PCHAR szPassword);

	///// �Ӽ����ļ��л�ȡ��������
	HCRYPTKEY GenKeyFromFile(HCRYPTPROV hCryptProv,FILE* hSource);
};