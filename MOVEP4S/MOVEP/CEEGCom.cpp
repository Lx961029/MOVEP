
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
	

	*myway = false;//Ĭ����λ��
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

// ����ѡ��Ĵ�������
void CEEGCom::SelectComm(CString strCommName)
{
	m_SelCommName = strCommName;
}


void CEEGCom::InitComm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString ShowWord = _T("���ڴ�������!");
	//SetDlgItemText(IDC_EDIT1, ShowWord);
	
	m_CommHandle = CreateFile(m_SelCommName,                             //�򿪴���
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, NULL, OPEN_EXISTING, //�򿪶����Ǵ���
		0,/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,*/ //�첽��ʽ
		NULL);

	if (m_CommHandle == INVALID_HANDLE_VALUE)//(HANDLE)-1) 
	{
		AfxMessageBox(_T("��COMʧ��"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}

	if (!SetupComm(m_CommHandle, 1024, 1024))   //���뻺����������������Ĵ�С����1024
	{
		AfxMessageBox(_T("���ڻ���������ʧ��"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = 100;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 500;
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	if (!SetCommTimeouts(m_CommHandle, &TimeOuts))	 //���ó�ʱ
	{
		AfxMessageBox(_T("�򿪴���ʧ��"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}

	DCB dcb;
	GetCommState(m_CommHandle, &dcb);
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	//	dcb.BaudRate = CBR_115200; //������Ϊ115200
	dcb.BaudRate = 921600; //������Ϊ921600
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
	bool state = SetCommState(m_CommHandle, &dcb);
	if (state != true)
	{
		AfxMessageBox(_T("�������ݸ�ʽ����ʧ��"), MB_OKCANCEL | MB_ICONQUESTION, 0);
	}
	int i = GetLastError();
	GetCommState(m_CommHandle, &dcb);

	PurgeComm(m_CommHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);//�ڶ�д����֮ǰ����Ҫ��PurgeComm()������ջ��������ú���ԭ�ͣ�
	//timer = SetTimer(1, period, NULL);          //���ö�ʱ��1�Ķ�ʱ���

	m_data.SetCurveCount(8);
	m_data.SetCurveLength(4096);
}

// �������ڶ��߳�
void CEEGCom::Start()
{
//	if (rs_initialize() == RS_SUCC)//�жϻ������Ƿ����ӳɹ�
//{
//
//	if (rs_create_context(rshdtest) == RS_SUCC)
//	{
//		if (rs_login(*rshdtest, "192.168.10.2", 8899) == RS_SUCC)
//		{
//			AfxMessageBox(_T("��е���Ѿ����ӣ�"));
//	
//		}
//	}
//}
	CString str;
	//�ļ����λ�úʹ򿪷�ʽ
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

	m_pCollectADSThread = AfxBeginThread(&CEEGCom::CollectUsbDataFowADSThread, this);	//��ʹ�õ�ADΪADS1278ʱ,ʹ�ô��߳������ݲɼ�;

	m_bIsbeginThread = true;
	m_bIsStartofData = false;

	////////////////////////////////////////////////////////////////////////////////////////////
	//����CCA������̣߳��ڸ��߳��з���ƥ��Ĳο��źŵ�����
	createCCAThread();
	detachCCAThread();
	/////////////////////////////////////////////////////////////////////////////////////////
}

//���ݲɼ��߳�(��Ҫ���ڽ���USB�ϴ���PC������)(ADS1278�汾�߳�);
UINT CEEGCom::CollectUsbDataFowADSThread(LPVOID pParam)
{
	CEEGCom* comm = (CEEGCom*)pParam;
	return comm->CollectUsbDataFowADS();
}

UINT  CEEGCom::CollectUsbDataFowADS()
{
	unsigned char pCollectData[BufferSize] = { 0 };					//�ɼ�������������1���ֽ�Ϊ��С;
	UINT pDataBuffer[ArrSize] = { 0 };
	double EEGBuffer[ArrSize] = { 0 };
	CString str;

	/////////////////////////////////////////////////////////////////////////////////
	//���ڱ�ʾһ������CCA���������������Ƿ񱣴�����
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
		//		WaitForSingleObject(m_hEventForCollect, INFINITE);	//ֻ���¼������ڷ���״̬ʱ�ú����ŷ���,Ȼ��ִ�б������Ĵ���,�ú���һ���������¼�������δ����״̬;
		bool IsDataColOk = false;	//IsDataColOk���ڱ�ʾ���ݲɼ��Ƿ�ɹ�;
		unsigned char Plot_time = 0;					//�ɼ�������������1���ֽ�Ϊ��С;
		DWORD readcount = 0;

		for (int time = 0; time < 1; time++)
		{
			if (pCollectData && pDataBuffer)
			{
				ZeroMemory(pCollectData, BufferSize);						//��մ��ԭʼ���ݻ�����;
				ZeroMemory(pDataBuffer, ArrSize);						//��մ��ƴ�Ӻ�����ݻ�����;

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
										//ʹ��˫�������CCA�ļ��㣬��һ����������������ݵĲɼ�������м���
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
												////����CCA������̣߳��ڸ��߳��з���ƥ��Ĳο��źŵ�����
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
												////����CCA������̣߳��ڸ��߳��з���ƥ��Ĳο��źŵ�����
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
					ZeroMemory(pCollectData, BufferSize);						//��մ��ԭʼ���ݻ�����;
					ZeroMemory(pDataBuffer, ArrSize);						//��մ��ƴ�Ӻ�����ݻ�����;
					IsDataColOk = ReadFile(m_CommHandle, pCollectData, BufferSize, &readcount, NULL);
				}
				if (IsDataColOk)													//�ж����ݲɼ��Ƿ�ɹ�;
				{
					unsigned int j = 0;
					if (!((pCollectData[j] == 0xaa) && pCollectData[j + 1] == 0x55))   //Ѱ�Ұ������λ��
					{
						m_bIsStartofData = false;
						// p_ADSCollect->SetDlgItemText(IDC_EDIT1, _T("���ݶ��벻׼Data_Wrong"));
					}
					else
					{
						// marker = pCollectData[j + 2];
						for (Plot_time = 0; Plot_time < (BufferSize / 27); Plot_time++)
						{
							j = j + 3;
							unsigned int index = 0;										//indexΪƴ�Ӻ�������±�����;

							// fprintf(p_ADSCollect->fmaker, "%d ", pCollectData[j - 1]);

							for (index = 0; index < ArrSize;)									//���´���Ϊ����ƴ�ӷ���;
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
								//ʹ��˫�������CCA�ļ��㣬��һ����������������ݵĲɼ�������м���
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
										////����CCA������̣߳��ڸ��߳��з���ƥ��Ĳο��źŵ�����
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
										////����CCA������̣߳��ڸ��߳��з���ƥ��Ĳο��źŵ�����
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

////4S���һ��ָ��
int CEEGCom::CCAProgress()
{
	int number=0;
	int sum ;
	int tmp;

	while (m_bIsbeginThread)
	{
		if (isEndForCollectData1 && !isEndForCollectData2 &&(*isssvepc))
		{
			//1950S ���һ��ָ��
			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData1);

			//4s���һ��ָ��
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
				//��е��ʵʱ����
				handmove(rshdtest, matchedIndexss, myway, isfanxofc);
				m_outfile9 << *matchedIndexss << " ";
				indexcollect.clear();
			}

			//rshd2;
			//estimate(matchedIndex, passmessage);
			//��е��ʵʱ����
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
				//��е��ʵʱ����
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


//2S���һ��ָ��
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
//			//1950S ���һ��ָ��
//			*matchedIndex = m_MyCCA.getTheIndexOfCCA(forCCAcaculateData1);
//
//			//��е��ʵʱ����
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
//			//��е��ʵʱ����
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
	//�ر��߳�
	m_bIsbeginThread = false;

}
void CEEGCom::handmove(RSHD* myrshd, int* rindex,bool* ways,bool* isfanx) {
	//���������(Ĭ��״̬)
	if (!*isfanx) 
	{
		//�����λ��(Ĭ��״̬)
		if (!*ways)
		{
			if (*rindex == 0) {
				double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//�ص���ʼλ��
							//	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//��һ�����յ�
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 1) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 2) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
			//	AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 3) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 4) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 5) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 6) {
				double point1[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}

		}
		//�����λ��
		else
		{
			if (*rindex == 0) {
				double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//�ص���ʼλ��
							//	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//��һ�����յ�
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
				max_acc.jointPara[1] = 8;
				max_acc.jointPara[2] = 8;
				max_acc.jointPara[3] = 8;
				max_acc.jointPara[4] = 8;
				max_acc.jointPara[5] = 8;
				rs_set_global_joint_maxacc(*rshdtest, &max_acc);
				rs_move_joint(*rshdtest, point1, true);
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 1) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//���2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x + 0.02;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//������Ŀ�����̬
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 2) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//���2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y - 0.02;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//������Ŀ�����̬
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
			//	AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 3) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.7;//���2.59
				max_velc.jointPara[1] = 1.7;
				max_velc.jointPara[2] = 1.7;
				max_velc.jointPara[3] = 1.7;
				max_velc.jointPara[4] = 1.7;
				max_velc.jointPara[5] = 1.7;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 10;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z - 0.02;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//������Ŀ�����̬
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 4) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx + 0.06;//������Ŀ�����̬
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 5) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//������Ŀ�����̬
				rpy0.ry = rpy.ry + 0.06;
				rpy0.rz = rpy.rz;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}
			if (*rindex == 6) {
				double point2[ARM_DOF];
				aubo_robot_namespace::JointVelcAccParam max_velc;
				max_velc.jointPara[0] = 1.5;//���2.59
				max_velc.jointPara[1] = 1.5;
				max_velc.jointPara[2] = 1.5;
				max_velc.jointPara[3] = 1.5;
				max_velc.jointPara[4] = 1.5;
				max_velc.jointPara[5] = 1.5;
				rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

				aubo_robot_namespace::JointVelcAccParam max_acc;
				max_acc.jointPara[0] = 8;//�����17
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
				aubo_robot_namespace::Rpy  rpy0;//ŷ����
				aubo_robot_namespace::Ori  ori0;//��Ԫ��
				aubo_robot_namespace::Ori  ori;//��Ԫ��
				aubo_robot_namespace::Rpy  rpy;//ŷ����
				aubo_robot_namespace::wayPoint_S waypoint0;
				pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
				pos0.y = waypointcurrent.cartPos.position.y;
				pos0.z = waypointcurrent.cartPos.position.z;
				ori.w = waypointcurrent.orientation.w;
				ori.x = waypointcurrent.orientation.x;
				ori.y = waypointcurrent.orientation.y;
				ori.z = waypointcurrent.orientation.z;
				rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
				rpy0.rx = rpy.rx;//������Ŀ�����̬
				rpy0.ry = rpy.ry;
				rpy0.rz = rpy.rz + 0.06;
				rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
				rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
				rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
				//	MessageBox();
				//AfxMessageBox(_T("��е���Ѿ����ܣ�"));

			}

		}
	}
	//����Ƿ���
	else 
     {
	 //�����λ��(Ĭ��״̬)
	 if (!*ways)
	 {
		 if (*rindex == 0) {
			 double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//�ص���ʼλ��
						 //	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//��һ�����յ�
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 1) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 2) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
		 //	AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 3) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 4) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 5) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 6) {
			 double point1[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }

	 }
	 //�����λ��
	 else
	 {
		 if (*rindex == 0) {
			 double point1[ARM_DOF] = { 0.000, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//�ص���ʼλ��
						 //	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//��һ�����յ�
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
			 max_acc.jointPara[1] = 8;
			 max_acc.jointPara[2] = 8;
			 max_acc.jointPara[3] = 8;
			 max_acc.jointPara[4] = 8;
			 max_acc.jointPara[5] = 8;
			 rs_set_global_joint_maxacc(*rshdtest, &max_acc);
			 rs_move_joint(*rshdtest, point1, true);
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 1) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//���2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x - 0.02;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//������Ŀ�����̬
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 2) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//���2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y + 0.02;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//������Ŀ�����̬
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
		 //	AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 3) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.7;//���2.59
			 max_velc.jointPara[1] = 1.7;
			 max_velc.jointPara[2] = 1.7;
			 max_velc.jointPara[3] = 1.7;
			 max_velc.jointPara[4] = 1.7;
			 max_velc.jointPara[5] = 1.7;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 10;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z + 0.02;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//������Ŀ�����̬
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 4) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx - 0.06;//������Ŀ�����̬
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 5) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//������Ŀ�����̬
			 rpy0.ry = rpy.ry - 0.06;
			 rpy0.rz = rpy.rz;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }
		 if (*rindex == 6) {
			 double point2[ARM_DOF];
			 aubo_robot_namespace::JointVelcAccParam max_velc;
			 max_velc.jointPara[0] = 1.5;//���2.59
			 max_velc.jointPara[1] = 1.5;
			 max_velc.jointPara[2] = 1.5;
			 max_velc.jointPara[3] = 1.5;
			 max_velc.jointPara[4] = 1.5;
			 max_velc.jointPara[5] = 1.5;
			 rs_set_global_joint_maxvelc(*rshdtest, &max_velc);

			 aubo_robot_namespace::JointVelcAccParam max_acc;
			 max_acc.jointPara[0] = 8;//�����17
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
			 aubo_robot_namespace::Rpy  rpy0;//ŷ����
			 aubo_robot_namespace::Ori  ori0;//��Ԫ��
			 aubo_robot_namespace::Ori  ori;//��Ԫ��
			 aubo_robot_namespace::Rpy  rpy;//ŷ����
			 aubo_robot_namespace::wayPoint_S waypoint0;
			 pos0.x = waypointcurrent.cartPos.position.x;//�������ƶ�Ŀ���λ��
			 pos0.y = waypointcurrent.cartPos.position.y;
			 pos0.z = waypointcurrent.cartPos.position.z;
			 ori.w = waypointcurrent.orientation.w;
			 ori.x = waypointcurrent.orientation.x;
			 ori.y = waypointcurrent.orientation.y;
			 ori.z = waypointcurrent.orientation.z;
			 rs_quaternion_to_rpy(*rshdtest, &ori, &rpy);
			 rpy0.rx = rpy.rx;//������Ŀ�����̬
			 rpy0.ry = rpy.ry;
			 rpy0.rz = rpy.rz - 0.06;
			 rs_rpy_to_quaternion(*rshdtest, &rpy0, &ori0);//ŷ����ת��Ԫ��
			 rs_inverse_kin(*rshdtest, point2, &pos0, &ori0, &waypoint0);//���
			 rs_move_joint(*rshdtest, waypoint0.jointpos, false);//�������ƶ�
			 //	MessageBox();
			 //AfxMessageBox(_T("��е���Ѿ����ܣ�"));

		 }

	 }




      }
}