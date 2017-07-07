
// eqsqlrestore.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "eqemu_logsys.h"
#include "resource.h"		// 主符号


// CeqsqlrestoreApp: 
// 有关此类的实现，请参阅 eqsqlrestore.cpp
//

class CeqsqlrestoreApp : public CWinApp
{
public:
	CeqsqlrestoreApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CeqsqlrestoreApp theApp;
