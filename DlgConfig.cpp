// DlgConfig.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgConfig.h"


CRect	gDefRect = { 100,100,100,100 };

static char* m_pConfigName[2] = {
	{"Configure"},
	NULL,
};


static char* m_pConfigMenu[8 + 1] = {
	/* 0 */	{"EQP Configure"},
	/* 1 */	{"Wave Setting"},
	/* 2 */	{"PLC Communication"},
	/* 3 */	{"Sensor Config"},
	/* 4 */	{"User Registration"},
		NULL,
};
static char* m_pPCMenu[PM_MAX + 1] = { "PC1","PC2","PC3","PC4","PC5" };


// CDlgConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogX)

CDlgConfig::CDlgConfig(CWnd* pParent /*=nullptr*/,CRect& rc)
	: CDialogX(IDD_CONFIG, pParent)
{
	m_rc_wnd = rc;
	m_pParentWnd = pParent;

	m_pImage = NULL;
	m_pCurrDlg = NULL;

	m_iCurrItemNo = ((CWaveEndPointApp*)AfxGetApp())->GetConfigItemNo();

	CDialogX::Create(IDD_CONFIG, pParent);
}

CDlgConfig::~CDlgConfig()
{
	 ((CWaveEndPointApp*)AfxGetApp())->SetConfigItemNo(m_iCurrItemNo);

	if (m_pImage)
		delete m_pImage;
	if (m_pCurrDlg)
		delete m_pCurrDlg;
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CFG, m_Tree_Config);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogX)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CFG, &CDlgConfig::OnTvnSelchangedTreeCfg)
END_MESSAGE_MAP()


// CDlgConfig メッセージ ハンドラー


BOOL CDlgConfig::OnInitDialog()
{

	CDialogX::OnInitDialog();

	MoveWindow(m_rc_wnd);

	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	long lState = GetWindowLong(m_Tree_Config.m_hWnd, GWL_STYLE);
	//	lState |= (TVS_HASLINES | TVS_LINESATROOT);
	lState |= (TVS_HASLINES);
	SetWindowLong(m_Tree_Config.m_hWnd, GWL_STYLE, lState);
	m_pImage = new CImageList();
	m_pImage->Create(IDB_CFG_TREE, 24, 2, RGB(255, 255, 255));
	m_Tree_Config.SetImageList(m_pImage, TVSIL_NORMAL);

	int		i, j;
	CString			szWork;
	HTREEITEM		hTreeHdl, hDataHdl, hSubHdl;
	HTREEITEM		hSelNoHdl = NULL;

	TV_INSERTSTRUCT	tcItem;

	tcItem.hParent = TVI_ROOT;
	tcItem.hInsertAfter = TVI_LAST;
	tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
	szWork = m_pConfigName[0];
	tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
	tcItem.item.lParam = 0;
	tcItem.item.iImage = 0;
	tcItem.item.iSelectedImage = 1;
	hTreeHdl = m_Tree_Config.InsertItem(&tcItem);

	//モジュール内のデータをセット
	for (i = 0; i < 5; i++) {

		szWork.Format("%s", m_pConfigMenu[i]);
		tcItem.hParent = hTreeHdl;
		tcItem.hInsertAfter = TVI_LAST;
		tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
		tcItem.item.lParam = 10 + i;
		tcItem.item.iImage = 0;
		tcItem.item.iSelectedImage = 1;
		hDataHdl = m_Tree_Config.InsertItem(&tcItem);
		if (m_iCurrItemNo == -1 && hSelNoHdl == NULL) {
			hSelNoHdl = hDataHdl;
		}
		// Wave Config
		if (i == 1) {
			for (j = 0; j < PM_MAX; j++) {
				if (m_pEPDConfig->m_tEPDConfig.iPCEnable[j]) {
					szWork.Format("%s", m_pPCMenu[j]);
					tcItem.hParent = hDataHdl;
					tcItem.hInsertAfter = TVI_LAST;
					tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
					tcItem.item.lParam = 100 + j;
					tcItem.item.iImage = 0;
					tcItem.item.iSelectedImage = 1;
					hSubHdl = m_Tree_Config.InsertItem(&tcItem);
				}
			}
		}
		// Sensor Config
		else if (i == 3) {
			for (j = 0; j < PM_MAX; j++) {
				if (m_pEPDConfig->m_tEPDConfig.iPCEnable[j]) {
					szWork.Format("%s", m_pPCMenu[j]);
					tcItem.hParent = hDataHdl;
					tcItem.hInsertAfter = TVI_LAST;
					tcItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tcItem.item.pszText = (LPSTR)((LPCSTR)szWork);
					tcItem.item.lParam = 1000+j;
					tcItem.item.iImage = 0;
					tcItem.item.iSelectedImage = 1;
					hSubHdl = m_Tree_Config.InsertItem(&tcItem);
				}
			}
		}
		else if (m_iCurrItemNo == i)
			hSelNoHdl = hDataHdl;
	}/* for */

	m_Tree_Config.Expand(hTreeHdl, TVE_EXPAND);
	if(hSelNoHdl)
		m_Tree_Config.SelectItem(hSelNoHdl);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgConfig::OnTvnSelchangedTreeCfg(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM		hTreeHdl;
	DWORD_PTR		index, iDrawIndex;
	int				iPCNo = -1;
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	hTreeHdl = m_Tree_Config.GetSelectedItem();
	if (hTreeHdl) {
		index = m_Tree_Config.GetItemData(hTreeHdl);

		if (index < 10)
			return;

		iDrawIndex = index;
		if (index < 100) {
			index %= 10;
			if (index == 1 || index == 3)
				return;

			if (m_pCurrDlg) {
				switch (m_iCurrItemNo) {
					// 0:EQ Config
				case 0:
					m_pDlgEQConfig->ModifyCheck();
					break;
				case 1:
					m_pDlgWaveConfig->ModifyCheck();
					break;
					// 2:PLC Config
				case 2:
					m_pDlgPLCConfig->ModifyCheck();
					break;
				case 4:
					m_pDlgUserList->ModifyCheck();
					break;
				}/* switch */
				m_pCurrDlg->SendMessage(WM_CLOSE);
				delete m_pCurrDlg;
				m_pCurrDlg = NULL;
			}
			switch (index) {
			case 0:
				m_pDlgEQConfig = new CDlgEQConfig(this);
				m_pCurrDlg = (CDialog*)m_pDlgEQConfig;
				break;
			case 2:			// PLC通信設定
				m_pDlgPLCConfig = new CDlgPLCConfig(this);
				m_pCurrDlg = (CDialog*)m_pDlgPLCConfig;
				break;
			case 4:			// User 登録
				m_pDlgUserList = new CDlgUserList(this);
				m_pCurrDlg = (CDialog*)m_pDlgUserList;
				break;
			}/* switch */
			m_iCurrItemNo = (int)index;
		}
		else if(index < 1000){
			index %= 100;

			if (m_pCurrDlg) {
				switch (m_iCurrItemNo) {
					// 0:EQ Config
				case 0:
					m_pDlgEQConfig->ModifyCheck();
					break;
				case 1:
					m_pDlgWaveConfig->ModifyCheck();
					break;
					// 2:PLC Config
				case 2:
					m_pDlgPLCConfig->ModifyCheck();
					break;
					// 4:User登録
				case 4:
					m_pDlgUserList->ModifyCheck();
					break;
				}/* switch */
				m_pCurrDlg->SendMessage(WM_CLOSE);
				delete m_pCurrDlg;
				m_pCurrDlg = NULL;
			}
			m_pDlgWaveConfig = new CDlgWaveConfig(this, index);
			m_pCurrDlg = m_pDlgWaveConfig;
			m_iCurrItemNo = 1;
		}
		else{
			index %= 1000;

			if (m_pCurrDlg) {
				switch (m_iCurrItemNo) {
				// 0:EQ Config
				case 0:
					m_pDlgEQConfig->ModifyCheck();
					break;
				case 1:
					m_pDlgWaveConfig->ModifyCheck();
					break;
					// 2:PLC Config
				case 2:
					m_pDlgPLCConfig->ModifyCheck();
					break;
					// 4:User登録
				case 4:
					m_pDlgUserList->ModifyCheck();
					break;
				}/* switch */
				m_pCurrDlg->SendMessage(WM_CLOSE);
				delete m_pCurrDlg;
				m_pCurrDlg = NULL;
			}
			switch (index) {
			case PM1:
				if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
					m_pDlgSensorConfig = new CDlgSensorConfig(this, PM1);
				else
					m_pDlgSensorConfig_USB = new CDlgSensorConfig_USB(this, PM1);
				break;
			case PM2:
				if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
					m_pDlgSensorConfig = new CDlgSensorConfig(this, PM2);
				else
					m_pDlgSensorConfig_USB = new CDlgSensorConfig_USB(this, PM2);
				break;
			case PM3:
				if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
					m_pDlgSensorConfig = new CDlgSensorConfig(this, PM3);
				else
					m_pDlgSensorConfig_USB = new CDlgSensorConfig_USB(this, PM3);
				break;
			case PM4:
				if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
					m_pDlgSensorConfig = new CDlgSensorConfig(this, PM4);
				else
					m_pDlgSensorConfig_USB = new CDlgSensorConfig_USB(this, PM4);
				break;
			default:
				if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
					m_pDlgSensorConfig = new CDlgSensorConfig(this, PM5);
				else
					m_pDlgSensorConfig_USB = new CDlgSensorConfig_USB(this, PM5);
				break;
			}/* switch */
			if (pConfig->m_tEPDConfig.Driver == ETHERNET_DRV)
				m_pCurrDlg = m_pDlgSensorConfig;
			else
				m_pCurrDlg = m_pDlgSensorConfig_USB;
			m_iCurrItemNo = 3;
		}
	}
	*pResult = 0;
}
void CDlgConfig::ModifyCheck()
{

	if (m_pCurrDlg) {
		switch (m_iCurrItemNo) {
			// 0:EQ Config
		case 0:
			m_pDlgEQConfig->ModifyCheck();
			break;
		case 1:
			m_pDlgWaveConfig->ModifyCheck();
			break;
			// 2:PLC Config
		case 2:
			m_pDlgPLCConfig->ModifyCheck();
			break;
		case 4:
			m_pDlgUserList->ModifyCheck();
			break;
		}/* switch */
	}
}

