#pragma once
#define _CRT_SECURE_NO_WARNINGS

//最多打包文件个数
#define MAX_FILE_COUNT  1024
//最大路径字符长度
#define MAX_PATH   260
//文件头
struct FileHead
{
	unsigned int FileCount;//文件个数
	unsigned int FileLen[MAX_FILE_COUNT];//文件大小
	char FileName[MAX_FILE_COUNT][MAX_PATH];//文件名
};
