// DldRowWaveMon.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DldRowWaveMon.h"
#include "CDlgManualLogSave.h"


// CDldRowWaveMon ダイアログ

IMPLEMENT_DYNAMIC(CDldRowWaveMon, CDialogX)

static	UINT	m_uidLevel[4] = { IDC_ED_LEVEL1 ,IDC_ED_LEVEL2 ,IDC_ED_LEVEL3 ,IDC_ED_LEVEL4 };
static	UINT	m_uidPCName[PM_MAX] ={  IDB_SMALL_PC1 , IDB_SMALL_PC2, IDB_SMALL_PC3, IDB_SMALL_PC4, IDB_SMALL_PC5};
static	double	m_dDataMax[PM_MAX + 1] = { 1024.0f,1024.0f ,1024.0f ,1024.0f ,1024.0f };


CDldRowWaveMon::CDldRowWaveMon(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_ROWWAVEMON, pParent)
{
	m_iPMNo = iPM;

	SetBackColor(255, 192, 255);
	CDialogX::Create(IDD_ROWWAVEMON, pParent);

}

CDldRowWaveMon::~CDldRowWaveMon()
{
}

void CDldRowWaveMon::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP_PC, m_bmpPCName);
}


BEGIN_MESSAGE_MAP(CDldRowWaveMon, CDialogX)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDldRowWaveMon::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CDldRowWaveMon メッセージ ハンドラー


BOOL CDldRowWaveMon::OnInitDialog()
{
	CRect		rc;
	int			iHight;
	int			iWidth;
	int			i;
	CString		szStr('\0', 16);


	CDialogX::OnInitDialog();
	
	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	m_pWaveData_Eth = NULL;
	m_pWaveData_USB = NULL;

	if(m_pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
		m_pWaveData_Eth = ((CWaveEndPointApp *)AfxGetApp())->GetWaveData_Eth();
	else
		m_pWaveData_USB = ((CWaveEndPointApp*)AfxGetApp())->GetWaveData_USB();

	m_pWaveSampling = ((CWaveEndPointApp*)AfxGetApp())->GetWaveSamplingSave();

	m_pBmpPC.LoadBitmapA(m_uidPCName[m_iPMNo]);
	m_bmpPCName.SetBitmap((HBITMAP)m_pBmpPC.m_hObject);

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(5, 5);
	m_Rect_Wnd = rc;
	iHight = rc.Height();
	iWidth = rc.Width();
	m_Rect_Wnd.top += (iHight / 3)*(m_iPMNo%3);
	m_Rect_Wnd.bottom = m_Rect_Wnd.top + (iHight / 3);
	if (m_iPMNo >= PM4)
		m_Rect_Wnd.left += (iWidth / 2);

	m_Rect_Wnd.right = m_Rect_Wnd.left + (iWidth / 2);

	MoveWindow(m_Rect_Wnd);

	GetClientRect(m_Rect_Graph);
	m_Rect_Graph.DeflateRect(10, 5);
	m_Rect_Graph.left += 30;
	m_Rect_Graph.bottom -= 30;

	GetDlgItem(IDC_DRAW_RECT)->MoveWindow(m_Rect_Graph);
	GetDlgItem(IDC_DRAW_RECT)->ShowWindow(SW_HIDE);

	m_iHeight = m_Rect_Graph.Height();
	m_iWidth = m_Rect_Graph.Width();


	rc.left = 5;
	rc.right = 32;
	rc.top = m_Rect_Graph.bottom-8;
	rc.bottom = rc.top + 18;

	for (i = 1; i < 5; i++) {

		rc.OffsetRect(0, (m_iHeight / 4) * -1);
		GetDlgItem(m_uidLevel[i - 1])->MoveWindow(rc);

		if(m_dDataMax[m_iPMNo]>=4096)
			szStr.Format("%2.2dK   ", ((int)m_dDataMax[m_iPMNo]/1024)/4 * i);
		else
			szStr.Format("%4.4d", (int)m_dDataMax[m_iPMNo] / 4 * i);
		GetDlgItem(m_uidLevel[i - 1])->SetWindowTextA(szStr);
	}
	if (m_pWaveSampling->m_iPCSamplingStatus[m_iPMNo] == SAMPLING_IDLE) {
		m_bSamplingStart = FALSE;
		GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Start..."));
	}
	else {
		m_bSamplingStart = TRUE;
		GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Stop"));
	}
	SetTimer(1, 400, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDldRowWaveMon::OnTimer(UINT_PTR nIDEvent)
{

	InvalidateRect(m_Rect_Graph,FALSE);
//	Invalidate();

	if (m_pWaveSampling->m_iPCSamplingStatus[m_iPMNo] == SAMPLING_IDLE) {
		if (m_bSamplingStart == TRUE) {
			m_bSamplingStart = FALSE;
			GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Start..."));
		}
	}
	else {
		if (m_bSamplingStart == FALSE) {
			m_bSamplingStart = TRUE;
			GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Stop"));
		}
	}


	CDialogX::OnTimer(nIDEvent);
}

void CDldRowWaveMon::OnPaint()
{
	int		i,j;
	POINT	LinePoint[2];
	double	dWidth;
	double	dHeight;
	double	dVal;
	CRect	rc;
	double	dMaxVal=1000.0f;
	CString	szStr;


		/* Pen ... 点線/緑 */
	CPen	DotPen;
	CPen	GraphPen;
	CBrush	GraphBrush;

	DotPen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));
	GraphPen.CreatePen(PS_SOLID, 2, RGB(255,0, 0));
	GraphBrush.CreateSolidBrush(RGB(255, 240, 240));


	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
//	dc.Draw3dRect(m_Rect_Graph, RGB(128,128,255), RGB(255, 192, 192));
	dc.FillSolidRect(m_Rect_Graph, RGB(200, 250, 250));

	CPen* oldPen = dc.SelectObject(&DotPen);

	LinePoint[0].x = m_Rect_Graph.left;
	LinePoint[1].x = m_Rect_Graph.right;

	for (i = 1; i < 4; i++) {
		LinePoint[0].y = m_Rect_Graph.bottom - (m_iHeight / 4 * i);
		LinePoint[1].y = LinePoint[0].y;
		dc.Polyline(&LinePoint[0], 2);
	}

	LinePoint[0].y = m_Rect_Graph.top;
	LinePoint[1].y = m_Rect_Graph.bottom;

	for (i = 1; i < 3; i++) {
		LinePoint[0].x = m_Rect_Graph.left + (m_iWidth / 3 * i);
		LinePoint[1].x = LinePoint[0].x;
		dc.Polyline(&LinePoint[0], 2);
	}
	szStr.Format("%5.1f",m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[0]);
	GetDlgItem(IDC_ED_WAVE1)->SetWindowTextA(szStr);
	szStr.Format("%5.1f", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[682]);
	GetDlgItem(IDC_ED_WAVE2)->SetWindowTextA(szStr);
	szStr.Format("%5.1f", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[1364]);
	GetDlgItem(IDC_ED_WAVE3)->SetWindowTextA(szStr);
	szStr.Format("%5.1f", m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[2047]);
	GetDlgItem(IDC_ED_WAVE4)->SetWindowTextA(szStr);


	// Wave 表示
	dWidth = (double)m_iWidth;
	dWidth /= (double)(EPD_WAVE_TOTAL+2);

	dHeight = (double)m_iHeight;

	dc.SelectObject(&GraphPen);
	CBrush* oldBrush = dc.SelectObject(&GraphBrush);

	for (i = 0; i < EPD_WAVE_TOTAL; i++) {
		if (i == 0) {
			m_pGraph[i].x = m_Rect_Graph.left;
			m_pGraph[i].y = m_Rect_Graph.bottom-1;
			continue;
		}
		m_pGraph[i].x = m_Rect_Graph.left + (long)((double)(i+1) * dWidth);

		if (m_pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
			dVal = (double)m_pWaveData_Eth->m_pWaveData[m_iPMNo]->usWaveData[0][i];
		else
			dVal = (double)m_pWaveData_USB->m_pWaveData[m_iPMNo]->usWaveData[0][i];
		if (i == 1)
			dMaxVal = dVal;
		else if (dMaxVal <= dVal)
			dMaxVal = dVal;

		if (m_dDataMax[m_iPMNo] < dVal) {
			dVal = m_dDataMax[m_iPMNo];

			for (j = 1; j < 5; j++) {

				if (m_dDataMax[m_iPMNo] >= 4096)
					szStr.Format("%2.2dK  ", ((int)m_dDataMax[m_iPMNo] / 1024) / 4 * j);
				else
					szStr.Format("%4.4d    ", (int)m_dDataMax[m_iPMNo] / 4 * j);
				GetDlgItem(m_uidLevel[j - 1])->SetWindowTextA(szStr);
			}

		}
		dVal /= m_dDataMax[m_iPMNo];
		dVal *= dHeight;
		m_pGraph[i].y = m_Rect_Graph.bottom - (long)(dVal);

#if 0
		rc.left = m_pGraph[i].x - 2;
		rc.right = m_pGraph[i].x + 2;
		rc.top = m_pGraph[i].y - 2;
		rc.bottom = m_pGraph[i].y + 2;
		if (rc.bottom > m_Rect_Graph.bottom)
			rc.bottom = m_Rect_Graph.bottom;
		if (rc.top < m_Rect_Graph.top)
			rc.top = m_Rect_Graph.top;

		dc.FillSolidRect(rc, RGB(255, 0, 0));
#endif
	}/* for */
	if (m_dDataMax[m_iPMNo] < dMaxVal)
		m_dDataMax[m_iPMNo] = ((int)(dMaxVal / 1024.0f) + 4) * 1024;
	else if ((m_dDataMax[m_iPMNo]-1024.0f) > dMaxVal)
		m_dDataMax[m_iPMNo] = ((int)(dMaxVal / 1024.0f) + 1) * 1024;

	m_pGraph[i].x = m_pGraph[i-1].x;
	m_pGraph[i].y = m_Rect_Graph.bottom - 1;

//	dc.Polygon(m_pGraph, (EPD_WAVE_TOTAL + 1));
	dc.Polyline (m_pGraph, EPD_WAVE_TOTAL);

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);

	DotPen.DeleteObject();
	GraphPen.DeleteObject();
	GraphBrush.DeleteObject();
}


void CDldRowWaveMon::OnBnClickedBtnSave()
{
	CDlgManualLogSave* pDlg;


	if (m_pWaveSampling->m_iPCSamplingStatus[m_iPMNo] == SAMPLING_IDLE) {
		pDlg = new CDlgManualLogSave(this, m_iPMNo);
		if (pDlg->DoModal() == IDOK) {
			m_bSamplingStart = TRUE;
			GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Stop"));
		}
		delete pDlg;
	}
	else {
		m_pWaveSampling->EndWaveSampling(m_iPMNo);
		m_bSamplingStart = FALSE;
		GetDlgItem(IDC_BTN_SAVE)->SetWindowTextA(_T("Start..."));
	}

}
