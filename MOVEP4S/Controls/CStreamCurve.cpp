#include "pch.h"
#include "CStreamCurve.h"

CStreamCurve::CStreamCurve(void)
{
	m_pOrgValues = NULL;
	m_dbCeil = 1.0;
	m_dbFloor = 0;

	m_nPointCount = 0;
	m_pPoints = NULL;
	m_pen = new Pen(Color::Red, 1.0f);	// ����

	m_nCanvasWidth = 0;
	m_nCanvasHeight = 0;
}

CStreamCurve::~CStreamCurve(void)
{
	ReleaseBuffer();
}

// �����ڴ�
void CStreamCurve::ReleaseBuffer()
{
	SAFE_DELETE(m_pen);
	SAFE_DELETE_ARY(m_pOrgValues);
	SAFE_DELETE_ARY(m_pPoints);
}

// �������߳���
void CStreamCurve::SetCurveLength(int nLength)
{
	// û�仯
	if (nLength == m_nPointCount)
	{
		return;
	}

	// �б仯
	m_nPointCount = nLength;

	// ԭʼֵ
	SAFE_DELETE_ARY(m_pOrgValues);
	m_pOrgValues = new double[m_nPointCount];
	if (!m_pOrgValues)
	{
		AfxMessageBox(_T("Memory Error!"));
		return;
	}

	// ����
	SAFE_DELETE_ARY(m_pPoints);
	m_pPoints = new PointF[m_nPointCount];
	if (!m_pPoints)
	{
		AfxMessageBox(_T("Memory Error!"));
		SAFE_DELETE_ARY(m_pOrgValues);
		return;
	}

	CanvasWidthChanged();
}

// ���û�����С
void CStreamCurve::SetCanvasWidth(int wid)
{
	m_nCanvasWidth = wid;
	CanvasWidthChanged();
}
void CStreamCurve::SetCanvasHeight(int hei)
{
	m_nCanvasHeight = hei;
	CanvasHeightChanged();
}



//���ݲɼ��������ݵĴ�С���Զ����������ĸ߶�
//void CStreamCurve::CanvasHeightChanged()
//{
//	double dif = m_dbCeil - m_dbFloor;
//	double scale = dif > 0 ? (m_nCanvasHeight * 0.9) / dif : 1.0;
//	for (int i = 0; i < m_nPointCount; i++)
//	{
//		m_pPoints[i].Y = (REAL)(m_nCanvasHeight * 0.05 + (m_pOrgValues[i] - m_dbFloor) * scale);
//	}
//}

void CStreamCurve::CanvasHeightChanged()
{
	//if (m_coordinate == 0)
	if (1)
	{
		//	int length = sizeof(m_pPoints[n]) / sizeof(m_pPoints[0]);
		int length = m_nPointCount;
		double max = 0, min = 0;
		min=m_pOrgValues[0];
		max=m_pOrgValues[0];
		for (int i = 0; i < m_nPointCount; i++)
		{
			if (m_pOrgValues[i] > max)
				max = m_pOrgValues[i];
			else if (m_pOrgValues[i] < min)
				min = m_pOrgValues[i];
		}
		if ((max - min) != 0)
		{
			m_dbCeil = max;
			m_dbFloor = min;
		}
		else
		{
			m_dbCeil = 10.0;
			m_dbFloor = -10.0;
		}
	}
	//	else if (m_coordinate == 1) 
	//{
	//      }
	double dif = m_dbCeil - m_dbFloor;
	double scale = dif > 0 ? (m_nCanvasHeight * 0.9) / dif : 1.0;
	for (int i = 0; i < m_nPointCount; i++)
	{
		m_pPoints[i].Y = (REAL)(m_nCanvasHeight * 0.05 + (m_pOrgValues[i] - m_dbFloor) * scale);
	}
}

void CStreamCurve::SetCanvasSize(int wid, int hei)
{
	SetCanvasWidth(wid);
	SetCanvasHeight(hei);
}

// ������С�������������б仯��������Ҫˢ����������
void CStreamCurve::CanvasWidthChanged()
{
	if (m_nPointCount <= 0)
	{
		return;
	}
	for (int i = 0; i < m_nPointCount; i++)
	{
		m_pPoints[i].X = (REAL)(( long double)i * m_nCanvasWidth / ((long double)m_nPointCount - 1));
	}
}



// ��������������
void CStreamCurve::SetCeil(double dbCeil)
{
	if (m_dbCeil != dbCeil)
	{
		m_dbCeil = dbCeil;
		CanvasHeightChanged();
	}
}
void CStreamCurve::SetFloor(double dbFloor)
{
	if (m_dbFloor != dbFloor)
	{
		m_dbFloor = dbFloor;
		CanvasHeightChanged();
	}
}
void CStreamCurve::SetLimitValue(double dbCeil, double dbFloor, BOOL bErase /*= FALSE*/)
{
	if ((m_dbCeil == dbCeil && m_dbFloor == dbFloor))
	{
		return;
	}
	m_dbCeil = dbCeil;
	m_dbFloor = dbFloor;

	if (bErase)	CanvasHeightChanged();
}

// ����һ������
// ��������ĳ���С���趨���ȣ��������������ƣ����������Ұ��
// ��������ĳ��ȴ����趨���ȣ�ֻ�����Ұ��
void CStreamCurve::SetCurveValues(double* pOrgValueBuffer, int nValueCount)
{
	if (nValueCount < m_nPointCount)
	{
		MoveMemory(m_pOrgValues, &m_pOrgValues[nValueCount], sizeof(double) * (m_nPointCount - nValueCount));
		CopyMemory(&m_pOrgValues[m_nPointCount - nValueCount], pOrgValueBuffer, sizeof(double) * nValueCount);
	}
	else
	{
		CopyMemory(m_pOrgValues, &pOrgValueBuffer[nValueCount - m_nPointCount], sizeof(double) * m_nPointCount);
	}
	CanvasHeightChanged();
}

// �������������һ�����ֵ
void CStreamCurve::AddValue(double newValue)
{
	// ԭʼֵ
	MoveMemory(m_pOrgValues, &m_pOrgValues[1], sizeof(double) * (m_nPointCount - 1));
	m_pOrgValues[m_nPointCount - 1] = newValue;

	// ����ֵ
	for (int i = 0; i < m_nPointCount - 1; i ++)
	{
		m_pPoints[i].Y = m_pPoints[i + 1].Y;
	}

	double dif = m_dbCeil - m_dbFloor;
	double scale = dif > 0 ? m_nCanvasHeight / dif : 1.0;
	m_pPoints[m_nPointCount - 1].Y = (REAL)((newValue - m_dbFloor) * scale);
}

// ���Ƴ�����
void CStreamCurve::DrawCurve(Graphics* graphics, Color bkColor)
{
	if (m_pPoints)
	{
		graphics->Clear(bkColor);
		graphics->DrawLines(m_pen, m_pPoints, m_nPointCount);
	}
}
