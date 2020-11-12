#pragma once

// CDlgUserInfo 对话框

class CDlgUserInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserInfo)

public:
	CDlgUserInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgUserInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_USERINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 控制List控件
	CListCtrl m_list;
	int itemcolumn;

	afx_msg void OnBnClickedAddUser();
	afx_msg void OnBnClickedChangeUser();
	afx_msg void OnBnClickedDeletUser();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
