#pragma once
#include "pch.h"
#include "../DataManager/CStreamCurveData.h"
#include "MyCCA.h"
//#include  "CSSVEP.h"
#include <thread>
#include <Windows.h>
#include <fstream>
#include "filter.h"
#include "rsdef.h"
//*rshdtest *rshdtest2 = -1;

//#include"CSSVEP.h"
//#define saveTimes 0
#define mydatalength 1950   //1000采样率  1.95s数据 可根据需要修改
//extern int mydatalengtest=1950;
class CEEGCom
{
public:
	//构造函数
	CEEGCom();
	//析构函数
	~CEEGCom();
	//串口选择，传入一个CStringArray的引用，返回串口的数目
	int GetCommNames(CStringArray& names);

	// 传入选择的串口编号
	void SelectComm(CString strCommName);

	// 启动串口读线程
	void Start();

	//关闭串口
	void CloseComm();

	//数据采集线程(主要用于接收USB上传到PC的数据)(ADS1278版本线程);
	static UINT CollectUsbDataFowADSThread(LPVOID pParam);

	//

	void estimate(int index, void (*pf)(int lines))
	{
		(*pf)(index);
	}
	//
	//void (*pf)(int lines);
	void (*passmessage)(int);
	void exchange(void (*lla)(int liness)) 
	{
		this->passmessage= lla;
	}
	void startisssvep() {
		this->isssvep = true;
	}
	void startismovep() 
	{
		this->ismovep = true;
	}

public:
	//初始化串口
	void InitComm();
	//线程读串口数据
	UINT  CollectUsbDataFowADS();
	//机械手臂运动解析

	void handmove(RSHD *rshdtest, int* rindex,bool* ways,bool* isfanx);


public:

	//串口名称
	CString m_SelCommName;
	//机械手臂句柄
	RSHD *rshdtest;
	bool* myway;
	bool* isfanxofc;
	// 已经打开的串口的句柄
	HANDLE m_CommHandle;

	// 读数据的线程
	CWinThread* m_pCollectADSThread;

	//数据收集线程标志位
	 bool m_bIsbeginThread;
	//IsDataColOk用于表示数据采集是否成功;
	bool m_bIsStartofData;
    //
	bool isssvep=false;
	//
	bool ismovep = false;
	//CCA计算后返回的匹配的参考信号的索引
	int *matchedIndex;
	int * matchedIndexss;
	bool* isssvepc;
	//int maindex = 0;
	// 放数据的地方
	CStreamCurveData m_data;
	//用于存放数据的文件
	CStdioFile m_outfile;
	CStdioFile m_outfile1;
	CStdioFile m_outfile2;
	CStdioFile m_outfile3;
	CStdioFile m_outfile4;
	CStdioFile m_outfile5;
	CStdioFile m_outfile6;
	CStdioFile m_outfile7;
	CStdioFile m_outfile8;

	vector<int>indexcollect;//存放两次指令

	//////////////////////////////////////////////////////////////////////
	//存放CCA计算的结果
	ofstream m_outfile9;
	int mylenthofcom;

	//进行CCA计算的主要函数
	int CCAProgress();

private:

	//创建进行CCA计算的线程
	int createCCAThread();
	//使得CCA线程和主线程分离
	int detachCCAThread();
	//进行CCA计算的线程
	std::thread* CCAThread{ nullptr };
	

	//八个IIR滤波器，用于8个通道数据的滤波
	Filter_IIR<double>* filter_IIR[8];

	//用于CCA计算的对象
	MyCCA m_MyCCA;
	//Cssvep对象
	//CSSVEP m_ssvep;
	//用于获取最新的脑电数据，CCA计算的临时缓存一
	double forCCAcaculateData1[8][mydatalength] = { 0 };
	//用于获取最新的脑电数据，CCA计算的临时缓存二
	double forCCAcaculateData2[8][mydatalength] = { 0 };
	//标志forCCAcaculateData1[15600]数组接收脑电数据
	bool isTimeForCollectData1 = false;
	//标志forCCAcaculateData2[15600]数组接收脑电数据
	bool isTimeForCollectData2 = false;
	//表示forCCAcaculateData1[15600]数组数据采集完成
	bool isEndForCollectData1 = false;
	//表示forCCAcaculateData2[15600]数组数据采集完成
	bool isEndForCollectData2 = false;
	//用于表示一轮用于CCA计算的数组的数据是否采集完整
	int saveTimes = 0;
	int msavetimes;
	//////////////////////////////////////////////////////////////////////
};

