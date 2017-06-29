#include "stdafx.h"
#include "MyGridCtrl.h"

#include <string>


using namespace std;

CMyGridCtrl::CMyGridCtrl()
{
	m_MxRow = 1;
	m_MxCol = 5;

	mainsheet.Add(NULL);
	keycol.Add(NULL);
	overkey.Add(NULL);

}


CMyGridCtrl::~CMyGridCtrl()
{
}



bool CMyGridCtrl::Create(CWnd* parent, UINT nID, DWORD dwStyle)
{

	if (parent == NULL&&nID == NULL)
		return false;

	ASSERT(parent->GetSafeHwnd());

	CWnd* pWnd = parent->GetDlgItem(IDC_GRID);

	if (!pWnd)
		return false;

	int captionHeight = GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME) * 2 + 1;
	int cyframe = GetSystemMetrics(SM_CYFRAME) * 2 + 1;

	parent->GetWindowRect(&rectdlg);//��ȡ�Ի��򴰿ڵ�λ����Ϣ


	pWnd->GetWindowRect(&rectgrid); //��ȡռλ�ؼ���λ����Ϣ

									//�����ؼ�  
	//m_GridCtrl = new CGridCtrl();
	CGridCtrl::Create(CRect(rectgrid.left - rectdlg.left - cxframe,
		rectgrid.top - rectdlg.top - cyframe - captionHeight,
		rectgrid.left + rectgrid.Width() - rectdlg.left - cyframe,
		rectgrid.top + rectgrid.Height() - rectdlg.top - captionHeight - cxframe), parent, nID, dwStyle);

	//if (!m_GridCtrl)
		//return false;
	//���õ�һ�к͵�һ��Ϊ�ؼ��̶��к��У��������кͱ�����
	this->SetFixedRowCount(1);
	this->SetFixedColumnCount(1);

	this->SetColumnCount(m_MxCol);
	this->SetRowCount(m_MxRow);
	return true;
}

bool CMyGridCtrl::upcell()
{


	//���ù̶��б���
	string nCol;
	string sCol[5];
	sCol[0] = "���";
	sCol[1] = "�����ݱ�";
	sCol[2] = "Ŀ���ֶ�";
	sCol[3] = "�����";
	sCol[4] = "״̬";
	m_MxRow = GetRowCount();
	m_MxCol = GetColumnCount();
	this->SetRowCount(m_MxRow);
	this->SetColumnCount(m_MxCol);
	
	for (int row = 0; row < this->GetRowCount(); row++)
		for (int col = 0; col < GetColumnCount(); col++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			if (row < 1) {
				Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				Item.strText.Format(_T("%s"), sCol[col].c_str());
				this->SetItem(&Item);
			}
			else if (col < 1) {
				Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				Item.strText.Format(_T("%d"), row);
				this->SetItem(&Item);
			}
			

		}

	

	//����ż��ż�е�ɫ
	SetRowColor(m_MxRow, m_MxCol, RGB(255, 250, 250));

	

	this->AutoSizeColumn(GVS_DEFAULT);//��һ���Զ����ڿ��

	int fixedcolwidth;
	fixedcolwidth = this->GetFixedColumnWidth() + 30;

	for (int i = 1; i < m_MxCol; i++)
		this->SetColumnWidth(i, (rectgrid.Width() - fixedcolwidth) / (m_MxCol - 1));

	//���Կؼ����
	/*
	CString s;
	s.Format("%d", rectgrid.Width());
	this->SetItemText(1, 2, s);
	s.Format("%d", rectgrid.Width() / m_MxCol);
	this->SetItemText(1, 3, s);
	*/


	Refresh();

	return true;
}

void CMyGridCtrl::SetRowColor(int nMxRow,int nMxCol, COLORREF clr)
{
	int nRow, nCol;
	
	for (nRow = 1; nRow < nMxRow; nRow++)
	{

			for (nCol = 1; nCol < nMxCol; nCol++)
			{
				if (!(nRow % 2))
				{
					if (nCol % 2)
						this->GetCell(nRow, nCol)->SetBackClr(clr - 10);
					else
						this->GetCell(nRow, nCol)->SetBackClr(clr);
				}
				else
				this->GetCell(nRow, nCol)->SetBackClr(RGB(255,255,255));
			}
		}

	
}

void CMyGridCtrl::SetCellCombo(int nRow, int nCol, CStringArray & items)
{
	//���õ�Ԫ������
	this->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCombo));
	CGridCellBase* pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL&&pCell->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		((CGridCellCombo*)pCell)->SetOptions(items);//����ѡ���б��ֵ
		((CGridCellCombo*)pCell)->SetStyle(/*CBS_DROPDOWNLIST| */CBS_DROPDOWN|CBS_SORT| CBS_AUTOHSCROLL);
		if (items.GetSize()>0)
		{
			((CGridCellCombo*)pCell)->SetText(items.GetAt(0));//���õ�Ԫ��ĵ�ǰֵ
		}
	}

}

void CMyGridCtrl::SetCellCheck(int nRow, int nCol, bool isCheck)
{
	this->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
	CGridCellBase* pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL&&pCell->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
	{
		//����checkBox��״̬
		((CGridCellCheck*)pCell)->SetCheck(isCheck);
	}

}


//�����У�ѡ�����У�����β���У���ѡ�����У�

int CMyGridCtrl::AddRowCount()
{
	//��ȡѡ��Ԫ��
	CCellRange Selection = this->GetSelectedCellRange();

	int nRow;

	//�ж���ѡ��
	if (IsValid(Selection))
	{
		if ((Selection.GetRowSpan() == 1) && (Selection.GetMaxCol() == m_MxCol - 1))
		{
			nRow=InsertRow("", Selection.GetMaxRow());
			SetSelectedRange(Selection);
		}
		else
			nRow = this->InsertRow("", -1);
	}
	else
	nRow = this->InsertRow("",Selection.GetMaxRow());


//��ʼ�����ֶ�

	SetCellCombo(nRow, 1, mainsheet);
	SetCellCombo(nRow, 2, keycol);
	SetCellCombo(nRow, 3, overkey);

	SetItemState(nRow, 4, GVIS_READONLY);

//ˢ�±��
	upcell();

	return 0;
}

//ɾ��ѡ����
int CMyGridCtrl::MudRowCount()
{
	//��ȡѡ��Ԫ��
	CCellRange Selection = this->GetSelectedCellRange();
	
	//�ж���ѡ��
	if (IsValid(Selection))
		if (Selection.GetMaxCol() == (m_MxCol - 1))
		{
			int row = Selection.GetMinRow();
			for (int nRow = 0; nRow < Selection.GetRowSpan(); nRow++)
			{
				this->DeleteRow(row);
			}
		}

	upcell();

	return 0;

}
