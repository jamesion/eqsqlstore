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

	parent->GetWindowRect(&rectdlg);//获取对话框窗口的位置信息


	pWnd->GetWindowRect(&rectgrid); //获取占位控件的位置信息

									//创建控件  
	//m_GridCtrl = new CGridCtrl();
	CGridCtrl::Create(CRect(rectgrid.left - rectdlg.left - cxframe,
		rectgrid.top - rectdlg.top - cyframe - captionHeight,
		rectgrid.left + rectgrid.Width() - rectdlg.left - cyframe,
		rectgrid.top + rectgrid.Height() - rectdlg.top - captionHeight - cxframe), parent, nID, dwStyle);

	//if (!m_GridCtrl)
		//return false;
	//设置第一行和第一列为控件固定行和列，即标题行和标题列
	this->SetFixedRowCount(1);
	this->SetFixedColumnCount(1);

	this->SetColumnCount(m_MxCol);
	this->SetRowCount(m_MxRow);
	return true;
}

bool CMyGridCtrl::upcell()
{


	//设置固定行标题
	string nCol;
	string sCol[5];
	sCol[0] = "序号";
	sCol[1] = "主数据表";
	sCol[2] = "目标字段";
	sCol[3] = "替代键";
	sCol[4] = "状态";
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

	

	//设置偶行偶列底色
	SetRowColor(m_MxRow, m_MxCol, RGB(255, 250, 250));

	

	this->AutoSizeColumn(GVS_DEFAULT);//第一列自动调节宽度

	int fixedcolwidth;
	fixedcolwidth = this->GetFixedColumnWidth() + 30;

	for (int i = 1; i < m_MxCol; i++)
		this->SetColumnWidth(i, (rectgrid.Width() - fixedcolwidth) / (m_MxCol - 1));

	//测试控件宽度
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
	//设置单元格类型
	this->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCombo));
	CGridCellBase* pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL&&pCell->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		((CGridCellCombo*)pCell)->SetOptions(items);//设置选择列表的值
		((CGridCellCombo*)pCell)->SetStyle(/*CBS_DROPDOWNLIST| */CBS_DROPDOWN|CBS_SORT| CBS_AUTOHSCROLL);
		if (items.GetSize()>0)
		{
			((CGridCellCombo*)pCell)->SetText(items.GetAt(0));//设置单元格的当前值
		}
	}

}

void CMyGridCtrl::SetCellCheck(int nRow, int nCol, bool isCheck)
{
	this->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck));
	CGridCellBase* pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL&&pCell->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
	{
		//设置checkBox的状态
		((CGridCellCheck*)pCell)->SetCheck(isCheck);
	}

}


//插入行（选中整行）及表尾增行（无选中整行）

int CMyGridCtrl::AddRowCount()
{
	//获取选中元素
	CCellRange Selection = this->GetSelectedCellRange();

	int nRow;

	//判断行选中
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


//初始化各字段

	SetCellCombo(nRow, 1, mainsheet);
	SetCellCombo(nRow, 2, keycol);
	SetCellCombo(nRow, 3, overkey);

	SetItemState(nRow, 4, GVIS_READONLY);

//刷新表格
	upcell();

	return 0;
}

//删除选中行
int CMyGridCtrl::MudRowCount()
{
	//获取选中元素
	CCellRange Selection = this->GetSelectedCellRange();
	
	//判断行选中
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
