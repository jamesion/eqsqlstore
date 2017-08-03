
// eqsqlrestoreDlg.cpp : ʵ���ļ�
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
	//m_hAdd = AfxGetApp()->LoadIcon(IDI_ADD);
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
	ON_NOTIFY(GVN_BEGINLABELEDIT,IDC_GRID,&CeqsqlrestoreDlg::OnGridStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, &CeqsqlrestoreDlg::OnGridEndEdit)
	ON_BN_CLICKED(IDC_SRULE, &CeqsqlrestoreDlg::OnBnClickedSrule)
	ON_BN_CLICKED(IDC_OPENRULE, &CeqsqlrestoreDlg::OnBnClickedOpenrule)
	ON_BN_CLICKED(IDC_RELOADDEAF, &CeqsqlrestoreDlg::OnBnClickedReloaddeaf)
	ON_BN_CLICKED(IDC_RUN, &CeqsqlrestoreDlg::OnBnClickedRun)
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
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
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


//	CString list;

	m_MyGridCtrl->upcell();

	if (!LogSys.setmainDlg(this))
		return-1;

	LogSys.LoadLogSettingsDefaults();
	myeqsql = new Ceqmyslq();

//��Ĭ���ļ�
	if (!m_MyGridCtrl->RuleOpen(&deshost, &schost, true))
	{
		Log(Logs::Error, Logs::Files, "[Default.rul]�޷��򿪣���ȷ��eqsqlrestore�ļ������ԣ�");
		MessageBox("[Default.rul]�޷��򿪣���ȷ��eqsqlrestore�ļ������ԣ�", "eqsqlrestore�ļ�ȱʧ", MB_ICONHAND);

		//OnCancel();
	}
	SethostInfo(deshost, schost);

	Setitemreadonly(false);
	//buttonselete();



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
			Log(Logs::Error, Logs::MysqlErro, "���ӣ�%s", back.c_str());
			((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);
			return;
		}

		if (!m_MyGridCtrl->SetComboList(LIST_MAINSHEET, myeqsql->getResult(), false))
		{
			Log(Logs::Error, Logs::MysqlErro, "�����б�ʧ�ܣ�");
			((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);
			return;
		}



		SetDlgItemTextA(IDC_CONNET, "�Ͽ�");
		conneted = myeqsql->getConnectionStatus();
		Setitemreadonly(conneted);
		m_MyGridCtrl->upcell();
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(1);

	}
	else 
	{
		((CButton*)GetDlgItem(IDC_CONNET))->EnableWindow(0);
		myeqsql->Close();
		SetDlgItemTextA(IDC_CONNET, "����");

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


// ��ȡ��������Ϣ
bool CeqsqlrestoreDlg::Gethostinfo()
{
	bool pas=true;
	CString val;
	//��ȡĿ�������
	GetDlgItemTextA(IDC_DESHOST,val);
	if (val =="0.0.0.0")
	{
		Log(Logs::Error, Logs::MysqlErro, "����ȷ����Ŀ��Mysql������IP��ַ��");
		//return false;
		pas = false;
	}
	strcpy(deshost.hostip, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESUSER, val);
	if (val== "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql�������û�����");
		//return false;
		pas = false;

	}
	strcpy(deshost.username, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESPASSWD, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql���������룡");
		pas = false;

	}
	strcpy(deshost.pwd, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESSTORE, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql������������");
		pas = false;

	}
	strcpy(deshost.store, val);
	val.Empty();

	GetDlgItemTextA(IDC_DESPORT, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����Ŀ��Mysql�������˿ڣ�");
		pas = false;

	}
	strcpy(deshost.port, val);
	val.Empty();

	//��ȡԴ������

	GetDlgItemTextA(IDC_SCHOST, val);
	if (val == "0.0.0.0")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql������IP��ַ��");
		//return false;
		pas = false;

	}
	strcpy(schost.hostip, val);
	val.Empty();

	GetDlgItemTextA(IDC_SCUSER, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql�������û�����");
		//return false;
		pas = false;

	}
	strcpy(schost.username, val);
	val.Empty();

	GetDlgItemTextA(IDC_SCPASSWD, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql���������룡");
		//return false;
		pas = false;

	}
	strcpy(schost.pwd, val);
	val.Empty();


	GetDlgItemTextA(IDC_SCRESTORE, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql������������");
		//return false;
		pas = false;

	}
	strcpy(schost.store, val);
	val.Empty();


	GetDlgItemTextA(IDC_DESPORT, val);
	if (val == "")
	{
		Log(Logs::General, Logs::MysqlErro, "����ȷ����ԴMysql�������˿ڣ�");
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
//		Log(Logs::General, Logs::Normal, "�������÷�����");

	
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
//					Log(Logs::General, Logs::Normal, "SQLָ�%s,maisheet:%s", sql, s_mainsheet);
					m_MyGridCtrl->SetComboList(LIST_KEYCOL, myeqsql->getResult(), false);
					m_MyGridCtrl->SetCellCombo(pItem->iRow, LIST_KEYCOL);

					break;
				}
				--irow;
				if (irow <= 0)
				{
					Log(Logs::Wranging, Logs::Setting, "������������");
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
				overkey.push_back("EMU������ID");
				overlist.push_back(overkey);
				vector<string>().swap(overkey);
				overkey.push_back("���ط�����ID");
				overlist.push_back(overkey);
				vector<string>().swap(overkey);
				overkey.push_back("��ɫID");
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
				Log(Logs::Wranging, Logs::Setting, "δ�������滻�ֶΣ�");
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
		Log(Logs::General, Logs::Normal, "���ݿ�δ���ӣ�");
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
		Log(Logs::Error, Logs::MysqlErro, "�������ӷ�������");
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
			m_MyGridCtrl->SetItemText(nrow_old, 4, "���ݱ�ά�����.");
		}

		rulegroup.clear();

		if(!nrow)
		{
			break;
		}
	}
	if (info == "")
	{
		Log(Logs::General, Logs::Normal, "�������ݻָ����.");

	}
	else
	{
		Log(Logs::Wranging, Logs::MysqlErro, "���ݻָ����� %s", info.c_str());
	}


	m_MyGridCtrl->Setreadonly(false);
}
