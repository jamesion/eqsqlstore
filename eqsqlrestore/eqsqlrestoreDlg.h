
// eqsqlrestoreDlg.h : ͷ�ļ�
//


#include "MyGridCtrl.h"

#pragma once

class CeqsqlrestoreDlgAutoProxy;


// CeqsqlrestoreDlg �Ի���
class CeqsqlrestoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CeqsqlrestoreDlg);
	friend class CeqsqlrestoreDlgAutoProxy;

// ����
public:
	CeqsqlrestoreDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CeqsqlrestoreDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQSQLRESTORE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CeqsqlrestoreDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnet();
	CMyGridCtrl* m_MyGridCtrl=NULL;
	void buttonselete();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedMud();
};
