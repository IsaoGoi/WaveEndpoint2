// CDlgPCSelect.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgPCSelect.h"
#include "MainWnd.h"


// CDlgPCSelect ダイアログ

IMPLEMENT_DYNAMIC(CDlgPCSelect, CDialogX)

static	UINT	m_uid_PCButton[PM_MAX] = { IDC_BTN_PC1,IDC_BTN_PC2,IDC_BTN_PC3,IDC_BTN_PC4,IDC_BTN_PC5 };


CDlgPCSelect::CDlgPCSelect(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_PC_SELECT, pParent)
{
	CDialogX::Create(IDD_PC_SELECT, pParent);
}

CDlgPCSelect::~CDlgPCSelect()
{

}

void CDlgPCSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPCSelect, CDialogX)
	ON_BN_CLICKED(IDC_BTN_PC1, &CDlgPCSelect::OnBnClickedBtnPc1)
	ON_BN_CLICKED(IDC_BTN_PC2, &CDlgPCSelect::OnBnClickedBtnPc2)
	ON_BN_CLICKED(IDC_BTN_PC3, &CDlgPCSelect::OnBnClickedBtnPc3)
	ON_BN_CLICKED(IDC_BTN_PC4, &CDlgPCSelect::OnBnClickedBtnPc4)
	ON_BN_CLICKED(IDC_BTN_PC5, &CDlgPCSelect::OnBnClickedBtnPc5)
	ON_BN_CLICKED(IDC_BTN_VIEWSORT, &CDlgPCSelect::OnBnClickedBtnViewsort)
END_MESSAGE_MAP()


// CDlgPCSelect メッセージ ハンドラー




BOOL CDlgPCSelect::OnInitDialog()
{
	CEPDConfig* pConfig;
	int			i;

	CDialogX::OnInitDialog();

	pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	
	for (i = 0; i < PM_MAX; i++) {
		if (pConfig->m_tEPDConfig.iPCEnable[i])
			GetDlgItem(m_uid_PCButton[i])->EnableWindow(TRUE);
		else
			GetDlgItem(m_uid_PCButton[i])->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgPCSelect::OnBnClickedBtnPC(int iPC)
{
	CMainWnd* pMainWnd;
	CRect		rc;



	if (GetDlgItem(m_uid_PCButton[iPC])->IsWindowEnabled()) {
		pMainWnd = (CMainWnd*)GetParent();
		if (pMainWnd->m_pDlgPCMonitor[iPC]->IsIconic())
			pMainWnd->m_pDlgPCMonitor[iPC]->ShowWindow(SW_RESTORE);
		else {
			pMainWnd->m_pDlgPCMonitor[iPC]->GetWindowRect(rc);
			pMainWnd->m_pDlgPCMonitor[iPC]->SetWindowPos(NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOSIZE | SWP_SHOWWINDOW);
		}
	}


}

extern CRect		m_rect_Window[PM_MAX];

void CDlgPCSelect::OnBnClickedBtnViewsort()
{
	int		i;
	CRect	rc;
	CRect	new_rc;
	int		iPMCnt;
	CMainWnd* pMainWnd;
	int		iRight, iBottom;


	pMainWnd = (CMainWnd*)GetParent();

	pMainWnd->GetClientRect(rc);
	iPMCnt = 0;

	iRight = rc.right;
	iBottom = rc.bottom;

	for (i = 0; i < PM_MAX; i++) {
		if (pMainWnd->m_pDlgPCMonitor[i]) {

			if (pMainWnd->m_pDlgPCMonitor[i]->IsIconic() == FALSE) {

				pMainWnd->m_pDlgPCMonitor[i]->MoveWindow(m_rect_Window[i]);

#if 0
				pMainWnd->m_pDlgPCMonitor[i]->GetWindowRect(m_rect_Window[i]);
				new_rc = rc;
				new_rc.OffsetRect(180, 0);
				new_rc.right = new_rc.left + m_rect_Window[i].Width();
				new_rc.bottom = new_rc.top + m_rect_Window[i].Height();
				pMainWnd->m_pDlgPCMonitor[i]->MoveWindow(new_rc);
				m_rect_Window[i] = new_rc;

				++iPMCnt;
				if (iPMCnt == 2 && rc.left+ m_rect_Window[i].Width() > iRight) {
					GetClientRect(rc);
					rc.OffsetRect(0, m_rect_Window[i].Height()-20);
				}
				else if (iPMCnt == 3 && rc.left + m_rect_Window[i].Width() > iRight) {
					GetClientRect(rc);
					rc.OffsetRect(0, m_rect_Window[i].Height()-20);
				}
				else {
					rc.OffsetRect(m_rect_Window[i].Width(), 0);
				}

#endif
			}
		}
	}
}
