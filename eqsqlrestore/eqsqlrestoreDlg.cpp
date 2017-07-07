
// eqsqlrestoreDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CeqsqlrestoreDlg �Ի���


IMPLEMENT_DYNAMIC(CeqsqlrestoreDlg, CDialogEx);

CeqsqlrestoreDlg::CeqsqlrestoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EQSQLRESTORE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;

}

CeqsqlrestoreDlg::~CeqsqlrestoreDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
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


// CeqsqlrestoreDlg ��Ϣ�������

BOOL CeqsqlrestoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CeqsqlrestoreDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CeqsqlrestoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

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
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
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
		Log(Logs::General, Logs::Normal, "sql��%s", sql.c_str());
		SetDlgItemTextA(IDC_CONNET, "�Ͽ�");
		conneted = true;
		
	}
	else 
	{
		myeqsql->Close();
		SetDlgItemTextA(IDC_CONNET, "����");
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


// ��ȡ��������Ϣ
bool CeqsqlrestoreDlg::Gethostinfo()
{
	bool pas=true;
	//��ȡĿ�������
	GetDlgItemTextA(IDC_DESHOST, deshost.hostip);
	if (deshost.hostip=="0.0.0.0")
	{
		Log(Logs::Error, Logs::MysqlErro, "����ȷ����Ŀ��Mysql������IP��ַ��");
		//return false;
		pas = false;
	}
	GetDlgItemTextA(IDC_DESUSER, deshost.username);
	if (deshost.username== "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql�������û�����");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPASSWD, deshost.pwd);
	if (deshost.pwd == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql���������룡");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESSTORE, deshost.store);
	if (deshost.store == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql������������");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPORT, deshost.port);
	if (deshost.port == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql�������˿ڣ�");
		//return false;
		pas = false;

	}


	//��ȡԴ������

	GetDlgItemTextA(IDC_SCHOST, schost.hostip);
	if (schost.hostip == "0.0.0.0")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql������IP��ַ��");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCUSER, schost.username);
	if (schost.username == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql�������û�����");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCPASSWD, schost.pwd);
	if (schost.pwd == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql���������룡");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_SCRESTORE, schost.store);
	if (schost.store == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql������������");
		//return false;
		pas = false;

	}
	GetDlgItemTextA(IDC_DESPORT, schost.port);
	if (schost.port == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql�������˿ڣ�");
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
