#include "pch.h"
#include "CStreamCurve.h"

CStreamCurve::CStreamCurve(void)
{
	m_pOrgValues = NULL;
	m_dbCeil = 1.0;
	m_dbFloor = 0;

	m_nPointCount = 0;
	m_pPoints = NULL;
	m_pen = new Pen(Color::Red, 1.0f);	// 画笔

	m_nCanvasWidth = 0;
	m_nCanvasHeight = 0;
}

CStreamCurve::~CStreamCurve(void)
{
	ReleaseBuffer();
}

// 清理内存
void CStreamCurve::ReleaseBuffer()
{
	SAFE_DELETE(m_pen);
	SAFE_DELETE_ARY(m_pOrgValues);
	SAFE_DELETE_ARY(m_pPoints);
}

// 设置曲线长度
void CStreamCurve::SetCurveLength(int nLength)
{
	// 没变化
	if (nLength == m_nPointCount)
	{
		return;
	}

	// 有变化
	m_nPointCount = nLength;

	// 原始值
	SAFE_DELETE_ARY(m_pOrgValues);
	m_pOrgValues = new double[m_nPointCount];
	if (!m_pOrgValues)
	{
		AfxMessageBox(_T("Memory Error!"));
		return;
	}

	// 曲线
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

// 设置画布大小
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



//根据采集到的数据的大小，自动调整画布的高度
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

// 画布大小或者曲线内容有变化，导致需要刷新曲线坐标
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



// 设置坐标上下限
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

// 设置一条曲线
// 如果给出的长度小于设定长度，旧曲线整体左移，新曲线在右半截
// 如果给出的长度大于设定长度，只接收右半截
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

// 向曲线最后增加一个点的值
void CStreamCurve::AddValue(double newValue)
{
	// 原始值
	MoveMemory(m_pOrgValues, &m_pOrgValues[1], sizeof(double) * (m_nPointCount - 1));
	m_pOrgValues[m_nPointCount - 1] = newValue;

	// 坐标值
	for (int i = 0; i < m_nPointCount - 1; i ++)
	{
		m_pPoints[i].Y = m_pPoints[i + 1].Y;
	}

	double dif = m_dbCeil - m_dbFloor;
	double scale = dif > 0 ? m_nCanvasHeight / dif : 1.0;
	m_pPoints[m_nPointCount - 1].Y = (REAL)((newValue - m_dbFloor) * scale);
}

// 绘制出曲线
void CStreamCurve::DrawCurve(Graphics* graphics, Color bkColor)
{
	if (m_pPoints)
	{
		graphics->Clear(bkColor);
		graphics->DrawLines(m_pen, m_pPoints, m_nPointCount);
	}
}
