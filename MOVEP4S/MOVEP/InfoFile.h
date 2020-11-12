#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>


#define _F_USERINFO "./info.dat"

using namespace std;

struct msg
{
	int id;				//���
	char name[50];	    //������
	int age;			//����
	char date[30];        //����
	char type[50];        //ʵ������
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	// ��ȡ��������Ϣ
	void ReadDocline();

	//��Ϣд���ļ�
	void WirteDocline();

	//����²�����
	void Addline(int id,CString name,  int age,CString date, CString type);

	list<msg> ls;	//�û���Ϣ����
	int num;
};

