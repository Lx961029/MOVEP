#include "pch.h"
#include "CDataPool.h"

CDataPool& DataPool()
{
	return CDataPool::instance;
}

CDataPool CDataPool::instance;

CDataPool::CDataPool(void)
{
}
CDataPool::~CDataPool(void)
{
	for (int i = 0; i < m_pool.GetCount(); i ++)
	{
		switch (m_types[i])
		{
		case StreamCurveData:
			delete ((CStreamCurveData*)m_pool[i]);
			break;
		}
	}
	m_pool.RemoveAll();
	m_types.RemoveAll();
	m_names.RemoveAll();
}

// 新生成一个数据区
// dt：数据类型
// strName：数据区的名字，用于引用
void CDataPool::AddData(DataType dt, LPCTSTR strName)
{
	VERIFY(Index(strName) < 0);

	switch (dt)
	{
	case StreamCurveData:
		m_pool.Add((void*)new CStreamCurveData());
		break;
	default:
		break;
	}

	m_types.Add(dt);
	m_names.Add(strName);
}

// 获取一个数据区
void* CDataPool::GetData(LPCTSTR strName)
{
	int i = Index(strName);
	VERIFY( i >= 0);

	return (void*)m_pool[i];
}

// 指定名字的下标，不存在返回-1
int CDataPool::Index(CString strName)
{
	int i = m_names.GetCount() - 1;
	for (i; i >= 0; i --)
	{
		if (strName == m_names[i])
			break;
	}
	return i;
}
