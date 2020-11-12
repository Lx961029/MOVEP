#include "pch.h"
#include "CEEGCom.h"

#include <iostream>
using namespace std;

#define BufferSize 54
#define ArrSize 8


int CEEGCom::GetCommNames(CStringArray& names)
{
	if (names.GetCount() > 0)
		names.RemoveAll();

	CString strTemp;

	HANDLE  hCom;

	//	CComboBox* h_Comb = (CComboBox*)GetDlgItem(IDC_COMBO1);
	//	h_Comb->ResetContent();
	int j = 0;
	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("COM%d"), i + 1);
		hCom = CreateFile(strTemp, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hCom)
			continue;
		else {
			//			h_Comb->AddString(strTemp);
			names.Add(strTemp);
			//			strCom[j] = strTemp;
			j++;
		}
		CloseHandle(hCom);
	}
	for (int i = 10; i < 20; i++)
	{
		strTemp.Format(_T("\\\\.\\COM%d"), i);
		hCom = CreateFile(strTemp, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hCom)
			continue;
		else {
			//			h_Comb->AddString(strTemp);
			names.Add(strTemp);
			//			strCom[j] = strTemp;
			j++;
		}
		CloseHandle(hCom);
	}

	return names.GetCount();
}

// 串入选择的串口名字
void CEEGCom::SelectComm(CString strCommName)
{
	m_SelCommName = strCommName;
}


void CEEGCom::InitComm()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString ShowWord = _T("正在传输数据!");
	//SetDlgItemText(IDC_EDIT1, ShowWord);
	m_CommHandle = CreateFile(m_SelCommName,                             //打开串口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, NULL, OPEN_EXISTING, //打开而不是创建
		0,/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,*/ //异步方式
		NULL);
	if (m_CommHandle == (HANDLE)-1) {
		//MessageBox(_T("打开COM失败!"));
		cout << "打开COM失败!" << endl;
	}

	if (!SetupComm(m_CommHandle, 1024, 1024))   //输入缓冲区和输出缓冲区的大小都是1024
	{
		//MessageBox(_T("串口缓冲区设置失败!"));
		cout << "串口缓冲区设置失败!" << endl;
	}
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 100;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 500;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	if (!SetCommTimeouts(m_CommHandle, &TimeOuts))	 //设置超时
	{
		//MessageBox(_T("串口超时设置失败!"));
		cout << "串口超时设置失败!" << endl;
	}

	DCB dcb;
	GetCommState(m_CommHandle, &dcb);
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	//	dcb.BaudRate = CBR_115200; //波特率为115200
	dcb.BaudRate = 921600; //波特率为115200
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //一个停止位
	bool state = SetCommState(m_CommHandle, &dcb);
	if (state != true)
	{
		//MessageBox(_T("串口数据格式设置失败!"));
		cout << "串口数据格式设置失败!" << endl;
	}
	int i = GetLastError();
	GetCommState(m_CommHandle, &dcb);

	PurgeComm(m_CommHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);//在读写串口之前，还要用PurgeComm()函数清空缓冲区，该函数原型：
	//timer = SetTimer(1, period, NULL);          //设置定时器1的定时间隔

	m_data.SetCurveCount(8);
	m_data.SetCurveLength(1000);
}

// 启动串口读线程
void CEEGCom::Start()
{
	//文件存放位置和打开方式
	m_outfile.Open(_T("1.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);


	InitComm();

	m_hEventForCollect = CreateEventA(NULL, FALSE, FALSE, NULL);				//创建一个匿名的自动重置事件对象，使用默认的安全性，处于未发信状态;
	m_pCollectADSThread = AfxBeginThread(&CEEGCom::CollectUsbDataFowADSThread, this);	//当使用的AD为ADS1278时,使用此线程做数据采集;

	SetEvent(m_hEventForCollect);
	m_bIsbeginThread = true;
	m_bIsStartofData = false;
}

//数据采集线程(主要用于接收USB上传到PC的数据)(ADS1278版本线程);
UINT CEEGCom::CollectUsbDataFowADSThread(LPVOID pParam)
{
	CEEGCom* comm = (CEEGCom*)pParam;
	//long bufflen = BufferSize;
	return comm->CollectUsbDataFowADS();
}

UINT  CEEGCom::CollectUsbDataFowADS()
{
	unsigned char pCollectData[BufferSize] = { 0 };					//采集到的数据是以1个字节为大小;
	UINT pDataBuffer[ArrSize] = { 0 };
	float EEGBuffer[ArrSize] = { 0 };
	//Sleep(3000);
	while (m_bIsbeginThread)                                                 /*p_Collect->m_IsCollectAndProcess*/
	{
		//		WaitForSingleObject(m_hEventForCollect, INFINITE);	//只有事件对象处于发信状态时该函数才返回,然后执行被保护的代码,该函数一旦返回则事件对象处于未发信状态;
		bool IsDataColOk = false;	//IsDataColOk用于表示数据采集是否成功;
		unsigned char Plot_time = 0;					//采集到的数据是以1个字节为大小;
		DWORD readcount = 0;

		for (int time = 0; time < 1; time++)
		{
			if (pCollectData && pDataBuffer)
			{
				ZeroMemory(pCollectData, BufferSize);						//清空存放原始数据缓冲区;
				ZeroMemory(pDataBuffer, ArrSize);						//清空存放拼接后的数据缓冲区;
			}
			if (!m_bIsStartofData)
			{
				ZeroMemory(pCollectData, 1);
				IsDataColOk = ReadFile(m_CommHandle, pCollectData, 1, &readcount, NULL);
				if (IsDataColOk)
				{
					if (pCollectData[0] == 0xaa)
					{
						ZeroMemory(pCollectData, 2);
						IsDataColOk = ReadFile(m_CommHandle, pCollectData, 2, &readcount, NULL);
						if (IsDataColOk)
						{
							if (pCollectData[0] == 0x55)
							{
								m_bIsStartofData = true;
								ZeroMemory(pCollectData, BufferSize - 3);
								IsDataColOk = ReadFile(m_CommHandle, pCollectData, BufferSize - 3 - 27, &readcount, NULL);
								if (IsDataColOk)
								{
									unsigned int j = 0;
									unsigned int index = 0;

									for (int i = 0; i < 8; i++)
									{
										pDataBuffer[index] = pCollectData[j];
										pDataBuffer[index] <<= 8;
										++j;
										pDataBuffer[index] |= pCollectData[j];
										pDataBuffer[index] <<= 8;
										++j;
										pDataBuffer[index] |= pCollectData[j];
										++j;
										EEGBuffer[index] = (float)((((float)(pDataBuffer[index] - (float)8388608) / (float)8388608)) * (float)2500000);  //No Gain
										m_data.AddValue(index, EEGBuffer[index] * 1000);
										index++;
									}
								}
							}

						}
					}
				}
			}
			else
			{
				if (pCollectData && pDataBuffer)
				{
					ZeroMemory(pCollectData, BufferSize);						//清空存放原始数据缓冲区;
					ZeroMemory(pDataBuffer, ArrSize);						//清空存放拼接后的数据缓冲区;
					IsDataColOk = ReadFile(m_CommHandle, pCollectData, BufferSize, &readcount, NULL);
				}
				if (IsDataColOk)													//判断数据采集是否成功;
				{
					unsigned int j = 0;
					if (!((pCollectData[j] == 0xaa) && pCollectData[j + 1] == 0x55))   //寻找包对齐的位置
					{
						m_bIsStartofData = false;
						// p_ADSCollect->SetDlgItemText(IDC_EDIT1, _T("数据对齐不准Data_Wrong"));
					}
					else
					{
						// marker = pCollectData[j + 2];
						for (Plot_time = 0; Plot_time < (BufferSize / 27); Plot_time++)
						{
							j = j + 3;
							unsigned int index = 0;										//index为拼接后的数据下标索引;

							// fprintf(p_ADSCollect->fmaker, "%d ", pCollectData[j - 1]);

							for (index = 0; index < ArrSize;)									//以下代码为数据拼接方法;
							{
								pDataBuffer[index] = pCollectData[j] ^ 0x80;
								pDataBuffer[index] <<= 8;
								++j;
								pDataBuffer[index] |= pCollectData[j];
								pDataBuffer[index] <<= 8;
								++j;
								pDataBuffer[index] |= pCollectData[j];
								++j;

								EEGBuffer[index] = (float)((((float)(pDataBuffer[index] - (float)8388608) / (float)8388608)) * (float)2500000);  //Gain = 437

								m_data.AddValue(index, EEGBuffer[index] * 1000);

								index++;
							}
						}
					}
				}

			}
		}
		CString str;
		str.Format(_T("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f "),
			m_data.GetLastData(0),
			m_data.GetLastData(1),
			m_data.GetLastData(2),
			m_data.GetLastData(3),
			m_data.GetLastData(4),
			m_data.GetLastData(5),
			m_data.GetLastData(6),
			m_data.GetLastData(7));
		m_outfile.WriteString(str);
		m_outfile.WriteString(_T("\n"));
	}

	return 0;
}


void CEEGCom::CloseComm()
{
	//关闭线程
	m_bIsbeginThread = false;
	//关闭文件
	m_outfile.Close();
}

