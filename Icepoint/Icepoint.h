// Icepoint.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIcepointApp:
// �йش����ʵ�֣������ Icepoint.cpp
//

class CIcepointApp : public CWinApp
{
public:
	CIcepointApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

class FreezingControl
{
public:
	FreezingControl(){};

public:
	void Freezing(DWORD pid, const char *file_dir);//�ɶ�ʵ�ֽӿ�
	void Hide(DWORD pid);
};

class UnfreezingControl
{
public:
	UnfreezingControl(){};

public:
	DWORD Unfreezing(const char *file_dir);//�ⶳʵ�ֽӿڣ����ؽⶳ����̵�pid
};

extern CIcepointApp theApp;