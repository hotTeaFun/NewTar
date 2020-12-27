#pragma once


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <conio.h>
//#include <error.h>
#include <direct.h>
using namespace std;




class Pack_file
{
private:
	FileHead fh;//文件头
	char ObjectFilePathName[MAX_PATH];//生成打包文件位置及名称
	char ObjectFilefoldPath[MAX_PATH];//需要打包的文件夹位置

	vector<string> filelist; //



public:
	Pack_file(void);
	~Pack_file(void);


	//添加文件到包内
	void AddFile();
	//设置打包路径
	void SetfilePath(char * filePutPath);
	//设置打包输出文件
	void SetOutPutFile(char * OutFile);
	//获取文件大小(传入以二进制方式打开的文件指针)
	long GetFileSize(FILE *pf);
	//制作打包文件
	void DoMakeCAB();
	//解包(为了节省时间不写错误处理了,可以根据上面自己添加)
	//void DoUnCAB(char *CabFilePathName);
	//得到需要解包的文件
	void get_filelist(char *foldname);
	//void ReadFile(const char* fileName);
	//void WriteFile(string fileName);

private:
	//显示打包内文件信息
	void printCAB();
	//创建文件夹
	void CheckTargetPath(string targetPath);
};
