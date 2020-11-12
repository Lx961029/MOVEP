#pragma once

#include "CStreamCurveData.h"

// ���ݳ�
//

enum DataType {
	StreamCurveData
};

class AFX_EXT_CLASS CDataPool
{
public:
	CDataPool(void);
	~CDataPool(void);

	static CDataPool instance;

	// ������һ��������
	// dt����������
	// strName�������������֣���������
	void AddData(DataType dt, LPCTSTR strName);

	// ��ȡһ��������
	void* GetData(LPCTSTR strName);

protected:
	// ָ�����ֵ��±꣬�����ڷ���-1
	int Index(CString strName);

private:
	// һ����������3��ֵ������ָ��/����/����
	CPtrArray m_pool;
	CIntArray m_types;
	CStringArray m_names;
};

AFX_EXT_API CDataPool& DataPool();