// DlgStringEdit.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgStringEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStringEdit ダイアログ


CDlgStringEdit::CDlgStringEdit(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgStringEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStringEdit)
	m_szFld = _T("");
	//}}AFX_DATA_INIT
	m_bPassword = FALSE;
}


void CDlgStringEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStringEdit)
	DDX_Control(pDX, IDC_ED_FLD, m_edFld);
	DDX_Text(pDX, IDC_ED_FLD, m_szFld);
	DDV_MaxChars(pDX, m_szFld, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStringEdit, CDialogX)
	//{{AFX_MSG_MAP(CDlgStringEdit)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_0, OnBtn0)
	ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
	ON_BN_CLICKED(IDC_BTN_2, OnBtn2)
	ON_BN_CLICKED(IDC_BTN_3, OnBtn3)
	ON_BN_CLICKED(IDC_BTN_4, OnBtn4)
	ON_BN_CLICKED(IDC_BTN_5, OnBtn5)
	ON_BN_CLICKED(IDC_BTN_6, OnBtn6)
	ON_BN_CLICKED(IDC_BTN_7, OnBtn7)
	ON_BN_CLICKED(IDC_BTN_8, OnBtn8)
	ON_BN_CLICKED(IDC_BTN_9, OnBtn9)
	ON_BN_CLICKED(IDC_BTN_A, OnBtnA)
	ON_BN_CLICKED(IDC_BTN_B, OnBtnB)
	ON_BN_CLICKED(IDC_BTN_C, OnBtnC)
	ON_BN_CLICKED(IDC_BTN_D, OnBtnD)
	ON_BN_CLICKED(IDC_BTN_E, OnBtnE)
	ON_BN_CLICKED(IDC_BTN_F, OnBtnF)
	ON_BN_CLICKED(IDC_BTN_G, OnBtnG)
	ON_BN_CLICKED(IDC_BTN_H, OnBtnH)
	ON_BN_CLICKED(IDC_BTN_I, OnBtnI)
	ON_BN_CLICKED(IDC_BTN_J, OnBtnJ)
	ON_BN_CLICKED(IDC_BTN_K, OnBtnK)
	ON_BN_CLICKED(IDC_BTN_L, OnBtnL)
	ON_BN_CLICKED(IDC_BTN_M, OnBtnM)
	ON_BN_CLICKED(IDC_BTN_N, OnBtnN)
	ON_BN_CLICKED(IDC_BTN_O, OnBtnO)
	ON_BN_CLICKED(IDC_BTN_P, OnBtnP)
	ON_BN_CLICKED(IDC_BTN_Q, OnBtnQ)
	ON_BN_CLICKED(IDC_BTN_R, OnBtnR)
	ON_BN_CLICKED(IDC_BTN_S, OnBtnS)
	ON_BN_CLICKED(IDC_BTN_T, OnBtnT)
	ON_BN_CLICKED(IDC_BTN_U, OnBtnU)
	ON_BN_CLICKED(IDC_BTN_V, OnBtnV)
	ON_BN_CLICKED(IDC_BTN_W, OnBtnW)
	ON_BN_CLICKED(IDC_BTN_X, OnBtnX)
	ON_BN_CLICKED(IDC_BTN_Y, OnBtnY)
	ON_BN_CLICKED(IDC_BTN_Z, OnBtnZ)
	ON_BN_CLICKED(IDC_BTN_ATMARK, OnBtnAtmark)
	ON_BN_CLICKED(IDC_BTN_BKSP, OnBtnBksp)
	ON_BN_CLICKED(IDC_BTN_CAPS, OnBtnCaps)
	ON_BN_CLICKED(IDC_BTN_DEL2, OnBtnDel2)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)
	ON_BN_CLICKED(IDC_BTN_PLUS, OnBtnPlus)
	ON_BN_CLICKED(IDC_BTN_SPACE, OnBtnSpace)
	ON_BN_CLICKED(IDC_BTN_CANMA, OnBtnCanma)
	ON_BN_CLICKED(IDC_BTN_COLON2, OnBtnColon)
	ON_BN_CLICKED(IDC_BTN_DOT, OnBtnDot)
	ON_BN_CLICKED(IDC_BTN_QUESTION, OnBtnQuestion)
	ON_BN_CLICKED(IDC_BTN_SEMICOLON, OnBtnSemicolon)
	ON_EN_CHANGE(IDC_ED_FLD, OnChangeEdFld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStringEdit メッセージ ハンドラ
BOOL CDlgStringEdit::DlgCreate(CWnd *pParent)
{
	SetBackColor(64,64,64);
	return CDialogX::Create(IDD, pParent);
}
int CDlgStringEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogX::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	return 0;
}
void CDlgStringEdit::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	// 呼び出し元I/Oフィールドを選択状態に
	m_pIOEdit->UnselectField();
	m_pIOEdit = NULL;
	if(::IsWindow(m_hWnd) != 0){
		CDialogX::OnClose();
		DestroyWindow();
	}
	m_bPassword = FALSE;
}
void CDlgStringEdit::SetValue(LPCSTR pszVal,int iMaxSize,CIOEdit *pEdit)
{
	m_szEdit = pszVal;
	m_pIOEdit = pEdit;
	m_iMaxSize = iMaxSize;
}
void CDlgStringEdit::GetValue(LPSTR pszVal)
{
	strncpy_s(pszVal,(m_iMaxSize+1),(LPCSTR)m_szEdit,m_iMaxSize);
	pszVal[m_iMaxSize] = '\0';
}

BOOL CDlgStringEdit::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_edFld.SetLimitText(m_iMaxSize);
	if(m_bPassword)
		m_edFld.SetPasswordChar('*');

	//	int	len = m_szEdit.GetLength();
	m_pIOEdit->SelectField();

	m_bCapsLock = TRUE;

	m_szFld = m_szEdit;
	UpdateData(FALSE);
	m_edFld.SetFocus();
	m_edFld.SetSel(0,0);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



void CDlgStringEdit::OnBtn0() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'0',1);	
}

void CDlgStringEdit::OnBtn1() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'1',1);	
}

void CDlgStringEdit::OnBtn2() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'2',1);	
}
void CDlgStringEdit::OnBtn3() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'3',1);	
}
void CDlgStringEdit::OnBtn4() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'4',1);	
}
void CDlgStringEdit::OnBtn5() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'5',1);	
}
void CDlgStringEdit::OnBtn6() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'6',1);	
}
void CDlgStringEdit::OnBtn7() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'7',1);	
}
void CDlgStringEdit::OnBtn8() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'8',1);	
}
void CDlgStringEdit::OnBtn9() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'9',1);	
}

void CDlgStringEdit::OnBtnA() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'A',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'a',1); 
}
void CDlgStringEdit::OnBtnB() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'B',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'b',1); 
}
void CDlgStringEdit::OnBtnC() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'C',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'c',1); 
}
void CDlgStringEdit::OnBtnD() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'D',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'d',1); 
}
void CDlgStringEdit::OnBtnE() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'E',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'e',1); 
}
void CDlgStringEdit::OnBtnF() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'F',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'f',1); 
}
void CDlgStringEdit::OnBtnG() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'G',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'g',1); 
}
void CDlgStringEdit::OnBtnH() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'H',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'h',1); 
}
void CDlgStringEdit::OnBtnI() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'I',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'i',1); 
}
void CDlgStringEdit::OnBtnJ() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'J',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'j',1); 
}
void CDlgStringEdit::OnBtnK() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'K',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'k',1); 
}
void CDlgStringEdit::OnBtnL() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'L',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'l',1); 
}
void CDlgStringEdit::OnBtnM() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'M',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'m',1); 
}
void CDlgStringEdit::OnBtnN() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'N',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'n',1); 
}
void CDlgStringEdit::OnBtnO() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'O',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'o',1); 
}
void CDlgStringEdit::OnBtnP() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'P',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'p',1); 
}
void CDlgStringEdit::OnBtnQ() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'Q',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'q',1); 
}
void CDlgStringEdit::OnBtnR() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'R',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'r',1); 
}
void CDlgStringEdit::OnBtnS() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'S',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'s',1); 
}
void CDlgStringEdit::OnBtnT() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'T',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'t',1); 
}
void CDlgStringEdit::OnBtnU() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'U',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'u',1); 
}
void CDlgStringEdit::OnBtnV() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'V',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'v',1); 
}
void CDlgStringEdit::OnBtnW() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'W',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'w',1); 
}
void CDlgStringEdit::OnBtnX() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'X',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'x',1); 
}
void CDlgStringEdit::OnBtnY() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'Y',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'y',1); 
}
void CDlgStringEdit::OnBtnZ() 
{
	m_edFld.SetFocus();
	if(m_bCapsLock==TRUE) 
		m_edFld.PostMessage(WM_CHAR,'Z',1); 
	else 
		m_edFld.PostMessage(WM_CHAR,'z',1); 
}

void CDlgStringEdit::OnBtnAtmark() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'@',1); 
}

void CDlgStringEdit::OnBtnBksp() 
{
	char szEdit[128+1];
	int	 iStart,iEnd;
	
	memset(szEdit,'\0',128);
	m_edFld.GetWindowText(szEdit,128);
	szEdit[128] = '\0';
	m_edFld.GetSel(iStart,iEnd);
	if(iStart == 0 && iEnd == 0){
		m_edFld.SetFocus();
		return;
	}
	if(iStart == 0 && iEnd == -1)
		iEnd = m_iMaxSize;

	if(iStart == iEnd){
		strncpy_s(&szEdit[iStart-1],sizeof(szEdit)-(iStart+1),&szEdit[iStart],m_iMaxSize);
		iStart--;
	}
	else{
		strncpy_s(&szEdit[iStart],sizeof(szEdit)-(iStart+1),&szEdit[iEnd],m_iMaxSize);

	}
	szEdit[m_iMaxSize] = '\0';
	m_szFld =  szEdit;
	UpdateData(FALSE);
	m_edFld.SetFocus();
	m_edFld.SetSel(iStart,iStart);

}

void CDlgStringEdit::OnBtnCaps() 
{
	if(m_bCapsLock == TRUE){
		GetDlgItem(IDC_BTN_A)->SetWindowText(_T("a"));
		GetDlgItem(IDC_BTN_B)->SetWindowText(_T("b"));
		GetDlgItem(IDC_BTN_C)->SetWindowText(_T("c"));
		GetDlgItem(IDC_BTN_D)->SetWindowText(_T("d"));
		GetDlgItem(IDC_BTN_E)->SetWindowText(_T("e"));
		GetDlgItem(IDC_BTN_F)->SetWindowText(_T("f"));
		GetDlgItem(IDC_BTN_G)->SetWindowText(_T("g"));
		GetDlgItem(IDC_BTN_H)->SetWindowText(_T("h"));
		GetDlgItem(IDC_BTN_I)->SetWindowText(_T("i"));
		GetDlgItem(IDC_BTN_J)->SetWindowText(_T("j"));
		GetDlgItem(IDC_BTN_K)->SetWindowText(_T("k"));
		GetDlgItem(IDC_BTN_L)->SetWindowText(_T("l"));
		GetDlgItem(IDC_BTN_M)->SetWindowText(_T("m"));
		GetDlgItem(IDC_BTN_N)->SetWindowText(_T("n"));
		GetDlgItem(IDC_BTN_O)->SetWindowText(_T("o"));
		GetDlgItem(IDC_BTN_P)->SetWindowText(_T("p"));
		GetDlgItem(IDC_BTN_Q)->SetWindowText(_T("q"));
		GetDlgItem(IDC_BTN_R)->SetWindowText(_T("r"));
		GetDlgItem(IDC_BTN_S)->SetWindowText(_T("s"));
		GetDlgItem(IDC_BTN_T)->SetWindowText(_T("t"));
		GetDlgItem(IDC_BTN_U)->SetWindowText(_T("u"));
		GetDlgItem(IDC_BTN_V)->SetWindowText(_T("v"));
		GetDlgItem(IDC_BTN_W)->SetWindowText(_T("w"));
		GetDlgItem(IDC_BTN_X)->SetWindowText(_T("x"));
		GetDlgItem(IDC_BTN_Y)->SetWindowText(_T("y"));
		GetDlgItem(IDC_BTN_Z)->SetWindowText(_T("z"));
		m_bCapsLock = FALSE;
	}
	else{
		GetDlgItem(IDC_BTN_A)->SetWindowText(_T("A"));
		GetDlgItem(IDC_BTN_B)->SetWindowText(_T("B"));
		GetDlgItem(IDC_BTN_C)->SetWindowText(_T("C"));
		GetDlgItem(IDC_BTN_D)->SetWindowText(_T("D"));
		GetDlgItem(IDC_BTN_E)->SetWindowText(_T("E"));
		GetDlgItem(IDC_BTN_F)->SetWindowText(_T("F"));
		GetDlgItem(IDC_BTN_G)->SetWindowText(_T("G"));
		GetDlgItem(IDC_BTN_H)->SetWindowText(_T("H"));
		GetDlgItem(IDC_BTN_I)->SetWindowText(_T("I"));
		GetDlgItem(IDC_BTN_J)->SetWindowText(_T("J"));
		GetDlgItem(IDC_BTN_K)->SetWindowText(_T("K"));
		GetDlgItem(IDC_BTN_L)->SetWindowText(_T("L"));
		GetDlgItem(IDC_BTN_M)->SetWindowText(_T("M"));
		GetDlgItem(IDC_BTN_N)->SetWindowText(_T("N"));
		GetDlgItem(IDC_BTN_O)->SetWindowText(_T("O"));
		GetDlgItem(IDC_BTN_P)->SetWindowText(_T("P"));
		GetDlgItem(IDC_BTN_Q)->SetWindowText(_T("Q"));
		GetDlgItem(IDC_BTN_R)->SetWindowText(_T("R"));
		GetDlgItem(IDC_BTN_S)->SetWindowText(_T("S"));
		GetDlgItem(IDC_BTN_T)->SetWindowText(_T("T"));
		GetDlgItem(IDC_BTN_U)->SetWindowText(_T("U"));
		GetDlgItem(IDC_BTN_V)->SetWindowText(_T("V"));
		GetDlgItem(IDC_BTN_W)->SetWindowText(_T("W"));
		GetDlgItem(IDC_BTN_X)->SetWindowText(_T("X"));
		GetDlgItem(IDC_BTN_Y)->SetWindowText(_T("Y"));
		GetDlgItem(IDC_BTN_Z)->SetWindowText(_T("Z"));
		m_bCapsLock = TRUE;
	}
	m_edFld.SetFocus();

}

void CDlgStringEdit::OnBtnDel2() 
{
	char szEdit[128+1];
	int	 iStart,iEnd;
	
	memset(szEdit,'\0',128);
	m_edFld.GetWindowText(szEdit,128);
	szEdit[128] = '\0';
	m_edFld.GetSel(iStart,iEnd);
	if(iStart == m_iMaxSize){
		m_edFld.SetFocus();
		return;
	}
	if(iStart == 0 && iEnd == -1)
		iEnd = m_iMaxSize;

	if(iStart == iEnd)
		strncpy_s(&szEdit[iStart],sizeof(szEdit)-(iStart+1),&szEdit[iStart+1],m_iMaxSize);
	else{
		strncpy_s(&szEdit[iStart],sizeof(szEdit)-(iStart+1),&szEdit[iEnd],m_iMaxSize);

	}
	szEdit[m_iMaxSize] = '\0';
	m_szFld =  szEdit;
	UpdateData(FALSE);
	m_edFld.SetFocus();
	m_edFld.SetSel(iStart,iStart);
}

void CDlgStringEdit::OnBtnMin() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'-',1); 
}

void CDlgStringEdit::OnBtnPlus() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'+',1); 
}

void CDlgStringEdit::OnBtnSpace() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,' ',1); 
}

void CDlgStringEdit::OnBtnCanma() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,',',1); 
}

void CDlgStringEdit::OnBtnColon() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,':',1); 
}

void CDlgStringEdit::OnBtnDot() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'.',1); 
}

void CDlgStringEdit::OnBtnQuestion() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,'?',1); 
}

void CDlgStringEdit::OnBtnSemicolon() 
{
	m_edFld.SetFocus();
	m_edFld.PostMessage(WM_CHAR,';',1); 
}

void CDlgStringEdit::OnChangeEdFld() 
{
	char szEdit[128+1];
	int	 iStart,iEnd;
	
	m_edFld.GetWindowText(szEdit,128);
	szEdit[128] = '\0';
	if(strlen(szEdit) >= (unsigned)m_iMaxSize){
		m_edFld.GetSel(iStart,iEnd);

		szEdit[m_iMaxSize] = '\0';
		m_szFld =  szEdit;
		UpdateData(FALSE);
		if(iStart>=(m_iMaxSize))
			m_edFld.SetSel(m_iMaxSize,m_iMaxSize);
		else{
//			iStart++;
			m_edFld.SetSel(iStart,iStart);
		}
	}
	else if(m_szFld.Compare((LPCSTR)szEdit)){
		m_szFld =  szEdit;
		UpdateData(FALSE);
	}
}

void CDlgStringEdit::OnOK() 
{
	m_pIOEdit->SetValue((LPCSTR)m_szFld);
	CDialogX::OnOK();
	SendMessage(WM_CLOSE);
}

void CDlgStringEdit::OnCancel() 
{
	CDialogX::OnCancel();
	SendMessage(WM_CLOSE);
}
void CDlgStringEdit::SetPasswordFld() 
{
	m_bPassword=TRUE;
}
