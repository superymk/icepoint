// Icepoint.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Icepoint.h"
#include "IcepointDlg.h"

#include "Chronosphere.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIcepointApp

BEGIN_MESSAGE_MAP(CIcepointApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIcepointApp ����

CIcepointApp::CIcepointApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	ChronInit();
}


// Ψһ��һ�� CIcepointApp ����

CIcepointApp theApp;


// CIcepointApp ��ʼ��

BOOL CIcepointApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CIcepointDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//�ɶ�ʵ�ֽӿ�
void FreezingControl::Freezing(DWORD pid, const char *file_dir)
{
	//�ɶ�ʵ�ִ���
	/*CString msg;
	msg.Format("ѡ�н��̺�: %d\n����·��: %s", pid, file_dir);
	AfxMessageBox(msg);*/
	SaveProcessState(pid, file_dir);

}

//�ⶳʵ�ֽӿڣ����ؽⶳ����̵�pid
DWORD UnfreezingControl::Unfreezing(const char *file_dir)
{
	//�ⶳʵ�ִ���
	CString msg;
	msg.Format("���ļ�·��: %s", file_dir);
	AfxMessageBox(msg);

	return NULL;
}
