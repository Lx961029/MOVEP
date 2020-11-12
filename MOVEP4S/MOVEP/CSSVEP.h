#pragma once
#include<ddraw.h>
#pragma comment(lib,"ddraw.lib")
#include "CEEGCom.h"
// CSSVEP 对话框
#define X_PIXEL 200
#define Y_PIXEL 200
//#define 
//extern int  mydatalength;
#define mydatalength3 1950

class CSSVEP : public CDialogEx
{
	DECLARE_DYNAMIC(CSSVEP)

public:
	CSSVEP(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSSVEP();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_SSVEP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CEEGCom *m_comm;//非模态只能用指针？ 模态可以直接使用实例对象
	BOOL m_check1;
	BOOL m_check2;
	BOOL m_check3;
	BOOL m_check4;
	BOOL m_check5;
	BOOL m_check6;
	BOOL m_check7;
	BOOL m_check8;
	static void* static_this;
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
	int rc[8][2];
	//文本框
	CEdit* m_ED;
	CFont* f;
	CFont* f1;
	CFont* f2;
	//cca结果
	int *matchedIndexofssvep;
	//启动计算
	bool* isssvep;
	//机械臂控制方式标志位
	bool* mywaysofs;
	//int maindex = 0;
	bool* isfanxofs;

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();

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

	CWinThread* m_picthread5;
	BOOL m_bIsbeginThread5;
	static UINT Pic_Thread5(LPVOID pParam);
	UINT  Draw_pic5();

	CWinThread* m_picthread6;
	BOOL m_bIsbeginThread6;
	static UINT Pic_Thread6(LPVOID pParam);
	UINT  Draw_pic6();

	CWinThread* m_picthread7;
	BOOL m_bIsbeginThread7;
	static UINT Pic_Thread7(LPVOID pParam);
	UINT  Draw_pic7();

	CWinThread* m_picthread8;
	BOOL m_bIsbeginThread8;
	static UINT Pic_Thread8(LPVOID pParam);
	UINT  Draw_pic8();


	CWinThread* mytimer;
	BOOL m_bIsbeginmytimer;
	static UINT Timer_Thread(LPVOID pParam);
	UINT  startcounter();

	CWinThread* mymessage;
	BOOL m_bIsbeginmymessage;
	static UINT Message_Thread(LPVOID pParam);
	UINT  startmessage();

	void Start();
	IDirectDrawSurface* DDLoadBitmap(IDirectDraw* pdd, LPCWSTR szBitmap, int dx, int dy);
	IDirectDrawSurface* CreateOffScreenSurface(IDirectDraw* pdd, int dx, int dy);
	HRESULT DDCopyBitmap(IDirectDrawSurface* pdds, HBITMAP hbm, int dx, int dy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();
	//afx_msg void startcounter();
	int counter = -1;
	afx_msg void OnPaint();
    afx_msg void addcommnd(int* index);//调用一般的函数 不能设为静态函数  之所以之前定义为静态函数 是因为静态的相对独立于类 可以直接访问
	virtual void OnCancel();
	afx_msg void OnBnClickedCheck7();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedRealse();
	afx_msg void OnBnClickedCheck8();
};
