// EditLimit.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "EditLimit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLimit

CEditLimit::CEditLimit()
{
	m_pCtrl = NULL;
	m_iMax = 0;
	m_iMin = 0;
}

CEditLimit::~CEditLimit()
{
}


BEGIN_MESSAGE_MAP(CEditLimit, CEdit)
	//{{AFX_MSG_MAP(CEditLimit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLimit メッセージ ハンドラ

void CEditLimit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if ((nChar < 0x30 || nChar > 0x39)
	 && (nChar != VK_TAB)
	 && (nChar != VK_BACK)	 && (nChar != VK_DELETE))
	{
		return;
	}

	CString stValue('\0',64);
	GetWindowText(stValue);

	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nChar >= 0x30 && nChar <= 0x39)
	{
		int iValue = 0;
		if(nStart < nEnd){
			CString stWork('\0',64);
			stWork = "";
			stWork = stValue.Left(nStart);
			stWork += (char)nChar;
			stWork += stValue.Right(stValue.GetLength()-nEnd);
			stValue = stWork;
			iValue = atoi(stValue);
		}
		else {
			iValue = atoi(stValue);
			iValue *= 10;
			iValue += nChar - 0x30;
		}


		int iMax, iMin;
		
		if(m_pCtrl){
			m_pCtrl->GetRange(iMin, iMax);
		}
		else {
			iMin = m_iMin;
			iMax = m_iMax;
		}

		if (iValue > iMax || iValue < iMin){
			return;
		}
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CEditLimit::SetRange(int iMin, int iMax)
{
	m_iMax = iMax;
	m_iMin = iMin;

}
/////////////////////////////////////////////////////////////////////////////
// CEditDLimit

CEditDLimit::CEditDLimit()
{
	m_pCtrl = NULL;
	m_dMax = 0.0f;
	m_dMin = 0.0f;
}

CEditDLimit::~CEditDLimit()
{
}


BEGIN_MESSAGE_MAP(CEditDLimit, CEdit)
	//{{AFX_MSG_MAP(CEditDLimit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLimit メッセージ ハンドラ

void CEditDLimit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if ((nChar < 0x30 || nChar > 0x39)
	 && (nChar != VK_TAB)
	 && (nChar != VK_BACK)
	 && (nChar != VK_DELETE)
	 && (nChar != '-')
	 && (nChar != '.'))
	{
		return;
	}

	CString stValue('\0',64);
	GetWindowText(stValue);

	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nChar >= 0x30 && nChar <= 0x39)
	{
		// 小数点以下の桁数ﾁｪｯｸ
		int iPoint = stValue.Find('.');
		if( iPoint != -1){
			if(nEnd > iPoint){
				if((stValue.GetLength() - (iPoint+1)) >= m_iPointColumn)
					return;
			}
		}
		
		double dValue = 0;
		if(nStart <= nEnd){
			CString stWork('\0',64);
			stWork = "";
			stWork = stValue.Left(nStart);
			stWork += (char)nChar;
			stWork += stValue.Right(stValue.GetLength()-nEnd);
			stValue = stWork;
			dValue = atof(stValue);
		}
		else {
			stValue += (char)nChar;
			dValue = atof(stValue);
		}

		double dMax, dMin;
		
		if(m_pCtrl){
			int iMax, iMin;
			m_pCtrl->GetRange(iMin, iMax);
			dMin = (double)iMin;
			dMax = (double)iMax;
			
		}
		else {
			dMin = m_dMin;
			dMax = m_dMax;
		}

		if (dValue > dMax){
			return;
		}
	}
	else if (nChar == '.')
	{
		if(stValue.GetLength() == 0)
			return;

		if(stValue.Find('.') != -1)
			return;
		SetSel((stValue.GetLength()),(stValue.GetLength()));
	}
	else if (nChar == '-')
	{
		if(stValue.Find('-') != -1)
			return;

		double dMax, dMin;
		double dValue = 0;
		
		if(m_pCtrl){
			int iMax, iMin;
			m_pCtrl->GetRange(iMin, iMax);
			dMin = (double)iMin;
			dMax = (double)iMax;
			
		}
		else {
			dMin = m_dMin;
			dMax = m_dMax;
		}
		dValue = atof((LPCSTR)stValue);
		dValue *= -1;
		if (dValue < dMin){
			return;
		}
		SetSel(0,0);
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CEditDLimit::SetRange(double dMin,double dMax,int iPointColumn)
{
	m_dMax = dMax;
	m_dMin = dMin;
	m_iPointColumn = iPointColumn;
}

/////////////////////////////////////////////////////////////////////////////
// CEdit_IIO

CEdit_IO::CEdit_IO()
{
}

CEdit_IO::~CEdit_IO()
{
}


BEGIN_MESSAGE_MAP(CEdit_IO, CEdit)
	//{{AFX_MSG_MAP(CEdit_IO)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLimit メッセージ ハンドラ

void CEdit_IO::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	double dValue = 0;

	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if ((nChar < 0x30 || nChar > 0x39)
	 && (nChar != VK_TAB)
	 && (nChar != VK_BACK)
	 && (nChar != VK_DELETE)
	 && (nChar != '-')
	 && (nChar != '.'))
	{
		return;
	}
	if(m_iotype !=  IO_FLOAT && m_iotype !=  IO_DOUBLE && m_iotype != IO_DOUBLE_E && (nChar == '.'))
		return;
	if(m_ioscale.d_log_low >= 0 && (nChar == '-'))
		return;

#if 0
	if(m_iotype != IO_CHAR && m_iotype != IO_SHORT && m_iotype != IO_INT && m_iotype != IO_LONG && (nChar == '-'))
		return;
	if(nChar == '-'){

	}
#endif
	CString stValue('\0',64);
	GetWindowText(stValue);

	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nChar >= 0x30 && nChar <= 0x39)
	{
		char	szchar[2];

		if(nStart <= nEnd){
			CString stWork('\0',64);
			stWork = "";
			stWork = stValue.Left(nStart);
			stWork += (char)nChar;
			stWork += stValue.Right(stValue.GetLength()-nEnd);
			stValue = stWork;
			dValue = atof(stValue);
		}
		else {
			szchar[0] = (char)nChar; 
			szchar[1] = '\0'; 
			stValue += szchar;
			dValue = atof(stValue);
		}

//		if (dValue > m_ioscale.d_log_high || dValue < m_ioscale.d_log_low){
		if (dValue > m_ioscale.d_log_high){
			return;
		}
	}
	else if (nChar == '.')
	{
		if(stValue.GetLength() == 0)
			return;

		if(stValue.Find('.') != -1)
			return;
		if (dValue > m_ioscale.d_log_high){
			return;
		}
		SetSel((stValue.GetLength()),(stValue.GetLength()));
	}
	else if (nChar == '-')
	{
		if(stValue.Find('-') != -1)
			return;

		dValue = atof((LPCSTR)stValue);
		dValue *= -1;
		if (dValue < m_ioscale.d_log_low){
			return;
		}
		if(nStart == nEnd && nStart != 0)
			SetSel(0,0);
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


