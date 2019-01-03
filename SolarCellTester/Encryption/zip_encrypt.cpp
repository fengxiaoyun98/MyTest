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

/////�ļ�ѹ��
BOOL zip_encrypt::OnZipit(PCHAR fileforzip/*��Ҫѹ�����ļ�*/, PCHAR filezip/*ѹ�����ļ�.zip*/)
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
/////�ļ���ѹ
BOOL zip_encrypt::OnExtract(PCHAR defilename/*��Ҫ��ѹ��zip�ļ�*/, PCHAR newfilename/*��ѹ���ļ���*/)
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
/////���ܺ�������������������
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
	
	hSource = fopen(szSource,"rb");  ///// ��ԭ�ļ�
	hDestination = fopen(szDestination,"wb"); ///// Ŀ���ļ�
	hCryptProv = GetCryptProv();		//��ȡ���ܷ����߾��

	// �����Ự��Կ.
	if(!szPassword || strcmp(szPassword,"")==0 )
	{
		hKey = GenKeyByRandom( hCryptProv, hDestination);
	}
	else
	{
		hKey=GenKeyByPassword( hCryptProv, szPassword);
	}

	///// ���ܳ���
	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

	///// ����ռ�
	if(ENCRYPT_BLOCK_SIZE > 1)
	{
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	}
	else
	{
		dwBufferLen = dwBlockLen;
	}
	pbBuffer = (BYTE *)malloc(dwBufferLen);  
	do ///// ѭ������ ԭ�ļ�
	{
		dwCount = fread(pbBuffer, 1, dwBlockLen, hSource);
		if(ferror(hSource))
		{
			///// ��ȡ�����ļ�����!\n
		}
		// ��������.
		if(!CryptEncrypt(hKey,0,feof(hSource),0,pbBuffer,&dwCount,dwBufferLen))
		{

		}
		// �Ѽ��ܺ�����д�������ļ���
		fwrite(pbBuffer, 1, dwCount, hDestination);
		if(ferror(hDestination))
		{

		}

	} while(!feof(hSource));

	///// �ر��ļ�
	if(hSource)
	{
		fclose(hSource);
	}
	if(hDestination)
	{
		fclose(hDestination);
	}
	///// �ͷ��ڴ�ռ�
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
///// ��ȡ���ܾ��
HCRYPTPROV zip_encrypt::GetCryptProv()
{
	HCRYPTPROV hCryptProv;
	if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		///// ��ȡ���ܷ������ɹ�
	}
	else ///// ���´���
	{
		CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET);
	}
	return hCryptProv;
}
///// �������룬��д����Ҫ���ܵ��ļ�
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
		MessageBox(NULL,_T("����������Կʱ����"),NULL,MB_OK);
	}

	if(CryptExportKey(hKey,hXchgKey,SIMPLEBLOB,0,NULL,&dwKeyBlobLen))
	{
		
	}
	else
	{
		MessageBox(NULL,_T("������Կ���ݿ鳤�ȳ���"),NULL,MB_OK);
	}
	if(pbKeyBlob =(BYTE *)malloc(dwKeyBlobLen))
	{

	}
	else
	{
		MessageBox(NULL,_T("�����ڴ治��"),NULL,MB_OK);
	}

	if(CryptExportKey(hKey,hXchgKey,SIMPLEBLOB,0,pbKeyBlob,&dwKeyBlobLen))
	{
		
	}
	else
	{
		MessageBox(NULL,_T("�����������"),NULL,MB_OK);
	}

	if(hXchgKey)
	{
		CryptDestroyKey(hXchgKey);
		hXchgKey = 0;
	}
	fwrite(&dwKeyBlobLen, sizeof(DWORD), 1, hDestination);
	if(ferror(hDestination))
	{
		MessageBox(NULL,_T("д����Կ�鳤�ȳ���"),NULL,MB_OK);
	}

	fwrite(pbKeyBlob, 1, dwKeyBlobLen, hDestination);
	if(ferror(hDestination))
	{
		MessageBox(NULL,_T("д����Կ���ݳ���"),NULL,MB_OK);
	}
	free(pbKeyBlob);
	return hKey;
}
///// ����д���ϣ���ݱ�����ȡ
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
///// ���ܺ����������������ļ������ܺ��ļ��������ļ����룩
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

	//��ȡ�򴴽��Ự��Կ
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

	// �ر��ļ�
	if(hSource)
	{
		fclose(hSource);
	}
	if(hDestination)
	{
		fclose(hDestination);
	}

	// �ͷ��ڴ�ռ�
	if(pbBuffer)
	{
		free(pbBuffer);
	}  
	// ���ٻỰ��Կ   
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
///// �Ӽ����ļ��л�ȡ��������
HCRYPTKEY zip_encrypt::GenKeyFromFile(HCRYPTPROV hCryptProv,FILE* hSource)
{
	HCRYPTKEY hKey;
	PBYTE pbKeyBlob;
	DWORD dwKeyBlobLen;

	//�������ļ��л�ȡ��Կ���ݿ鳤�ȣ��������ڴ�ռ�.
	fread(&dwKeyBlobLen, sizeof(DWORD), 1, hSource);
	if(ferror(hSource) || feof(hSource))
	{
		MessageBox(NULL,_T("��������"),NULL,MB_OK);
	}
	pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen);

	// �������ļ��л�ȡ��Կ���ݿ�
	fread(pbKeyBlob, 1, dwKeyBlobLen, hSource);
	if(ferror(hSource) || feof(hSource))
	{
		MessageBox(NULL,_T("��������"),NULL,MB_OK);
	}
	CryptImportKey(hCryptProv,pbKeyBlob,dwKeyBlobLen,0,0,&hKey);
	if(pbKeyBlob)
	{
		free(pbKeyBlob);
	}
	return hKey;
}
