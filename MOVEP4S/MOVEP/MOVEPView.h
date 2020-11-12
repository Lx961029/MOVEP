// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MOVEPView.h: CMOVEPView 类的接口
//

#pragma once

#include "..\Controls\CStreamCurveView.h"
#pragma comment(lib, "../Output/Controls.lib")


#include "CEEGCom.h"
#include "MOVEPDoc.h"
#include "CSSVEP.h"
#include "CMoveBall.h"



class CMOVEPView : public CStreamCurveView
{
public: // 仅从序列化创建
	CMOVEPView() noexcept;
	DECLARE_DYNCREATE(CMOVEPView)

// 特性
public:
	CMOVEPDoc* GetDocument() const;

// 操作
public:

// 重写
public:

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMOVEPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void UpdateCurveData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
public:
	CEEGCom m_comm;//CEEGCom类的对象
	CString SerialName;//选择串口名字
	BOOL Start;//开始按钮标志
	BOOL End;//结束按钮标志
	//CSSVEP *m_ssvep;
	HICON hIcon;
	CMoveBall* m_moveball;
	CSSVEP* m_ssvep;

public:
	afx_msg void OnComboxSerial();//串口选择按钮
	afx_msg void OnUpdateComboSerial(CCmdUI* pCmdUI);//串口数据加入下拉列表
	afx_msg void OnButtonStart();//开始
	afx_msg void OnButtonEnd();//结束
	afx_msg void OnUpdateButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEnd(CCmdUI* pCmdUI);
	afx_msg void OnComboxChoose();
	afx_msg void OnUpdateComboxChoose(CCmdUI* pCmdUI);
	afx_msg void OnButtonChooseUser();
	afx_msg void OnChooseFileLocation();
};

#ifndef _DEBUG  // MOVEPView.cpp 中的调试版本
inline CMOVEPDoc* CMOVEPView::GetDocument() const
   { return reinterpret_cast<CMOVEPDoc*>(m_pDocument); }
#endif

