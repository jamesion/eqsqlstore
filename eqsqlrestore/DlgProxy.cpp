
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "eqsqlrestore.h"
#include "DlgProxy.h"
#include "eqsqlrestoreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CeqsqlrestoreDlgAutoProxy

IMPLEMENT_DYNCREATE(CeqsqlrestoreDlgAutoProxy, CCmdTarget)

CeqsqlrestoreDlgAutoProxy::CeqsqlrestoreDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CeqsqlrestoreDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CeqsqlrestoreDlg)))
		{
			m_pDialog = reinterpret_cast<CeqsqlrestoreDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CeqsqlrestoreDlgAutoProxy::~CeqsqlrestoreDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CeqsqlrestoreDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CeqsqlrestoreDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CeqsqlrestoreDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_Ieqsqlrestore ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {F85E9902-0D2E-4863-BBB5-6D49F8F6EA6D}
static const IID IID_Ieqsqlrestore =
{ 0xF85E9902, 0xD2E, 0x4863, { 0xBB, 0xB5, 0x6D, 0x49, 0xF8, 0xF6, 0xEA, 0x6D } };

BEGIN_INTERFACE_MAP(CeqsqlrestoreDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CeqsqlrestoreDlgAutoProxy, IID_Ieqsqlrestore, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {9B7DD6F1-6C66-4861-9E73-200B6A3EC4F9}
IMPLEMENT_OLECREATE2(CeqsqlrestoreDlgAutoProxy, "eqsqlrestore.Application", 0x9b7dd6f1, 0x6c66, 0x4861, 0x9e, 0x73, 0x20, 0xb, 0x6a, 0x3e, 0xc4, 0xf9)


// CeqsqlrestoreDlgAutoProxy ��Ϣ�������
