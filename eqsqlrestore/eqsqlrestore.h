
// eqsqlrestore.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "eqemu_logsys.h"
#include "resource.h"		// ������


// CeqsqlrestoreApp: 
// �йش����ʵ�֣������ eqsqlrestore.cpp
//

class CeqsqlrestoreApp : public CWinApp
{
public:
	CeqsqlrestoreApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CeqsqlrestoreApp theApp;
