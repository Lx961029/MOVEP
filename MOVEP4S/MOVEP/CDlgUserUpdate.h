#pragma once


// CDlgUserUpdate 对话框

class CDlgUserUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserUpdate)

public:
	CDlgUserUpdate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgUserUpdate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DIALOG_USERUPDATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_updatename;
	int m_updateage;
	CString m_updatetype;
	int nSelected ;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
