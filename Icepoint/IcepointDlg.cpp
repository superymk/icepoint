// IcepointDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Icepoint.h"
#include "IcepointDlg.h"
#include "Tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CIcepointDlg �Ի���




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


// CIcepointDlg ��Ϣ�������

BOOL CIcepointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	plist.PListRefresh();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIcepointDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	DeleteAllItems();//ɾ���б������
	while(DeleteColumn(0));//ɾ���б�

	InsertColumn(0,(LPCTSTR)"����ID",LVCFMT_LEFT,80);//����0��
    InsertColumn(1,(LPCTSTR)"��������",LVCFMT_LEFT,150);//����1��

	HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//������ǰ�����б�

	PROCESSENTRY32* info=new PROCESSENTRY32;//����������Ϣ���������ڱ�����Ϣ
	info->dwSize=sizeof(PROCESSENTRY32);//���ÿ��С

	int i=0;
	if(Process32First(handle,info))//��ʼö�ٽ���
	{
		if(GetLastError()==ERROR_NO_MORE_FILES )
		{
			AfxMessageBox((LPCTSTR)"No More Process");
		}
		else
		{
			CString id;
			id.Format("%d",info->th32ProcessID);//��ȡ��ǰ����ID

			InsertItem(i,id);//����һ����Ŀ
			SetItemData(i,info->th32ProcessID);//������Ŀֵ
			id.Format("%s",info->szExeFile);//��ȡ��ǰ������
			SetItemText(i,1,id);//������
			i++;

			while(Process32Next(handle,info)!=FALSE)//�̳л�ȡ����
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
	CloseHandle(handle);//�رս��̾��
	//SetItemState(1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	return i;
}


BEGIN_MESSAGE_MAP(ProcessListCtrl, CListCtrl)
END_MESSAGE_MAP()



// ProcessListCtrl ��Ϣ�������



void CIcepointDlg::OnBnClickedButtonFreezing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// plist.PListRefresh();

	TCHAR szBuf[1024];
    LVITEM lvi;
	// ��ȡ��ǰѡ�����pid
	POSITION pos = plist.GetFirstSelectedItemPosition();
	if(pos)
	{
		//���ѡ���е�����
		lvi.iItem = plist.GetNextSelectedItem(pos);
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuf;
		lvi.cchTextMax = 1024;
		plist.GetItem(&lvi);
	}
	else
	{
		AfxMessageBox("����ѡ��Ҫ�ɶ����̣�");
		return;
	}

	CString FilePathName;
	CFileDialog dlg(FALSE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	if(dlg.DoModal()==IDOK)
	{
		//��ȡ�ɶ��ļ������ַ
		FilePathName=dlg.GetPathName();
		FreezingControl fc;
		fc.Freezing(atoi(lvi.pszText),(LPCTSTR)FilePathName);
	}
}

void CIcepointDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
	
	//����ˢ�½����б�
	//plist.PListRefresh();
}

void CIcepointDlg::OnBnClickedButtonUnfreezing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	if(dlg.DoModal()==IDOK)
	{
		//��ȡ���ļ���ַ
		FilePathName=dlg.GetPathName();
		UnfreezingControl ufc;
		ufc.Unfreezing((LPCTSTR)FilePathName);
	}
}
