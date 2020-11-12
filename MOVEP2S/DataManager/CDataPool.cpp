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

// ������һ��������
// dt����������
// strName�������������֣���������
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

// ��ȡһ��������
void* CDataPool::GetData(LPCTSTR strName)
{
	int i = Index(strName);
	VERIFY( i >= 0);

	return (void*)m_pool[i];
}

// ָ�����ֵ��±꣬�����ڷ���-1
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
