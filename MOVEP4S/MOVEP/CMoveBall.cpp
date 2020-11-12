// CMoveBall.cpp: 实现文件
//

#include "pch.h"
#include "MOVEP.h"
#include "CMoveBall.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MOVEPView.h"


// CMoveBall 对话框

IMPLEMENT_DYNAMIC(CMoveBall, CDialogEx)

CMoveBall::CMoveBall(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
    m_check1 = FALSE;
    m_check2 = FALSE;
    m_check3 = FALSE;
    m_check4 = FALSE;
    m_datethread = FALSE;
    m_dThread = FALSE;
    szBitmapWhite = L"white.bmp";
    szBitmapBlack = L"black.bmp";
    lpDD = NULL;
    lpDDSPrimary = NULL;
    lpDDSBack = NULL;
    lpDDBlack = NULL;
    lpDDWihte = NULL;
    getccadate = new int();
    imoveball = new bool();
    Width = 0;
    Height = 0;
    m_bIsbeginThread1 = FALSE;
    m_bIsbeginThread2 = FALSE;
    m_bIsbeginThread3 = FALSE;
    m_bIsbeginThread4 = FALSE;
    m_picthread1 = NULL;
    m_picthread2 = NULL;
    m_picthread3 = NULL;
    m_picthread4 = NULL;
    ddbltfx.dwSize = 1;
    ballr = 50;
    X = 600;
    Y = 400;

    rc[0][0] = 0;
    rc[0][1] = 0;

    rc[1][0] = 0;
    rc[1][1] = 0;

    rc[2][0] = 0;
    rc[2][1] = 0;

    rc[3][0] = 0;
    rc[3][1] = 0;



}

CMoveBall::~CMoveBall()
{
    m_dThread = FALSE;
    m_bIsbeginThread1 = FALSE;
    m_bIsbeginThread2 = FALSE;
    m_bIsbeginThread3 = FALSE;
    m_bIsbeginThread4 = FALSE;
}

void CMoveBall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMoveBall, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMoveBall::OnBnStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CMoveBall::OnBnEnd)
	ON_BN_CLICKED(IDC_CHECK1, &CMoveBall::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMoveBall::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMoveBall::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMoveBall::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CMoveBall 消息处理程序

//开始按钮
void CMoveBall::OnBnStart()
{
	// TODO: 在此添加控件通知处理程序代码
   // CString SerialName;
   // CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

   // ASSERT_VALID(pMainFrame);
   // //获取窗口控件
   // CMFCRibbonBar* pRibbonBar = pMainFrame->GetRibbonBar();
   // ASSERT_VALID(pRibbonBar);

   // CMFCRibbonComboBox* pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbonBar->FindByID(ID_COMBO2));
   // int nCurSel = pCombox->GetCurSel();
   // SerialName = pCombox->GetItem(nCurSel);

   // int n;
   // CStringArray names;
   // n = m_comm.GetCommNames(names);
   //// MessageBox(SerialName);
   // m_comm.SelectComm(SerialName);
   // m_comm.InitComm();
   // m_comm.Start();

    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
    m_dThread = TRUE;
    *imoveball = true;
    m_datethread = AfxBeginThread(&CMoveBall::Data_Thread, this);	//启动线程;
}


//结束按钮
void CMoveBall::OnBnEnd()
{
	// TODO: 在此添加控件通知处理程序代码
    //m_comm.CloseComm();
    GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
    m_dThread = FALSE;
}


//5.5Hz复选框  上
void CMoveBall::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check1 = !m_check1;
    if (m_check1)
    {
        m_bIsbeginThread1 = TRUE;
        m_picthread1 = AfxBeginThread(&CMoveBall::Pic_Thread1, this);	//启动线程;
    }
    else {
        m_bIsbeginThread1 = FALSE;
    }

}

//7.5Hz复选框控制按钮   下
void CMoveBall::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check2 = !m_check2;
    if (m_check2)
    {
        m_bIsbeginThread2 = TRUE;
        m_picthread2 = AfxBeginThread(&CMoveBall::Pic_Thread2, this);	//启动线程;
    }
    else {
        m_bIsbeginThread2 = FALSE;
    }
}

//8.6Hz复选框控制按钮   左
void CMoveBall::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check3 = !m_check3;
    if (m_check3)
    {
        m_bIsbeginThread3 = TRUE;
        m_picthread3 = AfxBeginThread(&CMoveBall::Pic_Thread3, this);	//启动线程;
    }
    else {
        m_bIsbeginThread3 = FALSE;
    }
}

//10Hz复选框控制按钮   右
void CMoveBall::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check4 = !m_check4;
    if (m_check4)
    {
        m_bIsbeginThread4 = TRUE;
        m_picthread4 = AfxBeginThread(&CMoveBall::Pic_Thread4, this);	//启动线程;
    }
    else {
        m_bIsbeginThread4 = FALSE;
    }
}


BOOL CMoveBall::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
    ShowWindow(SW_MAXIMIZE);//对话框默认最大化弹出
    GetClientRect(&rcClient);    //获取客户区域
    //将指定点，或者矩形的用户坐标转换成屏幕坐标
    ClientToScreen(&rcClient);
    Width = rcClient.right - rcClient.left;
    Height = rcClient.bottom - rcClient.top;


    top_rectangle = (Height - 500) / 2;
    left_rectangle= (Width - 600) / 2;
    bottom_rectangle = top_rectangle + 500;
    right_rectangle = left_rectangle + 500;

    X = left_rectangle + 100;
    Y = top_rectangle + 100;

    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;

    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = X_PIXEL;
    rcRect.bottom = Y_PIXEL;

    Start();
    //将X右移，否则会掩盖住左边的按钮
    rcClient.DeflateRect(100, 0, 0, 0);

    int xmid = rcClient.right / 2;
    int ymid = rcClient.bottom / 2;
    int h = (xmid - Y_PIXEL) / 2;
    rc[0][0] = 0;
    rc[0][1] = 10;

    rc[1][0] = rcClient.right - X_PIXEL;
    rc[1][1] = 10;

    rc[2][0] = 100;
    rc[2][1] = rcClient.bottom - Y_PIXEL - 50;

    rc[3][0] = rcClient.right - X_PIXEL;
    rc[3][1] = rcClient.bottom - Y_PIXEL - 50;

    //OnDraw();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


UINT  CMoveBall::Pic_Thread1(LPVOID pParam) {
    CMoveBall* ddraw = (CMoveBall*)pParam;
    return ddraw->Draw_pic1();
}
//5.5hz闪烁
UINT  CMoveBall::Draw_pic1() {
    //CStdioFile m_outfile;
    CString str;
    int i = 0;
    double a[100] = { 0 };
    CRect r1 = CRect(100, 10, 300, 210);
    //m_outfile.Open(_T("111.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);

    //lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    //lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    //lpDDSPrimary->Blt(&r1,  lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread1) {
        clock_t start = clock();
        Sleep(87);
        while (clock() - start < 91);
        lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDBlack, NULL, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &rcRect, DDBLT_WAIT, NULL);
        //lpDDSPrimary->BltFast(rc[0][0], rc[0][1], lpDDSBack, &rcRect, DDBLT_WAIT);

        Sleep(87);
        while (clock() - start < 182);
        lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDWihte, NULL, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &rcRect, DDBLT_WAIT, NULL);
        //lpDDSPrimary->BltFast(rc[0][0], rc[0][1], lpDDSBack, &rcRect, DDBLT_WAIT);
    }
    //m_outfile.Close();
    return 0;
}

UINT  CMoveBall::Pic_Thread2(LPVOID pParam) {
    CMoveBall* ddraw = (CMoveBall*)pParam;
    return ddraw->Draw_pic2();
}

//7.5hz闪烁
UINT  CMoveBall::Draw_pic2() {
    //CStdioFile m_outfile;
    CString str;
    CRect r1 = CRect(rcClient.right - X_PIXEL, 10, rcClient.right, 210);
    // m_outfile.Open(_T("222.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    //lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    //lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread2) {
        clock_t start = clock();
        Sleep(63);
        while (clock() - start < 66);
        lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, &ddbltfx);

        Sleep(63);
        while (clock() - start < 132);
        lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        // lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, &ddbltfx);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}

UINT  CMoveBall::Pic_Thread3(LPVOID pParam) {
    CMoveBall* ddraw = (CMoveBall*)pParam;
    return ddraw->Draw_pic3();
}

//8.6hz闪烁
UINT  CMoveBall::Draw_pic3() {
    //CStdioFile m_outfile;
    CString str;

    //m_outfile.Open(_T("333.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    //lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    //lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    CRect r1 = CRect(100, rcClient.bottom - Y_PIXEL - 50, 300, rcClient.bottom - 50);
    while (m_bIsbeginThread3) {
        clock_t start = clock();
        Sleep(55);
        while (clock() - start < 58);
        lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, NULL);
        Sleep(55);
        while (clock() - start < 116);
        lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    // m_outfile.Close();
    return 0;
}

UINT  CMoveBall::Pic_Thread4(LPVOID pParam) {
    CMoveBall* ddraw = (CMoveBall*)pParam;
    return ddraw->Draw_pic4();
}

//10hz闪烁
UINT  CMoveBall::Draw_pic4() {
    //CStdioFile m_outfile;
    CString str;

    //m_outfile.Open(_T("444.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    //lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    //lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    CRect r1 = CRect(rcClient.right - X_PIXEL, rcClient.bottom - Y_PIXEL - 50, rcClient.right, rcClient.bottom - 50);
    while (m_bIsbeginThread4) {
        clock_t start = clock();
        Sleep(46);
        while (clock() - start < 50);
        lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, NULL);
        Sleep(46);
        while (clock() - start < 100);
        lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        //lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        lpDDSPrimary->Blt(&r1, lpDDSBack, &r1, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}

//绘制小球
void CMoveBall::OnDraw()
{
    // TODO: 在此处添加实现代码.
    CString str;
    
    Graphics g(this->GetSafeHwnd(), TRUE);
    Pen mypen(Color(255, 0, 0, 255), 2);
    mypen.SetWidth(2);
    SolidBrush BrushRed(Color::Red);
    SolidBrush BrushWhite(Color::White);


    g.DrawRectangle(&mypen, left_rectangle, top_rectangle, 500, 500);
    g.FillRectangle(&BrushWhite, left_rectangle, top_rectangle, 500, 500);//擦除

    //date = *(m_comm.matchedIndex);
    //date = m_comm.CCAProgress();
    int date;
    date = *getccadate;
    //str.Format(_T("%d "), *getccadate);
    //SetDlgItemText(IDC_EDIT1, str);
    //m_outfile.WriteString(str);
    //取随机数测试
    //srand((unsigned)time(NULL));/* 初始化随机数发生器 */
    //date = rand() % 6;

    //10Hz   右
    if (date == 6)
    {
        X += ballr;
        if (X > left_rectangle+450)
        {
            X = left_rectangle;
        }
    }
    else if (date == 2)//5.5Hz   上
    {
        Y -= ballr;
        if (Y < top_rectangle)
        {
            Y = top_rectangle+450;
        }

    }
    else if (date == 4)//7.5Hz   下
    {
        Y += ballr;
        if (Y > top_rectangle + 450)
        {
            Y = top_rectangle;
        }
    }
    else if (date == 5)//8.6Hz   左
    {
        X -= ballr;
        if (X < left_rectangle)
        {
            X = right_rectangle - ballr;
        }
    }
    g.FillEllipse(&BrushRed, X, Y, ballr, ballr);//重绘
}


void CMoveBall::Start()
{
    // TODO: 在此处添加实现代码.
    HRESULT ddrval;
    IDirectDrawClipper* lpClipper;  //裁剪器
    DDSURFACEDESC	ddsd;
    DDSURFACEDESC	ddsd_back;
    //系统默认的驱动程序//接收指向由DirectDrawCreate所创建对象的指针//
    ddrval = DirectDrawCreate(NULL, &lpDD, NULL);    //DirectDraw图形接口创造器（每个显卡一个对象）
    if (ddrval != DD_OK)
    {
        return;
    }

    // Get exclusive mode
    this->m_hWnd;
    ddrval = lpDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);//DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);       ///窗口化为normal
    if (ddrval != DD_OK)
    {
        return;
    }

    // 创建主图画
    ddbltfx.dwFillColor = RGB(255, 255, 255);
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;//DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
    if (ddrval != DD_OK)
    {
        return;
    }

    //创建离屏页时，必须指定页面的长、宽
    memset(&ddsd_back, 0, sizeof(ddsd_back));
    ddsd_back.dwSize = sizeof(ddsd_back);
    ddsd_back.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd_back.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; ;

    ddsd_back.dwWidth = Width;
    ddsd_back.dwHeight = Height;
    ddrval = lpDD->CreateSurface(&ddsd_back, &lpDDSBack, NULL);
    if (ddrval != DD_OK)
    {
        return;
    }

    //创建剪裁

    ddrval = lpDD->CreateClipper(0, &lpClipper, NULL);
    if (ddrval != DD_OK)
    {
        return;
    }

    //将裁剪器和窗口关联
    ddrval = lpClipper->SetHWnd(0, m_hWnd);
    //将裁剪器与主图面相关联
    ddrval = lpDDSPrimary->SetClipper(lpClipper);
    lpClipper->Release();

    // loading our bitmap.
    lpDDWihte = DDLoadBitmap(lpDD, szBitmapWhite, 0, 0);

    if (lpDDWihte == NULL)
    {
        return;
    }
    lpDDBlack = DDLoadBitmap(lpDD, szBitmapBlack, 0, 0);

    if (lpDDBlack == NULL)
    {
        return;
    }
}

//加载位图
IDirectDrawSurface* CMoveBall::DDLoadBitmap(IDirectDraw* pdd, LPCWSTR szBitmap, int dx, int dy)
{
    // TODO: 在此处添加实现代码.
    HBITMAP hbm;
    BITMAP bm;
    IDirectDrawSurface* pdds;

    // LoadImage在windows 95上有一些附加功能，它使你可以从磁盘加载位图。
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    GetObject(hbm, sizeof(bm), &bm); // 获取BITMAP

   /*
    * 创建一个离屏页面存放位图
    */

    pdds = CreateOffScreenSurface(pdd, bm.bmWidth, bm.bmHeight);
    if (pdds) {
        DDCopyBitmap(pdds, hbm, bm.bmWidth, bm.bmHeight);
    }

    DeleteObject(hbm);

    return pdds;
}
//创建离屏页
IDirectDrawSurface* CMoveBall::CreateOffScreenSurface(IDirectDraw* pdd, int dx, int dy)
{
    // TODO: 在此处添加实现代码.
    DDSURFACEDESC ddsd;
    IDirectDrawSurface* pdds;

    //
    // 为位图创建一个位图页面
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = dx;
    ddsd.dwHeight = dy;

    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
    {
        return NULL;
    }
    else {
        return pdds;
    }
}
//拷贝位图
HRESULT CMoveBall::DDCopyBitmap(IDirectDrawSurface* pdds, HBITMAP hbm, int dx, int dy)
{
    // TODO: 在此处添加实现代码.
    HDC hdcImage;
    HDC hdc;
    HRESULT hr;
    HBITMAP hbmOld;

    //
    // 选择一个位图到DC中
    //
    hdcImage = CreateCompatibleDC(NULL);
    hbmOld = (HBITMAP)SelectObject(hdcImage, hbm);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        BitBlt(hdc, 0, 0, dx, dy, hdcImage, 0, 0, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    SelectObject(hdcImage, hbmOld);
    DeleteDC(hdcImage);

    return hr;
}

//数据线程
UINT CMoveBall::Data_Thread(LPVOID pParam) {
    CMoveBall* getdata = (CMoveBall*)pParam;
    return getdata->datainfo();
}

UINT  CMoveBall::datainfo() {
    int date;
    CString str;
    while (m_dThread) { 
        date = *getccadate;
        str.Format(_T("%d "), *getccadate);
        SetDlgItemText(IDC_EDIT1, str);

        Sleep(1950);
        OnDraw();
    }
    return 0;
}