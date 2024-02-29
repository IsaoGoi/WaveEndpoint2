#include "pch.h"
#include "WaveEndPoint.h"
#include "WaveWnd.h"

CWaveWnd::CWaveWnd(CWnd* pParent, CRect& rc, DWORD dwStyle)
{
	int		i;
	DWORD	dwExStyle;
	CString szClassName = ::AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_3DFACE));



	for (i = 0; i < PM_MAX; i++)
		m_pWaveMon[i]=NULL;

	// ウインドウの作成
	dwExStyle = WS_EX_WINDOWEDGE;
	CreateEx(dwExStyle, (const char*)szClassName, "", dwStyle, rc, pParent, 0);

	

}
CWaveWnd::~CWaveWnd()
{
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		if (m_pWaveMon[i]) {
			delete m_pWaveMon[i];
			m_pWaveMon[i] = NULL;
		}
	}
}
BEGIN_MESSAGE_MAP(CWaveWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


int CWaveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int		i;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	// TODO: ここに特定な作成コードを追加してください。
	for (i = 0; i < PM_MAX; i++) {
		if(pConfig->m_tEPDConfig.iPCEnable[i])
			m_pWaveMon[i] = new CDldRowWaveMon(this, PM1 + i);
	}


	return 0;
}


void CWaveWnd::OnClose()
{
	int		i;

	for (i = 0; i < PM_MAX; i++) {
		if (m_pWaveMon[i])
			m_pWaveMon[i]->SendMessage(WM_CLOSE);
	}
	CWnd::OnClose();
}
