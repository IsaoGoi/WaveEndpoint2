/////////////////////////////////////////////////////////////////////////////
// CEditDouble.cpp : �C���v�������e�[�V���� �t�@�C��
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
	
	CPoint pt =  GetCaretPos();							// �J���b�g�̃|�W�V�������擾
	int iRet = CharFromPos(pt);							// ����Ɉ�Ԑ����������INDEX���擾
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// �I������Ă��镶���̊J�n�ƏI����INDEX���擾
	short sStart = (short)(iSelChk & 0x0000FFFF);		// �J�n��INDEX�擾
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// �I����INDEX�擾
	
	int CurrentChar = (int)LOWORD(GetSel());			// Edit�̕������擾
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

	iLeng = str.GetLength();							// ���������擾
	iPos = str.Find('.');								// �����_�����邩�`�F�b�N����
	if(iPos < 0){										// ����������
		iPeriodChk = 0;
		iLengthChk = 0;
		if ((iLeng >= 3)								// 3�����ȏ���͂���Ă���
		 && (nChar != VK_BACK)							// �o�b�N�X�y�[�X�L�[�łȂ���
		 && (nChar != '.')								// �����_�ł��Ȃ��Ƃ�
		 && (iDecPt == 0)){								// 
			iDecPt = 1;									// �����I�ɏ����_��t������t���OON
		}
	}
	else if(nChar != VK_BACK && nChar != VK_TAB){		// �o�b�N�X�y�[�X�łȂ��ă^�u�L�[�łȂ���
		iPeriodChk = 1;
//		if(str.GetLength() < 7){
//			iLengthChk = 1;
//		}
		if((iLeng - (iPos + 1)) < 9){					// �E
			iLengthChk = 0;								// 
		}
		else {											// �����_�ȉ��Q�����͍ς�
//			if(sStart <= iPos){							// ��ԍŌ�ɃJ�[�\�������邩�H
			if(sEnd <= iPos){							// ��ԍŌ�ɃJ�[�\�������邩�H
				iLengthChk = 0;							// ��ԍŌ�ł͂Ȃ�
				if(str.GetLength() >= 11){
					if ((sEnd - sStart) > 0){				// �I������Ă���e�L�X�g�����邩�H
						iLengthChk = 0;						// ����ꍇ�͍ő包���͂���Ă��Ă�OK�I
					}
					else {
						iLengthChk = 1;
					}
				}
			}
			else {										
				if ((sEnd - sStart) > 0){				// �I������Ă���e�L�X�g�����邩�H
					iLengthChk = 0;						// ����ꍇ�͍ő包���͂���Ă��Ă�OK�I
				}
				else {
					iLengthChk = 1;						// �I������Ă��Ȃ����͂���ȏ���͕s��
				}
			}
		}
	}
#if 0
	else if(nChar == VK_BACK){							// �o�b�N�X�y�[�X�L�[�̎�
		if ((sStart == sEnd)
		 && ((iPos + 1) == (sStart - 1))){				// �Ō�̕����̑O�͏����_���H
			iDecPt = -1;								// �����_��������t���O��-1�𗧂Ă�
		}
		else if((iPos + 1) == sStart){					// �����_�����������Ƃ�����
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
	if(iLengthChk == 0){								// �ő咷���̓`�F�b�NOK�̂Ƃ�
		if (VK_BACK == nChar){
			iPaintFlg = sStart - 1;
		}
		else {
			iPaintFlg = sStart + 1;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		if(iDecPt > 0){									// �����_�ǉ�����������
			iPaintFlg++;
			Invalidate();								// �G�f�B�b�g�R���g���[����������
		}
		else if(iDecPt < 0){							// �����_�폜����������
			if (str.GetLength() > sStart){				// �����_���Q��������̂ɏ����_���폜���悤�Ƃ�����
				iPaintFlg++;							// �J���b�g�̈ʒu�͂P����
			}
			iPaintFlg--;								// �J���b�g�̈ʒu�͂P����
			Invalidate();								// �G�f�B�b�g�R���g���[����������
		}
		if ((str.Find('.') >= sStart)					// �����_���O�œ��͂����
		 && (str.Find('.') >= 3)						// �����_�̈ʒu��111.11�ȏ�E��
		 && (isdigit(nChar))){							// ���l�̎���
			Invalidate();								// �ĕ\��
		}
		if (((iPos + 1) == sStart)
		 && (nChar == VK_BACK)){						// �o�b�N�X�y�[�X�L�[�̎�// �����_�����������Ƃ�����
			iPaintFlg--;
			Invalidate();								// �G�f�B�b�g�R���g���[����������
		}
	}
}

void CEditDouble::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_DELETE)								// DeleteKey����������
	{

		CString str;										// �G�f�B�b�g��������擾
		int CurrentChar = (int)LOWORD(GetSel());
		int CurLine = LineFromChar(CurrentChar);
		char LineStr[100];
		int len = GetLine(CurLine, LineStr, 100);
		LineStr[len] = '\0';
		str = LineStr;

		int iSelChk = GetSel();								// �I������Ă��镶���̊J�n�ƏI����INDEX���擾
		short sStart = (short)(iSelChk & 0x0000FFFF);		// �J�n��INDEX�擾
		short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16) ;// �I����INDEX�擾
		if(sStart == 0){
			iMinusChk = 0;
		}
		Invalidate();										// �ĕ`��

		iPaintFlg = sStart;									// �J���b�g�̈ʒu��ݒ�
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
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
#if 0
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;										// �G�f�B�b�g��������擾
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

	if(iDecPt > 0){										// �����_�����t������
		CString st1, st2;
		st1 = str;
		st2 = st1.Left(3);
		st2 += '.';
		st2 += st1.Right(1);
		str = st2;
		iDecPt = 0;
	}
	else if(iDecPt < 0){								// �����_�����폜����
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
	if ((str.Find('.') < 0)								// �t�H�[�}�b�g�����ύX����
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
	else if(str.Find('.') > 3){							// �t�H�[�}�b�g�����ύX�������̂Q
		CString st1, st2;
		st1 = str.Left(3);
		st2 = str.Mid(3, 1);
		if ((str.GetLength() - 1) > str.Find('.')){
			st2 += str.Right(1);
		}
		str = st1 + '.' + st2;
	}
	SetWindowText("");									// ��������Ȃ��ƃG�f�B�b�g�������ꂢ�ɔ����Ȃ�Ȃ�
	SetSel(0, -1);										// �G�f�B�b�g���̕��������ׂđI��
	if(iMinusFlg == 1){
		if(iMinusChk == 1){
			str = "-" + str;
		}
	}

	ReplaceSel(str);									// �V���������Œu����������
#endif
	// �`��p���b�Z�[�W�Ƃ��� CEdit::OnPaint() ���Ăяo���Ă͂����܂���
	CEdit::OnPaint();									// �����Ȃ��Ă��Ă񂶂Ⴄ�I�I
}
void CEditDouble::OnChange() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� EM_SETEVENTMASK
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CEdit::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	SetSel(iPaintFlg, iPaintFlg);							// ��ɃJ���b�g�̈ʒu��ύX����
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
	
	CPoint pt =  GetCaretPos();							// �J���b�g�̃|�W�V�������擾
	int iRet = CharFromPos(pt);							// ����Ɉ�Ԑ����������INDEX���擾
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// �I������Ă��镶���̊J�n�ƏI����INDEX���擾
	short sStart = (short)(iSelChk & 0x0000FFFF);		// �J�n��INDEX�擾
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// �I����INDEX�擾
	
	int CurrentChar = (int)LOWORD(GetSel());			// Edit�̕������擾
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
	
	CPoint pt =  GetCaretPos();							// �J���b�g�̃|�W�V�������擾
	int iRet = CharFromPos(pt);							// ����Ɉ�Ԑ����������INDEX���擾
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// �I������Ă��镶���̊J�n�ƏI����INDEX���擾
	short sStart = (short)(iSelChk & 0x0000FFFF);		// �J�n��INDEX�擾
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// �I����INDEX�擾
	
	int CurrentChar = (int)LOWORD(GetSel());			// Edit�̕������擾
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

	CPoint pt =  GetCaretPos();							// �J���b�g�̃|�W�V�������擾
	int iRet = CharFromPos(pt);							// ����Ɉ�Ԑ����������INDEX���擾
	iRet &= 0x0000FFFF;

	int iSelChk = GetSel();								// �I������Ă��镶���̊J�n�ƏI����INDEX���擾
	short sStart = (short)(iSelChk & 0x0000FFFF);		// �J�n��INDEX�擾
	short sEnd   = (short)((iSelChk & 0xFFFF0000)>>16);	// �I����INDEX�擾
	
	int CurrentChar = (int)LOWORD(GetSel());			// Edit�̕������擾
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
