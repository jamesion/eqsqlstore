
// eqsqlrestoreDlg.h : 头文件
//


#include "MyGridCtrl.h"

#pragma once

class CeqsqlrestoreDlgAutoProxy;


// CeqsqlrestoreDlg 对话框
class CeqsqlrestoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CeqsqlrestoreDlg);
	friend class CeqsqlrestoreDlgAutoProxy;

// 构造
public:
	CeqsqlrestoreDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CeqsqlrestoreDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQSQLRESTORE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CeqsqlrestoreDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
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
