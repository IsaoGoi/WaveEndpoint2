// DlgEnumEdit.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgEnumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit ダイアログ


CDlgEnumEdit::CDlgEnumEdit(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgEnumEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnumEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDlgEnumEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnumEdit)
	DDX_Control(pDX, IDC_LST_ENUMSTR, m_list_EnumStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnumEdit, CDialogX)
	//{{AFX_MSG_MAP(CDlgEnumEdit)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit メッセージ ハンドラ
BOOL CDlgEnumEdit::DlgCreate(CWnd* pParent)
{
	SetBackColor(64,64,64);
	return CDialogX::Create(IDD, pParent);
}
int CDlgEnumEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogX::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 表示座標補正
	CRect rcWindow;
	CRect rcParent;
	CRect rcEdit;
	int	x,y;

	GetClientRect(rcWindow);

	GetParent()->GetWindowRect(rcParent);
	m_pIOEdit->GetWindowRect(rcEdit);

	if(rcParent.Height() < rcWindow.Height() || rcParent.Width() < (rcWindow.Width()*3))
		GetParent()->GetParent()->GetWindowRect(rcParent);

	if(rcParent.right >= (rcEdit.right + rcWindow.Width()))
		x = rcEdit.right;
	else{
		x = rcEdit.left - rcWindow.Width();
		if(x < 0)
			x = 0;
	}
	if(rcParent.bottom>= (rcEdit.bottom + rcWindow.Height()))
		y = rcEdit.bottom;
	else{
		y = rcEdit.top - rcWindow.Height();
		if(y < 0)
			y = 0;
	}

	/*
	** Recipe表示
	*/
	rcWindow.OffsetRect(x,y);
	MoveWindow(rcWindow);
	return 0;
}

void CDlgEnumEdit::SetValue(unsigned uint_val,LPSTR pEnumString[],CIOEdit *pEdit)
{
	for(int i=0;i<32;i++){
		if(pEnumString[i] == NULL){
			m_szEnumString[i][0] = '\0';
			break;
		}
// 2012/03/13		sprintf_s(m_szEnumString[i],sizeof(m_szEnumString[i]),"%d:%s",i,pEnumString[i]);
		sprintf_s(m_szEnumString[i],sizeof(m_szEnumString[i]),"%s",pEnumString[i]);
		m_szEnumString[i][64]='\0';
	}/* for */
	m_uint_Val = uint_val;
	m_pIOEdit = pEdit;
	return;
}
void CDlgEnumEdit::GetValue(unsigned *uint_val)
{
	*uint_val = m_uint_Val;
}

BOOL CDlgEnumEdit::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	for(int i=0;i<32;i++){
		if(m_szEnumString[i][0] == '\0')
			break;

		m_list_EnumStr.InsertString(i,m_szEnumString[i]);
		m_list_EnumStr.SetItemData(i,i);
	}/* for */
	m_list_EnumStr.SetCurSel(m_uint_Val);

	// 呼び出し元I/Oフィールドを選択状態に
	m_pIOEdit->SelectField();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
void CDlgEnumEdit::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_pIOEdit->UnselectField();
	m_pIOEdit = NULL;
	if(::IsWindow(m_hWnd) != 0){
		CDialogX::OnClose();
		DestroyWindow();
	}
}
void CDlgEnumEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	int item = m_list_EnumStr.GetCurSel();
	if (item == LB_ERR){
		return;
	}	
	m_uint_Val = item;
	m_pIOEdit->SetEnumValue(m_uint_Val);
	CDialogX::OnOK();
	SendMessage(WM_CLOSE);
}
void CDlgEnumEdit::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	CDialogX::OnCancel();
	SendMessage(WM_CLOSE);
}
