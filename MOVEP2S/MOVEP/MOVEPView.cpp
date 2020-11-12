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

// MOVEPView.cpp: CMOVEPView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MOVEP.h"
#endif

#include "MOVEPDoc.h"
#include "MOVEPView.h"

#include "MainFrm.h"
#include "CDlgUserInfo.h"
#include "..\DataManager\CDataPool.h"
#pragma comment(lib, "../Output/DataManager.lib")


#include "rsdef.h"
//RSHD *rshdtest;
#include "..\UserDef\UserDef.h"
#pragma comment(lib, "../Output/UserDef.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif






// CMOVEPView

IMPLEMENT_DYNCREATE(CMOVEPView, CView)

BEGIN_MESSAGE_MAP(CMOVEPView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_COMBO2, &CMOVEPView::OnComboxSerial)
	ON_UPDATE_COMMAND_UI(ID_COMBO2, &CMOVEPView::OnUpdateComboSerial)
	ON_COMMAND(ID_BUTTON2, &CMOVEPView::OnButtonStart)
	ON_COMMAND(ID_BUTTON3, &CMOVEPView::OnButtonEnd)
	ON_UPDATE_COMMAND_UI(ID_BUTTON2, &CMOVEPView::OnUpdateButtonStart)
	ON_UPDATE_COMMAND_UI(ID_BUTTON3, &CMOVEPView::OnUpdateButtonEnd)
	ON_COMMAND(ID_COMBO3, &CMOVEPView::OnComboxChoose)
	ON_UPDATE_COMMAND_UI(ID_COMBO3, &CMOVEPView::OnUpdateComboxChoose)
	ON_COMMAND(ID_BUTTON1, &CMOVEPView::OnButtonChooseUser)
	ON_COMMAND(BUTTON_CHOOSEF, &CMOVEPView::OnChooseFileLocation)
END_MESSAGE_MAP()

// CMOVEPView 构造/析构


CMOVEPView::CMOVEPView() noexcept
{
	// TODO: 在此处添加构造代码

	Start = TRUE;
	End = FALSE;
	m_moveball=NULL;
	m_ssvep = NULL;
	//*rshdtest = new *rshdtest();
	//**rshdtest = -1;//机械手臂句柄
	//m_ssvep = NULL;
	//wchar_t pFileName[MAX_PATH];
	//int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	//CString csFullPath(pFileName);
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//CMFCRibbonEdit* edit = (CMFCRibbonEdit*)((pFrame->m_wndRibbonBar).FindByID(ID_EDITFILE));
	//ASSERT_VALID(edit);
	//CString str = edit->GetEditText();
	hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
}

CMOVEPView::~CMOVEPView()
{
	//delete *rshdtest;
	if (m_moveball != NULL) {
		delete m_moveball;
		m_moveball = NULL;
	}
	if (m_ssvep != NULL) {
		delete m_ssvep;
		m_ssvep = NULL;
	}
}

BOOL CMOVEPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMOVEPView 绘图

void CMOVEPView::OnDraw(CDC* pDC)
{
	CMOVEPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CStreamCurveView::OnDraw(pDC);
}

void CMOVEPView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
}

void CMOVEPView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMOVEPView 诊断

#ifdef _DEBUG
void CMOVEPView::AssertValid() const
{
	CView::AssertValid();
}

void CMOVEPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMOVEPDoc* CMOVEPView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMOVEPDoc)));
	return (CMOVEPDoc*)m_pDocument;
}
#endif //_DEBUG


// CMOVEPView 消息处理程序


//更新图像数据
void CMOVEPView::UpdateCurveData()
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < m_comm.m_data.GetCurveCount(); i++)
	{
		GetCurve(i)->SetCurveValues(m_comm.m_data.GetCurveData(i), m_comm.m_data.GetCurveLength());
	}
}


void CMOVEPView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateCurveData();
	DrawImage();

	CStreamCurveView::OnTimer(nIDEvent);
}


void CMOVEPView::OnSize(UINT nType, int cx, int cy)
{
	CStreamCurveView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_wndSize.cx = cx;
	m_wndSize.cy = cy;

	if (cx > 0 && cy > 0 && IsWindow(GetSafeHwnd()))
	{
		NewWndBitmap();
		ResetCurveBitmaps();
	}
}


BOOL CMOVEPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: 在此处添加实现代码.
	return DoPreparePrinting(pInfo);
}


void CMOVEPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CMOVEPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CMOVEPView::OnComboxSerial()
{
	// TODO: 在此添加命令处理程序代码
	// 获取Combo Box控件的指针  
	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, ((CMainFrame*)GetParentFrame())->m_wndRibbonBar.FindByID(ID_COMBO2));
	// 获取Combo Box控件当前选中项的索引 
	int nCurSel = pComboBox->GetCurSel();
	SerialName = pComboBox->GetItem(nCurSel);
}


void CMOVEPView::OnUpdateComboSerial(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, ((CMainFrame*)GetParentFrame())->m_wndRibbonBar.FindByID(ID_COMBO2));// 获取Combo Box控件的指针  
	int n;
	CStringArray names;
	//将读取到的句柄添加到下拉框中
	n = m_comm.GetCommNames(names);
	for (int i = 0; i < n; i++) {
		pComboBox->AddItem(names[i]);
	}
}


void CMOVEPView::OnButtonStart()
{
	// TODO: 在此添加命令处理程序代码
	//if (rs_initialize() == RS_SUCC)//判断机器人是否连接成功
	//{

	//	if (rs_create_context(*rshdtest) == RS_SUCC)
	//	{
	//		if (rs_login(**rshdtest, "192.168.10.2", 8899) == RS_SUCC)
	//		{
	//			MessageBox(TEXT("机械手臂连接成功！"));
	//	
	//		}
	//	}
	//}
	//

	//机械臂运动
	//while (0) {
	//	double point1[ARM_DOF] = { 0.785, -0.127196, -1.3204414, 0.376753, -1.56999, -0.000000 };//回到初始位姿
	//					//	double point2[ARM_DOF] = { -0.878354, 0.082670, -1.725366, -0.23454, -1.570709, 1.465069 };//第一个拍照点
	//	aubo_robot_namespace::JointVelcAccParam max_velc;
	//	max_velc.jointPara[0] = 0.5;//最大2.59
	//	max_velc.jointPara[1] = 0.5;
	//	max_velc.jointPara[2] = 0.5;
	//	max_velc.jointPara[3] = 0.5;
	//	max_velc.jointPara[4] = 0.5;
	//	max_velc.jointPara[5] = 0.5;
	//	rs_set_global_joint_maxvelc(**rshdtest, &max_velc);

	//	aubo_robot_namespace::JointVelcAccParam max_acc;
	//	max_acc.jointPara[0] = 13;//最大是17
	//	max_acc.jointPara[1] = 13;
	//	max_acc.jointPara[2] = 13;
	//	max_acc.jointPara[3] = 13;
	//	max_acc.jointPara[4] = 13;
	//	max_acc.jointPara[5] = 13;
	//	rs_set_global_joint_maxacc(**rshdtest, &max_acc);
	//	rs_move_joint(**rshdtest, point1, true);
	//	//Sleep(2000);
	//}

	//传输句柄
	//m_comm.*rshdtesttest = *rshdtest;

	//先初始化串口
	m_comm.SelectComm(SerialName);
	m_comm.InitComm();
	m_comm.Start();

	////设置位图长宽
	SetCurveCount(m_comm.m_data.GetCurveCount());
	SetCurveLength(m_comm.m_data.GetCurveLength());
	//SetCurveLength(4096);
	for (int i = 0; i < m_CurveArray.GetCount(); i++)
	{
		m_CurveArray[i]->SetLimitValue(0, 0, TRUE);
	}






	////串口处理程序
	CRect rt;
	GetWindowRect(rt);
	m_wndSize.cx = rt.Width();
	m_wndSize.cy = rt.Height();
	NewWndBitmap();
	ResetCurveBitmaps();
	DrawImage();

	KillTimer(1);
	SetTimer(1, 100, NULL);
	Start = !Start;
	End = !End;

}


void CMOVEPView::OnButtonEnd()
{
	// TODO: 在此添加命令处理程序代码
	m_comm.CloseComm();
	//ReleaseBuffer();
	End = !End;
	Start = !Start;
}


void CMOVEPView::OnUpdateButtonStart(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(Start);
}


void CMOVEPView::OnUpdateButtonEnd(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(End);
}


void CMOVEPView::OnComboxChoose()
{
	// TODO: 在此添加命令处理程序代码
	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, ((CMainFrame*)GetParentFrame())->m_wndRibbonBar.FindByID(ID_COMBO3));// 获取Combo Box控件的指针  
	CString choice;
	int nCurSel = pComboBox->GetCurSel();
	choice = pComboBox->GetItem(nCurSel);

	//if (Start==FALSE)
	//{
	//	MessageBox(TEXT("请先停止采集！"));
		//m_comm.CloseComm();
	//}
	if(choice == "ssvep") {

		
		m_ssvep == NULL;
		//if (m_ssvep == NULL) {
	    m_ssvep = new CSSVEP();
	    m_ssvep->Create(DIALOG_SSVEP, this);
		//}
		//CSSVEP* MSSVEP = new CSSVEP();
		m_ssvep->ShowWindow(SW_SHOWMAXIMIZED);
		m_ssvep->matchedIndexofssvep = m_comm.matchedIndex;
		m_ssvep->isssvep = m_comm.isssvepc;
		m_ssvep->mywaysofs = m_comm.myway;
		m_ssvep->isfanxofs = m_comm.isfanxofc;
		m_ssvep->SetWindowTextW(_T("脑控机械臂控制系统"));

		m_ssvep->SetIcon(hIcon,true);
		//m_ssvep->Create(IDD_DIALOG1);
		//m_ssvep->ShowWindow(true);
		//CSSVEP m_ssvep;
		//m_ssvep.DoModal();
	}
	else if ( choice == "moveball") {
		m_moveball = new CMoveBall();
		m_moveball->Create(DIALOG_MOVEBALL, this);
		m_moveball->getccadate= m_comm.matchedIndex;
		m_moveball->imoveball = m_comm.isssvepc;


	}
}


void CMOVEPView::OnUpdateComboxChoose(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, ((CMainFrame*)GetParentFrame())->m_wndRibbonBar.FindByID(ID_COMBO3));// 获取Combo Box控件的指针  
	//将读取到的句柄添加到下拉框中
	pComboBox->AddItem(TEXT("ssvep"));
	pComboBox->AddItem(TEXT("moveball"));
}


void CMOVEPView::OnButtonChooseUser()
{
	// TODO: 在此添加命令处理程序代码
	wchar_t pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCRibbonEdit* edit = (CMFCRibbonEdit*)((pFrame->m_wndRibbonBar).FindByID(ID_EDITFILE));
	ASSERT_VALID(edit);
	edit->SetEditText(csFullPath);

	CDlgUserInfo m_userinfo;
	m_userinfo.DoModal();
}


void CMOVEPView::OnChooseFileLocation()
{
	// TODO: 在此添加命令处理程序代码
	char szPath[MAX_PATH + 1] = { 0 };
	wchar_t* pwText = NULL;

	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	bi.lpszTitle = L"请选择文件夹";
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	//将char*转化为wchar_t*
	int len = MultiByteToWideChar(CP_ACP, 0, szPath, strlen(szPath) + 1, NULL, 0);
	//pwText = new wchar_t[len + 1];
	pwText = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, szPath, strlen(szPath), pwText, len);
	pwText[len] = '\0';
	bi.pszDisplayName = pwText;

	//弹出目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp == NULL)
	{
		return;
	}
	else
	{
		SHGetPathFromIDListW(lp, pwText);
	}

	CString csFullPath(pwText);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCRibbonEdit* edit = (CMFCRibbonEdit*)((pFrame->m_wndRibbonBar).FindByID(ID_EDITFILE));
	ASSERT_VALID(edit);
	edit->SetEditText(csFullPath);
}
