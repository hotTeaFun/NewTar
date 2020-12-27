#pragma once
#pragma once


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
//#include <error.h>
#include <direct.h>
using namespace std;







class UnPack_file
{
private:
	FileHead fh;//文件头
	char ObjectFilePathName[MAX_PATH];//生成打包文件位置及名称
	//char OutputPath[MAX_PATH]; //解压后文件输出位置


public:
	UnPack_file(void);
	~UnPack_file(void);

	void DoUnCAB(char *CabFilePathName,char*OutputPath);


private:
	//显示打包内文件信息
	void printCAB();
	//创建文件夹
	void CheckTargetPath(string targetPath);
};