#pragma once
#include "GridCtrl.h"
#include "Resource.h"
#include <vector>
//#include "eqcharrestoreDlg.h"

#define LIST_MAINSHEET 0
#define LIST_KEYCOL	   1
#define LIST_OVERKEY   2


#pragma warning(disable:4996)
class CMyGridCtrl :
	public CGridCtrl
{
public:

	CMyGridCtrl();
	~CMyGridCtrl();


public:
	void SetRowColor(int nMxRow, int nMxCol, COLORREF clr);//设置某一列的背景颜色
	void SetColumnColor(int nRow, COLORREF  clr);//设置某一行的背景颜色
	void LoadImageList(int nImageId, int nSize);//加载图片列表
	void SetCellCombo(int nRow, int nCol, CStringArray& items);//设置某个单元格为ComboBox

	void SetCellCheck(int nRow, int nCol, bool isCheck);//设置某个单元格为CheckBox
	bool upcell();

	bool SetComboList(int nListID, std::vector < std::vector < std::string > > list, BOOL add = TRUE);


	bool Create(CWnd* parent,/*CRect rectdlg,CRect rectgrid, */UINT nID,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	int AddRowCount();
	int MudRowCount();

protected:
	CRect rectdlg, rectgrid;
	CGridCellCombo* m_GridCombo;
	int m_MxRow, m_MxCol;
	CStringArray mainsheet, keycol, overkey;
	bool readonly = true;
	void gridreadonly();



public:

	void Setreadonly(bool read) { readonly = read; upcell(); };

	bool Getgridreadonliy() {return readonly;};
};