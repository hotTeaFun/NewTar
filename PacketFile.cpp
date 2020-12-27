// PacketFile.cpp : 定义控制台应用程序的入口点。


#include "stdafx.h"
//#include "SearchAllFile.h"
//#include "MyCab.h"

#include "Pack_files.h"
#include "UnPack_files.h"
int main()
{
	//vector<string> vec_file;
	//vector<string>::iterator itemFileName;
	char filePacketPath[MAX_PATH] = "C:\\Users\\HP\\Desktop\\test\\test_file";
	char filePutPath[MAX_PATH] = "C:\\Users\\HP\\Desktop\\test\\output\\test_output.cab";
	
	char OutputPath[MAX_PATH] = "C:\\Users\\HP\\Desktop\\test\\output\\output_files\\";
	//CSerchAllFile::getInstance()->get_filelist(filePacketPath, vec_file);




	//--------------------打包过程--------------------
	//声明对象
	Pack_file mc;
	//设置输出文件
	mc.SetOutPutFile(filePutPath);
	//设置打包路径
	mc.SetfilePath(filePacketPath);
	//获得需要打包的文件
	mc.get_filelist(filePacketPath);
	//添加打包文件
	mc.AddFile();
	//执行打包
	mc.DoMakeCAB();


	//--------------------解过程--------------------
	//声明对象
	UnPack_file umc;
	//设置解包路径
	//执行解包
	umc.DoUnCAB(filePutPath, OutputPath);


	cin.clear();
	cin.get();


	return 0;
}

