#pragma once
#include "CEEGCom.h"
#include<ddraw.h>
#pragma comment(lib,"ddraw.lib")
// CMoveBall 对话框
//设置图片大小
#define X_PIXEL 200
#define Y_PIXEL 200


class CMoveBall : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveBall)

public:
	CMoveBall(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMoveBall();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEEGCom m_comm;
	BOOL m_check1;
	BOOL m_check2;
	BOOL m_check3;
	BOOL m_check4;

	LPDIRECTDRAW  lpDD;
	LPDIRECTDRAWSURFACE  lpDDSPrimary;
	LPDIRECTDRAWSURFACE  lpDDSBack;
	LPDIRECTDRAWSURFACE  lpDDWihte;
	LPDIRECTDRAWSURFACE  lpDDBlack;
	const wchar_t* szBitmapWhite;
	const wchar_t* szBitmapBlack;
	int Width;
	int Height;
	CRect rcRect;
	CRect rcClient;
	DDBLTFX ddbltfx;
	int rc[4][2];
	//获取CCA计算结果数据
	int *getccadate;
	//用于传递开启cca计算的标识符
	bool* imoveball;
	int ballr;//小球的直径
	int X;
	int Y;
	int top_rectangle;
	int bottom_rectangle;
	int left_rectangle;
	int right_rectangle;



	////DDraw绘图方式////
	IDirectDrawSurface* DDLoadBitmap(IDirectDraw* pdd, LPCWSTR szBitmap, int dx, int dy);
	IDirectDrawSurface* CreateOffScreenSurface(IDirectDraw* pdd, int dx, int dy);
	HRESULT DDCopyBitmap(IDirectDrawSurface* pdds, HBITMAP hbm, int dx, int dy);

	//4张图片闪烁线程
	CWinThread* m_picthread1;
	BOOL m_bIsbeginThread1;
	static UINT Pic_Thread1(LPVOID pParam);
	UINT  Draw_pic1();

	CWinThread* m_picthread2;
	BOOL m_bIsbeginThread2;
	static UINT Pic_Thread2(LPVOID pParam);
	UINT  Draw_pic2();

	CWinThread* m_picthread3;
	BOOL m_bIsbeginThread3;
	static UINT Pic_Thread3(LPVOID pParam);
	UINT  Draw_pic3();

	CWinThread* m_picthread4;
	BOOL m_bIsbeginThread4;
	static UINT Pic_Thread4(LPVOID pParam);
	UINT  Draw_pic4();

	//接收CCA结果，控制小球线程
	CWinThread* m_datethread;
	BOOL m_dThread;
	static UINT Data_Thread(LPVOID pParam);
	UINT  datainfo();


	afx_msg void OnBnStart();
	afx_msg void OnBnEnd();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	void Start();
	void OnDraw();//绘制小球位置

	virtual BOOL OnInitDialog();
};
