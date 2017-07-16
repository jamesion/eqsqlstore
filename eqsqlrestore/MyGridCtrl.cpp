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
	sCol[2] = "需替换字段";
	sCol[3] = "替代键";
	sCol[4] = "状态";
	m_MxRow = GetRowCount();
	m_MxCol = GetColumnCount();
	this->SetRowCount(m_MxRow);
	this->SetColumnCount(m_MxCol);
	this->gridreadonly();
	for (int row = 0; row < this->GetRowCount(); row++)
	{
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
		if (row > 0)
		{
			/*SetCellCombo(row, 1, mainsheet);
			SetCellCombo(row, 2, keycol);
			SetCellCombo(row, 3, overkey);*/
			SetItemState(row, 4, GVIS_READONLY);
		}

	}



	//设置偶行偶列底色
	SetRowColor(m_MxRow, m_MxCol, RGB(255, 250, 250));

	

	this->AutoSizeColumn(GVS_DEFAULT);//第一列自动调节宽度

	int fixedcolwidth;
	fixedcolwidth = this->GetFixedColumnWidth() + 30;

	for (int i = 1; i < m_MxCol; i++)
		this->SetColumnWidth(i, (rectgrid.Width() - fixedcolwidth) / (m_MxCol - 1));



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

		/*if (items.GetSize()>0)
		{
			((CGridCellCombo*)pCell)->SetText(items.GetAt(0));//设置单元格的当前值
		}*/
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
	if (readonly)
		return FALSE;
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

	//SetItemState(nRow, 4, GVIS_READONLY);

//刷新表格
	upcell();

	return TRUE;
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


bool CMyGridCtrl::SetComboList(int nListID, std::vector < std::vector < std::string > > list,BOOL add)
{
	/*CStringArray backlist;

	for (int i = 0; i <= ; i++)
	{
		backlist.Add(list[i]);
	}*/



	switch (nListID)
	{

	case LIST_MAINSHEET:
		if (!add)
		{
			mainsheet.RemoveAll();
			mainsheet.Add(NULL);

		}

		for (int i = 0; i < list.size(); ++i)
			mainsheet.Add(list[i][0].c_str());

		return TRUE;
		break;

	case LIST_KEYCOL:
		if (!add)
		{
			keycol.RemoveAll();
			keycol.Add(NULL);

		}

		for (int i = 0; i < list.size(); ++i)
			keycol.Add(list[i][0].c_str());

		return TRUE;
		break;
	case LIST_OVERKEY:
		if (!add)
		{
			overkey.RemoveAll();
			overkey.Add(NULL);

		}

		for (int i = 0; i < list.size(); ++i)
			overkey.Add(list[i][0].c_str());

		return TRUE;
		break;
	default:
		break;
	}
	
	return FALSE;
}


//设置规则表只读或解锁只读
void CMyGridCtrl::gridreadonly()
{

//		readonly = readoff;
		if (readonly)
		{
			for (int r = 1; r < m_MxRow; r++)
			{
				for (int c = 1; c < m_MxCol; c++)
				{
					this->SetItemState(r, c, GVIS_READONLY);
				}
			}
		}
		
		else
		{

			for (int r = 1; r < m_MxRow; r++)
			{
				for (int c = 1; c < m_MxCol; c++)
				{
					this->SetItemState(r, c, !GVIS_READONLY);
				}
			}
		}
		
}


//设置单元格为下拉列表样式
void CMyGridCtrl::SetCellCombo(int nRow, int uInt)
{
	switch (uInt)
	{	
	case LIST_MAINSHEET:
		SetCellCombo(nRow, LIST_MAINSHEET, mainsheet);
		break;
	case LIST_KEYCOL:
		SetCellCombo(nRow, LIST_KEYCOL, keycol);
		break;
	case LIST_OVERKEY:
		SetCellCombo(nRow, LIST_OVERKEY, overkey);
		break;

	default:
		break;
	}
}


//存储规则表为磁盘文件
void CMyGridCtrl::RuleSave(hostinfo deshost, hostinfo schost)
{
	if (!CheckRule())
	{
		Log(Logs::Error, Logs::Setting, "规则不完整，请维护规则！");
		return;
	}


	int nrow=1;


	rulefs.open("Default.rul", ios::out|ios::binary);

	CellText.clear();

//	if(GetItemText(nrow, LIST_MAINSHEET) == "")
	while (true)
	{		
		if (nrow <= 0)
			return;

		if (GetItemText(nrow, LIST_MAINSHEET) != "")
			break;
		nrow++;
	}
	while (true)
	{


		nrow=ReadRuleGroup(nrow, &CellText);

		if (nrow <= 0)
			break;
	}

	Log(Logs::General, Logs::Files, "celltext:%s", CellText.c_str());

	char* buffs;
	buffs = new(char[CellText.size()+sizeof(hostinfo)*2+sizeof(int)]);

	memcpy(buffs, &deshost, sizeof(hostinfo));
	memcpy(buffs + sizeof(hostinfo), &schost, sizeof(hostinfo));
	memcpy(buffs + sizeof(hostinfo)*2, &m_MxRow, sizeof(int));
	memcpy(buffs + sizeof(hostinfo)*2+sizeof(int), CellText.c_str(), CellText.size());

	rulefs.write(buffs, CellText.size() + sizeof(hostinfo) * 2 + sizeof(int));
	rulefs.close();

	delete buffs;
	return;
}

//获取规则表一组规则,并返回下组规则主表行数,无规则返回0
int CMyGridCtrl::ReadRuleGroup(int nRow, std::string* outrulemessage)
{
	if (nRow <= 0 || nRow > m_MxRow)
	{

		return 0;
	}

	if (outrulemessage->append(GetItemText(nRow, LIST_MAINSHEET)) != "")
	{
		outrulemessage->append(",");
		if (outrulemessage->append(GetItemText(nRow, LIST_KEYCOL)) != "")
		{
			outrulemessage->append(",");
			if (outrulemessage->append(GetItemText(nRow, LIST_OVERKEY)) == "")
			{
				return 0;
			}
			else
			{
				outrulemessage->append(",");
			}
		}
		else
		{
			return 0;
		}

//		
		nRow++;

	}
	else
	{
		return 0;
	}

	while (true)
	{
		if (nRow > m_MxRow - 1)
		{
			outrulemessage->pop_back();
			return 0;
		}
		if (GetItemText(nRow, LIST_MAINSHEET) != "")
			break;

		if (GetItemText(nRow, LIST_KEYCOL) != "")
		{
			outrulemessage->append(GetItemText(nRow, LIST_KEYCOL));
			outrulemessage->append(",");
			if (GetItemText(nRow, LIST_OVERKEY) == "")
			{
				return 0;
			}
			else
			{
				outrulemessage->append(GetItemText(nRow, LIST_OVERKEY));
				outrulemessage->append(",");
			}
			nRow++;
		}
		else
		{
				if (nRow >= m_MxRow)
				{
					outrulemessage->pop_back();
					return 0;
				}
				//if (GetItemText(nRow, LIST_MAINSHEET) != "")
				//	break;
				nRow++;
			
		}

	}
	outrulemessage->pop_back();
	outrulemessage->append("|");
	return nRow;
}

//较验规则完整性
bool CMyGridCtrl::CheckRule()
{
	bool pass = true;
	//COLORREF Color = RGB(255, 0, 0);
	for (int irow = 1; irow <= m_MxRow; irow++)
	{
		if (GetItemText(irow, LIST_KEYCOL) != "")
		{
			if (GetItemText(irow, LIST_OVERKEY) == "")
			{
				GetCell(irow, LIST_OVERKEY)->SetBackClr(RGB(255, 0, 0));
				pass = false;

			}
			else
			{
				GetCell(irow, LIST_OVERKEY)->SetBackClr(GetCell(irow, LIST_MAINSHEET)->GetBackClr());
			}
		}
	}
	this->Refresh();
	return pass;
}







