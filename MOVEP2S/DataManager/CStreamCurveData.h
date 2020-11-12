#pragma once

#include "DataManager.h"

// 流数据曲线数据
// 曲线初始值全为0，新增的数据在末尾
//

class AFX_EXT_CLASS CStreamCurveData
{
public:
	CStreamCurveData(void);
	virtual ~CStreamCurveData(void);

	// 清理资源
	void ReleaseBuffer();

	// 曲线值全部置0
	void ClearCurves();

	// 设置曲线长度（即点数），每条曲线一样
	void SetCurveLength(int nLen);
	inline int GetCurveLength();

	// 设置曲线数量
	void SetCurveCount(int nCount);
	inline int GetCurveCount();

	// 向某条曲线新增一个数据（based 0）
	void AddValue(int nCurve, double dbValue);

	// 获取一条曲线的数据区
	double* GetCurveData(int nCurve);

	// 获取最后一个数据
	double GetLastData(int nCurve);

private:
	int m_nCurveLength;
	CArray<double*> m_CurveDataArray;	// 曲线每个点的Y值
};

