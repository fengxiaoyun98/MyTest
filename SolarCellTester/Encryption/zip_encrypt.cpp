#include "stdafx.h"
#include "zip_encrypt.h"
#include "czip.h"


#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)   
#define KEYLENGTH  0x00800000  
#define ENCRYPT_ALGORITHM CALG_RC4    
#define ENCRYPT_BLOCK_SIZE 8    
zip_encrypt::zip_encrypt(void)
{
}

zip_encrypt::~zip_encrypt(void)
{
}

/////文件压缩
BOOL zip_encrypt::OnZipit(PCHAR fileforzip/*需要压缩的文件*/, PCHAR filezip/*压缩后文件.zip*/)
{
	try
	{
		CZip oZip(fileforzip);
		oZip.SwapSize(filezip);
	//	MessageBox(NULL,_T(".zip success"),NULL, MB_OK);
		return TRUE;
	}
	catch(CZipException e)
	{
		CString s;
		s.Format(_T("%s"), e.GetString());
	//	MessageBox(NULL, s, NULL, MB_OK);
		return FALSE;
	}
}
/////文件解压
BOOL zip_encrypt::OnExtract(PCHAR defilename/*需要解压的zip文件*/, PCHAR newfilename/*解压后文件名*/)
{
	try
	{
		CUnzip oUnzip( defilename );
		oUnzip.SwapSize( newfilename );
		return TRUE;
	}
	catch( CZipException e )
	{
		CString s;
		s.Format(_T("%s"),e.GetString());
	//	MessageBox(NULL, s, NULL, MB_OK);
		return FALSE;
	}
}
/////加密函数，参数见函数声明
BOOL zip_encrypt::EncryptFile(PCHAR szSource, PCHAR szDestination, PCHAR szPassword)
{
	FILE *hSource;
	FILE *hDestination;
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
	PBYTE pbBuffer;
	DWORD dwBlockLen;
	DWORD dwBufferLen;
	DWORD dwCount;
	
	hSource = fopen(szSource,"rb");  ///// 打开原文件
	hDestination = fopen(szDestination,"wb"); ///// 目标文件
	hCryptProv = GetCryptProv();		//获取加密服务者句柄

	// 创建会话密钥.
	if(!szPassword || strcmp(szPassword,"")==0 )
	{
		hKey = GenKeyByRandom( hCryptProv, hDestination);
	}
	else
	{
		hKey=GenKeyByPassword( hCryptProv, szPassword);
	}

	///// 加密长度
	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

	///// 分配空间
	if(ENCRYPT_BLOCK_SIZE > 1)
	{
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	}
	else
	{
		dwBufferLen = dwBlockLen;
	}
	pbBuffer = (BYTE *)malloc(dwBufferLen);  
	do ///// 循环加密 原文件
	{
		dwCount = fread(pbBuffer, 1, dwBlockLen, hSource);
		if(ferror(hSource))
		{
			///// 读取明文文件出错!\n
		}
		// 加密数据.
		if(!CryptEncrypt(hKey,0,feof(hSource),0,pbBuffer,&dwCount,dwBufferLen))
		{

		}
		// 把加密后数据写到密文文件中
		fwrite(pbBuffer, 1, dwCount, hDestination);
		if(ferror(hDestination))
		{

		}

	} while(!feof(hSource));

	///// 关闭文件
	if(hSource)
	{
		fclose(hSource);
	}
	if(hDestination)
	{
		fclose(hDestination);
	}
	///// 释放内存空间
	if(pbBuffer)
	{
		free(pbBuffer);   
	}

	if(hKey)
	{   
		CryptDestroyKey(hKey);
	}
	if(hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return TRUE;
}
///// 获取加密句柄
HCRYPTPROV zip_encrypt::GetCryptProv()
{
	HCRYPTPROV hCryptProv;
	if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		///// 获取加密服务句柄成功
	}
	else ///// 重新创建
	{
		CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET);
	}
	return hCryptProv;
}
///// 创建密码，并写入需要加密的文件
HCRYPTKEY zip_encrypt::GenKeyByRandom(HCRYPTPROV hCryptProv, FILE* hDestination)
{
	HCRYPTKEY hKey;
	HCRYPTKEY hXchgKey;
	PBYTE pbKeyBlob;
	DWORD dwKeyBlobLen;

	CryptGenKey(hCryptProv, ENCRYPT_ALGORITHM, KEYLENGTH | CRYPT_EXPORTABLE, &hKey);

	if(CryptGenKey(hCryptProv, AT_KEYEXCHANGE, 0, &hXchgKey))
	{
		
	}
	else 
	{
		MessageBox(NULL,_T("创建交换密钥时出错"),NULL,MB_OK);
	}

	if(CryptExportKey(hKey,hXchgKey,SIMPLEBLOB,0,NULL,&dwKeyBlobLen))
	{
		
	}
	else
	{
		MessageBox(NULL,_T("计算密钥数据块长度出错"),NULL,MB_OK);
	}
	if(pbKeyBlob =(BYTE *)malloc(dwKeyBlobLen))
	{

	}
	else
	{
		MessageBox(NULL,_T("所需内存不够"),NULL,MB_OK);
	}

	if(CryptExportKey(hKey,hXchgKey,SIMPLEBLOB,0,pbKeyBlob,&dwKeyBlobLen))
	{
		
	}
	else
	{
		MessageBox(NULL,_T("导出密码出错"),NULL,MB_OK);
	}

	if(hXchgKey)
	{
		CryptDestroyKey(hXchgKey);
		hXchgKey = 0;
	}
	fwrite(&dwKeyBlobLen, sizeof(DWORD), 1, hDestination);
	if(ferror(hDestination))
	{
		MessageBox(NULL,_T("写入密钥块长度出错"),NULL,MB_OK);
	}

	fwrite(pbKeyBlob, 1, dwKeyBlobLen, hDestination);
	if(ferror(hDestination))
	{
		MessageBox(NULL,_T("写入密钥数据出错"),NULL,MB_OK);
	}
	free(pbKeyBlob);
	return hKey;
}
///// 密码写入哈希数据表，并获取
HCRYPTKEY zip_encrypt::GenKeyByPassword(HCRYPTPROV hCryptProv,PCHAR szPassword)
{
	HCRYPTKEY hKey;
	HCRYPTHASH hHash;
	CryptCreateHash(hCryptProv,CALG_MD5,0,0,&hHash);

	CryptHashData(hHash,(BYTE *)szPassword,strlen(szPassword),0);

	CryptDeriveKey(hCryptProv,ENCRYPT_ALGORITHM,hHash,KEYLENGTH,&hKey);
	if(hHash)
	{
		CryptDestroyHash(hHash);
		hHash = 0;
	}
	return hKey;
}
///// 解密函数（参数：解密文件，解密后文件，加密文件密码）
BOOL zip_encrypt::DecryptFile(PCHAR szSource,PCHAR szDestination,PCHAR szPassword)
{
	FILE *hSource;
	FILE *hDestination;

	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;

	PBYTE pbBuffer;
	DWORD dwBlockLen;
	DWORD dwBufferLen;
	DWORD dwCount;
	BOOL status = FALSE;

	hSource = fopen(szSource,"rb");
	hDestination = fopen(szDestination,"wb");
	hCryptProv = GetCryptProv();

	//获取或创建会话密钥
	if(!szPassword|| strcmp(szPassword,"")==0 )
	{
		hKey = GenKeyFromFile( hCryptProv,hSource);
	}
	else
	{
		hKey=GenKeyByPassword( hCryptProv, szPassword);
	}

	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;
	dwBufferLen = dwBlockLen;

	pbBuffer = (BYTE *)malloc(dwBufferLen);  

	do 
	{   
		dwCount = fread(pbBuffer,1,dwBlockLen,hSource);
		if(ferror(hSource))
		{

		}
		if(!CryptDecrypt(hKey,0,feof(hSource),0,pbBuffer,&dwCount))
		{

		}   
		fwrite(pbBuffer,1,dwCount,hDestination);
		if(ferror(hDestination))
		{

		}
	}   while(!feof(hSource));

	status = TRUE;

	// 关闭文件
	if(hSource)
	{
		fclose(hSource);
	}
	if(hDestination)
	{
		fclose(hDestination);
	}

	// 释放内存空间
	if(pbBuffer)
	{
		free(pbBuffer);
	}  
	// 销毁会话密钥   
	if(hKey)
	{
		CryptDestroyKey(hKey);
	}
	if(hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	return status;
}
///// 从加密文件中获取加密密码
HCRYPTKEY zip_encrypt::GenKeyFromFile(HCRYPTPROV hCryptProv,FILE* hSource)
{
	HCRYPTKEY hKey;
	PBYTE pbKeyBlob;
	DWORD dwKeyBlobLen;

	//从密文文件中获取密钥数据块长度，并分配内存空间.
	fread(&dwKeyBlobLen, sizeof(DWORD), 1, hSource);
	if(ferror(hSource) || feof(hSource))
	{
		MessageBox(NULL,_T("密码有误"),NULL,MB_OK);
	}
	pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen);

	// 从密文文件中获取密钥数据块
	fread(pbKeyBlob, 1, dwKeyBlobLen, hSource);
	if(ferror(hSource) || feof(hSource))
	{
		MessageBox(NULL,_T("密码有误"),NULL,MB_OK);
	}
	CryptImportKey(hCryptProv,pbKeyBlob,dwKeyBlobLen,0,0,&hKey);
	if(pbKeyBlob)
	{
		free(pbKeyBlob);
	}
	return hKey;
}
