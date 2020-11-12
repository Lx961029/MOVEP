#include "pch.h"
#include "CurveData.h"

#include "..\UserDef\UserDef.h"

CCurveData::CCurveData(void)
{
	m_pPointX = NULL;

	m_nFrameCount = 0;
	m_nCurveCount = 0;
	m_nCurrentFrame = 0;	// based 1

	m_nBeginFrame = 0;
	m_nEndFrame = 0;
}

CCurveData::~CCurveData(void)
{
	ReleaseBuffer();
}

void CCurveData::ReleaseBuffer()
{
	m_nFrameCount = 0;
	m_nCurveCount = 0;
	m_nCurrentFrame = 0;	// based 1

	double* p = NULL;
	for (int i = 0; i < m_CurveDataArray.GetCount(); i ++)
	{
		p = (double*)m_CurveDataArray[i];
		SAFE_DELETE_ARY(p);
	}
	m_CurveDataArray.RemoveAll();

	SAFE_DELETE_ARY(m_pPointX);
}

// BOOL CCurveData::ReadDataFile(LPCTSTR lpszFilePath)
// {
// 	ReleaseBuffer();
// 
// 	CFile fIn;
// 	if (!fIn.Open(lpszFilePath, CFile::modeRead | CFile::typeBinary))
// 	{
// 		AfxMessageBox(_T("�������ļ�ʧ��!"));
// 		return FALSE;
// 	}
// 
// 	fIn.Read(&m_nFrameCount, sizeof(m_nFrameCount));
// 	fIn.Read(&m_nCurveCount, sizeof(m_nCurveCount));
// 	if (0 == m_nFrameCount || 0 == m_nCurveCount)
// 	{
// 		AfxMessageBox(_T("�����ļ�����"));
// 		fIn.Close();
// 		return FALSE;
// 	}
// 
// 	// �����д�����ݷ�����������
// 	m_CurveDataArray.SetSize(m_nCurveCount);
// 	int curvebuffersize = m_nFrameCount * sizeof(double);
// 	double * p = NULL;
// 	for (int i = 0; i < m_nCurveCount; i ++)
// 	{
// 		p = new double[m_nFrameCount];
// 		if (!p)
// 		{
// 			AfxMessageBox(_T("�ڴ�������"));
// 			fIn.Close();
// 			ReleaseBuffer();
// 			return FALSE;
// 		}
// 		ZeroMemory(p, curvebuffersize);
// 		m_CurveDataArray[i] = p;
// 	}
// 
// 	p = new double[m_nCurveCount];
// 	int framebuffersize = m_nCurveCount * sizeof(double);
// 	for (int i = 0; i < m_nFrameCount; i ++)
// 	{
// 		if (!p)
// 		{
// 			AfxMessageBox(_T("�ڴ�������"));
// 			fIn.Close();
// 			ReleaseBuffer();
// 			return FALSE;
// 		}
// 		ZeroMemory(p, framebuffersize);
// 
// 		fIn.Read(p, framebuffersize);
// 		for (int k = 0; k < m_CurveDataArray.GetCount(); k ++)
// 		{
// 			((double*)m_CurveDataArray[k])[i] = p[k];
// 		}
// 	}
// 	delete [] p;
// 
// 	fIn.Close();
// 
// 	m_nCurrentFrame = 0;
// 
// 	return TRUE;
// }

int CCurveData::GetCurve(int nCurveIndex, double* buffer, int nMaxPointCount)
{
	if (0 == m_nCurrentFrame)
	{
		return 0;
	}

	int n = __min(m_nCurrentFrame, nMaxPointCount);
	double* p = (double*)m_CurveDataArray[nCurveIndex];
	CopyMemory(buffer, &p[m_nCurrentFrame - n], sizeof(double) * n);

	return n;
}
int CCurveData::GetCurveAllData(int nCurveIndex, double* buffer, int nMaxPointCount)
{
	int n = __min(FrameCount(), nMaxPointCount);
	double* p = (double*)m_CurveDataArray[nCurveIndex];
	CopyMemory(buffer, p, sizeof(double) * n);

	return n;
}
int CCurveData::GetLimitCurve(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount)
{
	if (nCurveIndex < 0 || nCurveIndex > m_CurveDataArray.GetUpperBound()
		|| beginPos < 0 || endPos < beginPos || nMaxPointCount <= 0)
	{
		return 0;
	}

	double* p = (double*)m_CurveDataArray[nCurveIndex];

	int pointcount = endPos - beginPos + 1;
	pointcount = __min(pointcount, FrameCount());
	// ������֡��ֱ��copy
	if (pointcount <= nMaxPointCount)
	{
		CopyMemory(buffer, &p[beginPos], sizeof(double) * pointcount);

		return pointcount;
	}

	// ��Ҫ��֡��Ϊ��׼ȷ�����Ǽ򵥵���֡���Ƿ��鴦����¼ÿ��������Сֵ
	int group = nMaxPointCount / 2;
	double v = 0.0, mx = 0.0, mn = 0.0;
	int mxpos = 0, mnpos = 0;
	int bpos = beginPos;
	int epos = __min(endPos, bpos + (int)(pointcount / (double)group + 0.5));
	for (int i = 0; i < group; i ++)
	{
		mx = p[bpos];
		mn = p[bpos];
		mxpos = bpos;
		mnpos = bpos;
		for (int k = bpos + 1; k <= epos; k ++)
		{
			if (p[k] > mx)
			{
				mx = p[k];
				mxpos = k;
			}
			else if (p[k] < mn)
			{
				mn = p[k];
				mnpos = k;
			}
		}

		if (mnpos < mxpos)
		{
			buffer[i * 2] = mn;
			buffer[i * 2 + 1] = mx;
		}
		else
		{
			buffer[i * 2] = mx;
			buffer[i * 2 + 1] = mn;
		}
		bpos = epos + 1;
		epos = __min(endPos, beginPos + (int)(pointcount / (double)group * (i + 1) + 0.5));
	}

	// ���һ����
	if (nMaxPointCount % 2 != 0)
	{
		buffer[nMaxPointCount - 1] = buffer[nMaxPointCount - 2];
	}

	return nMaxPointCount;
}

int CCurveData::GetCurveData(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount)
{
	if (nCurveIndex < 0 || nCurveIndex > m_CurveDataArray.GetUpperBound()
		|| beginPos < 0 || endPos < beginPos || nMaxPointCount <= 0)
	{
		return 0;
	}

	double* p = (double*)m_CurveDataArray[nCurveIndex];

	int pointcount = endPos - beginPos + 1;
	VERIFY(pointcount <= nMaxPointCount);
	pointcount = __min(pointcount, FrameCount());
	// ����֡��ֱ��copy
	CopyMemory(buffer, &p[beginPos], sizeof(double) * pointcount);

	return pointcount;
}

int* CCurveData::GetPointX(int beginPos)
{
	return &m_pPointX[beginPos];
}
int CCurveData::GetPointXAt(int nIndex)
{
	return m_pPointX[nIndex];
}
double CCurveData::GetValueAt(int nCurveIndex, int nIndex)
{
	double* p = (double*)m_CurveDataArray[nCurveIndex];
	return p[nIndex];
}
