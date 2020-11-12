
#include "pch.h"
#include "CEEGCom.h"
#include <iostream>
#include"MainFrm.h"
#include"resource.h"
#include <list>

using namespace std;

#define BufferSize 54
#define ArrSize 8


CEEGCom::CEEGCom(){
	msavetimes = 8 * mydatalength;
	matchedIndex = new int();
	matchedIndexss = new int();
	isssvepc = new bool();
	myway = new bool();
	isfanxofc = new bool();
	
	rshdtest = new RSHD();
	//m_CommHandle = NULL;
	//matchedIndex = NULL;
	//matchedIndexss = NULL;
	//m_pCollectADSThread = NULL;


	m_bIsbeginThread = false;
	m_bIsStartofData = false;
	

	*myway = false;//默认是位姿
	*isssvepc = false;
	*isfanxofc = false;

	for (size_t i = 0; i < 8; i++)
	{
		filter_IIR[i] = new Filter_IIR<double>();
	}

}

CEEGCom::~CEEGCom()
{
	for (size_t i = 0; i < 8; i++)
	{
		delete filter_IIR[i];
	}
	//if (matchedIndex)
	//{
	//	delete matchedIndex;
	//}
	//if (matchedIndexss)
	//{
	//	delete matchedIndexss;
	//}
	//delete isssvepc;
	//delete myway;
	//delete rshdtest;
	//delete isfanxofc;
}

int CEEGCom::GetCommNames(CStringArray& names)
{	
	int tmp;
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
	tmp = (int)names.GetCount();
	return tmp;
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

	if (m_CommHandle == INVALID_HANDLE_VALUE)//(HANDLE)-1) 
	{
		AfxMessageBox(_T("打开COM失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}

	if (!SetupComm(m_CommHandle, 1024, 1024))   //输入缓冲区和输出缓冲区的大小都是1024
	{
		AfxMessageBox(_T("串口缓冲区设置失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
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
		AfxMessageBox(_T("打开串口失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}

	DCB dcb;
	GetCommState(m_CommHandle, &dcb);
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	//	dcb.BaudRate = CBR_115200; //波特率为115200
	dcb.BaudRate = 921600; //波特率为921600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //一个停止位
	bool state = SetCommState(m_CommHandle, &dcb);
	if (state != true)
	{
		AfxMessageBox(_T("串口数据格式设置失败"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}
	int i = GetLastError();
	GetCommState(m_CommHandle, &dcb);

	PurgeComm(m_CommHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);//在读写串口之前，还要用PurgeComm()函数清空缓冲区，该函数原型：
	//timer = SetTimer(1, period, NULL);          //设置定时器1的定时间隔

	m_data.SetCurveCount(8);
	m_data.SetCurveLength(4096);
}

// 启动串口读线程
void CEEGCom::Start()
{
//	if (rs_initialize() == RS_SUCC)//判断机器人是否连接成功
//{
//
//	if (rs_create_context(rshdtest) == RS_SUCC)
//	{
//		if (rs_login(*rshdtest, "192.168.10.2", 8899) == RS_SUCC)
//		{
//			AfxMessageBox(_T("机械臂已经连接！"));
//	
//		}
//	}
//}
	CString str;
	//文件存放位置和打开方式
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCRibbonEdit* edit = (CMFCRibbonEdit*)((pFrame->m_wndRibbonBar).FindByID(ID_EDITFILE));
	ASSERT_VALID(edit);
	str=edit->GetEditText();
	str=str+_T("\\");
	m_outfile1.Open(str+_T("1.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile2.Open(str + _T("2.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile3.Open(str + _T("3.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile4.Open(str + _T("4.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile5.Open(str + _T("5.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile6.Open(str + _T("6.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile7.Open(str + _T("7.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile8.Open(str + _T("8.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
	m_outfile.Open(str + _T("hello.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_outfile9.open("resultsOfCCA.txt");
	/////////////////////////////////////////////////////////////////////////////////////////////

	m_pCollectADSThread = AfxBeginThread(&CEEGCom::CollectUsbDataFowADSThread, this);	//当使用的AD为ADS1278时,使用此线程做数据采集;

	m_bIsbeginThread = true;
	m_bIsStartofData = false;

	////////////////////////////////////////////////////////////////////////////////////////////
	//创建CCA计算的线程，在该线程中返回匹配的参考信号的索引
	createCCAThread();
	detachCCAThread();
	/////////////////////////////////////////////////////////////////////////////////////////
}

//数据采集线程(主要用于接收USB上传到PC的数据)(ADS1278版本线程);
UINT CEEGCom::CollectUsbDataFowADSThread(LPVOID pParam)
{
	CEEGCom* comm = (CEEGCom*)pParam;
	return comm->CollectUsbDataFowADS();
}

UINT  CEEGCom::CollectUsbDataFowADS()
{
	unsigned char pCollectData[BufferSize] = { 0 };					//采集到的数据是以1个字节为大小;
	UINT pDataBuffer[ArrSize] = { 0 };
	double EEGBuffer[ArrSize] = { 0 };
	CString str;

	/////////////////////////////////////////////////////////////////////////////////
	//用于表示一轮用于CCA计算的数组的数据是否保存完整
	saveTimes = 0;
	isTimeForCollectData1 = true;
	isTimeForCollectData2 = false;
	isEndForCollectData1 = false;
	isEndForCollectData2 = false;

	double EEGBufferTemp[ArrSize];
	////////////////////////////////////////////////////////////////////////////////

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
										EEGBufferTemp[index] = (float)((((float)(pDataBuffer[index] - (float)8388608) / (float)8388608)) * (float)2500000);  //No Gain

										filter_IIR[index]->filterData(EEGBufferTemp[index],&EEGBuffer[index]);

										/////////////////////////////////////////////////////////////////////////////
										//使用双缓存进行CCA的计算，当一个数组完成最新数据的采集，便进行计算
										if (isTimeForCollectData1)
										{
											forCCAcaculateData1[index][saveTimes/8] = EEGBuffer[index];
											saveTimes++;
											if (saveTimes == msavetimes)
											{
												isTimeForCollectData1 = false;
												isTimeForCollectData2 = true;
												isEndForCollectData1 = true;
												isEndForCollectData2 = false;

												saveTimes = 0;
												////创建CCA计算的线程，在该线程中返回匹配的参考信号的索引
												//createCCAThread();
												//detachCCAThread();
											}
										}
										if (isTimeForCollectData2)
										{
											forCCAcaculateData2[index][saveTimes/8] = EEGBuffer[index];
											saveTimes++;
											if (saveTimes == msavetimes)
											{
												isTimeForCollectData1 = true;
												isTimeForCollectData2 = false;
												isEndForCollectData1 = false;
												isEndForCollectData2 = true;

												saveTimes = 0;
												////创建CCA计算的线程，在该线程中返回匹配的参考信号的索引
												//createCCAThread();
												//detachCCAThread();
											}
										}
										/////////////////////////////////////////////////////////////////////////////

										m_data.AddValue(index, EEGBuffer[index] /1e6 );
										str.Format(_T("%10.4f "), (double)EEGBuffer[index] / 1e6);
										if (index == 0) {
											m_outfile1.WriteString(str);
										}
										else if (index == 1)
										{
											m_outfile2.WriteString(str);
										}
										else if (index == 2)
										{
											m_outfile3.WriteString(str);
										}
										else if (index == 3)
										{
											m_outfile4.WriteString(str);
										}
										else if (index == 4)
										{
											m_outfile5.WriteString(str);
										}
										else if (index == 5)
										{
											m_outfile6.WriteString(str);
										}
										else if (index == 6)
										{
											m_outfile7.WriteString(str);
										}
										else if (index == 7)
										{
											m_outfile8.WriteString(str);
										}
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

								EEGBufferTemp[index] = (float)((((float)(pDataBuffer[index] - (float)8388608) / (float)8388608)) * (float)2500000);  //No Gain

								filter_IIR[index]->filterData(EEGBufferTemp[index], &EEGBuffer[index]);

								/////////////////////////////////////////////////////////////////////////////
								//使用双缓存进行CCA的计算，当一个数组完成最新数据的采集，便进行计算
								if (isTimeForCollectData1)
								{
									forCCAcaculateData1[index][saveTimes/8] = EEGBuffer[index];
									saveTimes++;
									if (saveTimes == msavetimes)
									{
										saveTimes = 0;
										isTimeForCollectData1 = false;
										isTimeForCollectData2 = true;
										isEndForCollectData1 = true;
										isEndForCollectData2 = false;
										////创建CCA计算的线程，在该线程中返回匹配的参考信号的索引
										//createCCAThread();
										//detachCCAThread();
									}
								}
								if (isTimeForCollectData2)
								{
									forCCAcaculateData2[index][saveTimes/8] = EEGBuffer[index];
									saveTimes++;
									if (saveTimes == msavetimes)
									{
										saveTimes = 0;
										isTimeForCollectData1 = true;
										isTimeForCollectData2 = false;
										isEndForCollectData1 = false;
										isEndForCollectData2 = true;
										////创建CCA计算的线程，在该线程中返回匹配的参考信号的索引
										//createCCAThread();
										//detachCCAThread();
									}
								}
								/////////////////////////////////////////////////////////////////////////////

								m_data.AddValue(index, EEGBuffer[index] / 1e6);
								//EEGBuffer[index] * 1000;
								str.Format(_T("%10.4f "), (double)EEGBuffer[index] / 1e6);
								if (index == 0) {
									m_outfile1.WriteString(str);
								}
								else if(index == 1)
								{
									m_outfile2.WriteString(str);
								}
								else if (index == 2)
								{
									m_outfile3.WriteString(str);
								}
								else if (index == 3)
								{
									m_outfile4.WriteString(str);
								}
								else if (index == 4)
								{
									m_outfile5.WriteString(str);
								}
								else if (index == 5)
								{
									m_outfile6.WriteString(str);
								}
								else if (index == 6)
								{
									m_outfile7.WriteString(str);
								}
								else if (index == 7)
								{
									m_outfile8.WriteString(str);
								}
								index++;
							}
						}
					}
				}

			}
		}
		//CString str1 = { TEXT("0 0 0 0 0 0 0 0 ") };
		//str1.Format(_T("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f "),
		//	m_data.GetLastData(0),
		//	m_data.GetLastData(1),
		//	m_data.GetLastData(2),
		//	m_data.GetLastData(3),
		//	m_data.GetLastData(4),
		//	m_data.GetLastData(5),
		//	m_data.GetLastData(6),
		//	m_data.GetLastData(7));
		//m_outfile.WriteString(str1);
		//m_outfile.WriteString(_T("\n"));
	}
	CloseHandle(m_CommHandle);
	m_outfile.Close();
	m_outfile1.Close();
	m_outfile2.Close();
	m_outfile3.Close();
	m_outfile4.Close();
	m_outfile5.Close();
	m_outfile6.Close();
	m_outfile7.Close();
	m_outfile8.Close();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
int CEEGCom::createCCAThread()
{
	CCAThread = new std::thread(&CEEGCom::CCAProgress, this);
	return 0;
}


int CEEGCom::detachCCAThread()
{
	(*CCAThread).detach();
	return 0;
}

////4S输出一个指令
int CEEGCom::CCAProgress()
{
	int number=0;
	int sum ;
	int tmp;

	while (m_bIsbeginThread)
	{
		if (isEndForCollectData1 && !isEndForCollectData2 &&(*isssvepc))
		{
			//1950S 输出一个指令
			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData1);

			//4s输出一个指令
			//if (number < 2) {
			//	if (number==0) {
			//		sum = 0;
			//	}
			//	tmp = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData1);
			//	number++;
			//	sum += tmp;
			//	if (number == 2)
			//	{
			//		*matchedIndex = sum / 2;
			//	}
			//}
			//number = 0;
			indexcollect.push_back(*matchedIndex);
			if (indexcollect.size() == 2) {
				if (indexcollect.at(0) == indexcollect.at(1)) {
					*matchedIndexss = indexcollect.at(0);

				}
				else {
					*matchedIndexss = 0;
				}
				//机械臂实时控制
				handmove(rshdtest, matchedIndexss, myway, isfanxofc);
				m_outfile9 << *matchedIndexss << " ";
				indexcollect.clear();
			}

			//rshd2;
			//estimate(matchedIndex, passmessage);
			//机械臂实时控制
			/*handmove(rshdtest, matchedIndex, myway,isfanxofc);
			m_outfile9 << *matchedIndex << " ";*/

			isEndForCollectData1 = false;
			isEndForCollectData2 = false;
		}
		if (!isEndForCollectData1 && isEndForCollectData2 &&(*isssvepc))
		{
			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData2);
			//estimate(matchedIndex, passmessage);
			indexcollect.push_back(*matchedIndex);
			if (indexcollect.size() == 2) {
				if (indexcollect.at(0) == indexcollect.at(1)) {
					*matchedIndexss = indexcollect.at(0);
				}
				else {
					*matchedIndexss = 0;
				}
				//机械臂实时控制
				handmove(rshdtest, matchedIndexss, myway, isfanxofc);
				m_outfile9 << *matchedIndexss << " ";
				indexcollect.clear();
			}

			

			isEndForCollectData1 = false;
			isEndForCollectData2 = false;
		}
	}

	m_outfile9.close();

	return *matchedIndex;
}
///////////////////////////////////////////////////////////////////////////////////


//2S输出一个指令
//int CEEGCom::CCAProgress()
//{
//	int number = 0;
//	int sum;
//	int tmp;
//
//	while (m_bIsbeginThread)
//	{
//		if (isEndForCollectData1 && !isEndForCollectData2 && (*isssvepc))
//		{
//			//1950S 输出一个指令
//			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData1);
//
//			//机械臂实时控制
//			handmove(rshdtest, matchedIndex, myway, isfanxofc);
//			m_outfile9 << *matchedIndex << " ";
//
//			isEndForCollectData1 = false;
//			isEndForCollectData2 = false;
//		}
//		if (!isEndForCollectData1 && isEndForCollectData2 && (*isssvepc))
//		{
//			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData2);
//			//estimate(matchedIndex, passmessage);
//			//机械臂实时控制
//			handmove(rshdtest, matchedIndex, myway, isfanxofc);
//			m_outfile9 << *matchedIndex << " ";
//
//			isEndForCollectData1 = false;
//			isEndForCollectData2 = false;
//		}
//	}
//
//	m_outfile9.close();
//
//	return *matchedIndex;
//}



void CEEGCom::CloseComm()
{
	//关闭线程
	m_bIsbeginThread = false;

}
void CEEGCom::handmove(RSHD* myrshd, int* rindex,bool* ways,bool* isfanx) {
	//如果是正向(默认状态)
	if (!*isfanx) 
	{
		//如果是位姿(默认状态)
		if (!*ways)
		{
			if (*rindex == 0) {
				double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//回到初始位姿
							//	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//第一个拍照点
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 1) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0] + 0.13;
				point1[1] = waypointcurrent.jointpos[1];
				point1[2] = waypointcurrent.jointpos[2];
				point1[3] = waypointcurrent.jointpos[3];
				point1[4] = waypointcurrent.jointpos[4];
				point1[5] = waypointcurrent.jointpos[5];
				rs_move_joint(*rshdtest, point1, true);

				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 2) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0];
				point1[1] = waypointcurrent.jointpos[1] - 0.06;
				point1[2] = waypointcurrent.jointpos[2];
				point1[3] = waypointcurrent.jointpos[3];
				point1[4] = waypointcurrent.jointpos[4];
				point1[5] = waypointcurrent.jointpos[5];
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
			//	AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 3) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0];
				point1[1] = waypointcurrent.jointpos[1];
				point1[2] = waypointcurrent.jointpos[2] - 0.06;
				point1[3] = waypointcurrent.jointpos[3];
				point1[4] = waypointcurrent.jointpos[4];
				point1[5] = waypointcurrent.jointpos[5];
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 4) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0];
				point1[1] = waypointcurrent.jointpos[1];
				point1[2] = waypointcurrent.jointpos[2];
				point1[3] = waypointcurrent.jointpos[3] + 0.1;//0.06
				point1[4] = waypointcurrent.jointpos[4];
				point1[5] = waypointcurrent.jointpos[5];
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 5) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0];
				point1[1] = waypointcurrent.jointpos[1];
				point1[2] = waypointcurrent.jointpos[2];
				point1[3] = waypointcurrent.jointpos[3];
				point1[4] = waypointcurrent.jointpos[4] + 0.18;  //0.13 
				point1[5] = waypointcurrent.jointpos[5];
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 6) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);

				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point1[0] = waypointcurrent.jointpos[0];
				point1[1] = waypointcurrent.jointpos[1];
				point1[2] = waypointcurrent.jointpos[2];
				point1[3] = waypointcurrent.jointpos[3];
				point1[4] = waypointcurrent.jointpos[4];
				point1[5] = waypointcurrent.jointpos[5] + 0.2;//0.13
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}

		}
		//如果是位置
		else
		{
			if (*rindex == 0) {
				double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//回到初始位姿
							//	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//第一个拍照点
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 1) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//最大2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//最大是17
				max_acc.jointPara[1] = 10;
				max_acc.jointPara[2] = 10;
				max_acc.jointPara[3] = 10;
				max_acc.jointPara[4] = 10;
				max_acc.jointPara[5] = 10;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x + 0.02;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//机器人目标点姿态
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 2) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//最大2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//最大是17
				max_acc.jointPara[1] = 10;
				max_acc.jointPara[2] = 10;
				max_acc.jointPara[3] = 10;
				max_acc.jointPara[4] = 10;
				max_acc.jointPara[5] = 10;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y - 0.02;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//机器人目标点姿态
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
			//	AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 3) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//最大2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//最大是17
				max_acc.jointPara[1] = 10;
				max_acc.jointPara[2] = 10;
				max_acc.jointPara[3] = 10;
				max_acc.jointPara[4] = 10;
				max_acc.jointPara[5] = 10;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z - 0.02;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//机器人目标点姿态
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 4) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx + 0.06;//机器人目标点姿态
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 5) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//机器人目标点姿态
				rpy0.ry = rpy.ry + 0.06;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}
			if (*rindex == 6) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//最大2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//最大是17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				aubo_robot_namespace::wayPoint_S waypointcurrent;
				rs_get_current_waypoint(*rshdtest, &waypointcurrent);
				point2[0] = waypointcurrent.jointpos[0];
				point2[1] = waypointcurrent.jointpos[1];
				point2[2] = waypointcurrent.jointpos[2];
				point2[3] = waypointcurrent.jointpos[3];
				point2[4] = waypointcurrent.jointpos[4];
				point2[5] = waypointcurrent.jointpos[5];
				double X, Y, Z, px, py, pz;
				aubo_robot_namespace::Pos  pos0;
				aubo_robot_namespace::Rpy  rpy0;//欧拉角
				aubo_robot_namespace::Ori  ori0;//四元数
				aubo_robot_namespace::Ori  ori;//四元数
				aubo_robot_namespace::Rpy  rpy;//欧拉角
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//机器人目标点姿态
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz + 0.06;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
				//	MessageBox();
				//AfxMessageBox(_T("机械臂已经接受！"));

			}

		}
	}
	//如果是反向
	else 
     {
	 //如果是位姿(默认状态)
	 if (!*ways)
	 {
		 if (*rindex == 0) {
			 double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//回到初始位姿
						 //	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//第一个拍照点
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 1) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0] -0.13;
			 point1[1] = waypointcurrent.jointpos[1];
			 point1[2] = waypointcurrent.jointpos[2];
			 point1[3] = waypointcurrent.jointpos[3];
			 point1[4] = waypointcurrent.jointpos[4];
			 point1[5] = waypointcurrent.jointpos[5];
			 rs_move_joint(*rshdtest, point1, true);

			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 2) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0];
			 point1[1] = waypointcurrent.jointpos[1] + 0.06;
			 point1[2] = waypointcurrent.jointpos[2];
			 point1[3] = waypointcurrent.jointpos[3];
			 point1[4] = waypointcurrent.jointpos[4];
			 point1[5] = waypointcurrent.jointpos[5];
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
		 //	AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 3) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0];
			 point1[1] = waypointcurrent.jointpos[1];
			 point1[2] = waypointcurrent.jointpos[2] + 0.06;
			 point1[3] = waypointcurrent.jointpos[3];
			 point1[4] = waypointcurrent.jointpos[4];
			 point1[5] = waypointcurrent.jointpos[5];
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 4) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0];
			 point1[1] = waypointcurrent.jointpos[1];
			 point1[2] = waypointcurrent.jointpos[2];
			 point1[3] = waypointcurrent.jointpos[3] - 0.1;
			 point1[4] = waypointcurrent.jointpos[4];
			 point1[5] = waypointcurrent.jointpos[5];
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 5) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0];
			 point1[1] = waypointcurrent.jointpos[1];
			 point1[2] = waypointcurrent.jointpos[2];
			 point1[3] = waypointcurrent.jointpos[3];
			 point1[4] = waypointcurrent.jointpos[4] - 0.18;
			 point1[5] = waypointcurrent.jointpos[5];
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 6) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);

			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point1[0] = waypointcurrent.jointpos[0];
			 point1[1] = waypointcurrent.jointpos[1];
			 point1[2] = waypointcurrent.jointpos[2];
			 point1[3] = waypointcurrent.jointpos[3];
			 point1[4] = waypointcurrent.jointpos[4];
			 point1[5] = waypointcurrent.jointpos[5] - 0.2;
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }

	 }
	 //如果是位置
	 else
	 {
		 if (*rindex == 0) {
			 double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//回到初始位姿
						 //	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//第一个拍照点
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 1) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//最大2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//最大是17
			 max_acc.jointPara[1] = 10;
			 max_acc.jointPara[2] = 10;
			 max_acc.jointPara[3] = 10;
			 max_acc.jointPara[4] = 10;
			 max_acc.jointPara[5] = 10;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x - 0.02;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//机器人目标点姿态
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 2) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//最大2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//最大是17
			 max_acc.jointPara[1] = 10;
			 max_acc.jointPara[2] = 10;
			 max_acc.jointPara[3] = 10;
			 max_acc.jointPara[4] = 10;
			 max_acc.jointPara[5] = 10;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y + 0.02;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//机器人目标点姿态
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
		 //	AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 3) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//最大2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//最大是17
			 max_acc.jointPara[1] = 10;
			 max_acc.jointPara[2] = 10;
			 max_acc.jointPara[3] = 10;
			 max_acc.jointPara[4] = 10;
			 max_acc.jointPara[5] = 10;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z + 0.02;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//机器人目标点姿态
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 4) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx - 0.06;//机器人目标点姿态
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 5) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//机器人目标点姿态
			 rpy0.ry = rpy.ry - 0.06;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }
		 if (*rindex == 6) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//最大2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//最大是17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 aubo_robot_namespace::wayPoint_S waypointcurrent;
			 rs_get_current_waypoint(*rshdtest, &waypointcurrent);
			 point2[0] = waypointcurrent.jointpos[0];
			 point2[1] = waypointcurrent.jointpos[1];
			 point2[2] = waypointcurrent.jointpos[2];
			 point2[3] = waypointcurrent.jointpos[3];
			 point2[4] = waypointcurrent.jointpos[4];
			 point2[5] = waypointcurrent.jointpos[5];
			 double X, Y, Z, px, py, pz;
			 aubo_robot_namespace::Pos  pos0;
			 aubo_robot_namespace::Rpy  rpy0;//欧拉角
			 aubo_robot_namespace::Ori  ori0;//四元数
			 aubo_robot_namespace::Ori  ori;//四元数
			 aubo_robot_namespace::Rpy  rpy;//欧拉角
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//机器人移动目标点位置
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//机器人目标点姿态
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz - 0.06;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//欧拉角转四元数
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//逆解
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//机器人移动
			 //	MessageBox();
			 //AfxMessageBox(_T("机械臂已经接受！"));

		 }

	 }




      }
}