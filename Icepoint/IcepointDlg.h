// IcepointDlg.h : ͷ�ļ�
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
	int PListRefresh();    //ˢ�½��̿��գ����ؿ����еĽ�����Ŀ
};

// CIcepointDlg �Ի���
class CIcepointDlg : public CDialog
{
// ����
public:
	CIcepointDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ICEPOINT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��ʾ�ʹ洢�����б������Ϣ
	ProcessListCtrl plist;
	afx_msg void OnBnClickedButtonFreezing();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonUnfreezing();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
#pragma once

