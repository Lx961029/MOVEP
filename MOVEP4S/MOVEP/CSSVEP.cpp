// CSSVEP.cpp: 实现文件
//

#include "pch.h"
#include "MOVEP.h"
#include "CSSVEP.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CSSVEP 对话框
//int mytime = 1950;
IMPLEMENT_DYNAMIC(CSSVEP, CDialogEx)

//void* CSSVEP::static_this = nullptr;

CSSVEP::CSSVEP(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_SSVEP, pParent)
{
    //static_this = (void *)this;
    //初始化所有变量
    //m_comm = new CEEGCom();
    matchedIndexofssvep = new int();
    isssvep = new bool();
    mywaysofs = new bool();
    isfanxofs = new bool();
    *mywaysofs = false;
    *isfanxofs = false;
	m_check1 = FALSE;
	m_check2 = FALSE;
	m_check3 = FALSE;
	m_check4 = FALSE;
	m_check5 = FALSE;
	m_check6 = FALSE;
    m_check7 = FALSE;
    m_check8 = FALSE;
    szBitmapWhite = L"white.bmp";
    szBitmapBlack = L"black.bmp";
    lpDD = NULL;
    lpDDSPrimary = NULL;
    lpDDSBack = NULL;
    lpDDWihte = NULL;
    lpDDBlack = NULL;
    Width = 0;
    Height = 0;
    m_bIsbeginThread1 = FALSE;
    m_bIsbeginThread2 = FALSE;
    m_bIsbeginThread3 = FALSE;
    m_bIsbeginThread4 = FALSE;
    m_bIsbeginThread5 = FALSE;
    m_bIsbeginThread6 = FALSE;
    m_bIsbeginThread7 = FALSE;
    m_bIsbeginThread8 = FALSE;
    m_bIsbeginmytimer=FALSE;
    m_picthread1 = NULL;
    m_picthread2 = NULL;
    m_picthread3 = NULL;
    m_picthread4 = NULL;
    m_picthread5 = NULL;
    m_picthread6 = NULL;
    m_picthread7 = NULL;
    m_picthread8 = NULL;
    ddbltfx.dwSize = 1;
    //for (int i = 0; i < 6; i++) {
    //    for (int j = 0; j < 2; j++)
    //    {
    //        rc[i][j] = 0;
    //    }
    //}
    rc[0][0] = 0;
    rc[0][1] = 0;

    rc[1][0] = 0;
    rc[1][1] = 0;

    rc[2][0] = 0;
    rc[2][1] = 0;

    rc[3][0] = 0;
    rc[3][1] = 0;

    rc[4][0] = 0;
    rc[4][1] = 0;

    rc[5][0] = 0;
    rc[5][1] = 0;

  //  ((CButton*)GetDlgItem(IDC_BUTTON3))->SetState(TRUE);
    //this->OnBnClickedButton3();

}

CSSVEP::~CSSVEP()
{
    //delete m_comm;
    delete matchedIndexofssvep;
    delete isssvep;
    delete mywaysofs;
    delete isfanxofs;
  
    //delete this;
    //this = NULL;
}

void CSSVEP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSSVEP, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CSSVEP::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSSVEP::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CSSVEP::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CSSVEP::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CSSVEP::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CSSVEP::OnBnClickedCheck6)
    ON_BN_CLICKED(IDC_BUTTON1, &CSSVEP::OnBnClickedStart)
    ON_BN_CLICKED(IDC_BUTTON2, &CSSVEP::OnBnClickedEnd)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_CHECK7, &CSSVEP::OnBnClickedCheck7)
    ON_BN_CLICKED(IDC_CHECK7, &CSSVEP::OnBnClickedCheck7)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON3, &CSSVEP::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CSSVEP::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CSSVEP::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON6, &CSSVEP::OnBnClickedRealse)
    ON_BN_CLICKED(IDC_CHECK8, &CSSVEP::OnBnClickedCheck8)
END_MESSAGE_MAP()


// CSSVEP 消息处理程序


void CSSVEP::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check1 = !m_check1;
    if (m_check1)
    {
        m_bIsbeginThread1 = TRUE;
        m_picthread1 = AfxBeginThread(&CSSVEP::Pic_Thread1, this);	//启动线程;
    }
    else {
        m_bIsbeginThread1 = FALSE;
    }
}


void CSSVEP::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check2 = !m_check2;
    if (m_check2)
    {
        m_bIsbeginThread2 = TRUE;
        m_picthread2 = AfxBeginThread(&CSSVEP::Pic_Thread2, this);	//启动线程;
    }
    else {
        m_bIsbeginThread2 = FALSE;
    }
}


void CSSVEP::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check3 = !m_check3;
    if (m_check3)
    {
        m_bIsbeginThread3 = TRUE;
        m_picthread3 = AfxBeginThread(&CSSVEP::Pic_Thread3, this);	//启动线程;
    }
    else {
        m_bIsbeginThread3 = FALSE;
    }
}


void CSSVEP::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check4 = !m_check4;
    if (m_check4)
    {
        m_bIsbeginThread4 = TRUE;
        m_picthread4 = AfxBeginThread(&CSSVEP::Pic_Thread4, this);	//启动线程;
    }
    else {
        m_bIsbeginThread4 = FALSE;
    }
}


void CSSVEP::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check5 = !m_check5;
    if (m_check5)
    {
        m_bIsbeginThread5 = TRUE;
        m_picthread5 = AfxBeginThread(&CSSVEP::Pic_Thread5, this);	//启动线程;
    }
    else {
        m_bIsbeginThread5= FALSE;
    }
}


void CSSVEP::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
    m_check6 = !m_check6;
    if (m_check6)
    {
        m_bIsbeginThread6 = TRUE;
        m_picthread6 = AfxBeginThread(&CSSVEP::Pic_Thread6, this);	//启动线程;
    }
    else {
        m_bIsbeginThread6 = FALSE;
    }
}


void CSSVEP::Start()
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


IDirectDrawSurface* CSSVEP::DDLoadBitmap(IDirectDraw* pdd, LPCWSTR szBitmap, int dx, int dy)
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


IDirectDrawSurface* CSSVEP::CreateOffScreenSurface(IDirectDraw* pdd, int dx, int dy)
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


HRESULT CSSVEP::DDCopyBitmap(IDirectDrawSurface* pdds, HBITMAP hbm, int dx, int dy)
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

BOOL CSSVEP::OnInitDialog() //在这里增加一个命令输出框 实时输出命令
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    ShowWindow(SW_MAXIMIZE);//对话框默认最大化弹出
    GetClientRect(&rcClient);    //获取客户区域
    //将指定点，或者矩形的用户坐标转换成屏幕坐标
    ClientToScreen(&rcClient);
    Width = rcClient.right - rcClient.left;
    Height = rcClient.bottom - rcClient.top;
    // CWnd* pWnd;
     //pWnd = GetDlgItem(IDC_CHECK7);
     //pWnd->MoveWindow(rcClient.right - 100, rcClient.bottom / 2 - 150,);
     //pWnd->SetWindowPos(NULL, rcClient.right - 100, rcClient.bottom / 2 - 150, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;

    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = X_PIXEL;
    rcRect.bottom = Y_PIXEL;

    Start();
    //将X右移，否则会掩盖住左边的按钮
    //rcClient.MoveToX(200);
    //rcClient.DeflateRect(100,0);
    rcClient.DeflateRect(100, 0, 0, 0);
    //int mid = rcClient.bottom/2;
    //int h = 10;//(mid - Y_PIXEL) / 2;

    //int w = 10;//(rcClient.right - 3 * X_PIXEL) / 4;
    //rc[0][0] = w;
    //rc[0][1] = h;

    //rc[1][0] = rcClient.right/2- X_PIXEL/2;//2*w+ X_PIXEL;
    //rc[1][1] = h;

    //rc[2][0] = rcClient.right - X_PIXEL;//3*w+ X_PIXEL*2;
    //rc[2][1] = h;

    //rc[3][0] = w;
    //rc[3][1] = rcClient.bottom- Y_PIXEL-50;

    //rc[4][0] = rcClient.right / 2 - X_PIXEL / 2;
    //rc[4][1] = rcClient.bottom - Y_PIXEL - 50;

    //rc[5][0] = rcClient.right - X_PIXEL;
    //rc[5][1] = rcClient.bottom - Y_PIXEL - 50;

    int mid = rcClient.bottom / 2;
    int h = (mid - Y_PIXEL) / 2;
    //int w = (rcClient.right - 3 * X_PIXEL) / 4;
    //4Hz闪烁区域
    rc[0][0] = 150;
    rc[0][1] = h;

    //5.4Hz闪烁区域
    rc[1][0] = 550;
    rc[1][1] = h;

    //6.7Hz闪烁区域
    rc[2][0] = 950;
    rc[2][1] = h;

    //7.5Hz闪烁区域
    rc[3][0] = 150;;
    rc[3][1] = 3 * h + Y_PIXEL;

    //8.6Hz闪烁区域
    rc[4][0] = 550;
    rc[4][1] = 3 * h + Y_PIXEL;

    //10Hz  
    rc[5][0] = 950;
    rc[5][1] = 3 * h + Y_PIXEL;

    //12Hz 复原
    rc[6][0] = 1350;
    rc[6][1] = 3 * h + Y_PIXEL;

    //信息提示框的位置 
    //4.6Hz  抓取
    rc[7][0] = 1350;
    rc[7][1] = h;




    //rc[3][0] = 150;
    //rc[3][1] = 3 * h + Y_PIXEL;

    //rc[4][0] = 550;
    //rc[4][1] = 3 * h + Y_PIXEL;

    //rc[5][0] = 950;
    //rc[5][1] = 3 * h + Y_PIXEL;

    //rc[6][0] = 1350;
    //rc[6][1] = h;

    ////信息提示框的位置 
    ////抓取
    //rc[7][0] = 1350;
    //rc[7][1] = 3 * h + Y_PIXEL;

    //添加一个信息提示框
   // m_ED = new CEdit();
  // CString str = _T("我是命令输出框！");
   ///m_ED->SetWindowPos(NULL,rc[7][0], rc[7][1], rcClient.right, rcClient.bottom,1);
   // m_Eyu788->SetWindowText(str);
    /*((CEdit * )GetDlgItem(IDC_EDIT1))->SetWindowPos(NULL,20,485,Width,Height, SWP_NOSIZE);
    (((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowPos(NULL, 20,450, Width, Height,SWP_NOSIZE));*/
   (((CStatic*)GetDlgItem(IDC_STATIC))->SetWindowPos(NULL,10,420,0,0, SWP_NOSIZE));

  
    f = new CFont;
    f->CreateFont(22, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_BOLD, // nWeight 
        TRUE, // bItalic 
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        _T("Arial")); // lpszFac 
    f1 = new CFont;
    f1->CreateFont(15, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_BOLD, // nWeight 
        TRUE, // bItalic 
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        _T("Arial")); // lpszFac 
    f2 = new CFont;
    f2->CreateFont(15, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        0, // nWeight 
        FALSE, // bItalic 
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        _T("Arial")); // lpszFac 
    ((CButton*)GetDlgItem(IDC_EDIT1))->SetFont(f);
    ((CButton*)GetDlgItem(IDC_EDIT3))->SetFont(f1);
    ((CStatic*)GetDlgItem(IDC_STATIC))->SetFont(f2);
  //  SetDlgItemText(IDC_EDIT1, str);
   // OnBnClickedButton3();
     



    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE

}


UINT  CSSVEP::Pic_Thread1(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic1();
}

//4hz闪烁
UINT  CSSVEP::Draw_pic1() {
    //CStdioFile m_outfile;
    CString str;
    int i = 0;
    double a[100] = { 0 };

    //m_outfile.Open(_T("111.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread1) {
        clock_t start = clock();
        Sleep(122);
        while (clock() - start < 125);
        lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(122);
        while (clock() - start < 250);
        lpDDSBack->BltFast(rc[0][0], rc[0][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    }
   // m_outfile.Close();
    return 0;
}

UINT  CSSVEP::Pic_Thread2(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic2();
}

//5.5hz闪烁
UINT  CSSVEP::Draw_pic2() {
    //CStdioFile m_outfile;
    CString str;

   // m_outfile.Open(_T("222.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread2) {
        clock_t start = clock();
        Sleep(87);
        while (clock() - start < 91);
        lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(87);
        while (clock() - start < 182);
        lpDDSBack->BltFast(rc[1][0], rc[1][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}

UINT  CSSVEP::Pic_Thread3(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic3();
}

//6.7hz闪烁
UINT  CSSVEP::Draw_pic3() {
    //CStdioFile m_outfile;
    CString str;

    //m_outfile.Open(_T("333.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    while (m_bIsbeginThread3) {
        clock_t start = clock();
        Sleep(70);
        while (clock() - start < 75);
        lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(70);
        while (clock() - start < 150);
        lpDDSBack->BltFast(rc[2][0], rc[2][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);


        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
   // m_outfile.Close();
    return 0;
}

UINT  CSSVEP::Pic_Thread4(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic4();
}

//7.5hz闪烁
UINT  CSSVEP::Draw_pic4() {
    //CStdioFile m_outfile;
    CString str;

    //m_outfile.Open(_T("444.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    while (m_bIsbeginThread4) {
        clock_t start = clock();
        Sleep(63);
        while (clock() - start < 66);
        lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(63);
        while (clock() - start < 132);
        lpDDSBack->BltFast(rc[3][0], rc[3][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}

UINT  CSSVEP::Pic_Thread5(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic5();
}

//8.6hz闪烁
UINT  CSSVEP::Draw_pic5() {
    //CStdioFile m_outfile;
    CString str;
    //m_outfile.Open(_T("555.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[4][0], rc[4][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread5) {
        clock_t start = clock();
        Sleep(55);
        while (clock() - start < 58);
        lpDDSBack->BltFast(rc[4][0], rc[4][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(55);
        while (clock() - start < 116);
        lpDDSBack->BltFast(rc[4][0], rc[4][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
       // m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}

UINT  CSSVEP::Pic_Thread6(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic6();
}

//12hz闪烁
UINT  CSSVEP::Draw_pic6() {
    //CStdioFile m_outfile;
    CString str;
    //m_outfile.Open(_T("666.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[6][0], rc[6][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread6) {
        clock_t start = clock();
        Sleep(35);
        while (clock() - start < 42);
        lpDDSBack->BltFast(rc[6][0], rc[6][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(35);
        while (clock() - start < 84);
        lpDDSBack->BltFast(rc[6][0], rc[6][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        //m_outfile.WriteString(str);
        //m_outfile.WriteString(_T("\n"));
    }
   // m_outfile.Close();
    return 0;
}


UINT  CSSVEP::Pic_Thread7(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic7();
}
//10hz闪烁
UINT  CSSVEP::Draw_pic7() {
    //CStdioFile m_outfile;
    CString str;
    //m_outfile.Open(_T("555.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[5][0], rc[5][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread7) {
        clock_t start = clock();
        Sleep(46);
        while (clock() - start < 50);
        lpDDSBack->BltFast(rc[5][0], rc[5][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(46);
        while (clock() - start < 100);
        lpDDSBack->BltFast(rc[5][0], rc[5][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        clock_t end = clock();
        double tmp = (double)end - start;
        str.Format(_T("%lf "), tmp);
        // m_outfile.WriteString(str);
         //m_outfile.WriteString(_T("\n"));
    }
    //m_outfile.Close();
    return 0;
}


void CSSVEP::OnBnClickedStart()
{
    // TODO: 在此添加控件通知处理程序代码
    //CString SerialName;
    //CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

    //ASSERT_VALID(pMainFrame);

    //////获取RibbonBar指针
    //CMFCRibbonBar* pRibbonBar = pMainFrame->GetRibbonBar();
    //ASSERT_VALID(pRibbonBar);
    //////获取RibbonComboBox控件指针
    //CMFCRibbonComboBox* pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbonBar->FindByID(ID_COMBO2));
    //int nCurSel = pCombox->GetCurSel();
    //SerialName = pCombox->GetItem(nCurSel);

    //int n;
    //CStringArray names;
    //n = m_comm->GetCommNames(names);
    ////m_comm.exchange(addcommnd);
    //m_comm->isssvep=true;
    //////MessageBox(SerialName);
    //m_comm->SelectComm(SerialName);
    //m_comm->InitComm();
    //m_comm->Start();
    *isssvep = true;
    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
    //开启计数线程
    m_bIsbeginmytimer = TRUE;
    mytimer = AfxBeginThread(&CSSVEP::Timer_Thread, this);
    //开启打印结果线程 1.95s输出一个
    m_bIsbeginmymessage = TRUE;
    mymessage = AfxBeginThread(&CSSVEP::Message_Thread, this);
}


//停止采集数据的同时 关闭闪烁的方框 并且关闭checkout选框
void CSSVEP::OnBnClickedEnd()
{
    // TODO: 在此添加控件通知处理程序代码
    m_bIsbeginmytimer = FALSE;
    m_bIsbeginmymessage = FALSE;
    *isssvep = false;
   // m_comm->isssvep = false;
   // m_comm.CloseComm();
    if (m_check1) {
        m_check1 = !m_check1;
        m_bIsbeginThread1 = FALSE;
    }
    if (m_check2) {
        m_check2 = !m_check2;
        m_bIsbeginThread2 = FALSE;
   }
    if (m_check3) {
        m_check3 = !m_check3;
        m_bIsbeginThread3 = FALSE;
    }
    if (m_check4) {
        m_check4 = !m_check4;
        m_bIsbeginThread4 = FALSE;
    }
    if (m_check5) {
        m_check5 = !m_check5;
        m_bIsbeginThread5 = FALSE;
    }
    if (m_check6) {
        m_check6 = !m_check6;
        m_bIsbeginThread6 = FALSE;
    }
    if (m_check7) {
        m_check7 = !m_check7;
        m_bIsbeginThread7 = FALSE;
    }
    if (m_check8) {
        m_check8 = !m_check8;
        m_bIsbeginThread8 = FALSE;
    }

    counter = 0;

    //OnBnClickedCheck1();
    //OnBnClickedCheck2();
    //OnBnClickedCheck3();
    //OnBnClickedCheck4();
    //OnBnClickedCheck5();
    //OnBnClickedCheck6();
    //OnBnClickedCheck7();
    ////关闭checkout的勾选状态
    ((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(FALSE);
    GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}



void CSSVEP::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
                       // 不为绘图消息调用 CDialogEx::OnPaint()
    CRect rect;
    GetClientRect(rect);
    dc.FillSolidRect(rect, RGB(0, 0, 0));  //设置对话框为黑色背景
}


void CSSVEP::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

    if (m_bIsbeginThread1 == TRUE || m_bIsbeginThread2 == TRUE || m_bIsbeginThread3 == TRUE || m_bIsbeginThread4 == TRUE || m_bIsbeginThread5 == TRUE || m_bIsbeginThread6 == TRUE) {
        MessageBox(TEXT("请关闭图片"));
    }
   /* else if (m_comm.m_bIsbeginThread == true) {
        MessageBox(TEXT("请停止采集"));
    }*/
    else
    {
        //DestroyWindow();//销毁窗口 以免内存泄漏
      //  delete this;
        CDialogEx::OnCancel();
       
       // DestroyWindow();
    }
}


//void CSSVEP::OnBnClickedCheck7()
//{
//    // TODO: 在此添加控件通知处理程序代码
//
//    m_check7 = !m_check7;    
//    if (m_check7) 
//    {        
//        //m_bIsbeginThread1 = TRUE;        
//        //m_picthread1 = AfxBeginThread(&CSSVEP::Pic_Thread1, this); //启动线程;        
//        m_bIsbeginThread7 = TRUE;        
//        m_picthread7 = AfxBeginThread(&CSSVEP::Pic_Thread7, this); //启动线程;    
//    }    
//    else 
//    {        
//        m_bIsbeginThread7 = FALSE;    
//    }
//}


void CSSVEP::OnBnClickedCheck7()
{
    // TODO: 在此添加控件通知处理程序代码
    //    // TODO: 在此添加控件通知处理程序代码
//
    m_check7 = !m_check7;    
    if (m_check7) 
    {        
       //m_bIsbeginThread1 = TRUE;        
       //m_picthread1 = AfxBeginThread(&CSSVEP::Pic_Thread1, this); //启动线程;        
        m_bIsbeginThread7 = TRUE;        
        m_picthread7 = AfxBeginThread(&CSSVEP::Pic_Thread7, this); //启动线程;    
   }    
    else 
    {        
        m_bIsbeginThread7 = FALSE;    
    }
}

void  CSSVEP::addcommnd(int* index) {
     //一排15个 一共两排  1min的数据 然后清空 继续
    //int num;
    //if (num>=30) {
    //  //清除信息提示框
    //}
    //else {
    //    if (num >= 15) {
    //        //换行
    //    }
    //  //添加命令到信息提示框      
    //}

   // CSSVEP *tmp = (CSSVEP*)static_this;
    int tmp;
    CString str;
    CString str2;
    int length;
   // CString p;
    ((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextW(str);
    length=((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextLengthW();
    tmp = *index;
    str2.Format(_T("%d"), tmp);
    str2 += "\r\n";
    str2 += str;  
   // if (length % 5==0)
    //{
       // str2 += "\r\n";
   // }
    ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(str2);
    //tmp->SetDlgItemText(IDC_EDIT1,str2);
   
}
UINT  CSSVEP::Timer_Thread(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->startcounter();
}

UINT  CSSVEP::startcounter()
{
    while (m_bIsbeginmytimer)
    {
        //计时变量
        counter++;
        //int yushu = counter % 59;

        // str 时 str2 分 str3 秒    str2.Format(_T("%d"),index);
        int s = 0;
        int m = 0;
        int h = 0;
        int h2 = 0;
        CString str;
        CString str2;
        CString str3;
        if (counter < 60) {
            s = counter;
            str2 = _T("00");
            str3 = _T("00");
            if (s < 10)
            {
                CString temp;
                temp.Format(_T("%d"), s);
                str = _T("0");
                str += temp;
            }
            str.Format(_T("%d"), s);
        }
        else if (counter < 3600) {
            s = counter % 60;
            m = counter / 60;
            // str2 = _T("00");
            str3 = _T("00");
            if (s < 10)
            {
                CString temp;
                temp.Format(_T("%d"), s);
                str = _T("0");
                str += temp;
            }
            else {
                str.Format(_T("%d"), s);
            }
            if (m < 10)
            {
                CString temp;
                temp.Format(_T("%d"),m);
                str2 = _T("0");
                str2 += temp;
            }
            else {
                str2.Format(_T("%d"), m);
            }
        }
        else {

            h = counter / 3600;
            h2 = counter % 3600;
            m = counter / 60;
            s = counter % 60;
            // str2 = _T("00");
           // str3 = _T("00");
            if (s < 10)
            {
                CString temp;
                temp.Format(_T("%d"), s);
                str = _T("0");
                str += temp;
            }
            else {
                str.Format(_T("%d"), s);
            }
            if (m < 10)
            {
                CString temp;
                temp.Format(_T("%d"), m);
                str2 = _T("0");
                str2 += temp;
            }
            else {
                str2.Format(_T("%d"), m);
            }
            if (h < 10)
            {
                CString temp;
                temp.Format(_T("%d"), h);
                str3 = _T("0");
                str3 += temp;
            }
            else {
                str3.Format(_T("%d"), h);
            }
        };
        CString ms = _T(":");
        CString timer = str3 + ms + str2 + ms + str;
        // CString  str = _T("00:00:50");
      /*  if (m_comm->isssvep) {
            timer = _T("true");
        }
        else {
            timer = _T("false");
        }*/
        (((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowTextW(timer));
        //(((CStatic*)GetDlgItem(IDC_STATIC))->SetWindowTextW(timer));
        //1s 计数一次
        Sleep(1000);
    }
    return 0;
}

UINT  CSSVEP::Message_Thread(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->startmessage();
}

//打印信息线程
UINT  CSSVEP::startmessage() {
    while (m_bIsbeginmymessage) {
        Sleep(mydatalength3);
       // addcommnd(m_comm->matchedIndex);
        addcommnd(matchedIndexofssvep);
    }
    return 0;
}


HBRUSH CSSVEP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    if (pWnd->GetDlgCtrlID() == IDC_EDIT3) {
        pDC->SetBkMode(TRANSPARENT);
    
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}


void CSSVEP::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    *mywaysofs = false;
    ((CButton*)GetDlgItem(IDC_CHECK1))->SetWindowTextW(_T("一轴"));
    ((CButton*)GetDlgItem(IDC_CHECK2))->SetWindowTextW(_T("二轴"));
    ((CButton*)GetDlgItem(IDC_CHECK3))->SetWindowTextW(_T("三轴"));
    ((CButton*)GetDlgItem(IDC_CHECK4))->SetWindowTextW(_T("四轴"));
    ((CButton*)GetDlgItem(IDC_CHECK5))->SetWindowTextW(_T("五轴"));
    ((CButton*)GetDlgItem(IDC_CHECK6))->SetWindowTextW(_T("六轴"));
    //((CButton*)GetDlgItem(IDC_CHECK7))->SetWindowTextW(_T("一轴"));
}


void CSSVEP::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码  
    //还设置一个标志位 代表已经切换到这个模式下了
    *mywaysofs = true;
    ((CButton*)GetDlgItem(IDC_CHECK1))->SetWindowTextW(_T("   X"));
    ((CButton*)GetDlgItem(IDC_CHECK2))->SetWindowTextW(_T("   Y"));
    ((CButton*)GetDlgItem(IDC_CHECK3))->SetWindowTextW(_T("   Z"));
    ((CButton*)GetDlgItem(IDC_CHECK4))->SetWindowTextW(_T("  Rx"));
    ((CButton*)GetDlgItem(IDC_CHECK5))->SetWindowTextW(_T("  Ry"));
    ((CButton*)GetDlgItem(IDC_CHECK6))->SetWindowTextW(_T("  Rz"));
}


void CSSVEP::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    *isfanxofs = !(*isfanxofs);
  /*  if (*isfanxofs) {
        ((CButton*)GetDlgItem(IDC_BUTTON5))->SetWindowTextW(_T("正向"));
    }
    else {
        ((CButton*)GetDlgItem(IDC_BUTTON5))->SetWindowTextW(_T("反向"));
    }*/
}


void CSSVEP::OnBnClickedRealse()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T(""));
    GetDlgItem(IDC_EDIT1)->SetWindowTextW(_T(""));
}


void CSSVEP::OnBnClickedCheck8()
{
    // TODO: 在此添加控件通知处理程序代码
    m_check8 = !m_check8;
    if (m_check8)
    {
        //m_bIsbeginThread1 = TRUE;        
        //m_picthread1 = AfxBeginThread(&CSSVEP::Pic_Thread1, this); //启动线程;        
        m_bIsbeginThread8 = TRUE;
        m_picthread8 = AfxBeginThread(&CSSVEP::Pic_Thread8, this); //启动线程;    
    }
    else
    {
        m_bIsbeginThread8 = FALSE;
    }
}

UINT  CSSVEP::Pic_Thread8(LPVOID pParam) {
    CSSVEP* ddraw = (CSSVEP*)pParam;
    return ddraw->Draw_pic8();
}

//4.6hz闪烁
UINT  CSSVEP::Draw_pic8() {
    //CStdioFile m_outfile;
    CString str;
    int i = 0;
    double a[100] = { 0 };

    //m_outfile.Open(_T("111.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite);
    lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
    lpDDSBack->BltFast(rc[7][0], rc[7][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
    lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);

    while (m_bIsbeginThread8) {
        clock_t start = clock();
        Sleep(105);
        while (clock() - start < 108);
        lpDDSBack->BltFast(rc[7][0], rc[7][1], lpDDBlack, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
        Sleep(105);
        while (clock() - start < 216);
        lpDDSBack->BltFast(rc[7][0], rc[7][1], lpDDWihte, &rcRect, DDBLTFAST_WAIT);
        lpDDSPrimary->Blt(&rcClient, lpDDSBack, NULL, DDBLT_WAIT, NULL);
    }
    // m_outfile.Close();
    return 0;
}
