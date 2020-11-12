// CStreamCurveView.cpp: 实现文件
//

#include "pch.h"
#include "CStreamCurveView.h"


// CStreamCurveView

IMPLEMENT_DYNCREATE(CStreamCurveView, CView)

CStreamCurveView::CStreamCurveView()
{
	m_backgroundColor = Color::Black;

	m_pWndBitmap = NULL;
	m_wndSize = CSize(0, 0);
}

CStreamCurveView::~CStreamCurveView()
{
	ReleaseBuffer();
}

BEGIN_MESSAGE_MAP(CStreamCurveView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CStreamCurveView 绘图

void CStreamCurveView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码

	if (!pDC) return;

	HDC hdc = pDC->m_hDC;
	Graphics* graphics = new Graphics(hdc);
	graphics->DrawImage(m_pWndBitmap, 0, 0);
	delete graphics;
}

// 更新图像数据
void CStreamCurveView::UpdateCurveData()
{}

// 绘制图像
void CStreamCurveView::DrawImage()
{
	CDC* pDC = GetDC();
	if (!pDC)
	{
		return;
	}
	HDC hdc = pDC->m_hDC;
	Graphics* graphics = new Graphics(hdc);

	Graphics* scene = new Graphics(m_pWndBitmap);
	scene->Clear(m_backgroundColor);

	if (m_CurveArray.GetCount() > 0)
	{
  		Matrix matrix(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, (REAL)(m_CurveBmpRectAry[0].Height() - 1.));
		for (int i = 0; i < m_CurveArray.GetCount(); i++)
		{
			Graphics* curveScene = new Graphics(m_CurveBitmapAry[i]);
  			curveScene->SetTransform(&matrix);// 将Y轴转变成垂直向上为正
			m_CurveArray[i]->DrawCurve(curveScene, m_backgroundColor);
			scene->DrawImage(m_CurveBitmapAry[i], m_CurveBmpRectAry[i].left, m_CurveBmpRectAry[i].top);
			delete curveScene;
		}

	}
	graphics->DrawImage(m_pWndBitmap, 0, 0);

	delete scene;
	delete graphics;
	ReleaseDC(pDC);
}

// CStreamCurveView 诊断

#ifdef _DEBUG
void CStreamCurveView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStreamCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStreamCurveView 消息处理程序

void CStreamCurveView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

// 清理资源
void CStreamCurveView::ReleaseBuffer()
{
	// 曲线对象
	if (m_CurveArray.GetCount() > 0)
	{
		for (int i = 0; i < m_CurveArray.GetCount(); i++)
		{
			m_CurveArray[i]->ReleaseBuffer();
			SAFE_DELETE(m_CurveArray[i]);
		}
		m_CurveArray.RemoveAll();
	}

	// 曲线位图
	if (m_CurveBitmapAry.GetCount() > 0)
	{
		for (int i = 0; i < m_CurveBitmapAry.GetCount(); i ++)
		{
			SAFE_DELETE(m_CurveBitmapAry[i]);
		}
		m_CurveBitmapAry.RemoveAll();
	}

	// 其它
	if(m_CurveBmpRectAry.GetCount() > 0) m_CurveBmpRectAry.RemoveAll();
	if(m_CurveVisibleAry.GetCount() > 0) m_CurveVisibleAry.RemoveAll();

	SAFE_DELETE(m_pWndBitmap);
}

// 设置曲线数量
void CStreamCurveView::SetCurveCount(int nCount)
{
	ReleaseBuffer();

	m_CurveArray.SetSize(nCount);
	m_CurveBitmapAry.SetSize(nCount);
	m_CurveBmpRectAry.SetSize(nCount);
	m_CurveVisibleAry.SetSize(nCount);

	for (int i = 0; i < nCount; i++)
	{
		m_CurveArray[i] = new CStreamCurve();
		m_CurveVisibleAry[i] = TRUE;
	}
}

// 设置曲线长度
void CStreamCurveView::SetCurveLength(int nLength)
{
	for (int i = 0; i < m_CurveArray.GetCount(); i ++)
	{
		m_CurveArray[i]->SetCurveLength(nLength);
	}
}


// 重置曲线绘制的位图和区域
void CStreamCurveView::ResetCurveBitmaps()
{
	if (m_wndSize.cx <= 0 || m_wndSize.cy <= 0) return;

	if (m_CurveBmpRectAry.GetCount() > 0)
	{
		int n = m_CurveBmpRectAry.GetCount();
		int h = m_wndSize.cy / n;
		CRect rt;
		rt.left = 0;
		rt.right = m_wndSize.cx - 1;

		for (int i = 0, t = 0; i < n; i++, t += h)
		{
			rt.top = t;
			rt.bottom = rt.top + h;
			m_CurveBmpRectAry[i] = rt;
		}
		m_CurveBmpRectAry[n - 1].bottom = m_wndSize.cy - 1;

		for (int i = 0; i < n; i ++)
		{
			SAFE_DELETE(m_CurveBitmapAry[i]);
			m_CurveBitmapAry[i] = new Bitmap(m_CurveBmpRectAry[i].Width(), m_CurveBmpRectAry[i].Height(), PixelFormat32bppRGB);
			m_CurveArray[i]->SetCanvasSize(m_CurveBmpRectAry[i].Width(), m_CurveBmpRectAry[i].Height());
		}
	}
}

// 获取一条曲线（注意：该方法不做有效性检测，使用时自己处理）
inline CStreamCurve* CStreamCurveView::GetCurve(int nCurve)
{
	return m_CurveArray[nCurve];
}

// 重新建一个m_pWndBitmap
void CStreamCurveView::NewWndBitmap()
{
	SAFE_DELETE(m_pWndBitmap);
	m_pWndBitmap = new Bitmap(m_wndSize.cx, m_wndSize.cy, PixelFormat32bppRGB);
}
