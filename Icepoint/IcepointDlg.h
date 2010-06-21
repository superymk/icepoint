// IcepointDlg.h : 头文件
//

#pragma once

// ProcessListCtrl

class ProcessListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(ProcessListCtrl)

public:
	ProcessListCtrl();
	virtual ~ProcessListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int PListRefresh();    //刷新进程快照，返回快照中的进程数目
};

// CIcepointDlg 对话框
class CIcepointDlg : public CDialog
{
// 构造
public:
	CIcepointDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ICEPOINT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 显示和存储进程列表快照信息
	ProcessListCtrl plist;
	afx_msg void OnBnClickedButtonFreezing();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonUnfreezing();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
#pragma once

