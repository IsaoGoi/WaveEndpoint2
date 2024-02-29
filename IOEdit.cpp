// IOEdit.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "IOEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIOEdit

CIOEdit::CIOEdit()
{
	m_bUpdate = FALSE;
	m_bSelcted = FALSE;
	m_iSpecialColor = FALSE;
	m_bPassword = FALSE;
	m_iIOHdl = INVALID_HDL;
	m_bHexDisplay = FALSE;
}

CIOEdit::~CIOEdit()
{
}


BEGIN_MESSAGE_MAP(CIOEdit, CEdit)
	//{{AFX_MSG_MAP(CIOEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CIOEdit::OnEnSetfocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOEdit メッセージ ハンドラ

void CIOEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if(GetStyle() & ES_READONLY){
		return;
	}
	CEdit::OnLButtonDown(nFlags, point);
	SetEditorPage();

}
void CIOEdit::SetEditorPage() 
{
	CString		szEdit('\0',64);
	BOOL		bWrite =FALSE;
	char		*pEnumString[40];
	int			iSize;
	unsigned	uVal;
	char			char_val;
	unsigned char	uchar_val;
	short			short_val;
	unsigned short  ushort_val;
	int				int_val;
	unsigned		uint_val;
	long			long_val;
	unsigned long	ulong_val;
	float			float_val;
	double			double_val;
	char			string_val[128+1];
	char			szDecimalFormat[32 + 2];

	CWaveEndPointApp *pApp = (CWaveEndPointApp*)AfxGetApp();

	m_DlgEnumEdit = pApp->m_pEnumEditPage;
	m_DlgNumEdit = pApp->m_pNumEditPage;
	m_DlgStringEdit = pApp->m_pStringEditPage;

	if(GetStyle() & ES_READONLY)
		return;

	if(::IsWindow(m_DlgNumEdit->m_hWnd) != 0){
		if(m_DlgNumEdit->IsWindowVisible() != 0){
			if(m_DlgNumEdit->GetEditFld() == this){
				m_DlgNumEdit->SendMessage(WM_CLOSE);
				return;
			}
			m_DlgNumEdit->SendMessage(WM_CLOSE);
		}
	}
	if(::IsWindow(m_DlgEnumEdit->m_hWnd) != 0){
		if(m_DlgEnumEdit->IsWindowVisible() != 0)
			if(m_DlgEnumEdit->GetEditFld() == this){
				m_DlgEnumEdit->SendMessage(WM_CLOSE);
				return;
			}
			m_DlgEnumEdit->SendMessage(WM_CLOSE);
	}
	if(::IsWindow(m_DlgStringEdit->m_hWnd) != 0){
		if(m_DlgStringEdit->IsWindowVisible() != 0)
			if(m_DlgStringEdit->GetEditFld() == this){
				m_DlgStringEdit->SendMessage(WM_CLOSE);
				return;
			}
			m_DlgStringEdit->SendMessage(WM_CLOSE);
	}

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	switch(m_iotype){
	case IO_CHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&char_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%d",(int)char_val);
		else
			szEdit.Format("%02,02X",(int)char_val);
		break;
	case IO_UCHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&uchar_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%d",(unsigned)uchar_val);
		else
			szEdit.Format("%02.02X",(unsigned)uchar_val);
		break;
	case IO_SHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&short_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%u",short_val);
		else
			szEdit.Format("%04.04X",short_val);
		break;
	case IO_USHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&ushort_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%u",ushort_val);
		else
			szEdit.Format("%04.04X",ushort_val);
		break;
	case IO_INT:
		m_pIODB ->IOPointRead(m_iIOHdl,&int_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%d",int_val);
		else
			szEdit.Format("%04.04X",int_val);
		break;
	case IO_UINT:
		m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%d",uint_val);
		else
			szEdit.Format("%04.04X",uint_val);
		break;
	case IO_LONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&long_val);
		szEdit.Format("%d",long_val);
		break;
	case IO_ULONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&ulong_val);
		szEdit.Format("%d",ulong_val);
		break;
	case IO_FLOAT:
		m_pIODB ->IOPointRead(m_iIOHdl,&float_val);
		strcpy(szDecimalFormat, m_pIODB->IOPointGetDecimalFormat(m_iIOHdl));
		szEdit.Format(szDecimalFormat, float_val);
		break;
	case IO_DOUBLE:
		m_pIODB ->IOPointRead(m_iIOHdl,&double_val);
		strcpy(szDecimalFormat, m_pIODB->IOPointGetDecimalFormat(m_iIOHdl));
		szEdit.Format(szDecimalFormat, double_val);
		break;
	case IO_DOUBLE_E:
		m_pIODB->IOPointRead(m_iIOHdl, &double_val);
		szEdit.Format("%e", double_val);
		break;
	case IO_STRING:
		m_pIODB ->IOPointRead(m_iIOHdl,string_val);
		szEdit = string_val;
		break;
	case IO_MASK:
		m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
		if(m_bHexDisplay==FALSE)
			szEdit.Format("%d",uint_val);
		else
			szEdit.Format("%04.04X",uint_val);
		break;
	}/* switch */

	switch(m_iotype){
	case IO_CHAR:
	case IO_UCHAR:
	case IO_SHORT:
	case IO_USHORT:
	case IO_INT:
	case IO_UINT:
	case IO_LONG:
	case IO_ULONG:
	case IO_FLOAT:
	case IO_DOUBLE:
	case IO_DOUBLE_E:
	case IO_MASK:
//		GetWindowText(szEdit);
		m_DlgNumEdit->SetValue((LPCSTR)szEdit,m_iotype,&m_ioscale,this);
		m_DlgNumEdit->DlgCreate(GetParent());
#ifdef _MODAL_
		iRet = m_DlgNumEdit->DoModal();
		if(iRet == IDOK){
			m_DlgNumEdit->GetValue((LPSTR)((LPCSTR)szEdit));
			bWrite = TRUE;
		}
#endif
		break;

	case IO_ENUM:
	case IO_BIT:
		m_pIODB ->IOPointRead(m_iIOHdl,&uVal);
		if(m_pIODB ->IOPointGetEnumString(m_iIOHdl,pEnumString)==TRUE){
			m_DlgEnumEdit->SetValue(uVal,pEnumString,this);
			m_DlgEnumEdit->DlgCreate(GetParent());
#ifdef _MODAL_
			iRet = m_DlgEnumEdit->DoModal();
			if(iRet == IDOK){
				m_DlgEnumEdit->GetValue(&uVal);
				bWrite = TRUE;
			}
#endif
		}
		break;
	case IO_STRING:
		iSize = m_pIODB ->IOPointGetStringMaxSize(m_iIOHdl);
		if(m_bPassword)
			m_DlgStringEdit->SetPasswordFld();
		m_DlgStringEdit->SetValue((LPCSTR)szEdit,iSize,this);
		m_DlgStringEdit->DlgCreate(GetParent());
		break;
	}/* switch */
}


BOOL CIOEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CIOEdit::SetNumEditPage(CDlgNumEdit *pPage)
{
	m_DlgNumEdit = pPage;
}

void CIOEdit::SetIOHdl(int iHandle)
{
	m_iIOHdl = iHandle;
	if(m_iIOHdl == INVALID_HDL)
		return;

	CWaveEndPointApp *pApp = (CWaveEndPointApp*)AfxGetApp();
	m_pIODB = pApp->GetIODB();

	m_iotype = m_pIODB->IOPointGetIOType(m_iIOHdl);
	m_pIODB ->IOPointGetScale(m_iIOHdl,&m_ioscale);

	switch(m_iotype){
	case IO_CHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_char_val);
		break;
	case IO_UCHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_uchar_val);
		break;
	case IO_SHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_short_val);
		break;
	case IO_USHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_ushort_val);
		break;
	case IO_INT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_int_val);
		break;
	case IO_UINT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_uint_val);
		break;
	case IO_LONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_long_val);
		break;
	case IO_ULONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_ulong_val);
		break;
	case IO_FLOAT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_float_val);
		break;
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_double_val);
		break;
	case IO_ENUM:
	case IO_BIT:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_uint_val);
		break;
	case IO_STRING:
		m_pIODB ->IOPointRead(m_iIOHdl,old_string_val);
		break;
	case IO_MASK:
		m_pIODB ->IOPointRead(m_iIOHdl,&old_uint_val);
		break;
	}/*	switch */

}


void CIOEdit::UpdateDisplay()
{
	Invalidate();
}

BOOL CIOEdit::ChkValueChange()
{
	CString		szEdit('\0',64);
	BOOL		bChange=FALSE;

	char			char_val;
	unsigned char	uchar_val;
	short			short_val;
	unsigned short  ushort_val;
	int				int_val;
	unsigned		uint_val;
	long			long_val;
	unsigned long	ulong_val;
	float			float_val;
	double			double_val;
	char			string_val[128+1];

		
	switch(m_iotype){
	case IO_CHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&char_val);
		if(char_val != old_char_val){
			old_char_val = char_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_UCHAR:
		m_pIODB ->IOPointRead(m_iIOHdl,&uchar_val);
		if(uchar_val != old_uchar_val){
			old_uchar_val = uchar_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_SHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&short_val);
		if(short_val != old_short_val){
			old_short_val = short_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_USHORT:
		m_pIODB ->IOPointRead(m_iIOHdl,&ushort_val);
		if(ushort_val != old_ushort_val){
			old_ushort_val = ushort_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_INT:
		m_pIODB ->IOPointRead(m_iIOHdl,&int_val);
		if(int_val != old_int_val){
			old_int_val = int_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_UINT:
		m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
		if(uint_val != old_uint_val){
			old_uint_val = uint_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_LONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&long_val);
		if(long_val != old_long_val){
			old_long_val = long_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_ULONG:
		m_pIODB ->IOPointRead(m_iIOHdl,&ulong_val);
		if(ulong_val != old_ulong_val){
			old_ulong_val = ulong_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_FLOAT:
		m_pIODB ->IOPointRead(m_iIOHdl,&float_val);
		if(float_val != old_float_val){
			old_float_val = float_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_DOUBLE:
	case IO_DOUBLE_E:
		m_pIODB ->IOPointRead(m_iIOHdl,&double_val);
		if(double_val != old_double_val){
			old_double_val = double_val;
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_ENUM:
	case IO_BIT:
		m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
		if(uint_val != old_uint_val){
			old_uint_val = uint_val;
			Invalidate();
#if 0
			if(m_pIODB ->IOPointGetEnumString(m_iIOHdl,pEnumString)==TRUE)
				SetWindowText(pEnumString[uint_val]);
#endif
			bChange = TRUE;
		}
		break;
	case IO_STRING:
		m_pIODB ->IOPointRead(m_iIOHdl,string_val);
		if(strncmp(old_string_val,string_val,128)){
			strncpy(old_string_val,string_val,128);
			Invalidate();
			bChange = TRUE;
		}
		break;
	case IO_MASK:
		m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
		if(uint_val != old_uint_val){
			old_uint_val = uint_val;
			Invalidate();
			bChange = TRUE;
		}
		break;


	}/*	switch */
	return(bChange);
}

void CIOEdit::SetValue(LPCSTR szEdit)
{
	char			char_val;
	unsigned char	uchar_val;
	short			short_val;
	unsigned short  ushort_val;
	int				int_val;
	unsigned		uint_val;
	long			long_val;
	unsigned long	ulong_val;
	float			float_val;
	double			double_val;
	char			string_val[129+1];

		
	switch(m_iotype){
	case IO_CHAR:
		char_val = (char)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&char_val);
		SetWindowText((LPCSTR)szEdit);
		old_char_val = char_val;
		break;
	case IO_UCHAR:
		uchar_val = (unsigned char)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&uchar_val);
		SetWindowText((LPCSTR)szEdit);
		old_uchar_val = uchar_val;
		break;
	case IO_SHORT:
		short_val = (short)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&short_val);
		SetWindowText((LPCSTR)szEdit);
		old_short_val = short_val;
		break;
	case IO_USHORT:
		ushort_val = (unsigned short)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&ushort_val);
		SetWindowText((LPCSTR)szEdit);
		old_ushort_val = ushort_val;
		break;
	case IO_INT:
		int_val = atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&int_val);
		SetWindowText((LPCSTR)szEdit);
		old_int_val = int_val;
		break;
	case IO_UINT:
		uint_val = (unsigned)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&uint_val);
		SetWindowText((LPCSTR)szEdit);
		old_uint_val = uint_val;
		break;
	case IO_LONG:
		long_val = (long)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&long_val);
		SetWindowText((LPCSTR)szEdit);
		old_long_val = long_val;
		break;
	case IO_ULONG:
		ulong_val = (unsigned long)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&ulong_val);
		SetWindowText((LPCSTR)szEdit);
		old_ulong_val = ulong_val;
		break;
	case IO_FLOAT:
		float_val = (float)atof((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&float_val);
		SetWindowText((LPCSTR)szEdit);
		old_float_val = float_val;
		break;
	case IO_DOUBLE:
		double_val = (double)atof((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&double_val);
		SetWindowText((LPCSTR)szEdit);
		old_double_val = double_val;
		break;
	case IO_DOUBLE_E:
		double_val = (double)atof((LPCSTR)szEdit);
//		sscanf((LPCSTR)szEdit ,"%e", &double_val);
		m_pIODB->IOPointWrite(m_iIOHdl, &double_val);
		SetWindowText((LPCSTR)szEdit);
		old_double_val = double_val;
		break;
	case IO_STRING:
		strncpy(string_val,(LPCSTR)szEdit,128);
		m_pIODB ->IOPointWrite(m_iIOHdl,string_val);
		SetWindowText((LPCSTR)szEdit);
		strncpy(old_string_val,string_val,128);
		break;
	case IO_MASK:
		uint_val = (unsigned)atoi((LPCSTR)szEdit);
		m_pIODB ->IOPointWrite(m_iIOHdl,&uint_val);
		SetWindowText((LPCSTR)szEdit);
		old_uint_val = uint_val;
		break;
#if 0
	case IO_ENUM:
		uint_val = uVal;
		m_pIODB ->IOPointWrite(m_iIOHdl,&uint_val);
		if(m_pIODB ->IOPointGetEnumString(m_iIOHdl,pEnumString)==TRUE)
			SetWindowText(pEnumString[uint_val]);
		break;
#endif
	}/*	switch */
	m_bUpdate = TRUE;

}
void CIOEdit::SetEnumValue(UINT uVal)
{
	unsigned		uint_val;
//	char			*pEnumString[32];

	uint_val = uVal;
	m_pIODB ->IOPointWrite(m_iIOHdl,&uint_val);
//	if(m_pIODB ->IOPointGetEnumString(m_iIOHdl,pEnumString)==TRUE){
//		SetWindowText(pEnumString[uint_val]);
	Invalidate();
//	}
	m_bUpdate = TRUE;
/////	old_uint_val = uVal;
}


void CIOEdit::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CString		szEdit('\0',64);
	char		*pEnumString[40];

	char			char_val;
	unsigned char	uchar_val;
	short			short_val;
	unsigned short  ushort_val;
	int				int_val;
	unsigned		uint_val;
	long			long_val;
	unsigned long	ulong_val;
	float			float_val;
	double			double_val;
	char			string_val[128+1];
	char			szPassword[128+1];
	CRect			rect;
	CFont			*p_CurrFont;
	CFont			*p_OldFont;
	IO_SCALE		ioscale;
	char			szDecimalFormat[32 + 2];

	p_CurrFont = GetParent()->GetFont();
	p_OldFont = dc.SelectObject(p_CurrFont);

	GetClientRect(rect);
	rect.DeflateRect(1,1);

	if(IsWindowEnabled()==0){
		dc.SetBkColor(RGB(192,192,192));
		dc.FillSolidRect(rect,RGB(192,192,192));
	}
	else if(m_iSpecialColor != 0){
		if(GetStyle() & ES_READONLY){
			if(m_iSpecialColor == 2){
				dc.SetBkColor(RGB(197,252,243));
				dc.FillSolidRect(rect,RGB(197,252,243));
			}
			else if(m_iSpecialColor == 5){
				dc.SetBkColor(RGB(200,255,255));
				dc.FillSolidRect(rect,RGB(200,255,255));
			}
			else if(m_iSpecialColor == 7){
				dc.SetBkColor(RGB(255,192,192));
				dc.FillSolidRect(rect,RGB(255,192,192));
			}
			else{
				dc.SetBkColor(RGB(233,255,233));
				dc.FillSolidRect(rect,RGB(233,255,233));
	//			dc.SetBkColor(RGB(255,192,255));
	//			dc.FillSolidRect(rect,RGB(255,192,255));
			}
		}
		else if(m_bSelcted == FALSE){
//			dc.SetBkColor(RGB(192,255,192));
//			dc.FillSolidRect(rect,RGB(192,255,192));
			if(m_iSpecialColor == 1){
				dc.SetBkColor(RGB(255,192,255));
				dc.FillSolidRect(rect,RGB(255,192,255));
			}
			else if(m_iSpecialColor == 2){
				dc.SetBkColor(RGB(197,252,243));
				dc.FillSolidRect(rect,RGB(197,252,243));
			}
			else if(m_iSpecialColor == 3){
				dc.SetBkColor(RGB(255,255,192));
				dc.FillSolidRect(rect,RGB(255,255,192));
			}
			else if(m_iSpecialColor == 4){
				dc.SetBkColor(RGB(255,192,100));
				dc.FillSolidRect(rect,RGB(255,192,100));
			}
			else if(m_iSpecialColor == 5){
				dc.SetBkColor(RGB(200,255,255));
				dc.FillSolidRect(rect,RGB(200,255,255));
			}
			else if(m_iSpecialColor == 6){
				dc.SetBkColor(RGB(233,255,233));
				dc.FillSolidRect(rect,RGB(233,255,233));
			}
		}
	}
		
//	dc.TextOut(rect.left,rect.top,szEdit);
	if(m_iIOHdl != INVALID_HDL){
		switch(m_iotype){
		case IO_CHAR:
			m_pIODB ->IOPointRead(m_iIOHdl,&char_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",(int)char_val);
			else
				szEdit.Format("%02,02X",(int)char_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_UCHAR:
			m_pIODB ->IOPointRead(m_iIOHdl,&uchar_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",(unsigned)uchar_val);
			else
				szEdit.Format("%02.02X",(unsigned)uchar_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_SHORT:
			m_pIODB ->IOPointRead(m_iIOHdl,&short_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",short_val);
			else
				szEdit.Format("%04.04X",short_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_USHORT:
			m_pIODB ->IOPointRead(m_iIOHdl,&ushort_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%u",ushort_val);
			else
				szEdit.Format("%04.04X",ushort_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_INT:
			m_pIODB ->IOPointRead(m_iIOHdl,&int_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",int_val);
			else
				szEdit.Format("%04.04X",int_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_UINT:
			m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",uint_val);
			else
				szEdit.Format("%04.04X",uint_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_LONG:
			m_pIODB ->IOPointRead(m_iIOHdl,&long_val);
			szEdit.Format("%d",long_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_ULONG:
			m_pIODB ->IOPointRead(m_iIOHdl,&ulong_val);
			szEdit.Format("%d",ulong_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_FLOAT:
			m_pIODB ->IOPointRead(m_iIOHdl,&float_val);
			strcpy(szDecimalFormat, m_pIODB->IOPointGetDecimalFormat(m_iIOHdl));
			szEdit.Format(szDecimalFormat, float_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_DOUBLE:
			m_pIODB->IOPointRead(m_iIOHdl,&double_val);
			strcpy(szDecimalFormat, m_pIODB->IOPointGetDecimalFormat(m_iIOHdl));
			szEdit.Format(szDecimalFormat, double_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		case IO_DOUBLE_E:
			m_pIODB->IOPointRead(m_iIOHdl, &double_val);
			szEdit.Format("%e", double_val);
			dc.DrawText(szEdit, rect, DT_RIGHT | DT_VCENTER);
			break;
		case IO_ENUM:
		case IO_BIT:
			m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
			if(m_pIODB ->IOPointGetEnumString(m_iIOHdl,pEnumString)==TRUE){
				szEdit = pEnumString[uint_val];
				dc.DrawText(szEdit,rect,DT_LEFT | DT_VCENTER);
			}
			break;
		case IO_STRING:
			m_pIODB ->IOPointRead(m_iIOHdl,string_val);
			if(m_bPassword==TRUE){
				memset(szPassword,'*',64);
				szPassword[strlen(string_val)] = '\0';
				szEdit = szPassword;
			}
			else
				szEdit = string_val;
			dc.DrawText(szEdit,rect,DT_LEFT | DT_VCENTER);
			break;
		case IO_MASK:
			m_pIODB ->IOPointRead(m_iIOHdl,&uint_val);
			if(m_bHexDisplay==FALSE)
				szEdit.Format("%d",uint_val);
			else
				szEdit.Format("%04.04X",uint_val);
			dc.DrawText(szEdit,rect,DT_RIGHT | DT_VCENTER);
			break;
		}/*	switch */
	}
	dc.SelectObject(p_OldFont);
	// 描画用メッセージとして CEdit::OnPaint() を呼び出してはいけません
}

void CIOEdit::SelectField()
{
	CRect rect;

	m_bSelcted = TRUE;

	GetClientRect(rect);
	rect.DeflateRect(1,1);
	InvalidateRect(rect,TRUE);
}

void CIOEdit::UnselectField()
{
	CRect rect;

	m_bSelcted = FALSE;

	GetClientRect(rect);
	rect.DeflateRect(1,1);
	InvalidateRect(rect,TRUE);

}

BOOL CIOEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CRect			rect;

	GetClientRect(rect);

	if(GetStyle() & ES_READONLY){
		if(m_iSpecialColor == 1)
			pDC->FillSolidRect(rect,RGB(255,192,255));
		else if(m_iSpecialColor == 2)
			pDC->FillSolidRect(rect,RGB(197,252,243));
		else if(m_iSpecialColor == 3)
			pDC->FillSolidRect(rect,RGB(255,255,192));
		else if(m_iSpecialColor == 4)
			pDC->FillSolidRect(rect,RGB(255,192,100));
		else if(m_iSpecialColor == 5)
			pDC->FillSolidRect(rect,RGB(200,255,255));
		else if(m_iSpecialColor == 6)
			pDC->FillSolidRect(rect,RGB(233,255,233));
		else if(m_iSpecialColor == 7)
			pDC->FillSolidRect(rect,RGB(255,192,192));
		else
			pDC->FillSolidRect(rect,RGB(192,192,192));

	}
	else{
		rect.DeflateRect(1,1);
	
		if(m_bSelcted)
			pDC->FillSolidRect(rect,RGB(140,200,200));
		else if(m_iSpecialColor == 1)
			pDC->FillSolidRect(rect,RGB(192,255,192));
		else if(m_iSpecialColor == 2)
			pDC->FillSolidRect(rect,RGB(197,252,243));
		else if(m_iSpecialColor == 3)
			pDC->FillSolidRect(rect,RGB(255,255,192));
		else if(m_iSpecialColor == 4)
			pDC->FillSolidRect(rect,RGB(255,192,100));
		else if(m_iSpecialColor == 5)
			pDC->FillSolidRect(rect,RGB(200,255,255));
		else if(m_iSpecialColor == 6)
			pDC->FillSolidRect(rect,RGB(233,255,233));
		else
			pDC->FillSolidRect(rect,RGB(255,255,255));
	}
	return CEdit::OnEraseBkgnd(pDC);
}

void CIOEdit::OnKillfocus() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CRect rect;
	GetClientRect(rect);
	InvalidateRect(rect,TRUE);
	
}

void CIOEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
//	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CIOEdit::SetPasswordFld() 
{
//	ModifyStyle(0,ES_PASSWORD,0);
	m_bPassword = TRUE; 

}


void CIOEdit::OnEnSetfocus()
{
	SetSel(0, 0);
}
