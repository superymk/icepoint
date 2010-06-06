// IcepointDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Icepoint.h"
#include "IcepointDlg.h"
#include "Tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIcepointDlg 对话框




CIcepointDlg::CIcepointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIcepointDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIcepointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTPROCESS, plist);
}

BEGIN_MESSAGE_MAP(CIcepointDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FREEZING, &CIcepointDlg::OnBnClickedButtonFreezing)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_UNFREEZING, &CIcepointDlg::OnBnClickedButtonUnfreezing)
END_MESSAGE_MAP()


// CIcepointDlg 消息处理程序

BOOL CIcepointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	plist.PListRefresh();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIcepointDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIcepointDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIcepointDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ProcessListCtrl

IMPLEMENT_DYNAMIC(ProcessListCtrl, CListCtrl)

ProcessListCtrl::ProcessListCtrl()
{
}

ProcessListCtrl::~ProcessListCtrl()
{
}

int ProcessListCtrl::PListRefresh()
{
	DeleteAllItems();//删除列表框所有
	while(DeleteColumn(0));//删除列标

	InsertColumn(0,(LPCTSTR)"进程ID",LVCFMT_LEFT,80);//插入0列
    InsertColumn(1,(LPCTSTR)"进程名称",LVCFMT_LEFT,150);//插入1列

	HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//创建当前快照列表

	PROCESSENTRY32* info=new PROCESSENTRY32;//创建进程信息变量，用于保存信息
	info->dwSize=sizeof(PROCESSENTRY32);//设置块大小

	int i=0;
	if(Process32First(handle,info))//开始枚举进程
	{
		if(GetLastError()==ERROR_NO_MORE_FILES )
		{
			AfxMessageBox((LPCTSTR)"No More Process");
		}
		else
		{
			CString id;
			id.Format("%d",info->th32ProcessID);//获取当前进程ID

			InsertItem(i,id);//插入一个项目
			SetItemData(i,info->th32ProcessID);//设置项目值
			id.Format("%s",info->szExeFile);//获取当前进程名
			SetItemText(i,1,id);//设置名
			i++;

			while(Process32Next(handle,info)!=FALSE)//继承获取进程
			{
				id.Format("%5d",info->th32ProcessID);

				InsertItem(i,id);
				SetItemData(i,info->th32ProcessID);
				id.Format("%s",info->szExeFile);
				SetItemText(i,1,id);
				i++;
			}
		}
	}
	CloseHandle(handle);//关闭进程句柄
	//SetItemState(1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	return i;
}


BEGIN_MESSAGE_MAP(ProcessListCtrl, CListCtrl)
END_MESSAGE_MAP()



// ProcessListCtrl 消息处理程序



void CIcepointDlg::OnBnClickedButtonFreezing()
{
	// TODO: 在此添加控件通知处理程序代码
	// plist.PListRefresh();

	TCHAR szBuf[1024];
    LVITEM lvi;
	// 获取当前选中项的pid
	POSITION pos = plist.GetFirstSelectedItemPosition();
	if(pos)
	{
		//获得选中行的内容
		lvi.iItem = plist.GetNextSelectedItem(pos);
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuf;
		lvi.cchTextMax = 1024;
		plist.GetItem(&lvi);
	}
	else
	{
		AfxMessageBox("请先选择要干冻进程！");
		return;
	}

	CString FilePathName;
	CFileDialog dlg(FALSE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if(dlg.DoModal()==IDOK)
	{
		//获取干冻文件保存地址
		FilePathName=dlg.GetPathName();
		FreezingControl fc;
		fc.Freezing(atoi(lvi.pszText),(LPCTSTR)FilePathName);
	}
}

void CIcepointDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
	
	//定期刷新进程列表
	//plist.PListRefresh();
}

void CIcepointDlg::OnBnClickedButtonUnfreezing()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if(dlg.DoModal()==IDOK)
	{
		//获取打开文件地址
		FilePathName=dlg.GetPathName();
		UnfreezingControl ufc;
		ufc.Unfreezing((LPCTSTR)FilePathName);
	}
}
