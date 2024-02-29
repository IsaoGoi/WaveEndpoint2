// CDlgManualLogSave.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgManualLogSave.h"


// CDlgManualLogSave ダイアログ

IMPLEMENT_DYNAMIC(CDlgManualLogSave, CDialogX)

static	UINT	m_uidPCBmp[PM_MAX + 1] = { IDB_SMALL_PC1,IDB_SMALL_PC2,IDB_SMALL_PC3,IDB_SMALL_PC4,IDB_SMALL_PC5 };


CDlgManualLogSave::CDlgManualLogSave(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_MANUAL_LOGSAVE, pParent)
{
	m_iPMNo = iPM;
}

CDlgManualLogSave::~CDlgManualLogSave()
{
}

void CDlgManualLogSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_SAMPLINGTIME, m_io_SamplingTime);
	DDX_Control(pDX, IDC_ED_MAXTIME, m_io_SaveMaxTime);
}


BEGIN_MESSAGE_MAP(CDlgManualLogSave, CDialogX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgManualLogSave::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CDlgManualLogSave メッセージ ハンドラー


BOOL CDlgManualLogSave::OnInitDialog()
{
	int		iVal;

	CDialogX::OnInitDialog();

	m_bmpPC.LoadBitmapA(m_uidPCBmp[m_iPMNo]);
	((CStatic*)GetDlgItem(IDC_BMP_PC))->SetBitmap(m_bmpPC);

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	m_pWaveSampling = ((CWaveEndPointApp*)AfxGetApp())->GetWaveSamplingSave();
//	m_pWaveData = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_Eth();


	m_io_SamplingTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.LogSaveCycle"));
	m_io_SaveMaxTime.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.LogSaveMaxTime"));
	m_io_SamplingTime.SetSpecialColor(2);
	m_io_SaveMaxTime.SetSpecialColor(2);

	iVal = m_pWaveSampling->m_tWriteWaveInfo[m_iPMNo].iSampling100msec;
	if (iVal)
		iVal--;
	m_pIODB->IOPointWrite(m_io_SamplingTime.GetIOHdl(), &iVal);
	++iVal;
	iVal *= 600;
	m_pIODB->IOPointWrite(m_io_SaveMaxTime.GetIOHdl(), &iVal);

	m_io_SamplingTime.UpdateDisplay();
	m_io_SaveMaxTime.UpdateDisplay();

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgManualLogSave::OnOK()
{
	int		iVal;

	m_pIODB->IOPointRead(m_io_SamplingTime.GetIOHdl(), &iVal);
	++iVal;

	// 2秒以上
	if (iVal > 10) 
		iVal = (iVal - 9) * 10;

	m_pWaveSampling->StartWaveSampling(m_iPMNo, iVal);
}


void CDlgManualLogSave::OnTimer(UINT_PTR nIDEvent)
{
	int		iVal;

	if(m_pWaveSampling->m_iPCSamplingStatus[m_iPMNo] == SAMPLING_IDLE){
		if (GetDlgItem(IDOK)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
	else{
		if (GetDlgItem(IDOK)->IsWindowEnabled() == TRUE)
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		if (GetDlgItem(IDC_BTN_STOP)->IsWindowEnabled() == FALSE)
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	}

	if (m_io_SamplingTime.m_bUpdate) {
		m_pIODB->IOPointRead(m_io_SamplingTime.GetIOHdl(), &iVal);
		++iVal;
		if (iVal > 10)
			iVal = (iVal - 9) * 10;
		iVal *= 600;
		m_pIODB->IOPointWrite(m_io_SaveMaxTime.GetIOHdl(), &iVal);
		m_io_SaveMaxTime.UpdateDisplay();
	}

	CDialogX::OnTimer(nIDEvent);
}


void CDlgManualLogSave::OnBnClickedBtnStop()
{
	m_pWaveSampling->EndWaveSampling(m_iPMNo);
}
