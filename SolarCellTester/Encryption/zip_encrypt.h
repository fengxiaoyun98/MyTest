#pragma once
#include <wincrypt.h> 
class zip_encrypt
{
public:
	zip_encrypt(void);
public:
	~zip_encrypt(void);
public:
	///// 文件压缩（zip）(参数：需要压缩的文件，压缩后的文件)
	 BOOL OnZipit(PCHAR fileforzip, PCHAR filezip);
	///// 文件解压 
	 BOOL OnExtract(PCHAR defilename, PCHAR newfilename);

	///// 加密函数（参数：加密文件，加密后文件，加密密码）
	BOOL EncryptFile(PCHAR szSource, PCHAR szDestination, PCHAR szPassword);

	///// 获取加密句柄
	HCRYPTPROV GetCryptProv();

	///// 创建密码，并写入需要加密的文件
	HCRYPTKEY GenKeyByRandom(HCRYPTPROV hCryptProv, FILE* hDestination);

	///// 密码写入哈希数据表，并获取
	HCRYPTKEY GenKeyByPassword(HCRYPTPROV hCryptProv,PCHAR szPassword);

	///// 解密函数（参数：解密文件，解密后文件，加密文件密码）
	BOOL DecryptFile(PCHAR szSource,PCHAR szDestination,PCHAR szPassword);

	///// 从加密文件中获取加密密码
	HCRYPTKEY GenKeyFromFile(HCRYPTPROV hCryptProv,FILE* hSource);
};