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

//将非连续像素数据从内存写至硬盘，discontinue pixels memory--->Driver JPEG
__declspec(dllexport) bool write_jpeg_file_discontinue (char * filename, int quality, 
				 int image_height, int image_width,
				 J_COLOR_SPACE color_space, unsigned char **pSrc);

//将连续像素数据从内存写至硬盘，continue pixels memory--->Driver JPEG
__declspec(dllexport) bool write_jpeg_file_continue (char * filename, int quality, 
				 int image_height, int image_width,
				 J_COLOR_SPACE color_space, unsigned char *pSrc);

//从硬盘上读取jpeg图像至内存 Driver Jpeg---->continue pixels memory
__declspec(dllexport) bool read_jpeg_file (char * filename, unsigned char *pData);


//用于将像素信息以jpeg的形式压缩至内存中,continue pixels memory--->JPEG memory
__declspec(dllexport) int jpeg_memory_compress(unsigned char *inbuf,
						 int jpegHeight,
						 int jpegWidth,
						 J_COLOR_SPACE color_space,
						 int quality,
						 unsigned char *outbuf,
						 unsigned long *outSize);

					
//将内存图像数据压缩至硬盘 memory pixels --->Driver jpeg[方式：内存映射]
__declspec(dllexport) bool memory_compress_to_file(char* filename,unsigned char* input_data,
					 int height, int width, J_COLOR_SPACE color_space,
					 int quality,bool overwrite);
					
//由内存中解压数据至内存，memory JPEG--->memory Pixels buffer
__declspec(dllexport) int jpeg_memory_uncompress(unsigned char* dst,
						   unsigned int max_line,unsigned char *src, unsigned long compress_size);

//将内存的一段数据映射到硬盘
__declspec(dllexport) bool memory_to_file(char* filename,char* input_data,int length,int overwrite);

//将硬盘数据映射到内存
__declspec(dllexport) bool file_to_memory(char* filename,void* out_data,int length);

#endif