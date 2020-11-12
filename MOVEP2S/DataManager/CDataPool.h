#pragma once

#include "CStreamCurveData.h"

// 数据池
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

	// 新生成一个数据区
	// dt：数据类型
	// strName：数据区的名字，用于引用
	void AddData(DataType dt, LPCTSTR strName);

	// 获取一个数据区
	void* GetData(LPCTSTR strName);

protected:
	// 指定名字的下标，不存在返回-1
	int Index(CString strName);

private:
	// 一个数据区有3个值：对象指针/类型/名字
	CPtrArray m_pool;
	CIntArray m_types;
	CStringArray m_names;
};

AFX_EXT_API CDataPool& DataPool();