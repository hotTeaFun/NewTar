#include "StdAfx.h"
#include "UnPack_files.h"


UnPack_file::UnPack_file(void)
{
	memset(&fh, 0x0, sizeof(fh));
	memset(ObjectFilePathName, 0x0, sizeof(ObjectFilePathName));
}

UnPack_file::~UnPack_file(void)
{
}


//解包(为了节省时间不写错误处理了,可以根据上面自己添加)
void UnPack_file::DoUnCAB(char *CabFilePathName, char*OutputPath)
{
	FILE *pCAB = NULL;
	FILE *pWork = NULL;
	long off = 0;
	//cout << "off: " << off << endl;
	off = sizeof(fh);
	//cout << "off: " << off << endl;
	pCAB = fopen(CabFilePathName, "rb");


	//读文件头
	fseek(pCAB, -off, SEEK_END);
	memset(&fh, 0x0, sizeof(fh));
	fread(&fh, sizeof(fh), 1, pCAB);
	fseek(pCAB, 0, SEEK_SET);

	//printCAB();


	//解包的所有文件放到当前目录下
	for (int i = 0; i < fh.FileCount; i++)
	{
		unsigned char *pTmpData = NULL;
		pTmpData = new unsigned char[fh.FileLen[i]];
		fread(pTmpData, fh.FileLen[i], 1, pCAB);
		//只取文件名,不要生成文件的路径名
		char tmpFileName[MAX_PATH];
		string aaa;
		cout << "fh.FileName: " << fh.FileName[i] << endl; //测试
		aaa.assign(fh.FileName[i], strlen(fh.FileName[i]));
		char ptmpC[MAX_PATH];
		strcpy(ptmpC, aaa.c_str());
		cout << "ptmpC:" << ptmpC << endl;

		memset(tmpFileName, 0x0, sizeof(tmpFileName));
		strcpy(tmpFileName, ptmpC);
		cout << "tmpFileName: " << tmpFileName << endl;
		//取CAB文件路径
		char tmpPathName[MAX_PATH];
		memset(tmpPathName, 0x0, sizeof(tmpPathName));
		strcpy(tmpPathName, OutputPath);
		cout << "CabFilePathName" << CabFilePathName << endl;
		//char* tmpC = tmpPathName + strlen(tmpPathName);
		//while ('\\' != *tmpC)
		//{
		//	tmpC--;
		//}
		//tmpC++;
		//*tmpC = '\0';
		strcat(tmpPathName, tmpFileName);
		cout << "OutputPath: " << OutputPath << endl;
		cout << "tmpPathName: " << tmpPathName << endl;
		pWork = fopen(tmpPathName, "wb");
		if (pWork == NULL)
		{
			CheckTargetPath(tmpPathName);
			pWork = fopen(tmpPathName, "wb");
		}
		fwrite(pTmpData, fh.FileLen[i], 1, pWork);
		fclose(pWork);
		delete[] pTmpData;
	}

	fclose(pCAB);
}


//显示打包内文件信息
void UnPack_file::printCAB()
{
	cout << "文件内信息如下:" << endl;
	cout << "文件总数:" << fh.FileCount << endl;
	for (int i = 0; i < fh.FileCount; i++)
	{
		cout << fh.FileName[i] << "\t\t\t\t" << fh.FileLen[i] << "字节" << endl;
	}
}


//创建文件夹
void UnPack_file::CheckTargetPath(string targetPath)
{
	//Log &log = Log::getLog("main", "CheckTargetPath");
	int e_pos = targetPath.length();
	int f_pos = targetPath.find("\\", 0);
	string subdir;
	do
	{
		e_pos = targetPath.find("\\", f_pos + 2);
		if (e_pos != -1)
		{
			subdir = targetPath.substr(0, e_pos);
			if (_mkdir(subdir.c_str()) == 0)
				printf("creat success %s", subdir.c_str());
			else
				printf("creat fail %s", subdir.c_str());
		}
		f_pos = e_pos;
	} while (f_pos != -1);
}
