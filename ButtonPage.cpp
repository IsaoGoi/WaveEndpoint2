#include "pch.h"
#include "WaveEndPoint.h"
#include "ButtonPage.h"
#include "MainWnd.h"
#include "WaveWnd.h"
#include "DlgConfig.h"
#include "DlgPCMonitor.h"
#include "CDlgAnalysis.h"
#include "CDlgSimulation.h"
#include "CDlgRecipeChk.h"
#include "CDlgRcpEdit.h"
#include "CDlgEPDLog.h"
#include "DlgAlarmLog.h"

static int	ID_Button_Bitmap[BUTTON_MAX+1][BITMAP_MAX] = {
	/*			Active				Sel					Focus				Disable		*/
	/*----------------------------------------------------------------------------------
	/* 0:DISPLAY_MAIN		*/ {IDB_MAIN_A,		IDB_MAIN_P,		IDB_MAIN_A,		IDB_MAIN_D},
	/* 1:DISPLAY_RECIPE		*/ {IDB_RECIPE_A,	IDB_RECIPE_P,	IDB_RECIPE_A,	IDB_RECIPE_D},
	/* 2:DISPLAY_LOG		*/ {IDB_LOG_A,		IDB_LOG_P,		IDB_LOG_A,		IDB_LOG_D},
	/* 3:DISPLAY_WAVE		*/ {IDB_WAVE_A,		IDB_WAVE_P,		IDB_WAVE_A,		IDB_WAVE_D},
	/* 4:DISPLAY_ANALYSIS	*/ {IDB_ANALYSIS_A,	IDB_ANALYSIS_P,	IDB_ANALYSIS_A,	IDB_ANALYSIS_D},
	/* 5:DISPLAY_CONFIG		*/ {IDB_CONFIG_A,	IDB_CONFIG_P,	IDB_CONFIG_A,	IDB_CONFIG_D},
	/* 6:DISPLAY_EXIT		*/ {IDB_EXIT_A,		IDB_EXIT_P,		IDB_EXIT_A,		IDB_EXIT_D},
	/* 7					*/ {0,0,0,0},// Stopper
};
static ACCESSLEVEL ID_Button_AccessLevel[BUTTON_MAX] = {
	/* 0:ﾒｲﾝ画面		*/	NONELEVEL,
	/* 1:Recipe画面		*/	OPERATOR,
	/* 2:Log画面		*/	OPERATOR,
	/* 3:WaveMon画面	*/	ENGINEER,
	/* 4:Analysis画面	*/	ENGINEER,
	/* 5:Config画面		*/	ENGINEER,
	/* 6:終了			*/	NONELEVEL,
};


CButtonPage::CButtonPage(CWnd* pParent, CRect& rc, DWORD dwStyle)
{
	DWORD	dwExStyle;

	/*
	** 選択機能ｸﾘｱ
	*/
	m_iSelScreenNo = -1;
	m_nSelectedItem = -1;
	m_pCurrFuncWnd = NULL;
	m_pCommandPage = NULL;
	/*
	** Viewﾎﾟｲﾝﾀ待避
	*/
	m_pView = (CChildView *)pParent;

	CString szClassName = ::AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_3DFACE));

	// ウインドウの作成
	dwExStyle = WS_EX_WINDOWEDGE;
	CreateEx(dwExStyle,(const char*)szClassName, "", dwStyle, rc, pParent, 0);


}

CButtonPage::~CButtonPage()
{
	if (m_pCurrFuncWnd)
		delete m_pCurrFuncWnd;

	for (int i = 0; ID_Button_Bitmap[i][0]; i++)
		delete m_buttons[i];
}
BEGIN_MESSAGE_MAP(CButtonPage, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()




void CButtonPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
					   // 描画メッセージで CWnd::OnPaint() を呼び出さないでください。
	CRect	rc;


	GetClientRect(rc);

	dc.FillSolidRect(rc, COLOR_WND_BASE);
}


int CButtonPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int		i;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	** ボタンの作成
	*/
	CRect	rcWindow(15, 10, 105, 100);

	for (i = 0; ID_Button_Bitmap[i][0]; i++) {

		m_buttons[i] = new CCmdButton();
		if (!i)
			m_buttons[i]->Create(NULL, WS_CHILD | WS_VISIBLE | WS_GROUP | BS_PUSHBUTTON | BS_OWNERDRAW, rcWindow, this, 0);
		else {
			rcWindow.OffsetRect(100, 0);
			m_buttons[i]->Create(NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, rcWindow, this, 0);
		}
		/*
		** ﾋﾞｯﾄﾏｯﾌﾟの設定
		*/
		m_buttons[i]->LoadBitmaps(ID_Button_Bitmap[i][0],
			ID_Button_Bitmap[i][1],
			ID_Button_Bitmap[i][2],
			ID_Button_Bitmap[i][3]);
		#if 0
		if (i == DISPLAY_MODULE) {
			if (pConfig->m_pConfigEQ->iEEQ_PM1Type)
				m_iSelectNo[i] = 0;
			else if (pConfig->m_pConfigEQ->iEEQ_PM2Type)
				m_iSelectNo[i] = 1;
			else if (pConfig->m_pConfigEQ->iEEQ_PM3Type)
				m_iSelectNo[i] = 2;
			else
				m_iSelectNo[i] = 3;
		}
		else
			m_iSelectNo[i] = 0;
#endif
	}/* for */

	for(i=0;i< BUTTON_MAX;i++)
		m_iSelectNo[i]=0;

	// Main Display 
	ChangeFunction(DISPLAY_MAIN, 0);
	DisplayUpdate();

	SetTimer(1,1000,NULL);

	return 0;
}
void CButtonPage::ChkMouseMove(CPoint point)
{
	CRect	rect;
	CRect	rc;
	int		iSelect = -1;
	CString	DbgStr;
	int		iStart_x, iEnd_y;

//	DbgStr.Format("\nMouseMove (%d,%d)", point.x, point.y);
//	TRACE(DbgStr);

	for (int i = 0; ID_Button_Bitmap[i][0]; i++) {
		m_buttons[i]->GetWindowRect(rect);

		if ((rect.top <= point.y && rect.bottom >= point.y) &&
			(rect.left <= point.x && rect.right >= point.x)) {
			iSelect = i;
			break;
		}
	}/* for */

	if (iSelect == -1) {
		if (m_pCommandPage) {
			m_pCommandPage->GetClientRect(rc);
			if (rc.top >= point.y || rc.bottom <= point.y || rc.left >= point.x || rc.right <= point.x) {
				m_pCommandPage->SendMessage(WM_CLOSE);
				delete m_pCommandPage;
				m_pCommandPage = NULL;
				//				m_nSelectedItem = -1;
			}
		}
		m_nSelectedItem = -1;
	}
	else if (m_nSelectedItem != iSelect) {
		if (m_pCommandPage) {
			m_pCommandPage->SendMessage(WM_CLOSE);
			delete m_pCommandPage;
			m_pCommandPage = NULL;
		}
		if (iSelect != DISPLAY_MAIN && iSelect != DISPLAY_RECIPE && iSelect != DISPLAY_CONFIG && iSelect != DISPLAY_EXIT) {
			GetWindowRect(rc);
			iStart_x = rect.left;
			iEnd_y = rc.top;

			m_pCommandPage = new CDlgCommadPage(this, iSelect, iStart_x, iEnd_y);
		}
		m_nSelectedItem = iSelect;
	}
	return;
}

BOOL CButtonPage::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// 0:メイン画面
	if (m_buttons[DISPLAY_MAIN]->m_hWnd == (HWND)lParam) {
		ChangeFunction(DISPLAY_MAIN, 0);
	}
	// 1:Recipe画面
	else if (m_buttons[DISPLAY_RECIPE]->m_hWnd == (HWND)lParam)
		ChangeFunction(DISPLAY_RECIPE, m_iSelectNo[DISPLAY_RECIPE]);

	// 2:Log画面
	else if (m_buttons[DISPLAY_LOG]->m_hWnd == (HWND)lParam)
		ChangeFunction(DISPLAY_LOG, m_iSelectNo[DISPLAY_LOG]);

	// 3:Wave画面
	else if (m_buttons[DISPLAY_WAVE]->m_hWnd == (HWND)lParam) {
		ChangeFunction(DISPLAY_WAVE, 0);
	}
	// 4:Analisys画面
	else if (m_buttons[DISPLAY_ANALYSIS]->m_hWnd == (HWND)lParam) {
		ChangeFunction(DISPLAY_ANALYSIS, m_iSelectNo[DISPLAY_ANALYSIS]);
	}
	// 5:装置環境設定画面
	else if (m_buttons[DISPLAY_CONFIG]->m_hWnd == (HWND)lParam) {
		ChangeFunction(DISPLAY_CONFIG, m_iSelectNo[DISPLAY_CONFIG]);
	}
	// 6:終了
	else if (m_buttons[DISPLAY_EXIT]->m_hWnd == (HWND)lParam){
		::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_CLOSE, NULL, NULL);

//		if (m_pCurrFuncWnd)
//			delete m_pCurrFuncWnd;
	}
	return CWnd::OnCommand(wParam, lParam);
}
void CButtonPage::ChangeFunction(int iFuncNo, int iSubFuncNo)
{
	if (m_iSelScreenNo == iFuncNo && m_iSelectNo[iFuncNo]== iSubFuncNo) {
		m_buttons[m_iSelScreenNo]->SetState(TRUE);
		return;
	}
	/*
	** 現在表示中選択中のボタンをOff
	*/
	if (m_iSelScreenNo != -1)
		m_buttons[m_iSelScreenNo]->SetState(FALSE);


	CDlgEnumEdit	*pDlgEnumEdit = ((CWaveEndPointApp *)AfxGetApp())->m_pEnumEditPage;
	CDlgNumEdit		*pDlgNumEdit = ((CWaveEndPointApp*)AfxGetApp())->m_pNumEditPage;
	CDlgStringEdit* pDlgStringEdit = ((CWaveEndPointApp*)AfxGetApp())->m_pStringEditPage;

	if (pDlgEnumEdit) {
		if (::IsWindow(pDlgEnumEdit->m_hWnd) != 0) {
			if (pDlgEnumEdit->IsWindowVisible() != 0)
				pDlgEnumEdit->SendMessage(WM_CLOSE);
		}
	}
	if (pDlgNumEdit) {
		if (::IsWindow(pDlgNumEdit->m_hWnd) != 0) {
			if (pDlgNumEdit->IsWindowVisible() != 0)
				pDlgNumEdit->SendMessage(WM_CLOSE);
		}
	}
	if (pDlgStringEdit) {
		if (::IsWindow(pDlgStringEdit->m_hWnd) != 0) {
			if (pDlgStringEdit->IsWindowVisible() != 0)
				pDlgStringEdit->SendMessage(WM_CLOSE);
		}
	}


	/*
	** Modify Check
	*/
	switch (m_iSelScreenNo) {
	case DISPLAY_MAIN:
		m_pMainWnd->SendMessage(WM_CLOSE);
		delete m_pMainWnd;
		m_pMainWnd = NULL;
		break;
	case DISPLAY_RECIPE:
		m_pRcpEdit->ModifyCheck();
		m_pRcpEdit->SendMessage(WM_CLOSE);
		delete m_pRcpEdit;
		m_pRcpEdit = NULL;
		break;

	case DISPLAY_LOG:
		if (m_iSelectNo[DISPLAY_LOG] == 0) {
			m_pEPDLog->SendMessage(WM_CLOSE);
			delete m_pEPDLog;
			m_pEPDLog = NULL;
		}
		else {
			m_pAlarmLog->SendMessage(WM_CLOSE);
			delete m_pAlarmLog;
			m_pAlarmLog = NULL;
		}

		break;
	case DISPLAY_WAVE:
		if (m_iSelectNo[DISPLAY_WAVE] == 0) {
			m_pWaveWnd->SendMessage(WM_CLOSE);
			delete m_pWaveWnd;
			m_pWaveWnd = NULL;
		}
		else {
			m_pPCMonitor->SendMessage(WM_CLOSE);
			delete m_pPCMonitor;
			m_pPCMonitor = NULL;
		}

		break;
	case DISPLAY_ANALYSIS:
		if (m_iSelectNo[DISPLAY_ANALYSIS] == 0) {
			m_pAnalysis->SendMessage(WM_CLOSE);
			delete m_pAnalysis;
			m_pAnalysis = NULL;
		}
		else if (m_iSelectNo[DISPLAY_ANALYSIS] == 1) {
			m_pSimulation->SendMessage(WM_CLOSE);
			delete m_pSimulation;
			m_pSimulation = NULL;
		}
		else if (m_iSelectNo[DISPLAY_ANALYSIS] == 2) {
			m_pRecipeChk->SendMessage(WM_CLOSE);
			delete m_pRecipeChk;
			m_pRecipeChk = NULL;
		}
		break;

	case DISPLAY_CONFIG:
		m_pConfigWnd->ModifyCheck();
		m_pConfigWnd->SendMessage(WM_CLOSE);
		delete m_pConfigWnd;
		m_pConfigWnd = NULL;
		break;
	}
	/*
	** New Screen
	*/
	CRect	rc;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_DLGFRAME;

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(10, 10);
	rc.left += HEAD_WINDOW_WIDTH + 5;
	rc.bottom -= 115;

	switch (iFuncNo) {
	case DISPLAY_MAIN:
		m_pMainWnd = new CMainWnd(GetParent(), rc, dwStyle);
		m_pCurrFuncWnd = m_pMainWnd;
		break;
	case DISPLAY_RECIPE:
		m_pRcpEdit = new CDlgRcpEdit(GetParent());
		m_pCurrFuncWnd = m_pRcpEdit;
		m_iSelectNo[DISPLAY_RECIPE] = iSubFuncNo;
		break;
	case DISPLAY_LOG:
		if (iSubFuncNo == 0) {
			m_pEPDLog = new CDlgEPDLog(GetParent());
			m_pCurrFuncWnd = m_pEPDLog;
			m_iSelectNo[DISPLAY_LOG] = iSubFuncNo;
		}
		else {
			m_pAlarmLog	= new CDlgAlarmLog(GetParent());
			m_pCurrFuncWnd = m_pAlarmLog;
			m_iSelectNo[DISPLAY_LOG] = iSubFuncNo;

		}
		break;
	case DISPLAY_WAVE:
		if (iSubFuncNo == 0) {
			m_pWaveWnd = new CWaveWnd(GetParent(), rc, dwStyle);
			m_pCurrFuncWnd = m_pWaveWnd;
			m_iSelectNo[DISPLAY_WAVE] = iSubFuncNo;
		}
		else{
			m_pPCMonitor = new CDlgPCMonitor(GetParent(),(iSubFuncNo-1));
			m_pCurrFuncWnd = m_pPCMonitor;
			m_iSelectNo[DISPLAY_WAVE] = iSubFuncNo;
		}
		break;
	case DISPLAY_ANALYSIS:
		if (iSubFuncNo == 0) {
			m_pAnalysis = new CDlgAnalysis(GetParent());
			m_pCurrFuncWnd = m_pAnalysis;
			m_iSelectNo[DISPLAY_ANALYSIS] = iSubFuncNo;
		}
		else if (iSubFuncNo == 1) {
			m_pSimulation = new CDlgSimulation(GetParent());
			m_pCurrFuncWnd = m_pSimulation;
			m_iSelectNo[DISPLAY_ANALYSIS] = iSubFuncNo;
		}
		else if (iSubFuncNo == 2) {
			m_pRecipeChk = new CDlgRecipeChk(GetParent());
			m_pCurrFuncWnd = m_pRecipeChk;
			m_iSelectNo[DISPLAY_ANALYSIS] = iSubFuncNo;
		}
		break;

	case DISPLAY_CONFIG:
		m_pConfigWnd = new CDlgConfig(GetParent(), rc);
		m_pCurrFuncWnd = (CWnd *)m_pConfigWnd;
		break;
	}
	m_iSelScreenNo = iFuncNo;
	m_buttons[m_iSelScreenNo]->SetState(TRUE);


	if (m_pCommandPage) {
		m_pCommandPage->SendMessage(WM_CLOSE);
		delete m_pCommandPage;
		m_pCommandPage = NULL;
		m_nSelectedItem = -1;
	}

}
LRESULT CButtonPage::OnNcHitTest(CPoint point)
{
	CRect	rect;

	if (m_pCommandPage) {
		m_buttons[0]->GetWindowRect(rect);

		if (rect.top < point.y) {
			m_pCommandPage->SendMessage(WM_CLOSE);
			delete m_pCommandPage;
			m_pCommandPage = NULL;
			m_nSelectedItem = -1;
		}
	}
	return CWnd::OnNcHitTest(point);
}





	void CButtonPage::OnTimer(UINT_PTR nIDEvent)
	{
		POINT	point;
		CRect	btn_rc;
		int		bDelete = TRUE;


		if (m_pCommandPage) {
			if (::GetCursorPos(&point)) {

				GetWindowRect(btn_rc);

				if ((btn_rc.top <= point.y && btn_rc.bottom >= point.y) &&
					(btn_rc.left <= point.x && btn_rc.right >= point.x)) {
					bDelete = FALSE;
				}
				if (bDelete == TRUE) {
					m_pCommandPage->GetWindowRect(btn_rc);
					if (btn_rc.top >= point.y || btn_rc.bottom <= point.y || btn_rc.left >= point.x || btn_rc.right <= point.x) {
						m_pCommandPage->SendMessage(WM_CLOSE);
						delete m_pCommandPage;
						m_pCommandPage = NULL;
						m_nSelectedItem = -1;
					}
				}
			}
		}
		CWnd::OnTimer(nIDEvent);
	}
	void CButtonPage::ChangeMainScreen()
	{
		GetParent()->SetFocus();

		ChangeFunction(DISPLAY_MAIN, 0);
		DisplayUpdate();

	}
	void CButtonPage::DisplayUpdate()
	{
		int		iScreenAccess[10 + 1];
		int		i;

		CAccessLevel* pAccessLevel = ((CWaveEndPointApp*)AfxGetApp())->GetAccessLevelClass();
		ACCESSLEVEL	level = pAccessLevel->GetCurrLevel();
		for (int i = 0; i < DISPLAY_EXIT; i++) {
			if (ID_Button_AccessLevel[i] <= level)
				m_buttons[i]->EnableWindow(TRUE);
			else
				m_buttons[i]->EnableWindow(FALSE);
		}/* for */
	}
