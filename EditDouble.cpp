/////////////////////////////////////////////////////////////////////////////
// CEditDouble.cpp : インプリメンテーション ファイル
//
#include "pch.h"
#include "WaveEndPoint.h"
#include "EditDouble.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditDouble
CEditDouble::CEditDouble()
{
	iDecPt = 0;
	iPaintFlg = -1;
	iMinusFlg = 0;

	m_bSelect = FALSE;
	m_dMin = 0.0f;
	m_dMax = 100.0f;
}

CEditDouble::~CEditDouble()
{
}


BEGIN_MESSAGE_MAP(CEditDouble, CEdit)
	//{{AFX_MSG_MAP(CEditDouble)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
//	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CEditDouble::OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditDouble::OnEnSetfocus)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_COPY, &CEditDouble::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CEditDouble::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CEditDouble::OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, &CEditDouble::OnEditUndo)
END_MESSAGE_MAP()


void CEditDouble::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int iPeriodFlg = 0;
	int iBackSpFlg = 0;
	int iPeriodChk = 0;
	int iLengthChk = 0;
	char LineStr[100];

	CString str;
	int iLeng, iPos;
	
	CPoint pt =  GetCaretPos();							// カレットのポジションを取得
	int iRet = CharFromPos(pt);							// それに一番誓い文字列のINDEXを取得
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// 選択されている文字の開始と終了のINDEXを取得
	short sStart = (short)(iSelChk & 0x0000FFFF);		// 開始のINDEX取得
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// 終了のINDEX取得
	
	int CurrentChar = (int)LOWORD(GetSel());			// Editの文字を取得
	int CurLine = LineFromChar(CurrentChar);
	int len = GetLine(CurLine, LineStr, 100);
	LineStr[len] = '\0';
	if(iMinusFlg == 1){
		if(LineStr[0] == '-'){
			iMinusChk = 1;
			str = &LineStr[1];
		}
		else {
			iMinusChk = 0;
			str = LineStr;
		}
	}
	else {
		str = LineStr;
	}

	iLeng = str.GetLength();							// 文字数を取得
	iPos = str.Find('.');								// 小数点があるかチェックする
	if(iPos < 0){										// 無かった時
		iPeriodChk = 0;
		iLengthChk = 0;
		if ((iLeng >= 3)								// 3文字以上入力されていて
		 && (nChar != VK_BACK)							// バックスペースキーでなくて
		 && (nChar != '.')								// 小数点でもないとき
		 && (iDecPt == 0)){								// 
			iDecPt = 1;									// 自動的に小数点を付加するフラグON
		}
	}
	else if(nChar != VK_BACK && nChar != VK_TAB){		// バックスペースでなくてタブキーでない時
		iPeriodChk = 1;
//		if(str.GetLength() < 7){
//			iLengthChk = 1;
//		}
		if((iLeng - (iPos + 1)) < 9){					// ・
			iLengthChk = 0;								// 
		}
		else {											// 小数点以下２桁入力済み
//			if(sStart <= iPos){							// 一番最後にカーソルがあるか？
			if(sEnd <= iPos){							// 一番最後にカーソルがあるか？
				iLengthChk = 0;							// 一番最後ではない
				if(str.GetLength() >= 11){
					if ((sEnd - sStart) > 0){				// 選択されているテキストがあるか？
						iLengthChk = 0;						// ある場合は最大桁入力されていてもOK！
					}
					else {
						iLengthChk = 1;
					}
				}
			}
			else {										
				if ((sEnd - sStart) > 0){				// 選択されているテキストがあるか？
					iLengthChk = 0;						// ある場合は最大桁入力されていてもOK！
				}
				else {
					iLengthChk = 1;						// 選択されていない時はこれ以上入力不可
				}
			}
		}
	}
#if 0
	else if(nChar == VK_BACK){							// バックスペースキーの時
		if ((sStart == sEnd)
		 && ((iPos + 1) == (sStart - 1))){				// 最後の文字の前は小数点か？
			iDecPt = -1;								// 小数点だったらフラグに-1を立てる
		}
		else if((iPos + 1) == sStart){					// 小数点だけ消そうとしたら
			iLengthChk = 1;
		}
	}
#endif

	if(!isdigit(nChar) && nChar != VK_BACK && nChar != VK_TAB){
		if(nChar != '.' || iPeriodChk != 0){
			if(iMinusFlg == 1){
				if((nChar != '-' || iMinusChk != 0)){
					MessageBeep(0);
					return;
				}
			}
			else {
				MessageBeep(0);
				return;
			}
		}
	}
	else if(isdigit(nChar) && iLengthChk != 0){
		MessageBeep(0);
		return;
	}
	if(iLengthChk == 0){								// 最大長入力チェックOKのとき
		if (VK_BACK == nChar){
			iPaintFlg = sStart - 1;
		}
		else {
			iPaintFlg = sStart + 1;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		if(iDecPt > 0){									// 小数点追加処理がある
			iPaintFlg++;
			Invalidate();								// エディットコントロール書き換え
		}
		else if(iDecPt < 0){							// 小数点削除処理がある
			if (str.GetLength() > sStart){				// 小数点が２けたあるのに小数点を削除しようとしたら
				iPaintFlg++;							// カレットの位置は１つ左へ
			}
			iPaintFlg--;								// カレットの位置は１つ左へ
			Invalidate();								// エディットコントロール書き換え
		}
		if ((str.Find('.') >= sStart)					// 小数点より前で入力されて
		 && (str.Find('.') >= 3)						// 小数点の位置が111.11以上右で
		 && (isdigit(nChar))){							// 数値の時は
			Invalidate();								// 再表示
		}
		if (((iPos + 1) == sStart)
		 && (nChar == VK_BACK)){						// バックスペースキーの時// 小数点だけ消そうとしたら
			iPaintFlg--;
			Invalidate();								// エディットコントロール書き換え
		}
	}
}

void CEditDouble::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_DELETE)								// DeleteKey押下時処理
	{

		CString str;										// エディット内文字列取得
		int CurrentChar = (int)LOWORD(GetSel());
		int CurLine = LineFromChar(CurrentChar);
		char LineStr[100];
		int len = GetLine(CurLine, LineStr, 100);
		LineStr[len] = '\0';
		str = LineStr;

		int iSelChk = GetSel();								// 選択されている文字の開始と終了のINDEXを取得
		short sStart = (short)(iSelChk & 0x0000FFFF);		// 開始のINDEX取得
		short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16) ;// 終了のINDEX取得
		if(sStart == 0){
			iMinusChk = 0;
		}
		Invalidate();										// 再描画

		iPaintFlg = sStart;									// カレットの位置を設定
	}

	else if (VK_UP == nChar){
		GetParent()->PostMessage(WM_COMMAND,VK_UP,NULL);
	}
	else if (VK_DOWN == nChar){
		GetParent()->PostMessage(WM_COMMAND,VK_DOWN,NULL);
	}
	else if (VK_LEFT == nChar){
		GetParent()->PostMessage(WM_COMMAND,VK_LEFT,NULL);
	}
	else if (VK_RIGHT == nChar){
		GetParent()->PostMessage(WM_COMMAND,VK_RIGHT,NULL);
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


int CEditDouble::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
void CEditDouble::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
#if 0
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CString str;										// エディット内文字列取得
	int CurrentChar = (int)LOWORD(GetSel());
	int CurLine = LineFromChar(CurrentChar);
	char LineStr[100];
	int len = GetLine(CurLine, LineStr, 100);
	LineStr[len] = '\0';
	if(iMinusFlg == 1){
		if(iMinusChk == 1){
			str = &LineStr[1];
		}
		else {
			str = LineStr;
		}
	}
	else {
		str = LineStr;
	}

	if(iDecPt > 0){										// 小数点自動付加処理
		CString st1, st2;
		st1 = str;
		st2 = st1.Left(3);
		st2 += '.';
		st2 += st1.Right(1);
		str = st2;
		iDecPt = 0;
	}
	else if(iDecPt < 0){								// 小数点自動削除処理
		CString st1, st2;
		st1 = str.Left(str.Find('.'));
		if(str.GetLength() > (str.Find('.') + 1))
		{
			st2 = str.Right(1);
			str = st1;
			str += st2;
		}
		else {
			str = st1;
		}
		iDecPt = 0;
	}
	if ((str.Find('.') < 0)								// フォーマット自動変更処理
	 && (str.GetLength() > 3)){
		CString st1, st2;
		st1 = str;
		st2 = st1.Left(3);
		st2 += '.';
		if(str.GetLength() == 4){
			st2 += st1.Right(1);
		}
		else if(str.GetLength() == 5){
			st2 += st1.Right(2);
		}
		str = st2;
	}
	else if(str.Find('.') > 3){							// フォーマット自動変更処理その２
		CString st1, st2;
		st1 = str.Left(3);
		st2 = str.Mid(3, 1);
		if ((str.GetLength() - 1) > str.Find('.')){
			st2 += str.Right(1);
		}
		str = st1 + '.' + st2;
	}
	SetWindowText("");									// これを入れないとエディット内がきれいに白くならない
	SetSel(0, -1);										// エディット内の文字をすべて選択
	if(iMinusFlg == 1){
		if(iMinusChk == 1){
			str = "-" + str;
		}
	}

	ReplaceSel(str);									// 新しい文字で置き換えする
#endif
	// 描画用メッセージとして CEdit::OnPaint() を呼び出してはいけません
	CEdit::OnPaint();									// いけなくても呼んじゃう！！
}
void CEditDouble::OnChange() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CEdit::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	SetSel(iPaintFlg, iPaintFlg);							// 常にカレットの位置を変更する
}
void CEditDouble::OnEnKillfocus()
{
	char	szVal[32+1];
	float	fVal;

	m_bSelect = FALSE;
	
	GetWindowTextA(szVal,32);
	fVal = (float)atof(szVal);

	if(fVal < (float)m_dMin)
		fVal = (float)m_dMin;
	else if(fVal > (float)m_dMax)
		fVal = (float)m_dMax;
#if 0
	if(m_dMax < 10)
		sprintf(szVal,"%3.1f",fVal);
	else if(m_dMax < 100)
		sprintf(szVal,"%4.1f",fVal);
	else if(m_dMax < 1000)
		sprintf(szVal,"%5.1f",fVal);
	else
		sprintf(szVal,"%6.1f",fVal);
#endif
	sprintf(szVal,"%12.9f",fVal);
	SetWindowTextA(szVal);
	Invalidate();
}

void CEditDouble::OnEnSetfocus()
{
	m_bSelect = TRUE;
	SetSel(0,-1);
	Invalidate();

}
void CEditDouble::SetScale(double dMin,double dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;

}


void CEditDouble::OnEditCopy()
{
	Copy();
}

void CEditDouble::OnEditCut()
{
	Cut();
}

void CEditDouble::OnEditPaste()
{
	Paste();
}

void CEditDouble::OnEditUndo()
{
	if(CanUndo())
		Undo();
}
/* 
**====================================================
** CEditIntegrer
**====================================================
*/
CEditInteger::CEditInteger()
{
	m_bSelect = FALSE;
	m_iMin = 0;
	m_iMax = 100;
}

CEditInteger::~CEditInteger()
{
}


BEGIN_MESSAGE_MAP(CEditInteger,CEditDouble)
	//{{AFX_MSG_MAP(CEditDouble)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CEditInteger::OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditInteger::OnEnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CEditInteger::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int iPeriodFlg = 0;
	int iBackSpFlg = 0;
	int iPeriodChk = 0;
	int iLengthChk = 0;
	char LineStr[100];

	CString str;
	
	CPoint pt =  GetCaretPos();							// カレットのポジションを取得
	int iRet = CharFromPos(pt);							// それに一番誓い文字列のINDEXを取得
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// 選択されている文字の開始と終了のINDEXを取得
	short sStart = (short)(iSelChk & 0x0000FFFF);		// 開始のINDEX取得
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// 終了のINDEX取得
	
	int CurrentChar = (int)LOWORD(GetSel());			// Editの文字を取得
	int CurLine = LineFromChar(CurrentChar);
	int len = GetLine(CurLine, LineStr, 100);
	LineStr[len] = '\0';

	if(!isdigit(nChar) && nChar != VK_BACK && nChar != VK_TAB && nChar != '-'){
		MessageBeep(0);
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}



int CEditInteger::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
void CEditInteger::OnEnKillfocus()
{
	char	szVal[32+1];
	int		iVal;
	int		iMin,iMax;

	m_bSelect = FALSE;
	
	GetWindowTextA(szVal,32);
	iVal = atoi(szVal);

	iMin = m_iMin;
	iMax = m_iMax;

	if(iMin ==0 && iMax < 0){
		iMin = iMax;
		iMax  = 0;
	}

	if(iVal < iMin)
		iVal = iMin;
	else if(iVal > iMax)
		iVal = iMax;

#if 0
	if(m_iMax < 10)
		sprintf(szVal,"%1d",iVal);
	else if(m_iMax < 100)
		sprintf(szVal,"%2d",iVal);
	else if(m_iMax < 1000)
		sprintf(szVal,"%3d",iVal);
	else
		sprintf(szVal,"%4d",iVal);
#endif
	sprintf(szVal,"%5d",iVal);
	SetWindowTextA(szVal);
	Invalidate();
}

void CEditInteger::OnEnSetfocus()
{
	m_bSelect = TRUE;
	SetSel(0,-1);
	Invalidate();

}
void CEditInteger::SetScale(int iMin,int iMax)
{
	m_iMin = iMin;
	m_iMax = iMax;

}
/* 
**====================================================
** CEditEnum
**====================================================
*/
CEditEnum::CEditEnum()
{
	m_bSelect = FALSE;
	m_iMin = 0;
	m_iMax = 10;
}

CEditEnum::~CEditEnum()
{
}


BEGIN_MESSAGE_MAP(CEditEnum,CEditDouble)
	//{{AFX_MSG_MAP(CEditDouble)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CEditEnum::OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditEnum::OnEnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CEditEnum::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int iPeriodFlg = 0;
	int iBackSpFlg = 0;
	int iPeriodChk = 0;
	int iLengthChk = 0;
	char LineStr[100];

	CString str;
	
	CPoint pt =  GetCaretPos();							// カレットのポジションを取得
	int iRet = CharFromPos(pt);							// それに一番誓い文字列のINDEXを取得
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// 選択されている文字の開始と終了のINDEXを取得
	short sStart = (short)(iSelChk & 0x0000FFFF);		// 開始のINDEX取得
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// 終了のINDEX取得
	
	int CurrentChar = (int)LOWORD(GetSel());			// Editの文字を取得
	int CurLine = LineFromChar(CurrentChar);
	int len = GetLine(CurLine, LineStr, 100);
	LineStr[len] = '\0';

	if(!isdigit(nChar) && nChar != VK_BACK && nChar != VK_TAB){
		MessageBeep(0);
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}



int CEditEnum::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
void CEditEnum::OnEnKillfocus()
{
	char	szVal[40+1];
	int		iVal;

	m_bSelect = FALSE;
	
	GetWindowTextA(szVal,32);
	if(szVal[0] >= '0' && szVal[0] <= '9'){
		if(szVal[1] >= '0' && szVal[1] <= '9')
			szVal[2]='\0';
		else
			szVal[1]='\0';

		iVal = atoi(szVal);
	}
	else
		iVal = m_iMin;

	if(iVal < m_iMin)
		iVal = m_iMin;
	else if(iVal >= m_iMax)
		iVal = m_iMax-1;

	sprintf(szVal,"%d:%s",iVal,m_pszEnumStr[iVal]);
	szVal[32] = '\0';
	SetWindowTextA(szVal);
	Invalidate();
}

void CEditEnum::OnEnSetfocus()
{
	m_bSelect = TRUE;
	SetSel(0,-1);
	Invalidate();

}
void CEditEnum::SetScale(int iMin,int iMax,char *pEnum[])
{
	m_iMin = iMin;
	m_iMax = iMax;
	m_pszEnumStr = pEnum;
}
/* 
**====================================================
** CEditString
**====================================================
*/
CEditString::CEditString()
{
	m_bSelect = FALSE;
	m_iMaxLen = 10;
}

CEditString::~CEditString()
{
}


BEGIN_MESSAGE_MAP(CEditString,CEditDouble)
	//{{AFX_MSG_MAP(CEditDouble)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CEditString::OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditString::OnEnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CEditString::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int iPeriodFlg = 0;
	int iBackSpFlg = 0;
	int iPeriodChk = 0;
	int iLengthChk = 0;
	char LineStr[100];

	CString str;
	if (GetKeyState(VK_CONTROL) & 0x8000)
		return;

	CPoint pt =  GetCaretPos();							// カレットのポジションを取得
	int iRet = CharFromPos(pt);							// それに一番誓い文字列のINDEXを取得
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// 選択されている文字の開始と終了のINDEXを取得
	short sStart = (short)(iSelChk & 0x0000FFFF);		// 開始のINDEX取得
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// 終了のINDEX取得
	
	int CurrentChar = (int)LOWORD(GetSel());			// Editの文字を取得
	int CurLine = LineFromChar(CurrentChar);
	int len = GetLine(CurLine, LineStr, 100);
	LineStr[len] = '\0';
	str = LineStr;

	if(nChar != VK_BACK && nChar != VK_TAB){
		if(str.GetLength() == m_iMaxLen){
			MessageBeep(0);
			return;
		}
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}



int CEditString::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
void CEditString::OnEnKillfocus()
{
	char	szVal[40+1];

	m_bSelect = FALSE;
	
	GetWindowTextA(szVal,32);

	if(strlen(szVal) > m_iMaxLen)
		szVal[m_iMaxLen] = '\0';

	SetWindowTextA(szVal);
	Invalidate();
}

void CEditString::OnEnSetfocus()
{
	m_bSelect = TRUE;
	SetSel(0,-1);
	Invalidate();

}
void CEditString::SetScale(int iMaxLen)
{
	m_iMaxLen = iMaxLen;
}
