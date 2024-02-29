// DlgCommadPage.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "ButtonPage.h"
#include "DlgCommadPage.h"

static char *gszCaption[10]={
/* 0 */ NULL,
/* 1 */ NULL,
/* 2 */ "Log",
/* 3 */ "Monitor",
/* 4 */ "NULL",
/* 5 */ "NULL",
};

typedef struct{
	char	szLabel[32];
	int		iMessageID;
	int		iModule;			// 0:共通/1:PM1/2:PM2/3:PM3
}BUTTON_ID;

typedef struct{
	char	szCaption[32+1];
	int		iBtnMax;
	BUTTON_ID	btn_id[12];
}COMMAND_BUTTON;


static COMMAND_BUTTON	comand_btn[10]={
/* 0:Main		*/	{NULL,0},
/* 1:Recipe		*/	{NULL,0},
/* 2:Log		*/	{"Log",2,
						{
							{{"EPD Log"},		0,0},
							{{"Alarm Log"},		1,0},
						},
					},
/* 3:Monitor	*/	{"Monitor",6,
						{
							{{"Wave Monitor"},	 0,0},
							{{"PC1 Monitor"},    1,1},
							{{"PC2 Monitor"},    2,2},
							{{"PC3 Monitor"},    3,3},
							{{"PC4 Monitor"},    4,4},
							{{"PC5 Monitor"},    5,5},
						},
					},
/* 4:Monitor	*/	{"Analysis",3,
											{
												{{"Wave Analysis"},	 0,0},
												{{"Create Base Wave"},     1,0},
												{{"Recipe Check"},   2,0},
											},
										},
/* 5:Config		*/	{NULL,0},
};

static unsigned int	m_ID_Button[11]={
IDB_CMD_BTN1,
IDB_CMD_BTN2,
IDB_CMD_BTN3,
IDB_CMD_BTN4,
IDB_CMD_BTN5,
IDB_CMD_BTN6,
IDB_CMD_BTN7,
IDB_CMD_BTN8,
IDB_CMD_BTN9,
IDB_CMD_BTN10,
IDB_CMD_BTN11
};



// CDlgCommadPage ダイアログ

IMPLEMENT_DYNAMIC(CDlgCommadPage, CDialog)

CDlgCommadPage::CDlgCommadPage(CWnd* pParent /*=NULL*/,int iDisplayID,int x,int btm_y)
	: CDialog(CDlgCommadPage::IDD, pParent)
{
	m_iDisplayID = iDisplayID;
	m_iStart_x = x;
	m_iBottomt_y = btm_y;
	m_pButtonPage = (CButtonPage *)pParent;

	Create(IDD,pParent);

}

CDlgCommadPage::~CDlgCommadPage()
{
}

void CDlgCommadPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCommadPage, CDialog)
	ON_WM_CREATE()
//	ON_WM_MOUSEMOVE()
ON_BN_CLICKED(IDB_CMD_BTN1, &CDlgCommadPage::OnBnClickedCmdBtn1)
ON_BN_CLICKED(IDB_CMD_BTN2, &CDlgCommadPage::OnBnClickedCmdBtn2)
ON_BN_CLICKED(IDB_CMD_BTN3, &CDlgCommadPage::OnBnClickedCmdBtn3)
ON_BN_CLICKED(IDB_CMD_BTN4, &CDlgCommadPage::OnBnClickedCmdBtn4)
ON_BN_CLICKED(IDB_CMD_BTN5, &CDlgCommadPage::OnBnClickedCmdBtn5)
ON_BN_CLICKED(IDB_CMD_BTN6, &CDlgCommadPage::OnBnClickedCmdBtn6)
ON_BN_CLICKED(IDB_CMD_BTN7, &CDlgCommadPage::OnBnClickedCmdBtn7)
ON_BN_CLICKED(IDB_CMD_BTN8, &CDlgCommadPage::OnBnClickedCmdBtn8)
ON_BN_CLICKED(IDB_CMD_BTN9, &CDlgCommadPage::OnBnClickedCmdBtn9)
ON_BN_CLICKED(IDB_CMD_BTN10, &CDlgCommadPage::OnBnClickedCmdBtn10)
ON_BN_CLICKED(IDB_CMD_BTN11, &CDlgCommadPage::OnBnClickedCmdBtn11)
END_MESSAGE_MAP()


// CDlgCommadPage メッセージ ハンドラ


int CDlgCommadPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CDlgCommadPage::OnInitDialog()
{
	CEPDConfig	*pConfig = ((CWaveEndPointApp *)AfxGetApp())->GetEPDConfig();

	CDialog::OnInitDialog();

	CRect	rc;
	int		i;

	rc.left = m_iStart_x;
	rc.bottom = m_iBottomt_y;
	rc.right = m_iStart_x + 260;
	rc.top = m_iBottomt_y - (46 * comand_btn[m_iDisplayID].iBtnMax + 40);

	for(i=0;i<comand_btn[m_iDisplayID].iBtnMax;i++){
		GetDlgItem(m_ID_Button[i])->SetWindowText(comand_btn[m_iDisplayID].btn_id[i].szLabel);
		if(comand_btn[m_iDisplayID].btn_id[i].iModule==0)
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else if(comand_btn[m_iDisplayID].btn_id[i].iModule==1 && pConfig->m_tEPDConfig.iPCEnable[PM1])
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else if(comand_btn[m_iDisplayID].btn_id[i].iModule==2 && pConfig->m_tEPDConfig.iPCEnable[PM2])
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else if(comand_btn[m_iDisplayID].btn_id[i].iModule==3 && pConfig->m_tEPDConfig.iPCEnable[PM3])
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else if(comand_btn[m_iDisplayID].btn_id[i].iModule==4 && pConfig->m_tEPDConfig.iPCEnable[PM4])
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else if(comand_btn[m_iDisplayID].btn_id[i].iModule==5 && pConfig->m_tEPDConfig.iPCEnable[PM5])
			GetDlgItem(m_ID_Button[i])->EnableWindow(TRUE);
		else
			GetDlgItem(m_ID_Button[i])->EnableWindow(FALSE);
	}/* for */
	for(;i<11;i++)
		GetDlgItem(m_ID_Button[i])->EnableWindow(FALSE);

	MoveWindow(rc);

	SetWindowText(comand_btn[m_iDisplayID].szCaption);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//void CDlgCommadPage::OnMouseMove(UINT nFlags, CPoint point)
//{
//	int		i;
//	
//
//	i =  point.y;
//	i /=46;
//
//	GetDlgItem(m_ID_Button[i])->SetFocus();
//
//	CDialog::OnMouseMove(nFlags, point);
//}

void CDlgCommadPage::OnBnClicked(int iSubNo)
{
	m_pButtonPage->ChangeFunction(m_iDisplayID,iSubNo);
}


