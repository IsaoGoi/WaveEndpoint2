#include "pch.h"
#include "WaveEndPoint.h"
#include "MainWnd.h"

CRect		m_rect_Window[PM_MAX] = { {0,0,0,0},{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
static		int m_iMinimumDisplay[PM_MAX] = { FALSE,FALSE, FALSE, FALSE, FALSE};


CMainWnd::CMainWnd(CWnd* pParent, CRect& rc, DWORD dwStyle)
{
	DWORD	dwExStyle;
	int		i;

	CString szClassName = ::AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_3DFACE));


	for (i = 0; i < PM_MAX; i++)
		m_pDlgPCMonitor[i] = NULL;
	// ウインドウの作成
	m_pDlgPCSelect = NULL;

	dwExStyle = WS_EX_WINDOWEDGE;
	CreateEx(dwExStyle, (const char*)szClassName, "", dwStyle, rc, pParent, 0);

}
CMainWnd::~CMainWnd()
{
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		if (m_pDlgPCMonitor[i])
			delete m_pDlgPCMonitor[i];
	}
	if (m_pDlgPCSelect)
		delete m_pDlgPCSelect;

}
BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CEPDConfig* pConfig;
	int			i,iPM;
	CRect		rc_parents;
	CRect		rc;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	GetWindowRect(rc_parents);
	for (i = iPM = 0; i < PM_MAX; i++) {
		if (pConfig->m_tEPDConfig.iPCEnable[i]) {
			m_pDlgPCMonitor[i] = new CDlgMainMonitor(this, i);

			if (m_rect_Window[i].top == 0 && m_rect_Window[i].bottom == 0) {
				m_pDlgPCMonitor[i]->GetClientRect(rc);
				rc.OffsetRect(rc_parents.left, rc_parents.top);
				if (iPM < 3) {
					rc.OffsetRect((rc.Width()+50) * iPM, 0);
				} 
				else{
					rc.OffsetRect((rc.Width() + 50) * (iPM-3), rc.Height()+30);
				}
//				rc.OffsetRect((rc.Width()/3*i), (rc.Height() /3 *i));
				m_pDlgPCMonitor[i]->MoveWindow(rc);
				m_rect_Window[i] = rc;
				++iPM;
			}
			else
				m_pDlgPCMonitor[i]->MoveWindow(m_rect_Window[i]);

			if (m_iMinimumDisplay[i]) {
				m_pDlgPCMonitor[i]->ShowWindow(SW_MINIMIZE);
			}
		}
	}

	m_pDlgPCSelect = new CDlgPCSelect(this);

	GetClientRect(rc);
	rc.top = rc.bottom - 50;
	rc.bottom = rc.bottom - 3;
	rc.DeflateRect(300, 0);
	m_pDlgPCSelect->MoveWindow(rc);
	return 0;
}


void CMainWnd::OnClose()
{
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		if (m_pDlgPCMonitor[i]){

			m_iMinimumDisplay[i] = m_pDlgPCMonitor[i]->IsIconic();
			if(m_iMinimumDisplay[i]==FALSE)
				m_pDlgPCMonitor[i]->GetWindowRect(m_rect_Window[i]);
			delete m_pDlgPCMonitor[i];
			m_pDlgPCMonitor[i] = NULL;
		}
	}
	if (m_pDlgPCSelect) {
		delete m_pDlgPCSelect;
		m_pDlgPCSelect = NULL;
	}

	CWnd::OnClose();
}


