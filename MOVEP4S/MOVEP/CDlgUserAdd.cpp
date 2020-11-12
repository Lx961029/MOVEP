// CDlgUserAdd.cpp: 实现文件
//

#include "pch.h"
#include "MOVEP.h"
#include "CDlgUserAdd.h"
#include "afxdialogex.h"
#include "CDlgUserInfo.h"
#include "InfoFile.h"


// CDlgUserAdd 对话框

IMPLEMENT_DYNAMIC(CDlgUserAdd, CDialogEx)

CDlgUserAdd::CDlgUserAdd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_USERADD, pParent)
	, m_addname(_T(""))
	, m_addage(0)
	, m_addtype(_T(""))
{

}

CDlgUserAdd::~CDlgUserAdd()
{
}

void CDlgUserAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_addname);
	DDX_Text(pDX, IDC_EDIT2, m_addage);
	DDX_Text(pDX, IDC_EDIT3, m_addtype);
}


BEGIN_MESSAGE_MAP(CDlgUserAdd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgUserAdd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserAdd::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgUserAdd 消息处理程序


void CDlgUserAdd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	UpdateData(TRUE);
	if (m_addname.IsEmpty() || m_addage <= 0)
	{
		MessageBox(TEXT("输入信息有误"));
		return;
	}
	CInfoFile file;
	char str1[100];
	CString str;
	CString strdate;
	struct tm t;
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间

	localtime_s(&t, &now);
	sprintf(str1, "%d-%02d-%02d %02d:%02d:%02d",
		t.tm_year + 1900,
		t.tm_mon + 1,
		t.tm_mday,
		t.tm_hour,
		t.tm_min,
		t.tm_sec);
	strdate = str1;


	//添加到list控件里面去
	CDlgUserInfo* uer = (CDlgUserInfo*)GetParent();

	
	int x = uer->m_list.GetItemCount();
	str.Format(_T("%d"), x + 1);
	uer->m_list.InsertItem(x, str);
	int column = 1;
	uer->m_list.SetItemText(x, column++, m_addname);
	str.Format(_T("%d"), m_addage);
	uer->m_list.SetItemText(x, column++, str);
	string s(str1);
	uer->m_list.SetItemText(x, column++, (CString)s.c_str());
	uer->m_list.SetItemText(x, column++, m_addtype);


	file.ReadDocline(); //读取商品信息
	file.Addline(x+1,m_addname, m_addage, strdate, m_addtype); //添加商品
	file.WirteDocline(); //写文件
	file.ls.clear(); //清空list的内容

	UpdateData(FALSE);
}


void CDlgUserAdd::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
