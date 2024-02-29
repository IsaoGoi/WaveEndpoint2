// CDlgChangeEPDMode.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgChangeEPDMode.h"


// CDlgChangeEPDMode ダイアログ
static UINT	m_uiModeID[PM_MAX] = { IDC_ED_MODE1 ,IDC_ED_MODE2 ,IDC_ED_MODE3 ,IDC_ED_MODE4,IDC_ED_MODE5 };
static UINT	m_uiRemoteBtnID[PM_MAX] = { IDC_BTN_REMOTE1 ,IDC_BTN_REMOTE2,IDC_BTN_REMOTE3,IDC_BTN_REMOTE4,IDC_BTN_REMOTE5 };
static UINT	m_uiLocalBtnID[PM_MAX] = { IDC_BTN_LOCAL1 ,IDC_BTN_LOCAL2,IDC_BTN_LOCAL3,IDC_BTN_LOCAL4,IDC_BTN_LOCAL5 };


IMPLEMENT_DYNAMIC(CDlgChangeEPDMode, CDialogX)

CDlgChangeEPDMode::CDlgChangeEPDMode(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_CHGEPDMODE, pParent)
{

}

CDlgChangeEPDMode::~CDlgChangeEPDMode()
{
}

void CDlgChangeEPDMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_MODE1, m_io_EPDMode1);
	DDX_Control(pDX, IDC_ED_MODE2, m_io_EPDMode2);
	DDX_Control(pDX, IDC_ED_MODE3, m_io_EPDMode3);
	DDX_Control(pDX, IDC_ED_MODE4, m_io_EPDMode4);
	DDX_Control(pDX, IDC_ED_MODE5, m_io_EPDMode5);
}


BEGIN_MESSAGE_MAP(CDlgChangeEPDMode, CDialogX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_REMOTE1, &CDlgChangeEPDMode::OnBnClickedBtnRemote1)
	ON_BN_CLICKED(IDC_BTN_LOCAL1, &CDlgChangeEPDMode::OnBnClickedBtnLocal1)
	ON_BN_CLICKED(IDC_BTN_REMOTE2, &CDlgChangeEPDMode::OnBnClickedBtnRemote2)
	ON_BN_CLICKED(IDC_BTN_LOCAL2, &CDlgChangeEPDMode::OnBnClickedBtnLocal2)
	ON_BN_CLICKED(IDC_BTN_REMOTE3, &CDlgChangeEPDMode::OnBnClickedBtnRemote3)
	ON_BN_CLICKED(IDC_BTN_LOCAL3, &CDlgChangeEPDMode::OnBnClickedBtnLocal3)
	ON_BN_CLICKED(IDC_BTN_REMOTE4, &CDlgChangeEPDMode::OnBnClickedBtnRemote4)
	ON_BN_CLICKED(IDC_BTN_LOCAL4, &CDlgChangeEPDMode::OnBnClickedBtnLocal4)
	ON_BN_CLICKED(IDC_BTN_REMOTE5, &CDlgChangeEPDMode::OnBnClickedBtnRemote5)
	ON_BN_CLICKED(IDC_BTN_LOCAL5, &CDlgChangeEPDMode::OnBnClickedBtnLocal5)
END_MESSAGE_MAP()


// CDlgChangeEPDMode メッセージ ハンドラー

BOOL CDlgChangeEPDMode::OnInitDialog()
{
	unsigned	unsMode;
	CString szStr('\0', 32);
	int			i;

	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	for (i = 0; i < PM_MAX; i++) {
		szStr.Format("PM%d.EPD.Mode", (i + 1));
		((CIOEdit*)GetDlgItem(m_uiModeID[i]))->SetIOHdl(m_pIODB->IOPointGetHandle(szStr));
		((CIOEdit*)GetDlgItem(m_uiModeID[i]))->SetSpecialColor(1);
		((CIOEdit*)GetDlgItem(m_uiModeID[i]))->UpdateDisplay();


		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(m_uiModeID[i]))->GetIOHdl(), &unsMode);
		if (unsMode == 1) {
			GetDlgItem(m_uiRemoteBtnID[i])->EnableWindow(TRUE);
			GetDlgItem(m_uiLocalBtnID[i])->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(m_uiRemoteBtnID[i])->EnableWindow(FALSE);
			GetDlgItem(m_uiLocalBtnID[i])->EnableWindow(TRUE);
		}
	}

	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgChangeEPDMode::OnTimer(UINT_PTR nIDEvent)
{
	unsigned	unsMode;
	int			i;

	for (i = 0; i < PM_MAX; i++) {
		((CIOEdit*)GetDlgItem(m_uiModeID[i]))->ChkValueChange();

		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(m_uiModeID[i]))->GetIOHdl(), &unsMode);
		if (unsMode == 1) {
			if (GetDlgItem(m_uiRemoteBtnID[i])->IsWindowEnabled() == 0)
				GetDlgItem(m_uiRemoteBtnID[i])->EnableWindow(TRUE);
			if (GetDlgItem(m_uiLocalBtnID[i])->IsWindowEnabled())
				GetDlgItem(m_uiLocalBtnID[i])->EnableWindow(FALSE);
		}
		else {
			if (GetDlgItem(m_uiRemoteBtnID[i])->IsWindowEnabled())
				GetDlgItem(m_uiRemoteBtnID[i])->EnableWindow(FALSE);
			if (GetDlgItem(m_uiLocalBtnID[i])->IsWindowEnabled() == 0)
				GetDlgItem(m_uiLocalBtnID[i])->EnableWindow(TRUE);
		}
	}/* for */
	CDialogX::OnTimer(nIDEvent);
}


void CDlgChangeEPDMode::BtnRemote(int iPCNo)
{
	unsigned	unsMode=2;

	m_pIODB->IOPointWrite(((CIOEdit*)GetDlgItem(m_uiModeID[iPCNo]))->GetIOHdl(), &unsMode);
}


void CDlgChangeEPDMode::BtnLocal(int iPCNo)
{
	unsigned	unsMode = 1;

	m_pIODB->IOPointWrite(((CIOEdit*)GetDlgItem(m_uiModeID[iPCNo]))->GetIOHdl(), &unsMode);
}
