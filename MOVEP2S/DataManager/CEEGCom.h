#pragma once
#include "pch.h"
//#include "CStreamCurveData.h"

#ifndef SHARED_HANDLERS
#include "CStreamCurveData.h"
#endif


class CEEGCom
{
public:
	//����ѡ�񣬴���һ��CStringArray�����ã����ش��ڵ���Ŀ
	int GetCommNames(CStringArray& names);

	// ����ѡ��Ĵ��ڱ��
	void SelectComm(CString strCommName);

	// �������ڶ��߳�
	void Start();

	//�رմ���
	void CloseComm();

	//���ݲɼ��߳�(��Ҫ���ڽ���USB�ϴ���PC������)(ADS1278�汾�߳�);
	static UINT CollectUsbDataFowADSThread(LPVOID pParam);

public:
	//��ʼ������
	void InitComm();
	//�̶߳���������
	UINT  CollectUsbDataFowADS();


public:
	//��������
	CString m_SelCommName;

	// �Ѿ��򿪵Ĵ��ڵľ��
	HANDLE m_CommHandle;

	// �����ݵ��߳�
	CWinThread* m_pCollectADSThread;

	// ͬ���߳���
	HANDLE m_hEventForCollect;
	//�����ռ��̱߳�־λ
	bool m_bIsbeginThread;
	//IsDataColOk���ڱ�ʾ���ݲɼ��Ƿ�ɹ�;
	bool m_bIsStartofData;

	// �����ݵĵط�
	CStreamCurveData m_data;
	//���ڴ�����ݵ��ļ�
	CStdioFile m_outfile;
};

