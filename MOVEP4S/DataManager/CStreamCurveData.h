#pragma once

#include "DataManager.h"

// ��������������
// ���߳�ʼֵȫΪ0��������������ĩβ
//

class AFX_EXT_CLASS CStreamCurveData
{
public:
	CStreamCurveData(void);
	virtual ~CStreamCurveData(void);

	// ������Դ
	void ReleaseBuffer();

	// ����ֵȫ����0
	void ClearCurves();

	// �������߳��ȣ�����������ÿ������һ��
	void SetCurveLength(int nLen);
	inline int GetCurveLength();

	// ������������
	void SetCurveCount(int nCount);
	inline int GetCurveCount();

	// ��ĳ����������һ�����ݣ�based 0��
	void AddValue(int nCurve, double dbValue);

	// ��ȡһ�����ߵ�������
	double* GetCurveData(int nCurve);

	// ��ȡ���һ������
	double GetLastData(int nCurve);

private:
	int m_nCurveLength;
	CArray<double*> m_CurveDataArray;	// ����ÿ�����Yֵ
};

