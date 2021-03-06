// Icepoint.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Icepoint.h"
#include "IcepointDlg.h"

#include "Chronosphere.h"
//#include "HideProcess.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIcepointApp

BEGIN_MESSAGE_MAP(CIcepointApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIcepointApp 构造

CIcepointApp::CIcepointApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	ChronInit();
}


// 唯一的一个 CIcepointApp 对象

CIcepointApp theApp;


// CIcepointApp 初始化

BOOL CIcepointApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CIcepointDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//干冻实现接口
void FreezingControl::Freezing(DWORD pid, const char *file_dir)
{
	//干冻实现代码
	/*CString msg;
	msg.Format("选中进程号: %d\n保存路径: %s", pid, file_dir);
	AfxMessageBox(msg);*/
	SaveProcessState(pid, file_dir);

}

//隐藏进程实现接口
void FreezingControl::Hide(DWORD pid)
{
	//实现代码
	CString msg;
	msg.Format("选中进程号: %d\n", pid);
	AfxMessageBox(msg);
//	HideProcess();
}

//解冻实现接口，返回解冻后进程的pid
DWORD UnfreezingControl::Unfreezing(const char *file_dir)
{
	//解冻实现代码
	/*CString msg;
	msg.Format("打开文件路径: %s", file_dir);
	AfxMessageBox(msg);*/
	LoadProcessState(file_dir);

	return NULL;
}
