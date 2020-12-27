
#include "StdAfx.h"
#include "Pack_files.h"


Pack_file::Pack_file(void)
{
	memset(&fh, 0x0, sizeof(fh));
	memset(ObjectFilePathName, 0x0, sizeof(ObjectFilePathName));
}


Pack_file::~Pack_file(void)
{
}
void Pack_file:: SetfilePath(char * filePutPath)
{
	strcpy(ObjectFilefoldPath, filePutPath);
}


//遍历文件夹下的文件名列表(包括嵌套文件夹)
void Pack_file::get_filelist(char *foldname)
{
	HANDLE hFind;
	WIN32_FIND_DATAA fileData;
	string line;
	char fn[MAX_PATH];
	char tmpfn[MAX_PATH];
	strcpy_s(fn, foldname);
	


	//需要对文件夹名的字符串进行处理
	if (fn[strlen(fn) - 1] != '\\')
	{
		strcat_s(fn, "\\");
	}


	//留意顺序，此时fn已加入"\\"
	strcpy(tmpfn, fn);
	//不加*会出错！
	strcat_s(fn, "*");


	hFind = FindFirstFileA(fn, &fileData);
	FindNextFile(hFind, &fileData);
	while (FindNextFile(hFind, &fileData))
	{
		//如果扫描到的当前为文件夹
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (fileData.cFileName[0] != '.')
			{
				char szFile[MAX_PATH];
				strcpy_s(szFile, tmpfn);
				strcat_s(szFile, fileData.cFileName);
				get_filelist(szFile);
			}
		}
		//扫描到文件
		else
		{
			line = (string)tmpfn;
			line += fileData.cFileName;
			/*if (line.find(".h",0)!=string::npos)
			{
			filelist.push_back(line);
			}
			else
			{
			continue;
			}*/
			filelist.push_back(line);
		}
		//cout<<line<<endl;
	}
}


//添加文件到包内
void Pack_file::AddFile()
{	
	vector<string>::iterator itemFileName;
	//添加要打包的文件
	for (itemFileName = filelist.begin(); itemFileName < filelist.end(); itemFileName++)
	{

		if (fh.FileCount >= MAX_FILE_COUNT - 1)
		{
			cout << "最多支持" << MAX_FILE_COUNT << "个文件" << endl;
			return;
		}
		strcpy_s(fh.FileName[fh.FileCount], (*itemFileName).c_str());
		fh.FileCount++;
	}

}


//设置打包输出文件
void Pack_file::SetOutPutFile(char * OutFile)
{
	memset(ObjectFilePathName, 0x0, sizeof(ObjectFilePathName));
	strcpy_s(ObjectFilePathName, OutFile);
}


//获取文件大小(传入以二进制方式打开的文件指针)
long Pack_file::GetFileSize(FILE *pf)
{
	//指针移到文件尾
	fseek(pf, 0,/*SEEK_END*/ 2);
	return ftell(pf);
}


//制作打包文件
void Pack_file::DoMakeCAB()
{
	if (fh.FileCount < 1)
	{
		cout << "没有文件添加到打包" << endl;
		return;
	}
	if (strlen(ObjectFilePathName) < 1)
	{
		cout << "没有指定打包文件输出位置" << endl;
		return;
	}


	FILE *pOutFile = NULL;
	FILE *pWorkFile = NULL;


	//获取所有文件大小
	for (int i = 0; i < fh.FileCount; i++)
	{
		pWorkFile = fopen(fh.FileName[i], "rb");
		if (NULL == pWorkFile)
		{
			cout << "文件:" << fh.FileName[i] << "无法读取[" << strerror(errno) << "]" << endl;
			return;
		}
		fh.FileLen[i] = GetFileSize(pWorkFile);
		fclose(pWorkFile);
	}


	//检查是否有对应的文件夹
	CheckTargetPath(ObjectFilePathName);
	//开始合并写文件
	pOutFile = fopen(ObjectFilePathName, "wb");
	if (NULL == pOutFile)
	{
		cout << "输出文件创建失败[" << strerror(errno) << "]" << endl;
		return;
	}

	
	//写入各文件
	for (int i = 0; i < fh.FileCount; i++)
	{
		unsigned char *pTmpData = NULL;
		//cout << "fh.FileName: " << fh.FileName[i] << endl;
		pWorkFile = fopen(fh.FileName[i], "rb");
		if (NULL == pWorkFile)
		{
			cout << "文件:" << fh.FileName[i] << "无法读取[" << strerror(errno) << "]" << endl;
			fclose(pWorkFile);
			fclose(pOutFile);
			return;
		}
		pTmpData = new unsigned char[fh.FileLen[i]];
		fread(pTmpData, fh.FileLen[i], 1, pWorkFile);
		if (ferror(pWorkFile))
		{
			cout << "文件:" << fh.FileName[i] << "无法读取[" << strerror(errno) << "]" << endl;
			fclose(pWorkFile);
			fclose(pOutFile);
			return;
		}
		fwrite(pTmpData, fh.FileLen[i], 1, pOutFile);
		if (ferror(pOutFile))
		{
			cout << "文件:" << ObjectFilePathName << "无法写入[" << strerror(errno) << "]" << endl;
			fclose(pWorkFile);
			fclose(pOutFile);
			return;
		}
		delete[] pTmpData;
		fclose(pWorkFile);
		//*fh.FileName[i] += strlen(ObjectFilefoldPath);
		string str1 = fh.FileName[i];
		str1 = str1.substr(strlen(ObjectFilefoldPath)+1,MAX_PATH);
		//cout << "Object: " << ObjectFilefoldPath << endl;
		//cout << "strl: " << str1 << endl;
		strcpy(fh.FileName[i], str1.c_str());
		//cout << "fh.FileName modify: " << fh.FileName[i] << endl;

	}
	for (int i = 0; i < fh.FileCount; i++)
	{
		cout << "fh.FileName modify: " << fh.FileName[i] << endl;
	}
	//写入文件头

	fwrite(&fh, sizeof(fh), 1, pOutFile);



	

	fclose(pOutFile);
	cout << "打包完成" << endl;
}




//显示打包内文件信息
void Pack_file::printCAB()
{
	cout << "文件内信息如下:" << endl;
	cout << "文件总数:" << fh.FileCount << endl;
	for (int i = 0; i < fh.FileCount; i++)
	{
		cout << fh.FileName[i] << "\t\t\t\t" << fh.FileLen[i] << "字节" << endl;
	}
}


//创建文件夹
void Pack_file::CheckTargetPath(string targetPath)
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
