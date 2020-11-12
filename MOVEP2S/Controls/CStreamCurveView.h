#pragma once

#include "..\UserDef\UserDef.h"
#include "CStreamCurve.h"

// CStreamCurveView 视图
// 显示流数据曲线的视图
//

class AFX_EXT_CLASS CStreamCurveView : public CView
{
	DECLARE_DYNCREATE(CStreamCurveView)

protected:
	CStreamCurveView();           // 动态创建所使用的受保护的构造函数
	virtual ~CStreamCurveView();

	

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	

public:
	// 清理资源
	void ReleaseBuffer();

	// 设置曲线数量
	void SetCurveCount(int nCount);

	// 设置曲线长度（需要先设置曲线数量）
	void SetCurveLength(int nLength);

	// 重置曲线绘制的位图和区域
	void ResetCurveBitmaps();

	// 获取一条曲线（注意：该方法不做有效性检测，使用时自己处理）
	inline CStreamCurve* GetCurve(int nCurve);

	// 重新建一个m_pWndBitmap
	void NewWndBitmap();

	// 更新图像数据
	virtual void UpdateCurveData()/*{}*/;

	// 绘制图像
	virtual void DrawImage();

	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

	Color m_backgroundColor;

	// 窗口
	Bitmap* m_pWndBitmap;	// 整个窗口采用贴图方式
	CSize m_wndSize;

	// 曲线
	CArray<CStreamCurve*> m_CurveArray;	// CStreamCurve的对象数组
	CArray<Bitmap*> m_CurveBitmapAry;	// 曲线位图的数组
	CArray<CRect> m_CurveBmpRectAry;	// 曲线位图的绘制区域
	CArray<BOOL> m_CurveVisibleAry;		// 曲线是否显示的数组
};


