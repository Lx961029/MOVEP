#pragma once
#include "pch.h"
//#include "CStreamCurveData.h"

#ifndef SHARED_HANDLERS
#include "CStreamCurveData.h"
#endif


class CEEGCom
{
public:
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

public:
	//初始化串口
	void InitComm();
	//线程读串口数据
	UINT  CollectUsbDataFowADS();


public:
	//串口名称
	CString m_SelCommName;

	// 已经打开的串口的句柄
	HANDLE m_CommHandle;

	// 读数据的线程
	CWinThread* m_pCollectADSThread;

	// 同步线程用
	HANDLE m_hEventForCollect;
	//数据收集线程标志位
	bool m_bIsbeginThread;
	//IsDataColOk用于表示数据采集是否成功;
	bool m_bIsStartofData;

	// 放数据的地方
	CStreamCurveData m_data;
	//用于存放数据的文件
	CStdioFile m_outfile;
};

