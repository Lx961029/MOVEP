#pragma once

#include "..\UserDef\UserDef.h"

// CStreamCurve
// （一条）流数据的曲线，不停地接收数据显示
//

class AFX_EXT_CLASS CStreamCurve
{
public:
	CStreamCurve(void);
	virtual ~CStreamCurve(void);

	// 清理内存
	void ReleaseBuffer();

	// 设置曲线长度
	void SetCurveLength(int nLength);

	// 设置画布大小
	void SetCanvasWidth(int wid);
	void SetCanvasHeight(int hei);
	void SetCanvasSize(int wid, int hei);

	// 设置坐标上下限
	void SetCeil(double dbCeil);
	void SetFloor(double dbFloor);
	void SetLimitValue(double dbCeil, double dbFloor, BOOL bErase = TRUE);

	// 设置一条曲线
	// 如果给出的长度小于设定长度，旧曲线整体左移，新曲线在右半截
	// 如果给出的长度大于设定长度，只接收右半截
	void SetCurveValues(double* pOrgValueBuffer, int nValueCount);

	// 向曲线最后增加一个点的值
	void AddValue(double newValue);

	// 绘制出曲线
	void DrawCurve(Graphics* graphics, Color bkColor);

protected:
	// 画布大小或者曲线内容有变化，导致需要刷新曲线坐标
	void CanvasWidthChanged();
	void CanvasHeightChanged();

private:
	// 原始值
	double* m_pOrgValues;	// 曲线的原始值
	double m_dbCeil;		// 坐标轴上限
	double m_dbFloor;		// 坐标轴下限

	// 曲线
	int m_nPointCount;		// 曲线上有多少个点
	PointF* m_pPoints;		// 曲线各个点在窗口中的坐标
	Pen* m_pen;				// 画笔

	// 画布
	int m_nCanvasWidth;		// 画布宽度
	int m_nCanvasHeight;	// 画布高度
};

