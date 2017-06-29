#pragma once
#include "GridCtrl.h"
#include "Resource.h"
//#include "eqcharrestoreDlg.h"

#pragma warning(disable:4996)
class CMyGridCtrl :
	public CGridCtrl
{
public:
	void SetRowColor(int nMxRow,int nMxCol, COLORREF clr);//����ĳһ�еı�����ɫ
	void SetColumnColor(int nRow, COLORREF  clr);//����ĳһ�еı�����ɫ
	void LoadImageList(int nImageId, int nSize);//����ͼƬ�б�
	void SetCellCombo(int nRow, int nCol, CStringArray& items);//����ĳ����Ԫ��ΪComboBox
	
	void SetCellCheck(int nRow, int nCol, bool isCheck);//����ĳ����Ԫ��ΪCheckBox
	bool upcell();
	
	CMyGridCtrl();
	~CMyGridCtrl();
	bool Create(CWnd* parent,/*CRect rectdlg,CRect rectgrid, */UINT nID,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	int AddRowCount();
	int MudRowCount();

protected:
	CRect rectdlg, rectgrid;
	CGridCellCombo* m_GridCombo;
	int m_MxRow, m_MxCol;
	CStringArray mainsheet, keycol, overkey;


};

