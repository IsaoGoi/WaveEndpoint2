// CDlgPLCConfig.cpp : 実装ファイル
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "CDlgPLCConfig.h"


// CDlgPLCConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgPLCConfig, CDialogX)

CDlgPLCConfig::CDlgPLCConfig(CWnd* pParent /*=nullptr*/)
	: CDialogX(IDD_PLCCONFIG, pParent)
{
	SetBackColor(180, 255, 180);
	CDialogX::Create(IDD_PLCCONFIG, pParent);
}

CDlgPLCConfig::~CDlgPLCConfig()
{
}

void CDlgPLCConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PCNO, m_io_PCNo);
	DDX_Control(pDX, IDC_ED_PCNETWORK, m_io_PCNetNo);
	DDX_Control(pDX, IDC_ED_PCNODE, m_io_PCNode);
	DDX_Control(pDX, IDC_ED_PLC_MACHINE, m_io_PLCNo);
	DDX_Control(pDX, IDC_ED_PLC_NETWORK, m_io_PLCNetNo);
	DDX_Control(pDX, IDC_ED_PLC_NODE, m_io_PLCNode);
	DDX_Control(pDX, IDC_ED_PORTNO, m_io_PLC_PortNo);
	DDX_Control(pDX, IDC_ED_IP1, m_io_PLC_IP1);
	DDX_Control(pDX, IDC_ED_IP2, m_io_PLC_IP2);
	DDX_Control(pDX, IDC_ED_IP3, m_io_PLC_IP3);
	DDX_Control(pDX, IDC_ED_IP4, m_io_PLC_IP4);
	DDX_Control(pDX, IDC_ED_OMRON_ENABLE, m_io_Omron_Enable);
	DDX_Control(pDX, IDC_ED_KEYENCE_ENABLE, m_io_KeyenceEnable);
	DDX_Control(pDX, IDC_ED_KEY_IP1, m_io_Keyence_IP1);
	DDX_Control(pDX, IDC_ED_KEY_IP2, m_io_Keyence_IP2);
	DDX_Control(pDX, IDC_ED_KEY_IP3, m_io_Keyence_IP3);
	DDX_Control(pDX, IDC_ED_KEY_IP4, m_io_Keyence_IP4);
	DDX_Control(pDX, IDC_ED_PC_IP1, m_io_PC_IP1);
	DDX_Control(pDX, IDC_ED_PC_IP2, m_io_PC_IP2);
	DDX_Control(pDX, IDC_ED_PC_IP3, m_io_PC_IP3);
	DDX_Control(pDX, IDC_ED_PC_IP4, m_io_PC_IP4);
	DDX_Control(pDX, IDC_ED_KEY_PORTNO, m_io_KeyencePort);
}


BEGIN_MESSAGE_MAP(CDlgPLCConfig, CDialogX)
END_MESSAGE_MAP()


// CDlgPLCConfig メッセージ ハンドラー


BOOL CDlgPLCConfig::OnInitDialog()
{
	CRect		rc;
	CString		szIOName('\0', 32);
	int			i;
	int			iIPAddr[4 + 1];
	char* p1, * p2;
	char		szIPAddr[128];

	CDialogX::OnInitDialog();

	m_pEPDConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();
	m_pIODB = ((CWaveEndPointApp*)AfxGetApp())->GetIODB();

	GetParent()->GetClientRect(rc);
	rc.DeflateRect(160, 30);
	rc.OffsetRect(140, 0);
	MoveWindow(rc);


	m_io_Omron_Enable.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.Enable"));
	m_io_PCNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PCNo"));
	m_io_PCNetNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PCNet"));
	m_io_PCNode.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PCNode"));
	m_io_PLCNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLCNo"));
	m_io_PLCNetNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLCNet"));
	m_io_PLCNode.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLCNode"));
	m_io_PLC_PortNo.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PortNo"));
	m_io_PLC_IP1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLC_IP1"));
	m_io_PLC_IP2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLC_IP2"));
	m_io_PLC_IP3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLC_IP3"));
	m_io_PLC_IP4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.OMRON.PLC_IP4"));

	m_io_KeyenceEnable.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.Enable"));
	m_io_Keyence_IP1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PLC_IP1"));
	m_io_Keyence_IP2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PLC_IP2"));
	m_io_Keyence_IP3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PLC_IP3"));
	m_io_Keyence_IP4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PLC_IP4"));
	m_io_PC_IP1.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PC_IP1"));
	m_io_PC_IP2.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PC_IP2"));
	m_io_PC_IP3.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PC_IP3"));
	m_io_PC_IP4.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PC_IP4"));
	m_io_KeyencePort.SetIOHdl(m_pIODB->IOPointGetHandle("CTC.KEY.PortNo"));



	m_pIODB->IOPointWrite(m_io_Omron_Enable.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iEnable);
	m_pIODB->IOPointWrite(m_io_PCNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_No);
	m_pIODB->IOPointWrite(m_io_PCNetNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_Network_Addr);
	m_pIODB->IOPointWrite(m_io_PCNode.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_Node_Addr);
	m_pIODB->IOPointWrite(m_io_PLCNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Machine_No);
	m_pIODB->IOPointWrite(m_io_PLCNetNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Network_Addr);
	m_pIODB->IOPointWrite(m_io_PLCNode.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Node_Addr);
	m_pIODB->IOPointWrite(m_io_PLC_PortNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPortNo);

	m_pIODB->IOPointWrite(m_io_KeyenceEnable.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.iEnable);
	m_pIODB->IOPointWrite(m_io_KeyencePort.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.iPortNo);
	m_pIODB->IOPointWrite(m_io_Keyence_IP1.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr1);
	m_pIODB->IOPointWrite(m_io_Keyence_IP2.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr2);
	m_pIODB->IOPointWrite(m_io_Keyence_IP3.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr3);
	m_pIODB->IOPointWrite(m_io_Keyence_IP4.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr4);
	m_pIODB->IOPointWrite(m_io_PC_IP1.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr1);
	m_pIODB->IOPointWrite(m_io_PC_IP2.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr2);
	m_pIODB->IOPointWrite(m_io_PC_IP3.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr3);
	m_pIODB->IOPointWrite(m_io_PC_IP4.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr4);

	strcpy_s(szIPAddr, sizeof(szIPAddr), m_pEPDConfig->m_tPLCConfig.szPLC_IP_Addr);
	p1 = szIPAddr;
	p2 = p1;

	for (i = 0; i < 3; i++) {
		p1 = strchr(p1, '.');
		if (p1 == NULL)
			break;
		*p1 = '\0';
		p1++;
		iIPAddr[i] = atoi(p2);
		p2 = p1;
	}/* for */

	if (*p2 != '\0')
		iIPAddr[3] = atoi(p2);

	m_pIODB->IOPointWrite(m_io_PLC_IP1.GetIOHdl(), &iIPAddr[0]);
	m_pIODB->IOPointWrite(m_io_PLC_IP2.GetIOHdl(), &iIPAddr[1]);
	m_pIODB->IOPointWrite(m_io_PLC_IP3.GetIOHdl(), &iIPAddr[2]);
	m_pIODB->IOPointWrite(m_io_PLC_IP4.GetIOHdl(), &iIPAddr[3]);

	m_io_Omron_Enable.SetSpecialColor(2);
	m_io_PCNo.SetSpecialColor(2);
	m_io_PCNetNo.SetSpecialColor(2);
	m_io_PCNode.SetSpecialColor(2);
	m_io_PLCNo.SetSpecialColor(2);
	m_io_PLCNetNo.SetSpecialColor(2);
	m_io_PLCNode.SetSpecialColor(2);
	m_io_PLC_PortNo.SetSpecialColor(2);
	m_io_PLC_IP1.SetSpecialColor(2);
	m_io_PLC_IP2.SetSpecialColor(2);
	m_io_PLC_IP3.SetSpecialColor(2);
	m_io_PLC_IP4.SetSpecialColor(2);

	m_io_KeyenceEnable.SetSpecialColor(2);
	m_io_Keyence_IP1.SetSpecialColor(2);
	m_io_Keyence_IP2.SetSpecialColor(2);
	m_io_Keyence_IP3.SetSpecialColor(2);
	m_io_Keyence_IP4.SetSpecialColor(2);
	m_io_PC_IP1.SetSpecialColor(2);
	m_io_PC_IP2.SetSpecialColor(2);
	m_io_PC_IP3.SetSpecialColor(2);
	m_io_PC_IP4.SetSpecialColor(2);
	m_io_KeyencePort.SetSpecialColor(2);


	m_io_Omron_Enable.UpdateDisplay();
	m_io_PCNo.UpdateDisplay();
	m_io_PCNetNo.UpdateDisplay();
	m_io_PCNode.UpdateDisplay();
	m_io_PLCNo.UpdateDisplay();
	m_io_PLCNetNo.UpdateDisplay();
	m_io_PLCNode.UpdateDisplay();
	m_io_PLC_PortNo.UpdateDisplay();
	m_io_PLC_IP1.UpdateDisplay();
	m_io_PLC_IP2.UpdateDisplay();
	m_io_PLC_IP3.UpdateDisplay();
	m_io_PLC_IP4.UpdateDisplay();

	m_io_KeyenceEnable.UpdateDisplay();
	m_io_Keyence_IP1.UpdateDisplay();
	m_io_Keyence_IP2.UpdateDisplay();
	m_io_Keyence_IP3.UpdateDisplay();
	m_io_Keyence_IP4.UpdateDisplay();
	m_io_PC_IP1.UpdateDisplay();
	m_io_PC_IP2.UpdateDisplay();
	m_io_PC_IP3.UpdateDisplay();
	m_io_PC_IP4.UpdateDisplay();
	m_io_KeyencePort.UpdateDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgPLCConfig::OnOK()
{
	int		iIPAddr[4 + 1];

	m_pIODB->IOPointRead(m_io_Omron_Enable.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iEnable);
	m_pIODB->IOPointRead(m_io_PCNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_No);
	m_pIODB->IOPointRead(m_io_PCNetNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_Network_Addr);
	m_pIODB->IOPointRead(m_io_PCNode.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPC_Node_Addr);
	m_pIODB->IOPointRead(m_io_PLCNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Machine_No);
	m_pIODB->IOPointRead(m_io_PLCNetNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Network_Addr);
	m_pIODB->IOPointRead(m_io_PLCNode.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPLC_Node_Addr);
	m_pIODB->IOPointRead(m_io_PLC_PortNo.GetIOHdl(), &m_pEPDConfig->m_tPLCConfig.iPortNo);

	m_pIODB->IOPointRead(m_io_PLC_IP1.GetIOHdl(), &iIPAddr[0]);
	m_pIODB->IOPointRead(m_io_PLC_IP2.GetIOHdl(), &iIPAddr[1]);
	m_pIODB->IOPointRead(m_io_PLC_IP3.GetIOHdl(), &iIPAddr[2]);
	m_pIODB->IOPointRead(m_io_PLC_IP4.GetIOHdl(), &iIPAddr[3]);

	sprintf(m_pEPDConfig->m_tPLCConfig.szPLC_IP_Addr, "%d.%d.%d.%d", iIPAddr[0], iIPAddr[1], iIPAddr[2], iIPAddr[3]);

	m_pIODB->IOPointRead(m_io_KeyenceEnable.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.iEnable);
	m_pIODB->IOPointRead(m_io_KeyencePort.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.iPortNo);
	m_pIODB->IOPointRead(m_io_Keyence_IP1.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr1);
	m_pIODB->IOPointRead(m_io_Keyence_IP2.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr2);
	m_pIODB->IOPointRead(m_io_Keyence_IP3.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr3);
	m_pIODB->IOPointRead(m_io_Keyence_IP4.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.PLCIPAddr.iIPAddr4);
	m_pIODB->IOPointRead(m_io_PC_IP1.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr1);
	m_pIODB->IOPointRead(m_io_PC_IP2.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr2);
	m_pIODB->IOPointRead(m_io_PC_IP3.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr3);
	m_pIODB->IOPointRead(m_io_PC_IP4.GetIOHdl(), &m_pEPDConfig->m_tKeyenceComm.MyIPAddr.iIPAddr4);


	m_pEPDConfig->PLCCfgFileSave();
}
void CDlgPLCConfig::ModifyCheck()
{

}
