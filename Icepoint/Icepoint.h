// Icepoint.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CIcepointApp:
// 有关此类的实现，请参阅 Icepoint.cpp
//

class CIcepointApp : public CWinApp
{
public:
	CIcepointApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

class FreezingControl
{
public:
	FreezingControl(){};

public:
	void Freezing(DWORD pid, const char *file_dir);//干冻实现接口
	void Hide(DWORD pid);
};

class UnfreezingControl
{
public:
	UnfreezingControl(){};

public:
	DWORD Unfreezing(const char *file_dir);//解冻实现接口，返回解冻后进程的pid
};

extern CIcepointApp theApp;