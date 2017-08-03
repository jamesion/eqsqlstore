
// eqsqlrestoreDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "eqsqlrestore.h"
#include "eqsqlrestoreDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "eqemu_logsys.h"

#pragma once

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CeqsqlrestoreDlg 对话框


IMPLEMENT_DYNAMIC(CeqsqlrestoreDlg, CDialogEx);

CeqsqlrestoreDlg::CeqsqlrestoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EQSQLRESTORE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hAdd = AfxGetApp()->LoadIcon(IDI_ADD);
	m_pAutoProxy = NULL;


}

CeqsqlrestoreDlg::~CeqsqlrestoreDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;

	if (m_MyGridCtrl != NULL)
		delete m_MyGridCtrl;
	
}

void CeqsqlrestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CeqsqlrestoreDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNET, &CeqsqlrestoreDlg::OnBnClickedConnet)
	ON_BN_CLICKED(IDC_ADD, &CeqsqlrestoreDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MUD, &CeqsqlrestoreDlg::OnBnClickedMud)
	ON_NOTIFY(GVN_BEGINLABELEDIT,IDC_GRID,&CeqsqlrestoreDlg::OnGridStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, &CeqsqlrestoreDlg::OnGridEndEdit)
	ON_BN_CLICKED(IDC_SRULE, &CeqsqlrestoreDlg::OnBnClickedSrule)
	ON_BN_CLICKED(IDC_OPENRULE, &CeqsqlrestoreDlg::OnBnClickedOpenrule)
	ON_BN_CLICKED(IDC_RELOADDEAF, &CeqsqlrestoreDlg::OnBnClickedReloaddeaf)
	ON_BN_CLICKED(IDC_RUN, &CeqsqlrestoreDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// CeqsqlrestoreDlg 消息处理程序

BOOL CeqsqlrestoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//buttonselete();


	if (m_MyGridCtrl != NULL)
		delete m_MyGridCtrl;

	m_MyGridCtrl = new CMyGridCtrl();

	if (m_MyGridCtrl == NULL)
		return FALSE;

	m_MyGridCtrl->Create(this, IDC_GRID);


//	CString list;

	m_MyGridCtrl->upcell();

	if (!LogSys.setmainDlg(this))
		return-1;

	LogSys.LoadLogSettingsDefaults();
	myeqsql = new Ceqmyslq();

//打开默认文件
	if (!m_MyGridCtrl->RuleOpen(&deshost, &schost, true))
	{
		Log(Logs::Error, Logs::Files, "[Default.rul]无法打开，请确保eqsqlrestore文件完整性！");
		MessageBox("[Default.rul]无法打开，请确保eqsqlrestore文件完整性！", "eqsqlrestore文件缺失", MB_ICONHAND);

		//OnCancel();
	}
	SethostInfo(deshost, schost);

	Setitemreadonly(false);
	//buttonselete();



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CeqsqlrestoreDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CeqsqlrestoreDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CeqsqlrestoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CeqsqlrestoreDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CeqsqlrestoreDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CeqsqlrestoreDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CeqsqlrestoreDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CeqsqlrestoreDlg::OnBnClickedConnet()
{
	string back;
	for(int nrow=1;nrow<m_MyGridCtrl->GetRowCount();nrow++)
	m_MyGridCtrl->SetItemText(nrow, 4, "");

	if (!conneted)
	{

		if (!Gethostinfo())
			return;
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(0);
		if (!myeqsql->mysql_connect(deshost, schost))
		{
			((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);
			return;
		}
		sql.Format("select table_name from information_schema.tables where table_schema='jameeq' and table_type='base table'");
		if ((back = myeqsql->runSQLCommand(sql)) != "")
		{
			Log(Logs::Error, Logs::MysqlErro, "连接：%s", back.c_str());
			((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);
			return;
		}

		if (!m_MyGridCtrl->SetComboList(LIST_MAINSHEET, myeqsql->getResult(), false))
		{
			Log(Logs::Error, Logs::MysqlErro, "设置列表失败！");
			((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);
			return;
		}



		SetDlgItemTextA(IDC_CONNET, "断开");
		conneted = myeqsql->getConnectionStatus();
		Setitemreadonly(conneted);
		m_MyGridCtrl->upcell();
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);

	}
	else 
	{
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(0);
		myeqsql->Close();
		SetDlgItemTextA(IDC_CONNET, "连接");

		conneted = myeqsql->getConnectionStatus();
		Setitemreadonly(conneted);	
		m_MyGridCtrl->upcell();
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);

	}
	return ;
}


void CeqsqlrestoreDlg::buttonselete()
{
	CButton* btn = (CButton*)GetDlgItem(IDC_ADD);

	//btn->EnableFullTextTooltip(true);
	//btn->SetIcon(m_hAdd);

	
}


void CeqsqlrestoreDlg::OnBnClickedAdd()
{
	m_MyGridCtrl->AddRowCount();

}


void CeqsqlrestoreDlg::OnBnClickedMud()
{
	m_MyGridCtrl->MudRowCount();
	
}


// 获取服务器信息
bool CeqsqlrestoreDlg::Gethostinfo()
{
	bool pas=true;
	CString val;
	//获取目标服务器
	GetDlgItemTextA(IDC_DESHOST,val);
	if (val =="0.0.0.0")
	{
		Log(Logs::Error, Logs::MysqlErro, "请正确输入目标Mysql服务器IP地址！");
		//return false;
		pas = false;
	}
	strcpy(deshost.hostip, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESUSER, val);
	if (val== "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器用户名！");
		//return false;
		pas = false;

	}
	strcpy(deshost.username, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESPASSWD, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器密码！");
		pas = false;

	}
	strcpy(deshost.pwd, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESSTORE, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器库名！");
		pas = false;

	}
	strcpy(deshost.store, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESPORT, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器端口！");
		pas = false;

	}
	strcpy(deshost.port, val);
	val.Empty();

	//获取源服务器

	GetDlgItemTextA(IDC_SCHOST, val);
	if (val == "0.0.0.0")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器IP地址！");
		//return false;
		pas = false;

	}
	strcpy(schost.hostip, val);
	val.Empty();

	GetDlgItemTextA(IDC_SCUSER, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器用户名！");
		//return false;
		pas = false;

	}
	strcpy(schost.username, val);
	val.Empty();

	GetDlgItemTextA(IDC_SCPASSWD, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器密码！");
		//return false;
		pas = false;

	}
	strcpy(schost.pwd, val);
	val.Empty();


	GetDlgItemTextA(IDC_SCRESTORE, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器库名！");
		//return false;
		pas = false;

	}
	strcpy(schost.store, val);
	val.Empty();


	GetDlgItemTextA(IDC_DESPORT, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器端口！");
		//return false;
		pas = false;

	}
	strcpy(schost.port, val);
	val.Empty();
	
	if (!pas)
		return false;
	
		return true;
}


void CeqsqlrestoreDlg::SethostInfo(hostinfo dshost,hostinfo shost)
{


		SetDlgItemTextA(IDC_DESHOST, dshost.hostip);
		SetDlgItemTextA(IDC_DESUSER, dshost.username);
		SetDlgItemTextA(IDC_DESPASSWD, dshost.pwd);
		SetDlgItemTextA(IDC_DESSTORE, dshost.store);
		SetDlgItemTextA(IDC_DESPORT, dshost.port);

		SetDlgItemTextA(IDC_SCHOST, shost.hostip);
		SetDlgItemTextA(IDC_SCUSER, shost.username);
		SetDlgItemTextA(IDC_SCPASSWD, shost.pwd);
		SetDlgItemTextA(IDC_SCRESTORE, shost.store);
		SetDlgItemTextA(IDC_SCPORT, shost.port);
//		Log(Logs::General, Logs::Normal, "重新设置服务器");

	
}



void CeqsqlrestoreDlg::Setitemreadonly(bool b_Read)
{

		Sethostinforeadonly(b_Read);
		m_MyGridCtrl->Setreadonly(!b_Read);
	

}



void CeqsqlrestoreDlg::Sethostinforeadonly(bool b_Read)
{
	((CEdit*)GetDlgItem(IDC_DESHOST))->EnableWindow(!b_Read);
	((CEdit*)GetDlgItem(IDC_DESUSER))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_DESPASSWD))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_DESSTORE))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_DESPORT))->SetReadOnly(b_Read);

	((CEdit*)GetDlgItem(IDC_SCHOST))->EnableWindow(!b_Read);
	((CEdit*)GetDlgItem(IDC_SCUSER))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_SCPASSWD))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_SCRESTORE))->SetReadOnly(b_Read);
	((CEdit*)GetDlgItem(IDC_SCPORT))->SetReadOnly(b_Read);

}
void CeqsqlrestoreDlg::OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if (map)
	{
		map = false;
		string back;

		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
		CString s_mainsheet, s_overkey;

		int irow = pItem->iRow;
		m_Cellstartstring.IsEmpty();
		m_Cellstartstring = m_MyGridCtrl->GetItemText(pItem->iRow, pItem->iColumn);

		if (pItem->iColumn == LIST_MAINSHEET)
		{

			sql.Format("select table_name from information_schema.tables where table_schema='jameeq' and table_type='base table'");
			if ((back = myeqsql->runSQLCommand(sql)) != "")
			{
				Log(Logs::Error, Logs::MysqlErro, "%s", back);
				return;
			}
//			Log(Logs::General, Logs::Normal, "%s", back.c_str());
			m_MyGridCtrl->SetComboList(LIST_MAINSHEET, myeqsql->getResult(), false);
			m_MyGridCtrl->SetCellCombo(pItem->iRow, LIST_MAINSHEET);
				
			
		}
		
		if (pItem->iColumn == LIST_KEYCOL)
		{
			while (s_mainsheet = m_MyGridCtrl->GetItemText(irow, LIST_MAINSHEET))
			{

				if (!s_mainsheet.IsEmpty())
				{
					sql.Format("select column_name from information_schema.columns where table_schema = 'jameeq' and table_name = '%s'", s_mainsheet);
					if ((back = myeqsql->runSQLCommand(sql)) != "")
					{
						Log(Logs::Error, Logs::MysqlErro, "%s", back);
						return;
					}
//					Log(Logs::General, Logs::Normal, "SQL指令：%s,maisheet:%s", sql, s_mainsheet);
					m_MyGridCtrl->SetComboList(LIST_KEYCOL, myeqsql->getResult(), false);
					m_MyGridCtrl->SetCellCombo(pItem->iRow, LIST_KEYCOL);

					break;
				}
				--irow;
				if (irow <= 0)
				{
					Log(Logs::Wranging, Logs::Setting, "请先设置主表！");
					break;
				}
			}
		}
		if (pItem->iColumn == LIST_OVERKEY)
		{
			s_overkey = m_MyGridCtrl->GetItemText(pItem->iRow, LIST_KEYCOL);


			if (!s_overkey.IsEmpty())
			{
				vector<vector<string> >().swap(overlist);
				overkey.push_back("EMU服务器ID");
				overlist.push_back(overkey);
				vector<string>().swap(overkey);
				overkey.push_back("本地服务器ID");
				overlist.push_back(overkey);
				vector<string>().swap(overkey);
				overkey.push_back("角色ID");
				overlist.push_back(overkey);
				vector<string>().swap(overkey);
				m_MyGridCtrl->SetComboList(LIST_OVERKEY, overlist, false);
				m_MyGridCtrl->SetCellCombo(pItem->iRow, LIST_OVERKEY);
				map = true;
				return;
			}
			else
			{				
				vector<vector<string> >().swap(overlist);

				m_MyGridCtrl->SetComboList(LIST_OVERKEY, overlist, false);
				m_MyGridCtrl->SetCellCombo(pItem->iRow, LIST_OVERKEY);
				Log(Logs::Wranging, Logs::Setting, "未设置需替换字段！");
				map = true;
				return;

			}
		}
	map = true;
	}
}


void CeqsqlrestoreDlg::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if (map)
	{
		map = false;

		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;

		//Log(Logs::General, Logs::Setting, "m_Cellstartstring:%s", m_Cellstartstring);

		if (pItem->iColumn == LIST_OVERKEY)
		{
			map = true;
			return;
		}
		if (m_Cellstartstring == m_MyGridCtrl->GetItemText(pItem->iRow, pItem->iColumn))
		{
//			Log(Logs::General, Logs::Setting, "m_Cellstartstring:%s", m_Cellstartstring);
			map = true;
			return;
		}

		CCellRange Selection;
		int irow = pItem->iRow;
		int maxrow = m_MyGridCtrl->GetRowCount() - 1;
		if (maxrow > pItem->iRow)
		{
			irow = pItem->iRow+1;

			while (true)
			{		
				if (irow > maxrow)
				{
					irow = maxrow;
					break;
				}

				if (irow <= maxrow)
				{
					if (m_MyGridCtrl->GetItemText(irow, pItem->iColumn) != "")
					{
						irow -= 1;
						break;
					}

					irow++;
				}



			}
		}
		if (pItem->iColumn == LIST_MAINSHEET)
		{
			Selection.SetMinRow(pItem->iRow);
			Selection.SetMinCol(LIST_KEYCOL);
			Selection.SetMaxRow(irow);
			Selection.SetMaxCol(LIST_OVERKEY);
			m_MyGridCtrl->ClearCells(Selection);

		}

		if (pItem->iColumn == LIST_KEYCOL)
		{
			Selection.SetMinRow(pItem->iRow);
			Selection.SetMinCol(LIST_OVERKEY);
			Selection.SetMaxRow(irow);
			Selection.SetMaxCol(LIST_OVERKEY);
			m_MyGridCtrl->ClearCells(Selection);

		}
	map = true;
	}
	
}


void CeqsqlrestoreDlg::OnBnClickedSrule()
{
	if (!conneted)
	{
		Log(Logs::General, Logs::Normal, "数据库未连接！");
		return;
	}
	m_MyGridCtrl->RuleSave(deshost, schost,true);

}



void CeqsqlrestoreDlg::OnBnClickedOpenrule()
{
	m_MyGridCtrl->RuleOpen(&deshost,&schost);
	SethostInfo(deshost, schost);
}


void CeqsqlrestoreDlg::OnBnClickedReloaddeaf()
{
	m_MyGridCtrl->RuleOpen(&deshost,&schost,true);
	SethostInfo(deshost, schost);
}


void CeqsqlrestoreDlg::OnBnClickedRun()
{

	if (!conneted)
	{
		Log(Logs::Error, Logs::MysqlErro, "请先连接服务器！");
		return;
	}
	if (!m_MyGridCtrl->CheckRule())
	{
		return;
	}

	string rulegroup,info;

	int nrow = 1, nrow_old = 0;

	m_MyGridCtrl->Setreadonly(true);

	while (true)
	{
		nrow_old = nrow;
		nrow = m_MyGridCtrl->ReadRuleGroup(nrow, &rulegroup);

		if ((info=myeqsql->CopyData(rulegroup))!="")
		{

			Log(Logs::Error, Logs::MysqlErro, "%s", info.c_str());
			m_MyGridCtrl->SetItemFgColour(nrow_old, 4, RGB(255, 0, 0));
			m_MyGridCtrl->SetItemText(nrow_old, 4, info.c_str());
		}
		else
		{
			m_MyGridCtrl->SetItemFgColour(nrow_old, 4, RGB(0, 255, 0));
			m_MyGridCtrl->SetItemText(nrow_old, 4, "数据表维护完成.");
		}

		rulegroup.clear();

		if(!nrow)
		{
			break;
		}
	}
	if (info == "")
	{
		Log(Logs::General, Logs::Normal, "所有数据恢复完成.");

	}
	else
	{
		Log(Logs::Wranging, Logs::MysqlErro, "数据恢复有误： %s", info.c_str());
	}


	m_MyGridCtrl->Setreadonly(false);
}
