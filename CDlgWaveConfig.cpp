// CDlgWaveConfig.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgWaveConfig.h"


// CDlgWaveConfig ダイアログ

static UINT	m_uiParamNoID[PAGE_DATA_MAX + 1] = {
	IDC_ED_NO1,  IDC_ED_NO2,  IDC_ED_NO3,  IDC_ED_NO4,  IDC_ED_NO5,  IDC_ED_NO6,  IDC_ED_NO7,  IDC_ED_NO8,  IDC_ED_NO9,  IDC_ED_NO10,
	IDC_ED_NO11, IDC_ED_NO12, IDC_ED_NO13, IDC_ED_NO14, IDC_ED_NO15, IDC_ED_NO16, IDC_ED_NO17, IDC_ED_NO18, IDC_ED_NO19, IDC_ED_NO20,
	IDC_ED_NO21, IDC_ED_NO22, IDC_ED_NO23, IDC_ED_NO24, IDC_ED_NO25,
	IDC_ED_NO26, IDC_ED_NO27, IDC_ED_NO28, IDC_ED_NO29, IDC_ED_NO30, IDC_ED_NO31, IDC_ED_NO32, IDC_ED_NO33, IDC_ED_NO34, IDC_ED_NO35,
	IDC_ED_NO36, IDC_ED_NO37, IDC_ED_NO38, IDC_ED_NO39, IDC_ED_NO40, IDC_ED_NO41, IDC_ED_NO42, IDC_ED_NO43, IDC_ED_NO44, IDC_ED_NO45,
	IDC_ED_NO46, IDC_ED_NO47, IDC_ED_NO48, IDC_ED_NO49, IDC_ED_NO50,
	IDC_ED_NO51, IDC_ED_NO52, IDC_ED_NO53, IDC_ED_NO54, IDC_ED_NO55, IDC_ED_NO56, IDC_ED_NO57, IDC_ED_NO58, IDC_ED_NO59, IDC_ED_NO60,
	IDC_ED_NO61, IDC_ED_NO62, IDC_ED_NO63, IDC_ED_NO64, IDC_ED_NO65, IDC_ED_NO66, IDC_ED_NO67, IDC_ED_NO68, IDC_ED_NO69, IDC_ED_NO70,
	IDC_ED_NO71, IDC_ED_NO72, IDC_ED_NO73, IDC_ED_NO74, IDC_ED_NO75 };

IMPLEMENT_DYNAMIC(CDlgWaveConfig, CDialogX)

CDlgWaveConfig::CDlgWaveConfig(CWnd* pParent /*=nullptr*/,int iPM)
	: CDialogX(IDD_WAVECONFIG, pParent)
{
	m_iPMNo = iPM;
	SetBackColor(180, 255, 180);
	CDialogX::Create(IDD_WAVECONFIG, pParent);
}

CDlgWaveConfig::~CDlgWaveConfig()
{
	int		i;

	for (i = 0; i < PAGE_DATA_MAX; i++) {
		delete m_pEditData[0][i];
		delete m_pEditData[1][i];
	}/* for */

}

void CDlgWaveConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgWaveConfig, CDialogX)
	ON_COMMAND(VK_UP, OnKeyUp)
	ON_COMMAND(VK_DOWN, OnKeyDown)
	ON_COMMAND(VK_LEFT, OnKeyLeft)
	ON_COMMAND(VK_RIGHT, OnKeyRight)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgWaveConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_LOADWAVE, &CDlgWaveConfig::OnBnClickedBtnLoadwave)
	ON_BN_CLICKED(IDC_BTN_CREATE, &CDlgWaveConfig::OnBnClickedBtnCreate)
END_MESSAGE_MAP()


// CDlgWaveConfig メッセージ ハンドラー


BOOL CDlgWaveConfig::OnInitDialog()
{
	int			i;
	CRect		rc;

	CDialogX::OnInitDialog();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(160, 10);
	rc.OffsetRect(140, 0);
	MoveWindow(rc);

	m_pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_iDataMax = m_pConfig->m_tEPDConfig.iWaveMax;
	m_iPageTopNo = 0;

	m_iCurrX = -1;
	m_iCurrY = -1;

	m_brushBack.CreateSolidBrush(RGB(192, 255, 192));
	m_colText = RGB(0, 0, 0);

	for (i = 0; i < PAGE_DATA_MAX; i++) {
		rc.top = 20 + ((i% EDITFLD_Y_MAX) * 33.4f);
		rc.bottom = rc.top + 25;	

		rc.left = 80 + ((i / EDITFLD_Y_MAX)*451);
		rc.right = rc.left + 110;

		m_pEditData[0][i] = new CEditDouble();
		m_pEditData[0][i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rc, this, (i + 1));

		rc.left += 120;
		rc.right = rc.left + 180;
		m_pEditData[1][i] = (CEditDouble*)new CEditString();
		m_pEditData[1][i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rc, this, (i + 1));
	}/* for */

	SetTitle();
	m_tEditWaveData = m_pConfig->m_tWaveConfig2[m_iPMNo];

	SetData();
	UpdateData(FALSE);

	m_iCurrX = 0;
	m_iCurrY = 0;
	m_pEditData[0][0]->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
void CDlgWaveConfig::SetTitle()
{
	int			i;
	CString		szStr('\0', 64);

	m_iFldCnt = 0;

	for (i = 0; i < PAGE_DATA_MAX; i++) {
		if (m_iPageTopNo + i >= m_iDataMax) {
			m_bFldEnable[i] = -1;
			szStr = _T("   ");
			GetDlgItem(m_uiParamNoID[i])->SetWindowTextA(szStr);

			GetDlgItem(m_uiParamNoID[i])->EnableWindow(FALSE);

			m_pEditData[0][i]->EnableWindow(FALSE);
			m_pEditData[1][i]->EnableWindow(FALSE);

			continue;
		}
		m_bFldEnable[i] = i;
		m_pEditData[0][i]->EnableWindow(TRUE);
		m_pEditData[1][i]->EnableWindow(TRUE);
		GetDlgItem(m_uiParamNoID[i])->EnableWindow(TRUE);
		szStr.Format("%d", (m_iPageTopNo + i + 1));
		GetDlgItem(m_uiParamNoID[i])->SetWindowTextA(szStr);
		m_iFldCnt++;
	}/* for */
}
void CDlgWaveConfig::SetData()
{
	int		i;
	CString	szStr('\0', 32);
	double		dMax, dMin;

	dMax = 1000.0f;
	dMin = 150.0f;

	for (i = 0; i < m_iFldCnt; i++) {


		szStr.Format("%12.9f", m_tEditWaveData.fWaveLength[m_iPageTopNo + i]);
		m_pEditData[0][i]->SetWindowTextA(szStr);
		m_pEditData[0][i]->SetScale(dMin, dMax);

		szStr.Format("%s", m_tEditWaveData.szWaveLenName[m_iPageTopNo + i]);
		m_pEditData[1][i]->SetWindowTextA(szStr);
		((CEditString*)m_pEditData[1][i])->SetScale(32);
	}
}
void CDlgWaveConfig::GetData()
{
	int		i;
	CString	szStr('\0', 32);

	for (i = 0; i < m_iFldCnt; i++) {
		m_pEditData[0][i]->GetWindowTextA(szStr);
		m_tEditWaveData.fWaveLength[m_iPageTopNo + i] = (float)atof(szStr);

		m_pEditData[1][i]->GetWindowTextA(szStr);
		strncpy(m_tEditWaveData.szWaveLenName[m_iPageTopNo + i], szStr,32);
	}
}
void CDlgWaveConfig::OnKeyUp()
{
	int	x, y;

	if (GetFldXY(&x, &y) == FALSE)
		return;

	if (y == 0)
		y = m_iFldCnt - 1;
	else
		y--;

	m_iCurrX = x;
	m_iCurrY = y;
	m_pEditData[x][y]->SetFocus();

}
void CDlgWaveConfig::OnKeyDown()
{
	int	x, y;

	if (GetFldXY(&x, &y) == FALSE)
		return;

	if (y == (m_iFldCnt - 1))
		y = 0;
	else
		y++;

	m_iCurrX = x;
	m_iCurrY = y;
	m_pEditData[x][y]->SetFocus();
}
void CDlgWaveConfig::OnKeyLeft()
{
	int	x, y;

	if (GetKeyState(VK_SHIFT) & 0x8000)
		return;

	if (GetFldXY(&x, &y) == FALSE)
		return;

	if (x == 1)
		x = 0;
	else {
		if (y >= EDITFLD_Y_MAX) {
			y -= EDITFLD_Y_MAX;
			x = 1;
		}
		else {
			if (m_iPageTopNo) {
				GetData();
				m_iPageTopNo-= EDITFLD_Y_MAX;
				SetTitle();
				SetData();
			}
			else
				return;
		}
	}
	m_iCurrX = x;
	m_iCurrY = y;
	m_pEditData[x][y]->SetFocus();
}

void CDlgWaveConfig::OnKeyRight()
{
	int	x, y;

	if(GetKeyState(VK_SHIFT) & 0x8000)
		return;


	if (GetFldXY(&x, &y) == FALSE)
		return;

	if (x == 0)
		x = 1;
	else {
		if (y + EDITFLD_Y_MAX < PAGE_DATA_MAX) {
			y += EDITFLD_Y_MAX;
			x = 0;
		}
		else {
			if ((m_iPageTopNo + PAGE_DATA_MAX) < m_iDataMax) {
				GetData();
				m_iPageTopNo += EDITFLD_Y_MAX;
				SetTitle();
				SetData();
				
				if ((m_iPageTopNo + y) >= m_iDataMax)
					y = (m_iDataMax-1) % PAGE_DATA_MAX;
			}

			else
				return;
		}
	}
	m_iCurrX = x;
	m_iCurrY = y;
	m_pEditData[x][y]->SetFocus();
}
int	CDlgWaveConfig::GetFldXY(int* px, int* py)
{
	int		i;
	CWnd* pWnd;

	pWnd = GetFocus();
	if (pWnd) {

		for (i = 0; i < m_iFldCnt; i++) {
			if (pWnd == m_pEditData[0][i]){
				*px = 0;
				*py = i;
				return(TRUE);
			}
			else if (pWnd == m_pEditData[1][i]) {
				*px = 1;
				*py = i;
				return(TRUE);
			}
		}/* for */
	}
	return(FALSE);
}

HBRUSH CDlgWaveConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogX::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if (nCtlColor == CTLCOLOR_EDIT) {
		if (((CEditDouble*)pWnd)->m_bSelect == TRUE) {
			pDC->SetBkMode(OPAQUE);
			pDC->SetTextColor(m_colText);
			pDC->SetBkColor(RGB(192, 255, 192));
			return (HBRUSH)m_brushBack;
		}
		else {
			pDC->SetBkMode(OPAQUE);
			pDC->SetTextColor(m_colText);
			pDC->SetBkColor(RGB(255, 255, 255));
			return hbr;
		}
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CDlgWaveConfig::OnBnClickedOk()
{
	GetData();
	m_pConfig->m_tWaveConfig2[m_iPMNo] = m_tEditWaveData;
	m_pConfig->WaveCfgFileSave();
}
BOOL CDlgWaveConfig::PreTranslateMessage(MSG* pMsg)
{
	int	x, y;

	if (pMsg->message == WM_KEYDOWN) {

		if (GetFldXY(&x, &y) == TRUE) {
			if ((pMsg->wParam == 'C' || pMsg->wParam == 'c') && (GetKeyState(VK_CONTROL) & 0x8000))
				m_pEditData[x][y]->OnEditCopy();
			else if ((pMsg->wParam == 'V' || pMsg->wParam == 'v') && (GetKeyState(VK_CONTROL) & 0x8000))
				m_pEditData[x][y]->OnEditPaste();
			else if ((pMsg->wParam == 'X' || pMsg->wParam == 'x') && (GetKeyState(VK_CONTROL) & 0x8000))
				m_pEditData[x][y]->OnEditCut();
			else if ((pMsg->wParam == 'Z' || pMsg->wParam == 'z') && (GetKeyState(VK_CONTROL) & 0x8000))
				m_pEditData[x][y]->OnEditUndo();
			else if ((pMsg->wParam == VK_TAB) && (GetKeyState(VK_SHIFT) & 0x8000)) {
				OnKeyLeft();
				return(0);
			}
			else if (pMsg->wParam == VK_TAB) {
				OnKeyRight();
				return(0);
			}
			else if (pMsg->wParam == VK_RETURN) {
				OnKeyRight();
				return(0);
			}
		}
	}
	return CDialogX::PreTranslateMessage(pMsg);
}
void	CDlgWaveConfig::ModifyCheck()
{
	int		i;
	int		bChanged = FALSE;

	GetData();

	for (i = 0; i < m_pConfig->m_tEPDConfig.iWaveMax; i++) {
		if (m_pConfig->m_tWaveConfig2[m_iPMNo].fWaveLength[i] != m_tEditWaveData.fWaveLength[i]) {
			bChanged = TRUE;
			break;
		}
		if (strcmp(m_pConfig->m_tWaveConfig2[m_iPMNo].szWaveLenName[i],m_tEditWaveData.szWaveLenName[i])) {
			bChanged = TRUE;
			break;
		}
	}
	if (bChanged) {
		if (MessageBox("Some parameters are changed.\nDo you want to save?", "EQ Conig", MB_YESNO) == IDYES) {
			m_pConfig->m_tWaveConfig2[m_iPMNo] = m_tEditWaveData;
			m_pConfig->WaveCfgFileSave();
		}
	}
}

void CDlgWaveConfig::OnBnClickedBtnLoadwave()
{
	CStdioFile fl;
	int			i;
	CString	szStr('\0',32);
	char		szBuff[256];
	char		szName[65];
	char*		p;


	if (!fl.Open("WaveLength.csv", CFile::modeRead | CFile::typeText)) {
		MessageBox("File Not Found!(WaveLength.txt)", "Load Wave Length", MB_ICONERROR);
		return;
	}

	for (i = 0; i < WAVE_CNT_MAX; i++) {
		if (fl.ReadString(szStr) == FALSE)
			break;

		szStr.Trim();
		strcpy_s(szBuff, 255, szStr);
		p=strtok(szBuff, ",");
		if (p) {
			m_tEditWaveData.fWaveLength[i] = atof(p);
			strcpy(m_tEditWaveData.szWaveLenName[i], p);
			p = strtok(NULL, ",");
			if (p) {
				strcat(m_tEditWaveData.szWaveLenName[i], "-<");
				strcat(m_tEditWaveData.szWaveLenName[i], p);
				strcat(m_tEditWaveData.szWaveLenName[i], ">");
			}
		}
		else {
			m_tEditWaveData.fWaveLength[i] = 0.0f;
			memset(m_tEditWaveData.szWaveLenName[i], '\0',sizeof(m_tEditWaveData.szWaveLenName[i]));
		}
	}/* for */
	fl.Close();

	SetTitle();
	SetData();
}


void CDlgWaveConfig::OnBnClickedBtnCreate()
{
	double	dVal[6];
	int		iohdl;
	int		i;

	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
#if 0
	CString	szIOName('\0', 32);
	CIODB* pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	szIOName.Format("PM%d.CalibRate_A", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[0]);

	szIOName.Format("PM%d.CalibRate_B1", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[1]);

	szIOName.Format("PM%d.CalibRate_B2", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[2]);

	szIOName.Format("PM%d.CalibRate_B3", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[3]);

	szIOName.Format("PM%d.CalibRate_B4", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[4]);

	szIOName.Format("PM%d.CalibRate_B5", (m_iPMNo + 1));
	iohdl = pIODB->IOPointGetHandle(szIOName);
	pIODB->IOPointRead(iohdl, &dVal[5]);
#endif
	dVal[0] = pConfig->m_tWaveCaribrate[m_iPMNo].dA0;
	dVal[1] = pConfig->m_tWaveCaribrate[m_iPMNo].dB1;
	dVal[2] = pConfig->m_tWaveCaribrate[m_iPMNo].dB2;
	dVal[3] = pConfig->m_tWaveCaribrate[m_iPMNo].dB3;
	dVal[4] = pConfig->m_tWaveCaribrate[m_iPMNo].dB4;
	dVal[5] = pConfig->m_tWaveCaribrate[m_iPMNo].dB4;

	for (i = 0; i < WAVE_CNT_MAX; i++) {
		m_tEditWaveData.fWaveLength[i] = dVal[0] +(float)(dVal[1] * (float)(i + 1)) + (float)(dVal[2] * (float)(pow((i + 1), 2))) + (float)(dVal[3] * (float)(pow((i + 1), 3))) + (float)(dVal[4] * (float)(pow((i + 1), 4))) + (float)(dVal[5] * (float)(pow((i + 1), 5)));
		sprintf(m_tEditWaveData.szWaveLenName[i], "No%d.%5.1fnm", (i + 1), m_tEditWaveData.fWaveLength[i]);
	}

	m_pConfig->m_tWaveConfig2[m_iPMNo] = m_tEditWaveData;
	m_pConfig->WaveCfgFileSave();

	SetTitle();
	SetData();
}
