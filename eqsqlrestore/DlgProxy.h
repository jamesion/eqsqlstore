
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CeqsqlrestoreDlg;


// CeqsqlrestoreDlgAutoProxy ����Ŀ��

class CeqsqlrestoreDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CeqsqlrestoreDlgAutoProxy)

	CeqsqlrestoreDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CeqsqlrestoreDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CeqsqlrestoreDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CeqsqlrestoreDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

