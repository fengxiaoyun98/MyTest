#ifndef JPEGFASTOP_H_
#define JPEGFASTOP_H_
#include <stdio.h>
#include <sys/utime.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
//#include <stdlib.h>
//#include <string>
//#include <sys/stat.h>
//#include <signal.h>
//#include <sys/types.h>
//#include <sys/utime.h>
//#include <time.h>
//#include <time.h>
#include <errno.h>
#include "mman.h"
#include <setjmp.h>
#include "libjpeg-turbo\\include\\jpeglib.h"
#include "libjpeg-turbo\\include\\turbojpeg.h"
#include "libjpeg-turbo\\include\\jerror.h"
#include <string>


typedef struct my_error_mgr * my_error_ptr;

__declspec(dllexport) void error_exit(j_common_ptr cinfo);
__declspec(dllexport) void my_error_exit (j_common_ptr cinfo);

//���������������ݴ��ڴ�д��Ӳ�̣�discontinue pixels memory--->Driver JPEG
__declspec(dllexport) bool write_jpeg_file_discontinue (char * filename, int quality, 
				 int image_height, int image_width,
				 J_COLOR_SPACE color_space, unsigned char **pSrc);

//�������������ݴ��ڴ�д��Ӳ�̣�continue pixels memory--->Driver JPEG
__declspec(dllexport) bool write_jpeg_file_continue (char * filename, int quality, 
				 int image_height, int image_width,
				 J_COLOR_SPACE color_space, unsigned char *pSrc);

//��Ӳ���϶�ȡjpegͼ�����ڴ� Driver Jpeg---->continue pixels memory
__declspec(dllexport) bool read_jpeg_file (char * filename, unsigned char *pData);


//���ڽ�������Ϣ��jpeg����ʽѹ�����ڴ���,continue pixels memory--->JPEG memory
__declspec(dllexport) int jpeg_memory_compress(unsigned char *inbuf,
						 int jpegHeight,
						 int jpegWidth,
						 J_COLOR_SPACE color_space,
						 int quality,
						 unsigned char *outbuf,
						 unsigned long *outSize);

					
//���ڴ�ͼ������ѹ����Ӳ�� memory pixels --->Driver jpeg[��ʽ���ڴ�ӳ��]
__declspec(dllexport) bool memory_compress_to_file(char* filename,unsigned char* input_data,
					 int height, int width, J_COLOR_SPACE color_space,
					 int quality,bool overwrite);
					
//���ڴ��н�ѹ�������ڴ棬memory JPEG--->memory Pixels buffer
__declspec(dllexport) int jpeg_memory_uncompress(unsigned char* dst,
						   unsigned int max_line,unsigned char *src, unsigned long compress_size);

//���ڴ��һ������ӳ�䵽Ӳ��
__declspec(dllexport) bool memory_to_file(char* filename,char* input_data,int length,int overwrite);

//��Ӳ������ӳ�䵽�ڴ�
__declspec(dllexport) bool file_to_memory(char* filename,void* out_data,int length);

#endif