#pragma once

#include "..\UserDef\UserDef.h"
#pragma comment(lib, "../Output/UserDef.lib")

// DataManager.h : 数据类的基类
//

class CDataManager
{
public:
	CDataManager(void);
	virtual ~CDataManager(void);

	// current frame : based 1
	virtual bool SetCurrentFrame(int nNewFrame);
	inline int CurrentFrame(){return m_nCurrentFrame;}

	inline int FrameCount(){return m_nFrameCount;}

protected:
	int m_nFrameCount;
	int m_nCurrentFrame;	// based 1
};