// DataManager.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "DataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CDataManager::CDataManager(void)
{
	m_nFrameCount = 0;
	m_nCurrentFrame = 0;
}

CDataManager::~CDataManager(void)
{
}

bool CDataManager::SetCurrentFrame(int nNewFrame)
{
	int n = 0;
	if (nNewFrame < 0)
	{
		n = 0;
	}
	else if (nNewFrame > m_nFrameCount)
	{
		n = 0;
	}
	else
	{
		n = nNewFrame;
	}

	if (n == m_nCurrentFrame)
	{
		return FALSE;
	}

	m_nCurrentFrame = n;

	return TRUE;
}