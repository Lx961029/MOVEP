#include "pch.h"
#include "InfoFile.h"
#include <time.h>
#include <stdlib.h>


CInfoFile::CInfoFile()
{
	num = 0;
}

CInfoFile::~CInfoFile()
{
}

//读取被测试人员信息
void CInfoFile::ReadDocline()
{
	int i=0;
	CFile file;
	msg tmp;
	int n;
	file.Open(_T(_F_USERINFO),CFile::modeRead);
	if (!file)
	{
		AfxMessageBox(_T("文件打开失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}

	while (n = file.Read(&tmp,sizeof(tmp))> 0)
	{
		ls.push_back(tmp);
	}
	file.Abort();
}

//写入文件
void CInfoFile::WirteDocline()
{
	int i = 0, j = 0, k = 0, m = 0;
	CFile file;
	file.Open(_T(_F_USERINFO), CFile::modeCreate|CFile::modeWrite);
	if (!file)
	{
		AfxMessageBox(_T("文件打开失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}
	msg tmp;
	if (ls.size() >= 0)	//商品链表有内容才执行
	{
		m = ls.size();
		//通过迭代器取出链表内容，写入文件
		for (list<msg>::iterator it = ls.begin(); it != ls.end(); it++)
		{
			++k;
			string str;
			tmp.id = k;
			str = it->name;
			j =  str.length();
			for (i=0;i <j;i++)
			{
				tmp.name[i]=str[i];
			}
			tmp.name[i]='\0';
			tmp.age = it->age;

			str = it->date;
			j =  str.length();
			for (i=0;i < j;i++)
			{
				tmp.date[i]=str[i];
			}
			tmp.date[i]='\0';

			str = it->type;
			j =  str.length();
			for (i=0;i < j;i++)
			{
				tmp.type [i]=str[i];
			}
			tmp.type [i]='\0';
			file.Write(&tmp,sizeof(tmp));
		}
	}
	file.Close();
}


//添加新商品
//name:商品名称，num：库存，price：价格
void CInfoFile::Addline(int id,CString name, int age,CString date,CString type)
{
	int i =0,j =0;
	msg tmp;
	if (ls.size() >= 0)
	{
		if (!name.IsEmpty() )
		{
			tmp.id = id;	//id自动加1
			CStringA str;
			string str1;
			str = name;	//CString转CStirngA
			str1=str.GetBuffer();
			j=str1.length();
			for (i=0;i < j;i++)
			{
				tmp.name[i]=str[i];
			}
			tmp.name[i]='\0';
			tmp.age = age;

			str = date;	//CString转CStirngA
			str1 = str.GetBuffer();
			j=str1.length();
			for (i=0;i < j;i++)
			{
				tmp.date[i]= str[i];
			}
			tmp.date[i]='\0';

			str = type;
			str1=str.GetBuffer();
			j=str1.length();
			for (i=0;i < j;i++)
			{
				tmp.type[i]=str[i];
			}
			tmp.type[i]='\0';
			ls.push_back(tmp);	//放在链表的后面
		}
	}
}
