
// eqsqlrestoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "eqsqlrestore.h"
#include "eqsqlrestoreDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "eqemu_logsys.h"


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
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
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


	CString list;
//	list.Append("this's one para|this's two para|this's three para|");
//	m_MyGridCtrl->SetComboList(LIST_OVERKEY, , false);

	m_MyGridCtrl->upcell();

	if (!LogSys.setmainDlg(this))
		return-1;

	LogSys.LoadLogSettingsDefaults();
	myeqsql = new Ceqmyslq();

	SethostInfo();

	Setitemreadonly(false);

	// TODO: 在此添加额外的初始化代码

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
	if (!conneted)
	{
		if (!Gethostinfo())
			return;
		if (!myeqsql->mysql_connect(deshost, schost))
			return;
		string sql;
		sql.append("select table_name from information_schema.tables where table_schema='jameeq' and table_type='base table'");
		myeqsql->runSQLCommand(sql.c_str());
		m_MyGridCtrl->SetComboList(LIST_MAINSHEET, myeqsql->getResult(), false);
		Setitemreadonly(TRUE);
		Log(Logs::General, Logs::Normal, "sql：%s", sql.c_str());
		SetDlgItemTextA(IDC_CONNET, "断开");
		conneted = true;
		
	}
	else 
	{
		myeqsql->Close();
		SetDlgItemTextA(IDC_CONNET, "连接");
		Setitemreadonly(false);
		conneted = false;
	}
	return ;
}


/*void CeqsqlrestoreDlg::buttonselete()
{
	CButton* btn = (CButton*)GetDlgItem(IDC_CONNET);

	//btn->EnableFullTextTooltip(true);
	btn->SetIcon(NULL);

	
}
*/

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
	//获取目标服务器
	GetDlgItemTextA(IDC_DESHOST, deshost.hostip);
	if (deshost.hostip=="0.0.0.0")
	{
		Log(Logs::Error, Logs::MysqlErro, "请正确输入目标Mysql服务器IP地址！");
		//return false;
		pas = false;
	}
	GetDlgItemTextA(IDC_DESUSER, deshost.username);
	if (deshost.username== "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器用户名！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPASSWD, deshost.pwd);
	if (deshost.pwd == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器密码！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESSTORE, deshost.store);
	if (deshost.store == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器库名！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPORT, deshost.port);
	if (deshost.port == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入目标Mysql服务器端口！");
		//return false;
		pas = false;

	}


	//获取源服务器

	GetDlgItemTextA(IDC_SCHOST, schost.hostip);
	if (schost.hostip == "0.0.0.0")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器IP地址！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCUSER, schost.username);
	if (schost.username == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器用户名！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCPASSWD, schost.pwd);
	if (schost.pwd == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器密码！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCRESTORE, schost.store);
	if (schost.store == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器库名！");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPORT, schost.port);
	if (schost.port == "")
	{
		Log(Logs::General, Logs::MysqlErro, "请正确输入源Mysql服务器端口！");
		//return false;
		pas = false;

	}
	
	if (!pas)
		return false;
	
		return true;
}


bool CeqsqlrestoreDlg::SethostInfo()
{
	SetDlgItemTextA(IDC_DESHOST, "127.0.0.1");
	SetDlgItemTextA(IDC_DESUSER, "root");
	SetDlgItemTextA(IDC_DESPASSWD, "tylz");
	SetDlgItemTextA(IDC_DESSTORE, "jameeq");
	SetDlgItemTextA(IDC_DESPORT, "3306");

	SetDlgItemTextA(IDC_SCHOST, "127.0.0.1");
	SetDlgItemTextA(IDC_SCUSER, "root");
	SetDlgItemTextA(IDC_SCPASSWD, "tylz");
	SetDlgItemTextA(IDC_SCRESTORE, "jameeq");
	SetDlgItemTextA(IDC_SCPORT, "3306");



	return false;
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
