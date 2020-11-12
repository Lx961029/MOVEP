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
#define mydatalength 1950   //1000������  1.95s���� �ɸ�����Ҫ�޸�
//extern int mydatalengtest=1950;
class CEEGCom
{
public:
	//���캯��
	CEEGCom();
	//��������
	~CEEGCom();
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
	//��ʼ������
	void InitComm();
	//�̶߳���������
	UINT  CollectUsbDataFowADS();
	//��е�ֱ��˶�����

	void handmove(RSHD *rshdtest, int* rindex,bool* ways,bool* isfanx);


public:

	//��������
	CString m_SelCommName;
	//��е�ֱ۾��
	RSHD *rshdtest;
	bool* myway;
	bool* isfanxofc;
	// �Ѿ��򿪵Ĵ��ڵľ��
	HANDLE m_CommHandle;

	// �����ݵ��߳�
	CWinThread* m_pCollectADSThread;

	//�����ռ��̱߳�־λ
	 bool m_bIsbeginThread;
	//IsDataColOk���ڱ�ʾ���ݲɼ��Ƿ�ɹ�;
	bool m_bIsStartofData;
    //
	bool isssvep=false;
	//
	bool ismovep = false;
	//CCA����󷵻ص�ƥ��Ĳο��źŵ�����
	int *matchedIndex;
	int * matchedIndexss;
	bool* isssvepc;
	//int maindex = 0;
	// �����ݵĵط�
	CStreamCurveData m_data;
	//���ڴ�����ݵ��ļ�
	CStdioFile m_outfile;
	CStdioFile m_outfile1;
	CStdioFile m_outfile2;
	CStdioFile m_outfile3;
	CStdioFile m_outfile4;
	CStdioFile m_outfile5;
	CStdioFile m_outfile6;
	CStdioFile m_outfile7;
	CStdioFile m_outfile8;

	vector<int>indexcollect;//�������ָ��

	//////////////////////////////////////////////////////////////////////
	//���CCA����Ľ��
	ofstream m_outfile9;
	int mylenthofcom;

	//����CCA�������Ҫ����
	int CCAProgress();

private:

	//��������CCA������߳�
	int createCCAThread();
	//ʹ��CCA�̺߳����̷߳���
	int detachCCAThread();
	//����CCA������߳�
	std::thread* CCAThread{ nullptr };
	

	//�˸�IIR�˲���������8��ͨ�����ݵ��˲�
	Filter_IIR<double>* filter_IIR[8];

	//����CCA����Ķ���
	MyCCA m_MyCCA;
	//Cssvep����
	//CSSVEP m_ssvep;
	//���ڻ�ȡ���µ��Ե����ݣ�CCA�������ʱ����һ
	double forCCAcaculateData1[8][mydatalength] = { 0 };
	//���ڻ�ȡ���µ��Ե����ݣ�CCA�������ʱ�����
	double forCCAcaculateData2[8][mydatalength] = { 0 };
	//��־forCCAcaculateData1[15600]��������Ե�����
	bool isTimeForCollectData1 = false;
	//��־forCCAcaculateData2[15600]��������Ե�����
	bool isTimeForCollectData2 = false;
	//��ʾforCCAcaculateData1[15600]�������ݲɼ����
	bool isEndForCollectData1 = false;
	//��ʾforCCAcaculateData2[15600]�������ݲɼ����
	bool isEndForCollectData2 = false;
	//���ڱ�ʾһ������CCA���������������Ƿ�ɼ�����
	int saveTimes = 0;
	int msavetimes;
	//////////////////////////////////////////////////////////////////////
};

