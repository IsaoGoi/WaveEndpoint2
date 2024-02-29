// DlgNumEdit.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgNumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNumEdit ダイアログ


CDlgNumEdit::CDlgNumEdit(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgNumEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNumEdit)
	//}}AFX_DATA_INIT
}


void CDlgNumEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNumEdit)
//	DDX_Control(pDX, IDC_BTN_UP, m_btn_up);
//	DDX_Control(pDX, IDC_BTN_DOWN, m_btn_down);
	DDX_Control(pDX, IDC_ED_FLD, m_edit_fld);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNumEdit, CDialogX)
	//{{AFX_MSG_MAP(CDlgNumEdit)
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
	ON_BN_CLICKED(IDC_BTN_COLON, OnBtnColon)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MINUS, OnBtnMinus)
	ON_BN_CLICKED(IDC_BTN_MAX, OnBtnMax)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNumEdit メッセージ ハンドラ

BOOL CDlgNumEdit::DlgCreate(CWnd *pParent)
{
	SetBackColor(64,64,64);
	return CDialogX::Create(IDD, pParent);
}
int CDlgNumEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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


	if(rcParent.Height() < rcWindow.Height() || rcParent.Width() < (rcWindow.Width()*2))
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

#if 0
	CRect rc;
	rc.SetRect(100,100,120,120);
	m_btn_up.Create(NULL, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_OWNERDRAW ,rc, this,0);
	rc.SetRect(100,120,120,140);
	m_btn_down.Create(NULL, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_OWNERDRAW ,rc, this,0);
	m_btn_up.LoadBitmaps(IDB_UP_ACT,IDB_UP_ACT,IDB_UP_ACT,IDB_UP_ACT);
	m_btn_down.LoadBitmaps(IDB_DOWN_ACT,IDB_DOWN_ACT,IDB_DOWN_ACT,IDB_DOWN_ACT);
#endif
	/*
	** Recipe表示
	*/
	rcWindow.OffsetRect(x,y);
	MoveWindow(rcWindow);

	
	return 0;
}

void CDlgNumEdit::SetValue(LPCSTR pszVal, IO_TYPE iotype, IO_SCALE *pioscale,CIOEdit *pEdit)
{
	double	dVal;
	m_iotype = iotype;
	m_ioscale = *pioscale;
	m_szEdit = pszVal;
	m_pIOEdit = pEdit;
	

	if(m_ioscale.d_log_high < m_ioscale.d_log_low){
		dVal = m_ioscale.d_log_low;
		m_ioscale.d_log_low = m_ioscale.d_log_high; 
		m_ioscale.d_log_high = dVal; 
	}

}
void CDlgNumEdit::GetValue(LPSTR pszVal)
{
	strncpy_s(pszVal,(m_szEdit.GetLength()+1),(LPCSTR)m_szEdit,m_szEdit.GetLength());
	pszVal[m_szEdit.GetLength()] = '\0';
}

BOOL CDlgNumEdit::OnInitDialog() 
{

	char		szDecimalFormat[32 + 2];


	CDialogX::OnInitDialog();

	CIODB* pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	CRect	wRect;
	CRect	newRect;
	GetWindowRect(wRect);
	GetClientRect(newRect);


	m_btn_up.AutoLoad(IDC_BTN_UP,this);
	m_btn_down.AutoLoad(IDC_BTN_DOWN,this);

	// TODO: この位置に初期化の補足処理を追加してください
	m_edit_fld.LimitText(10);
	m_edit_fld.SetIOType(m_iotype);
	m_edit_fld.SetIOScale(&m_ioscale);
	m_edit_fld.SetWindowText(m_szEdit);
	m_edit_fld.SetFocus();
//	int	len = m_szEdit.GetLength();
	m_edit_fld.SetSel(0,-1);

	CString	szMaxLabel('\0',32);
	CString	szMinLabel('\0',32);
	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		szMaxLabel.Format("%d",(int)m_ioscale.d_log_high);
		szMinLabel.Format("%d",(int)m_ioscale.d_log_low);
		break;

	case IO_FLOAT:
		strcpy(szDecimalFormat, pIODB->IOPointGetDecimalFormat(m_pIOEdit->GetIOHdl()));
		szMaxLabel.Format(szDecimalFormat, m_ioscale.d_log_high);
		szMinLabel.Format(szDecimalFormat, m_ioscale.d_log_low);
#if 0
		if(m_ioscale.d_log_high < 10.0f)
			szMaxLabel.Format("%4.2f",m_ioscale.d_log_high);
		else
			szMaxLabel.Format("%7.2f",m_ioscale.d_log_high);

		if(m_ioscale.d_log_low < 10.0f)
			szMinLabel.Format("%4.2f",m_ioscale.d_log_low);
		else
			szMinLabel.Format("%7.2f",m_ioscale.d_log_low);
#endif
		break;
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		strcpy(szDecimalFormat, pIODB->IOPointGetDecimalFormat(m_pIOEdit->GetIOHdl()));
		szMaxLabel.Format(szDecimalFormat, m_ioscale.d_log_high);
		szMinLabel.Format(szDecimalFormat, m_ioscale.d_log_low);
		break;

	}/* switch */	
	GetDlgItem(IDC_BTN_MAX)->SetWindowText((LPCSTR)szMaxLabel);
	GetDlgItem(IDC_BTN_MIN)->SetWindowText((LPCSTR)szMinLabel);

	// 呼び出し元I/Oフィールドを選択状態に
	m_pIOEdit->SelectField();

	SetTimer(1,100,NULL);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CDlgNumEdit::OnBtn0() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'0',1);	
}
void CDlgNumEdit::OnBtn1() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'1',1);	
}
void CDlgNumEdit::OnBtn2() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'2',1);	
}
void CDlgNumEdit::OnBtn3() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'3',1);	
}
void CDlgNumEdit::OnBtn4() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'4',1);	
}
void CDlgNumEdit::OnBtn5() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'5',1);	
}
void CDlgNumEdit::OnBtn6() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'6',1);	
}
void CDlgNumEdit::OnBtn7() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'7',1);	
}
void CDlgNumEdit::OnBtn8() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'8',1);	
}
void CDlgNumEdit::OnBtn9() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'9',1);	
}

void CDlgNumEdit::OnBtnColon() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'.',1);	
}
void CDlgNumEdit::OnBtnMinus() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,'-',1);	
	
}
void CDlgNumEdit::OnBtnDel() 
{
	m_edit_fld.SetFocus();
	m_edit_fld.PostMessage(WM_CHAR,VK_BACK,1);	
}

void CDlgNumEdit::OnBtnMax() 
{
	CString szValue('\0',32);

	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		szValue.Format("%d",(int)m_ioscale.d_log_high);
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		if(m_ioscale.d_log_high < 1.0f)
			szValue.Format("%8.2f",m_ioscale.d_log_high);
		else
			szValue.Format("%7.2f",m_ioscale.d_log_high);
		break;
	}/* switch */	
	szValue.TrimLeft();
	m_edit_fld.SetFocus();
	m_edit_fld.SetWindowText((LPCSTR)szValue);
	m_edit_fld.SetSel(0,-1);
}

void CDlgNumEdit::OnBtnMin() 
{
	CString szValue('\0',32);

	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		szValue.Format("%d",(int)m_ioscale.d_log_low);
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		if(m_ioscale.d_log_low < 1.0f)
			szValue.Format("%8.2f",m_ioscale.d_log_low);
		else
			szValue.Format("%7.2f",m_ioscale.d_log_low);
		break;
	}/* switch */	
	szValue.TrimLeft();
	m_edit_fld.SetFocus();
	m_edit_fld.SetWindowText((LPCSTR)szValue);
	m_edit_fld.SetSel(0,-1);
}

void CDlgNumEdit::StartUpDown(int iUpDpwn)
{
	m_iValUpDown = iUpDpwn;
	m_iUpDownCycle = 0;
	if(m_iValUpDown == 1)
		SpinUp();
	else if(m_iValUpDown == 2)
		SpinDown();
}

void CDlgNumEdit::StopUpDown()
{
	m_iValUpDown = 0;
	m_edit_fld.SetFocus();
	m_edit_fld.SetSel(0,-1);
}

void CDlgNumEdit::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(m_iValUpDown == 1){
		if(m_iUpDownCycle != 0)
			SpinUp();
		m_iUpDownCycle++;
	}
	else if(m_iValUpDown == 2){
		if(m_iUpDownCycle != 0)
			SpinDown();
		m_iUpDownCycle++;
	}
	CDialogX::OnTimer(nIDEvent);
}
void CDlgNumEdit::SpinUp() 
{
	double	dval;
	int		ival;
	double	dCountUp;
	
	m_edit_fld.GetWindowText(m_szEdit);
	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		ival = atoi(LPCSTR(m_szEdit));
		if(ival >= (int)m_ioscale.d_log_high)
			return;
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		dval = atof(LPCSTR(m_szEdit));
		if(dval >= m_ioscale.d_log_high)
			return;
		break;
	}/* switch */	

	if(m_iUpDownCycle < 10)
		dCountUp = 1.0;
	else if(m_iUpDownCycle < 20)
		dCountUp = 2.0;
	else if(m_iUpDownCycle < 30)
		dCountUp = 11.0;
	else if(m_iUpDownCycle < 40)
		dCountUp = 25.0;
	else if(m_iUpDownCycle < 50)
		dCountUp = 51.0;
	else if(m_iUpDownCycle < 100)
		dCountUp = 111.0;
	else
		dCountUp = 1111.0;

	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		if((ival + (int)dCountUp) > (int)m_ioscale.d_log_high)
			ival = (int)m_ioscale.d_log_high;
		else
			ival += (int)dCountUp;
		m_szEdit.Format("%d",ival);
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		dCountUp /= 100;
		if((dval + dCountUp) > m_ioscale.d_log_high)
			dval = m_ioscale.d_log_high;
		else
			dval += dCountUp;
		if(dval < 1.0f)
			m_szEdit.Format("%8.2f",dval);
		else
			m_szEdit.Format("%7.2f",dval);
		break;
	}/* switch */	
	m_szEdit.TrimLeft();
	m_edit_fld.SetWindowText((LPCSTR)m_szEdit);
}
void CDlgNumEdit::SpinDown() 
{
	double	dval;
	int		ival;
	double	dCountDown;
	
	m_edit_fld.GetWindowText(m_szEdit);
	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		ival = atoi(LPCSTR(m_szEdit));
		if(ival <= (int)m_ioscale.d_log_low)
			return;
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		dval = atof(LPCSTR(m_szEdit));
		if(dval <= m_ioscale.d_log_low)
			return;
		break;
	}/* switch */	

	if(m_iUpDownCycle < 10)
		dCountDown = 1.0;
	else if(m_iUpDownCycle < 20)
		dCountDown = 2.0;
	else if(m_iUpDownCycle < 30)
		dCountDown = 11.0;
	else if(m_iUpDownCycle < 40)
		dCountDown = 25.0;
	else if(m_iUpDownCycle < 50)
		dCountDown = 51.0;
	else if(m_iUpDownCycle < 100)
		dCountDown = 111.0;
	else
		dCountDown = 1111.0;

	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
		if((ival - (int)dCountDown) < (int)m_ioscale.d_log_low)
			ival = (int)m_ioscale.d_log_low;
		else
			ival -= (int)dCountDown;
		m_szEdit.Format("%d",ival);
		break;

	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		dCountDown /= 100;
		if((dval - dCountDown) < m_ioscale.d_log_low)
			dval = m_ioscale.d_log_low;
		else
			dval -= dCountDown;
		if(dval < 1.0f)
			m_szEdit.Format("%8.2f",dval);
		else
			m_szEdit.Format("%7.2f",dval);
		break;
	}/* switch */	
	m_szEdit.TrimLeft();
	m_edit_fld.SetWindowText((LPCSTR)m_szEdit);
}

void CDlgNumEdit::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	// 呼び出し元I/Oフィールドを選択状態に
	m_pIOEdit->UnselectField();
	m_pIOEdit = NULL;
	if(::IsWindow(m_hWnd) != 0){
		CDialogX::OnClose();
		DestroyWindow();
	}
}

void CDlgNumEdit::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	CDialogX::OnCancel();
	SendMessage(WM_CLOSE);
	
}
void CDlgNumEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	double	dval;
	
	m_edit_fld.GetWindowText(m_szEdit);
	dval = atof(LPCSTR(m_szEdit));

	if(dval < m_ioscale.d_log_low || dval >m_ioscale.d_log_high){
		MessageBox("Out of Range!");
		return;
	}
	m_pIOEdit->SetValue((LPCSTR)m_szEdit);
	CDialogX::OnOK();
	SendMessage(WM_CLOSE);
}

