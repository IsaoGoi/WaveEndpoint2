// CDlgRecipeSelect.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgRecipeSelect.h"


// CDlgRecipeSelect ダイアログ

IMPLEMENT_DYNAMIC(CDlgRecipeSelect, CDialogX)

static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };
/*
** #define
*/
#define NUM_COLUMNS				4								/* Lot List表示項目数	*/

#define	SUB_ITEM_NO				0
#define	SUB_ITEM_NAME			1
#define	SUB_ITEM_COMMENT		2
#define	SUB_ITEM_DATE			3

/*
** ﾃﾞｰﾀ定義
*/

/* Lot一覧表示ﾀｲﾄﾙ */
static _TCHAR* _gszColumnLabel[NUM_COLUMNS] =
{
	_T("Rcp No."),
	_T("Recipe Name"),
	_T("Comment"),
	_T("Update"),
};

/* ﾀｲﾄﾙ表示ﾌｫｰﾏｯﾄ */
static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
	LVCFMT_CENTER,
};

/* 表示ﾚﾝｸﾞｽﾃﾞﾌｫﾙﾄ値 */
static int _gnColumnWidth[NUM_COLUMNS] =
{
	80,300, 300, 160
};



CDlgRecipeSelect::CDlgRecipeSelect(CWnd* pParent,int iPM)
	: CDialogX(IDD_RCPSELECT, pParent)
{
	m_iPMNo = iPM;
}

CDlgRecipeSelect::~CDlgRecipeSelect()
{
}

void CDlgRecipeSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RCP, m_list_Recipe);
}


BEGIN_MESSAGE_MAP(CDlgRecipeSelect, CDialogX)
END_MESSAGE_MAP()


// CDlgRecipeSelect メッセージ ハンドラー


BOOL CDlgRecipeSelect::OnInitDialog()
{
	int			i;
	int			iCnt;
	LV_COLUMN	lvc;
	LV_ITEM		lvi;
	CString		szStr('\0', 64);


	CDialogX::OnInitDialog();

	szStr.Format("PC%d Recipe Select", (m_iPMNo + 1));
	SetWindowText(szStr);


	m_bmpPC.LoadBitmapA(m_uidPCBmp[m_iPMNo]);
	((CStatic*)GetDlgItem(IDC_BMP_PC))->SetBitmap(m_bmpPC);

	m_pRcpCtrl = ((CWaveEndPointApp*)AfxGetApp())->GetPCRcpCtrl();

	/*
	** Recipeリスト初期化
	*/
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_list_Recipe.InsertColumn(i, &lvc);
	}/* for */
	m_list_Recipe.ModifyStyle(LVS_TYPEMASK, ((LVS_REPORT & LVS_TYPEMASK)
		| LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_EDITLABELS));

	m_list_Recipe.SetModeGridLine(TRUE, TRUE);


	for (iCnt = i = 0; i < RCP_FILE_MAX; i++) {
		if (m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iEnable == FALSE)
			continue;

		lvi.mask = LVIF_TEXT;
		lvi.iItem = iCnt;
		lvi.iSubItem = SUB_ITEM_NO;

		szStr.Format("%03.03d", i + 1);
		lvi.pszText = (LPSTR)((LPCSTR)szStr);
		m_list_Recipe.InsertItem(&lvi);

		m_list_Recipe.SetItemData(iCnt, (i + (int)1));

		szStr = m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].szRecipeName;
		m_list_Recipe.SetItemText(iCnt, SUB_ITEM_NAME, (LPCSTR)szStr);

		szStr = m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].szComment;
		m_list_Recipe.SetItemText(iCnt, SUB_ITEM_COMMENT, (LPCSTR)szStr);

		szStr.Format("%04.04d/%02.02d/%02.02d %02.02d:%02.02d:%02.02d",
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateYear,
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateMonth,
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateDay,
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateHour,
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateMinute,
			m_pRcpCtrl->m_TblPCRecipe[m_iPMNo][i].iUpdateSecond);
		m_list_Recipe.SetItemText(iCnt, SUB_ITEM_DATE, (LPCSTR)szStr);

		iCnt++;
	}/* for */
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgRecipeSelect::OnOK()
{
	int	item;

	item = m_list_Recipe.GetSelectedItem();
	if (item == -1)
		return;

	m_iSelectRcpNo = m_list_Recipe.GetItemData(item);
	CDialogX::OnOK();
}
