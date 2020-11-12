#pragma once

#include "..\UserDef\UserDef.h"

// CStreamCurve
// ��һ���������ݵ����ߣ���ͣ�ؽ���������ʾ
//

class AFX_EXT_CLASS CStreamCurve
{
public:
	CStreamCurve(void);
	virtual ~CStreamCurve(void);

	// �����ڴ�
	void ReleaseBuffer();

	// �������߳���
	void SetCurveLength(int nLength);

	// ���û�����С
	void SetCanvasWidth(int wid);
	void SetCanvasHeight(int hei);
	void SetCanvasSize(int wid, int hei);

	// ��������������
	void SetCeil(double dbCeil);
	void SetFloor(double dbFloor);
	void SetLimitValue(double dbCeil, double dbFloor, BOOL bErase = TRUE);

	// ����һ������
	// ��������ĳ���С���趨���ȣ��������������ƣ����������Ұ��
	// ��������ĳ��ȴ����趨���ȣ�ֻ�����Ұ��
	void SetCurveValues(double* pOrgValueBuffer, int nValueCount);

	// �������������һ�����ֵ
	void AddValue(double newValue);

	// ���Ƴ�����
	void DrawCurve(Graphics* graphics, Color bkColor);

protected:
	// ������С�������������б仯��������Ҫˢ����������
	void CanvasWidthChanged();
	void CanvasHeightChanged();

private:
	// ԭʼֵ
	double* m_pOrgValues;	// ���ߵ�ԭʼֵ
	double m_dbCeil;		// ����������
	double m_dbFloor;		// ����������

	// ����
	int m_nPointCount;		// �������ж��ٸ���
	PointF* m_pPoints;		// ���߸������ڴ����е�����
	Pen* m_pen;				// ����

	// ����
	int m_nCanvasWidth;		// �������
	int m_nCanvasHeight;	// �����߶�
};

