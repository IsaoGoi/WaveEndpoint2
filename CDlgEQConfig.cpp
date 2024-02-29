// CDlgEQConfig.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgEQConfig.h"


// CDlgEQConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgEQConfig, CDialogX)

static	UINT	m_uidPC[PM_MAX] = {IDC_ED_PC1,IDC_ED_PC2, IDC_ED_PC3, IDC_ED_PC4, IDC_ED_PC5 };




CDlgEQConfig::CDlgEQConfig(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_EQCONFIG, pParent)
{
	SetBackColor(180, 255, 180);
	CDialogX::Create(IDD_EQCONFIG, pParent);
}

CDlgEQConfig::~CDlgEQConfig()
{
}

void CDlgEQConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PC1, m_io_PC1);
	DDX_Control(pDX, IDC_ED_PC2, m_io_PC2);
	DDX_Control(pDX, IDC_ED_PC3, m_io_PC3);
	DDX_Control(pDX, IDC_ED_PC4, m_io_PC4);
	DDX_Control(pDX, IDC_ED_PC5, m_io_PC5);
	DDX_Control(pDX, IDC_ED_LOGCYCLE, m_io_LogSaveCycle);
	DDX_Control(pDX, IDC_ED_WAVECNT, m_io_WaveCnt);
	DDX_Control(pDX, IDC_ED_DRIVERTYPE, m_io_Driver);
}


BEGIN_MESSAGE_MAP(CDlgEQConfig, CDialogX)
END_MESSAGE_MAP()


// CDlgEQConfig メッセージ ハンドラー


BOOL CDlgEQConfig::OnInitDialog()
{
	CRect		rc;
	CString		szIOName('\0', 32);
	int			i;

	CDialogX::OnInitDialog();

	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(160, 30);
	rc.OffsetRect(140, 0);
	MoveWindow(rc);

	for (i = 0; i < PM_MAX; i++) {
		szIOName.Format("CTC.PC%d.Enable", (i + 1));
		((CIOEdit*)GetDlgItem(m_uidPC[i]))->SetIOHdl(m_pIODB->IOPointGetHandle(szIOName));
		m_pIODB->IOPointWrite(((CIOEdit*)GetDlgItem(m_uidPC[i]))->GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iPCEnable[i]);

		((CIOEdit*)GetDlgItem(m_uidPC[i]))->SetSpecialColor(1);
		((CIOEdit*)GetDlgItem(m_uidPC[i]))->UpdateDisplay();



	}/* for */

	m_io_LogSaveCycle.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.Log.Save.Period"));
	m_pIODB->IOPointWrite(m_io_LogSaveCycle.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iLogSaveCycle);
	m_io_LogSaveCycle.SetSpecialColor(1);
	m_io_LogSaveCycle.UpdateDisplay();

	m_io_WaveCnt.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.WaveLenMax"));
	m_pIODB->IOPointWrite(m_io_WaveCnt.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iWaveMax);
	m_io_WaveCnt.SetSpecialColor(1);
	m_io_WaveCnt.UpdateDisplay();

	m_io_Driver.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.DriverType"));
	m_pIODB->IOPointWrite(m_io_Driver.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.Driver);
	m_io_Driver.SetSpecialColor(1);
	m_io_Driver.UpdateDisplay();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgEQConfig::OnOK()
{
	int		i;

	for (i = 0; i < PM_MAX; i++)
		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(m_uidPC[i]))->GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iPCEnable[i]);

	m_pIODB->IOPointRead(m_io_LogSaveCycle.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iLogSaveCycle);
	m_pIODB->IOPointRead(m_io_WaveCnt.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.iWaveMax);
	m_pIODB->IOPointRead(m_io_Driver.GetIOHdl(), &m_pEPDConfig->m_tEPDConfig.Driver);
	
	m_pEPDConfig->ConfigFileSave();


}
void CDlgEQConfig::ModifyCheck()
{
	int		i;
	int		iVal;
	int		bModify = FALSE;

	for (i = 0; i < PM_MAX; i++) {
		m_pIODB->IOPointRead(((CIOEdit*)GetDlgItem(m_uidPC[i]))->GetIOHdl(), &iVal);
		if (iVal != m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			bModify = TRUE;
			break;
		}
	}/* for */

	m_pIODB->IOPointRead(m_io_LogSaveCycle.GetIOHdl(), &iVal);
	if(iVal != m_pEPDConfig->m_tEPDConfig.iLogSaveCycle)
		bModify = TRUE;

	m_pIODB->IOPointRead(m_io_WaveCnt.GetIOHdl(), &iVal);
	if (iVal != m_pEPDConfig->m_tEPDConfig.iWaveMax)
		bModify = TRUE;

	m_pIODB->IOPointRead(m_io_Driver.GetIOHdl(), &iVal);
	if (iVal != m_pEPDConfig->m_tEPDConfig.Driver)
		bModify = TRUE;

	if (bModify == TRUE) {
		if (MessageBox("Some parameters are changed.\nDo you want to save?", "EQ Conig", MB_YESNO) == IDYES)
			OnOK();
	}
}

