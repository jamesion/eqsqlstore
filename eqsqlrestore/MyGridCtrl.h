
#include "GridCtrl.h"
#include "Resource.h"
#include <vector>
#include <fstream>
#include "types.h"
#include "eqemu_logsys.h"

//#include "eqcharrestoreDlg.h"

#define LIST_MAINSHEET 1
#define LIST_KEYCOL	   2
#define LIST_OVERKEY   3

#pragma once
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

	//设置规则表只读或解锁只读
	void gridreadonly();



public:

	//设置规则表只读标记true为只读，false解锁只读
	void Setreadonly(bool read) { readonly = read; Refresh(); };

	//读取规则表只读标记
	bool Getgridreadonliy() {return readonly;};

	//设置单元格为下拉列表样式
	void SetCellCombo(int nRow, int nCol);

	//存储规则表为磁盘文件
	void RuleSave(hostinfo deshost, hostinfo schost, BOOL default=FALSE);

	//获取规则表一组规则,并返回下组规则主表行数,无规则返回0
	int ReadRuleGroup(int nRow, std::string* outrulemessage);
	int ReadRule(std::string* outrulemessage);

	//较验规则完整性
	bool CheckRule();

	//打开规则文件
	bool RuleOpen(hostinfo* deshost, hostinfo* schost, bool default = false);


private:
	std::string CellText;

	//将文件读入规则表
	void filetogrid(std::string filetext);

};