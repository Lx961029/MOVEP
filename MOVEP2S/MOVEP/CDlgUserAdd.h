#pragma once


// CDlgUserAdd 对话框

class CDlgUserAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserAdd)

public:
	CDlgUserAdd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgUserAdd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_USERADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_addname;
	int m_addage;
	CString m_addtype;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
