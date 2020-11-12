#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>


#define _F_USERINFO "./info.dat"

using namespace std;

struct msg
{
	int id;				//序号
	char name[50];	    //被测者
	int age;			//年龄
	char date[30];        //日期
	char type[50];        //实验类型
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	// 读取测试者信息
	void ReadDocline();

	//信息写入文件
	void WirteDocline();

	//添加新测试者
	void Addline(int id,CString name,  int age,CString date, CString type);

	list<msg> ls;	//用户信息容器
	int num;
};

