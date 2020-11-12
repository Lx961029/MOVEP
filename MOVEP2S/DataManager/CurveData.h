#pragma once

#include "DataManager.h"

// �����ļ�����
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

	// �õ�[beginPos,endPos]֮������ߣ�������ΪnMaxPointCount������Ҫ��֡
	// ����ʵ��copy�ĵ���
	int GetLimitCurve(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount);

	// �õ�[beginPos,endPos]֮������ߣ�������ΪnMaxPointCount������֡
	// ����ʵ��copy�ĵ���
	int GetCurveData(int nCurveIndex, int beginPos, int endPos, double* buffer, int nMaxPointCount);

	int* GetPointX(int beginPos);
	int GetPointXAt(int nIndex);
	double GetValueAt(int nCurveIndex, int nIndex);

	inline int CurveCount(){return m_nCurveCount;}

	inline int BeginFrame(){return m_nBeginFrame;}
	inline int EndFrame(){return m_nEndFrame;}

protected:
	int* m_pPointX;	// ������ÿ�����Ӧ��ʵ�ʵ�λ�ã��������е�֡����
					// һ�������������еĵ㶼����ͬ��Xֵ

	INT32 m_nCurveCount;
	CArray<double*> m_CurveDataArray;	// ����ÿ�����Yֵ

	int m_nBeginFrame;
	int m_nEndFrame;
};
