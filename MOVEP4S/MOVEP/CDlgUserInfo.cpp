// CDlgUserInfo.cpp: 实现文件
//

#include "pch.h"
#include "MOVEP.h"
#include "CDlgUserInfo.h"
#include "afxdialogex.h"
#include "InfoFile.h"
#include "CDlgUserAdd.h"
#include "CDlgUserUpdate.h"
#include <comdef.h> 
#include <stdlib.h>
// CDlgUserInfo 对话框

IMPLEMENT_DYNAMIC(CDlgUserInfo, CDialogEx)

CDlgUserInfo::CDlgUserInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_USERINFO, pParent)
{
	itemcolumn = 0;
}

CDlgUserInfo::~CDlgUserInfo()
{
}

void CDlgUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgUserInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgUserInfo::OnBnClickedAddUser)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgUserInfo::OnBnClickedChangeUser)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgUserInfo::OnBnClickedDeletUser)
	ON_BN_CLICKED(IDOK, &CDlgUserInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgUserInfo 消息处理程序


void CDlgUserInfo::OnBnClickedAddUser()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgUserAdd m_useradd;
	m_useradd.DoModal();
}


void CDlgUserInfo::OnBnClickedChangeUser()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgUserUpdate m_updateuser;
	m_updateuser.DoModal();
}


void CDlgUserInfo::OnBnClickedDeletUser()
{
	// TODO: 在此添加控件通知处理程序代码
	CInfoFile file;
	int i = 0;
	int nSelected;
	CString str;
	// TODO: 在此添加控件通知处理程序代码
	POSITION p = m_list.GetFirstSelectedItemPosition(); //获取首选中行位置

	while (p)
	{
		int x = m_list.GetItemCount();//获得总行数
		nSelected = m_list.GetNextSelectedItem(p); //获取选中行的索引
		m_list.DeleteItem(nSelected); //根据索引删除

		p = m_list.GetFirstSelectedItemPosition();
		file.ReadDocline(); //读取商品信息
		int longth = file.ls.size();
		for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); i++)
		{
			if (i == nSelected == (longth -1)) {
				file.ls.pop_back();
				break;
			}
			if (i == nSelected)
			{
				it = file.ls.erase(it);
			}
			else
				it++;
		}
		file.WirteDocline(); //写文件
		file.ls.clear(); //清空list的内容


		for (i = nSelected; i < x - 1; i++)
		{
			str.Format(_T("%d"), i + 1);
			m_list.SetItemText(i, 0, str);
		}
	}
	UpdateData(FALSE);
}


void CDlgUserInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDialogEx::OnOK();
}


void CDlgUserInfo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgUserInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CInfoFile file;
	// TODO:  在此添加额外的初始化
		// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 初始化表头
	CString field[] = { _T("序号"), _T("测试者"), _T("年龄"), _T("测试日期"), _T("实验类型") };

	m_list.InsertColumn(0, field[0], LVCFMT_CENTER, 50);
	for (int i = 1; i < sizeof(field) / sizeof(field[0]); i++)
	{
		m_list.InsertColumn(i, field[i], LVCFMT_CENTER, 120);
	}

	//需要包含#include "InfoFile.h"

	file.ReadDocline(); //读取商品信息

	//添加数据
	int i = 0;
	CString str;
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		str.Format(_T("%d"), it->id);
		m_list.InsertItem(i, str);
		int column = 1;
		str = it->name;
		m_list.SetItemText(i, column++, str);
		str.Format(_T("%d"), it->age);
		m_list.SetItemText(i, column++, str);
		str = it->date;
		m_list.SetItemText(i, column++, str);
		str = it->type;
		m_list.SetItemText(i, column++, str);
		i++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


