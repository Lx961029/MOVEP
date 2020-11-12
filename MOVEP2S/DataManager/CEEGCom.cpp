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
	if (m_CommHandle == (HANDLE)-1) {
		//MessageBox(_T("��COMʧ��!"));
		cout << "��COMʧ��!" << endl;
	}

	if (!SetupComm(m_CommHandle, 1024, 1024))   //���뻺����������������Ĵ�С����1024
	{
		//MessageBox(_T("���ڻ���������ʧ��!"));
		cout << "���ڻ���������ʧ��!" << endl;
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
		//MessageBox(_T("���ڳ�ʱ����ʧ��!"));
		cout << "���ڳ�ʱ����ʧ��!" << endl;
	}

	DCB dcb;
	GetCommState(m_CommHandle, &dcb);
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	//	dcb.BaudRate = CBR_115200; //������Ϊ115200
	dcb.BaudRate = 921600; //������Ϊ115200
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
	bool state = SetCommState(m_CommHandle, &dcb);
	if (state != true)
	{
		//MessageBox(_T("�������ݸ�ʽ����ʧ��!"));
		cout << "�������ݸ�ʽ����ʧ��!" << endl;
	}
	int i = GetLastError();
	GetCommState(m_CommHandle, &dcb);

	PurgeComm(m_CommHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);//�ڶ�д����֮ǰ����Ҫ��PurgeComm()������ջ��������ú���ԭ�ͣ�
	//timer = SetTimer(1, period, NULL);          //���ö�ʱ��1�Ķ�ʱ���

	m_data.SetCurveCount(8);
	m_data.SetCurveLength(1000);
}

// �������ڶ��߳�
void CEEGCom::Start()
{
	//�ļ����λ�úʹ򿪷�ʽ
	m_outfile.Open(_T("1.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);


	InitComm();

	m_hEventForCollect = CreateEventA(NULL, FALSE, FALSE, NULL);				//����һ���������Զ������¼�����ʹ��Ĭ�ϵİ�ȫ�ԣ�����δ����״̬;
	m_pCollectADSThread = AfxBeginThread(&CEEGCom::CollectUsbDataFowADSThread, this);	//��ʹ�õ�ADΪADS1278ʱ,ʹ�ô��߳������ݲɼ�;

	SetEvent(m_hEventForCollect);
	m_bIsbeginThread = true;
	m_bIsStartofData = false;
}

//���ݲɼ��߳�(��Ҫ���ڽ���USB�ϴ���PC������)(ADS1278�汾�߳�);
UINT CEEGCom::CollectUsbDataFowADSThread(LPVOID pParam)
{
	CEEGCom* comm = (CEEGCom*)pParam;
	//long bufflen = BufferSize;
	return comm->CollectUsbDataFowADS();
}

UINT  CEEGCom::CollectUsbDataFowADS()
{
	unsigned char pCollectData[BufferSize] = { 0 };					//�ɼ�������������1���ֽ�Ϊ��С;
	UINT pDataBuffer[ArrSize] = { 0 };
	float EEGBuffer[ArrSize] = { 0 };
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
	//�ر��߳�
	m_bIsbeginThread = false;
	//�ر��ļ�
	m_outfile.Close();
}

