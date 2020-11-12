#pragma once

#include "DataManager.h"

// 曲线文件数据
//

class AFX_EXT_CLASS CCurveData : public CDataManager
{
public:
	CCurveData(void);
	virtual ~CCurveData(void);

	virtual void ReleaseBuffer();

	virtual BOOL ReadDataFile(LPCTSTR lpszFilePath, int nBeginFrame = 0, int nEndFrame = 0, int nInterval = 1){return FALSE;}

	// nCurveIndex : based 0
	int GetCurve(int nCurveIndex, double* buffer, int nMaxPointCount);
	int GetCurveAllData(int nCurveIndex, double* buffer, int nMaxPointCount);

	// 得到[beginPos,endPos]之间的曲线，最大点数为nMaxPointCount，超过要跳帧
	// 返回实际copy的点数
	int GetLimitCurve(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount);

	// 得到[beginPos,endPos]之间的曲线，最大点数为nMaxPointCount，不跳帧
	// 返回实际copy的点数
	int GetCurveData(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount);

	int* GetPointX(int beginPos);
	int GetPointXAt(int nIndex);
	double GetValueAt(int nCurveIndex, int nIndex);

	inline int CurveCount(){return m_nCurveCount;}

	inline int BeginFrame(){return m_nBeginFrame;}
	inline int EndFrame(){return m_nEndFrame;}

protected:
	int* m_pPointX;	// 曲线中每个点对应的实际点位置（在数据中的帧数）
					// 一个数据器中所有的点都有相同的X值

	INT32 m_nCurveCount;
	CArray<double*> m_CurveDataArray;	// 曲线每个点的Y值

	int m_nBeginFrame;
	int m_nEndFrame;
};
