// CDlgWaveRef.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgWaveRef.h"


// CDlgWaveRef ダイアログ

IMPLEMENT_DYNAMIC(CDlgWaveRef, CDialogX)

CDlgWaveRef::CDlgWaveRef(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_WAVEREFERENCE, pParent)
{
	SetBackColor(128, 128, 255);
	CDialogX::Create(IDD_WAVEREFERENCE, pParent);

}

CDlgWaveRef::~CDlgWaveRef()
{
}

void CDlgWaveRef::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_START_LEVEL, m_io_RefStartLevel);
	DDX_Control(pDX, IDC_ED_START_TIME, m_io_RefStartTime);
	DDX_Control(pDX, IDC_ED_END_LEVEL, m_io_RefEndLevel);
	DDX_Control(pDX, IDC_ED_END_TIME, m_io_RefEndTime);
	DDX_Control(pDX, IDC_ED_PEAK_LEVEL, m_io_RefPeakLevel);
	DDX_Control(pDX, IDC_ED_PEAK_TIME, m_io_RefPeakTime);
	DDX_Control(pDX, IDC_ED_MIN_LEVEL, m_io_RefMinLevel);
	DDX_Control(pDX, IDC_ED_MIN_TIME, m_io_RefMinTime);
	DDX_Control(pDX, IDC_ED_DIFF_LEVEL, m_io_RefDiffLevel);
}


BEGIN_MESSAGE_MAP(CDlgWaveRef, CDialogX)
END_MESSAGE_MAP()


// CDlgWaveRef メッセージ ハンドラー


BOOL CDlgWaveRef::OnInitDialog()
{
	CDialogX::OnInitDialog();

	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();
	
	m_io_RefStartTime.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.StartTime")));
	m_io_RefStartLevel.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.StartLevel")));
	m_io_RefEndTime.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.EndTime")));
	m_io_RefEndLevel.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.EndLevel")));
	m_io_RefPeakTime.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.PeakTime")));
	m_io_RefPeakLevel.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.PeakLevel")));
	m_io_RefMinTime.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.MinimumTime")));
	m_io_RefMinLevel.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.MinimumLevel")));
	m_io_RefDiffLevel.SetIOHdl(m_pIODB->IOPointGetHandle(_T("CTC.Ref.Difference")));

	m_io_RefStartTime.SetSpecialColor(1);
	m_io_RefStartLevel.SetSpecialColor(1);
	m_io_RefEndTime.SetSpecialColor(1);
	m_io_RefEndLevel.SetSpecialColor(1);
	m_io_RefPeakTime.SetSpecialColor(1);
	m_io_RefPeakLevel.SetSpecialColor(1);
	m_io_RefMinTime.SetSpecialColor(1);
	m_io_RefMinLevel.SetSpecialColor(1);
	m_io_RefDiffLevel.SetSpecialColor(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgWaveRef::ShowDlgWaveRef()
{
	ShowWindow(SW_SHOW);

	m_io_RefStartTime.UpdateDisplay();
	m_io_RefStartLevel.UpdateDisplay();
	m_io_RefEndTime.UpdateDisplay();
	m_io_RefEndLevel.UpdateDisplay();
	m_io_RefPeakTime.UpdateDisplay();
	m_io_RefPeakLevel.UpdateDisplay();
	m_io_RefMinTime.UpdateDisplay();
	m_io_RefMinLevel.UpdateDisplay();
	m_io_RefDiffLevel.UpdateDisplay();

}
void CDlgWaveRef::HideDlgWaveRef()
{
	if(IsWindowVisible())
		ShowWindow(SW_HIDE);
}



void CDlgWaveRef::OnCancel()
{
	ShowWindow(SW_HIDE);
}
