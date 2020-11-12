#include "pch.h"
#include "CStreamCurveData.h"

CStreamCurveData::CStreamCurveData(void)
{
	m_nCurveLength = 0;
}

CStreamCurveData::~CStreamCurveData(void)
{
	ReleaseBuffer();
}

// ������Դ
void CStreamCurveData::ReleaseBuffer()
{
	if (m_CurveDataArray.GetCount() > 0)
	{
		for (int i = 0; i < m_CurveDataArray.GetCount(); i ++)
		{
			SAFE_DELETE_ARY(m_CurveDataArray.GetAt(i));
		}
		m_CurveDataArray.RemoveAll();
	}
}

// ����ֵȫ����0
void CStreamCurveData::ClearCurves()
{
	for (int i = 0; i < m_CurveDataArray.GetCount(); i++)
	{
		if (m_CurveDataArray[i])
		{
			ZeroMemory(m_CurveDataArray[i], sizeof(double) * m_nCurveLength);
		}
	}
}

// �������߳��ȣ�����������ÿ������һ��
void CStreamCurveData::SetCurveLength(int nLen)
{
	VERIFY(nLen > 0);

	m_nCurveLength = nLen;
	for (int i = 0; i < m_CurveDataArray.GetCount(); i++)
	{
		SAFE_DELETE_ARY(m_CurveDataArray[i]);
		m_CurveDataArray[i] = new double[m_nCurveLength];
		if (!m_CurveDataArray[i])
		{
			AfxMessageBox(_T("�ڴ�������"));
			m_nCurveLength = 0;
			ReleaseBuffer();
		}
		ZeroMemory(m_CurveDataArray[i], sizeof(double) * m_nCurveLength);
	}
}
inline int CStreamCurveData::GetCurveLength()
{
	return m_nCurveLength;
}

// ������������
void CStreamCurveData::SetCurveCount(int nCount)
{
	VERIFY(nCount >= 0);
	if (m_CurveDataArray.GetCount() == nCount) return;
	ReleaseBuffer();

	m_CurveDataArray.SetSize(nCount);
	if (m_nCurveLength <= 0)
	{
		for (int i = 0; i < m_CurveDataArray.GetCount(); i++)
		{
			m_CurveDataArray[i] = NULL;
		}
	}
	else
		SetCurveLength(m_nCurveLength);
}
inline int CStreamCurveData::GetCurveCount()
{
	return m_CurveDataArray.GetCount();
}

// ��ĳ����������һ�����ݣ�based 0��
void CStreamCurveData::AddValue(int nCurve, double dbValue)
{
	VERIFY(nCurve >= 0 && nCurve < m_CurveDataArray.GetCount());

	double* p = m_CurveDataArray[nCurve];
	if (!p) return;
	
	MoveMemory(&p[0], &p[1], sizeof(double) * (m_nCurveLength - 1));
	p[m_nCurveLength - 1] = dbValue;
}

// ��ȡһ�����ߵ�������
double* CStreamCurveData::GetCurveData(int nCurve)
{
	VERIFY(nCurve >= 0 && nCurve < m_CurveDataArray.GetCount());

	return m_CurveDataArray[nCurve];
}

double CStreamCurveData::GetLastData(int nCurve)
{
	VERIFY(nCurve >= 0 && nCurve < m_CurveDataArray.GetCount());

	if (m_nCurveLength > 0)
		return m_CurveDataArray[nCurve][m_nCurveLength - 1];

	return 0.0;
}