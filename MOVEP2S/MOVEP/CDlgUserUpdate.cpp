// CDlgUserUpdate.cpp: 实现文件
//

#include "pch.h"
#include "MOVEP.h"
#include "CDlgUserUpdate.h"
#include "afxdialogex.h"
#include "InfoFile.h"
#include "CDlgUserInfo.h"

// CDlgUserUpdate 对话框

IMPLEMENT_DYNAMIC(CDlgUserUpdate, CDialogEx)

CDlgUserUpdate::CDlgUserUpdate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_USERUPDATE, pParent)
	, m_updatename(_T(""))
	, m_updateage(0)
	, m_updatetype(_T(""))
{
	nSelected = 0;
}

CDlgUserUpdate::~CDlgUserUpdate()
{
}

void CDlgUserUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_updatename);
	DDX_Text(pDX, IDC_EDIT2, m_updateage);
	DDX_Text(pDX, IDC_EDIT3, m_updatetype);
}


BEGIN_MESSAGE_MAP(CDlgUserUpdate, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgUserUpdate::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserUpdate::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgUserUpdate 消息处理程序


void CDlgUserUpdate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = 0;
	CString str;
	CInfoFile file;

	CDlgUserInfo* uer = (CDlgUserInfo*)GetParent();
	//更新时间
	UpdateData(TRUE);
	char str1[100];
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
	file.ReadDocline(); //读取商品信息

	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if (nSelected == i)
		{
			CStringA strA;
			it->id = nSelected;
			strA = m_updatename;
			strcpy(it->name, strA.GetBuffer());
			it->age = m_updateage;
			strcpy(it->date, str1);
			strA = m_updatetype;
			strcpy(it->type, strA.GetBuffer());
		}
		i++;
	}
	file.WirteDocline(); //写文件
	file.ls.clear(); //清空list的内容

//添加到list控件里面去
	int column = 1;
	uer->m_list.SetItemText(nSelected, column++, m_updatename);
	str.Format(_T("%d"), m_updateage);
	uer->m_list.SetItemText(nSelected, column++, str);
	string s(str1);
	uer->m_list.SetItemText(nSelected, column++, (CString)s.c_str());
	uer->m_list.SetItemText(nSelected, column++, m_updatetype);
	UpdateData(FALSE);

	CDialogEx::OnOK();
}


void CDlgUserUpdate::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgUserUpdate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDlgUserInfo* uer = (CDlgUserInfo*)GetParent();
	POSITION p = uer->m_list.GetFirstSelectedItemPosition(); //获取首选中行位置
	while (p)
	{
		int x = uer->m_list.GetItemCount();//获得总行数
		nSelected = uer->m_list.GetNextSelectedItem(p); //获取选中行的索引
		m_updatename=uer->m_list.GetItemText(nSelected,1);
		m_updateage = _ttoi( uer->m_list.GetItemText(nSelected, 2));
		m_updatetype = uer->m_list.GetItemText(nSelected, 4);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
