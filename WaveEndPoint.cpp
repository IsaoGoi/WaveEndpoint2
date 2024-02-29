
// WaveEndPoint.cpp : アプリケーションのクラス動作を定義します。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "WaveEndPoint.h"
#include "PCMonitorTask.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEndPointApp

BEGIN_MESSAGE_MAP(CWaveEndPointApp, CWinApp)
END_MESSAGE_MAP()


// CWaveEndPointApp の構築

CWaveEndPointApp::CWaveEndPointApp() noexcept
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("WaveEndPoint.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CWaveEndPointApp オブジェクト

CWaveEndPointApp theApp;


// CWaveEndPointApp の初期化

BOOL CWaveEndPointApp::InitInstance()
{
	int		i;

	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

#if 0
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
#endif
	//------------------------------>>>
	// 2007/11/12 WinSock2.0を使用
	WSADATA wsadata;
	int		iRet;

	// WinSock 2.0の初期化
	WORD wVersionRequired = MAKEWORD(2, 0);
	wsadata.iMaxUdpDg = 1024 * 60;		// 2006/10/17
	iRet = WSAStartup(wVersionRequired, &wsadata);
	if (iRet != 0) {
		AfxMessageBox(_T("WinSock2.0 Initialil Error"));
		return(FALSE);
	}
	// Versionチェック
	if (wsadata.wVersion != wVersionRequired) {
		WSACleanup();
		AfxMessageBox(_T("WinSock2.0 Initialil Error"));
		return(FALSE);
	}
	//<<<--------------------------------

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));



	InitializeCriticalSection(&m_Recipe_Interlock);

	m_iIdleProcessCnt = 0;
	m_RowLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;


	LoadEPDUnitID();

	m_pEPDConfig = new CEPDConfig();
	m_pEPDConfig->ConfigFileLoad();
	m_pEPDConfig->WaveCfgFileLoad();
	m_pEPDConfig->PLCCfgFileLoad();


	m_pAlarm = new CAlarm();
	m_pAlarm->LoadAlarmList();
	m_pAlarm->LoadAlarmLog();

	m_pPCRcpCtrl = new CPCRcpCtrl();
	m_pPCRcpCtrl->MakeTable();

	m_pWaveCfgCtrl = new CWaveCfgCtrl();
	m_pWaveCfgCtrl->MakeTable();

	m_pAccessLevel = new CAccessLevel();
	m_pAccessLevel->UserInit();
	m_iLogOn = FALSE;

	// PLC通信モニターデータ領域の確保
	hPLCBuffHdl = GlobalAlloc(GPTR, 1000 * 2);
	if (hPLCBuffHdl != NULL)
		m_usPLCMonData = (unsigned short*)GlobalLock(hPLCBuffHdl);
	else
		m_usPLCMonData = NULL;

	// IOPoint 初期化
	m_pIODB = new CIODB();
	m_pIODB->LoadIODB("IO\\IODBInit.TXT");
	m_pIODB->IODBUpdateInit();


	/*
	** OMRON PLC Enable
	*/
	m_FinsSock = NULL;
	m_PLCMonitor = NULL;

	m_FinsSock = new CFinsSock();
	if (m_FinsSock->FinsInitial() == TRUE) {
		if (m_pEPDConfig->m_tPLCConfig.iEnable) {
			Sleep(5000);
			m_PLCMonitor = new CPLCMonitor();
			m_PLCMonitor->CreateThread(1024 * 16, 100, THREAD_PRIORITY_HIGHEST);
		}
		else
			m_PLCMonitor = NULL;
	}
	/*
	** Keyence PLC
	*/
	hKeyenceBuffHdl = GlobalAlloc(GPTR, 0x2000 * 2);
	if (hKeyenceBuffHdl != NULL)
		m_usKeyenceMonData = (unsigned short*)GlobalLock(hKeyenceBuffHdl);
	else
		m_usKeyenceMonData = NULL;

	m_pKeyenceMonitor = NULL;
	m_pKeyenceSock = NULL;

	m_pKeyenceSock = new CKeyenceSock();
	if (m_pKeyenceSock->MelsecInitial()) {
		if (m_pEPDConfig->m_tKeyenceComm.iEnable) {
			m_pKeyenceMonitor = new CKeyenceMonitor();
			m_pKeyenceMonitor->CreateThread(1024 * 8, 1000, THREAD_PRIORITY_NORMAL);
		}
	}

	m_pPLCLogThread = new CPLCComLogThread();
	m_pPLCLogThread->CreateThread(1024 * 16, 1000, THREAD_PRIORITY_NORMAL);

	m_pWaveSamplingSave = new CWaveSamplingSave();
	m_pWaveSamplingSave->CreateThread(1024 * 16, 1000, THREAD_PRIORITY_NORMAL);

	if (m_pEPDConfig->m_tEPDConfig.Driver == ETHERNET_DRV) {
		m_pWaveData_Eth = new CWaveData_Eth();
		m_pWaveData_Eth->StartWaveSampling();
		m_pWaveData_USB = NULL;
	}
	else {
		m_pWaveData_USB = new CWaveData_USB();
		m_pWaveData_USB->StartWaveSampling();
		m_pWaveData_Eth = NULL;
	}

	m_pEPDLog = new CEPDLog();
	m_pEPDLog->InitialLogTable();

	StartEPDRawData();

#if 0
	for (i = 0; i < PM_MAX; i++) {
		m_iCurrRecipe[i] = 0;
		m_iRowDataSaveMode[i] = 0;

		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			m_pEPDThread[i] = new CEPDThread(i);
			m_pEPDThread[i]->CreateThread(1024 * 16, 100, THREAD_PRIORITY_NORMAL);
		}
		else
			m_pEPDThread[i] = NULL;
	}/* for */
#endif

	m_iCfgScreenNo = -1;
	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// フレームをリソースからロードして作成します
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, nullptr,
		nullptr);

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();


	m_pDlgAlarm = new CDlgAlarm(pFrame);
	m_pDlgAlarm->HideAlarm();

	for (i = 0; i < PM_MAX; i++) {
		m_iCurrRecipe[i] = 0;
		m_iRowDataSaveMode[i] = 1;

		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			m_pEPDThread[i] = new CEPDThread(i);
			m_pEPDThread[i]->CreateThread(1024 * 16, 100, THREAD_PRIORITY_NORMAL);
		}
		else
			m_pEPDThread[i] = NULL;
	}/* for */

	((CMainFrame *)pFrame)->InitialScreen();
	m_pNumEditPage = new CDlgNumEdit();
	m_pEnumEditPage = new CDlgEnumEdit();
	m_pStringEditPage = new CDlgStringEdit();

	m_pEPDSimulation = new CEPDSimulation();
	m_pEPDSimulation->CreateThread(1024 * 16, 50, THREAD_PRIORITY_NORMAL);


	/*
	** 2023/12/21 PC Monitor Task
	*/
	StartPCMonitorTask();

#if 0
	for (i = 0; i < PM_MAX; i++) {
		m_iCurrRecipe[i] = 0;
		m_iRowDataSaveMode[i] = 0;

		if (m_pEPDConfig->m_tEPDConfig.iPCEnable[i]) {
			m_pEPDThread[i] = new CEPDThread(i);
			m_pEPDThread[i]->CreateThread(1024 * 16, 100, THREAD_PRIORITY_NORMAL);
		}
		else
			m_pEPDThread[i] = NULL;
	}/* for */
#endif
#if 0
	 /*
	** Log 保存期間Check
	*/
	m_iIdleProcessCnt = 0;
	m_iDel_RunEPDLogStatus = 0;
	m_iDel_RunRCPLogStatus = 0;
	m_iDel_RunCFGLogStatus = 0;
#endif


	return TRUE;
}

int CWaveEndPointApp::ExitInstance()
{
	int			i,j;
	int			ioHdl;
	unsigned	unsVal;
	CString szIOName('\0', 32);

	// Local Mode切り替え
	for (i = 0; i < PM_MAX; i++) {
		szIOName.Format("PM%d.EPD.Mode", (i + 1));
		ioHdl = m_pIODB->IOPointGetHandle(szIOName);
		unsVal = 1;
		m_pIODB->IOPointWrite(ioHdl, &unsVal);
	}/* for */
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	SaveEPDUnitID();


	StopPCMonitorTask();

	m_pEPDSimulation->KillThread();
	for (j = 0; m_pEPDSimulation->m_bEnded == FALSE && j < 50; j++)
		Sleep(100);
	delete m_pEPDSimulation;

	for (i = 0; i < PM_MAX; i++) {
		if (m_pEPDThread[i]) {
			m_pEPDThread[i]->KillThread();
			for(j=0;m_pEPDThread[i]->m_bEnded == FALSE && j < 50;j++)
				Sleep(200);
			delete m_pEPDThread[i];
		}
	}/* for */
	StopEPDRawData();

	delete m_pEPDLog;
	if (m_pWaveData_Eth) {
		delete m_pWaveData_Eth;
		m_pWaveData_Eth = NULL;
	}
	if (m_pWaveData_USB) {
		delete m_pWaveData_USB;
		m_pWaveData_USB = NULL;
	}
	delete m_pEPDConfig;
	delete m_pAlarm;
	delete m_pDlgAlarm;

	delete m_pNumEditPage;
	delete m_pEnumEditPage;
	delete m_pStringEditPage;

	if (m_PLCMonitor) {
		m_PLCMonitor->KillThread();
		for(j=0;m_PLCMonitor->m_bEnded == FALSE && j <50 ;j++)
			Sleep(200);
		delete m_PLCMonitor;
	}
	// Keyence PLC Monitor Thread終了
	if (m_pKeyenceMonitor) {
		m_pKeyenceMonitor->Stop();
		m_pKeyenceMonitor->KillThread();
		Sleep(100);
		delete m_pKeyenceMonitor;
	}

	if (m_pKeyenceSock)
		delete m_pKeyenceSock;

	GlobalUnlock(hKeyenceBuffHdl);
	GlobalFree(hKeyenceBuffHdl);


	m_pWaveSamplingSave->KillThread();
	for (j = 0; m_pWaveSamplingSave->m_bEnded == FALSE && j < 50; j++)
		Sleep(200);
	delete m_pWaveSamplingSave;

	m_pPLCLogThread->KillThread();
	for (j = 0; m_pPLCLogThread->m_bEnded == FALSE && j < 50; j++)
		Sleep(200);
	delete m_pPLCLogThread;

	delete m_FinsSock;
	delete m_pIODB;

	delete m_pPCRcpCtrl;
	delete m_pWaveCfgCtrl;
	delete m_pAccessLevel;

	Sleep(1000);
	GlobalUnlock(hPLCBuffHdl);
	GlobalFree(hPLCBuffHdl);

	DeleteCriticalSection(&m_Recipe_Interlock);

	WSACleanup();

	return CWinApp::ExitInstance();
}
void CWaveEndPointApp::LoadEPDUnitID()
{
	CStdioFile	fp;
	int			i;
	char		szBuff[64+1];
	char*		p;
	char*		pContext;
	int			iPM;

	if (fp.Open(_T("EPD_UnitID.TXT"), CFile::modeRead) == NULL) {
		for (i = 0; i < PM_MAX; i++)
			memset(m_szUnitID[i], '\0', sizeof(m_szUnitID[i]));
		return;
	}

	for (i = 0; i < PM_MAX; i++) {
		if (fp.ReadString(szBuff,64) == NULL)
			break;

		p = strtok_s(szBuff, "=\n", &pContext);
		if (p) {
			if (strncmp(p, "PC1", 3)==0)
				iPM = PM1;
			else if (strncmp(p, "PC2", 3) == 0)
				iPM = PM2;
			else if (strncmp(p, "PC3", 3) == 0)
				iPM = PM3;
			else if (strncmp(p, "PC4", 3) == 0)
				iPM = PM4;
			else if (strncmp(p, "PC5", 3) == 0)
				iPM = PM5;
			else
				continue;
		}
		memset(m_szUnitID[iPM], '\0', sizeof(m_szUnitID[i]));
		p = strtok_s(NULL, "=\n", &pContext);
		if (p) {
			strcpy(m_szUnitID[iPM], p);
			m_szUnitID[i][8] = '\0';
		}
	}/* for */
	fp.Close();
}
void CWaveEndPointApp::SaveEPDUnitID()
{
	CStdioFile	fp;
	int			i;
	CString		szIOName('\0', 32);
	int			ioHdl;
	char		szBuff[64+1];
	char		szUnitID[32 + 1];


	if (fp.Open(_T("EPD_UnitID.TXT"), CFile::modeCreate | CFile::modeWrite) == NULL) {
		return;
	}

	for (i = 0; i < PM_MAX; i++) {
		szIOName.Format("PM%d.UnitID", (i + 1));
		ioHdl = m_pIODB->IOPointGetHandle(szIOName);
		m_pIODB->IOPointRead(ioHdl, szUnitID);

		if (strchr(szUnitID, ' '))
			*(strchr(szUnitID, ' ')) = '\0';

		if (strchr(m_szUnitID[i], ' '))
			*(strchr(m_szUnitID[i], ' ')) = '\0';

		if (szUnitID[0] == '\0' && m_szUnitID[i][0] != '\0')
			sprintf(szBuff, "PC%d=%s\n", (i + 1), m_szUnitID[i]);
		else if(m_szUnitID[i][0] == '\0' && szUnitID[0] != '\0')
			sprintf(szBuff, "PC%d=%s\n", (i + 1), szUnitID);
		else if (m_szUnitID[i][0] != '\0' && szUnitID[0] != '\0')
			sprintf(szBuff, "PC%d=%s\n", (i + 1), m_szUnitID[i]);
		else
			sprintf(szBuff, "PC%d=\n", (i + 1));

		fp.WriteString(szBuff);
	}/* for */
	fp.Close();

}

// CWaveEndPointApp メッセージ ハンドラー




BOOL CWaveEndPointApp::OnIdle(LONG lCount)
{
	// Auto Log Off
	if (m_iLogOn == TRUE) {
		CTime t = CTime::GetCurrentTime();
		CTimeSpan tSpan;

		tSpan = t - m_tLogOnTime;

		if (tSpan.GetTotalMinutes() >= m_pAccessLevel->m_iAutoLogOffTime) {
			AutoLogOff_Stop();
			m_pAccessLevel->LogOff(TRUE);
			((CMainFrame*)m_pMainWnd)->m_wndView.m_pButtonPage->DisplayUpdate();
			//			((CWaveEndPointApp*)AfxGetApp())->ChangeMainScreen();
		}
	}


	/*
	** Log処理で10000枚リミットで削除するため、ここの削除は不要 
	*/
	// Row Log 削除
	++m_iIdleProcessCnt;

	if (m_iIdleProcessCnt == 50){
		CheckEthNetLog();
	}
	else if (m_iIdleProcessCnt == 100) {
		CheckRowLog();
	}
	else if (m_iIdleProcessCnt == 150) {
		CheckPLCLog();
		m_iIdleProcessCnt = 0;
	}

	return CWinApp::OnIdle(lCount);
}
#if 0
void CWaveEndPointApp::CheckRunEPDLog() 
{
	CString		szFileName('\0', 64);
	char		szDate[32];
	char		szComp[32];
	int			rv;
	int			iYear, iMonth, iDay;
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();


	if (m_iDel_RunEPDLogStatus == 0) {
		// Log Folder存在チェック
		szFileName = _T("RUN_EPD_LOG\\*.Log");
		if (m_RunEPDLog_filefind.FindFile((LPCSTR)szFileName, 0) == FALSE)
			return;

		m_iDel_RunEPDLogStatus++;

	}
	else {
		CTime		t = CTime::GetCurrentTime();
		iYear = t.GetYear();
		iMonth = t.GetMonth();
		iDay = t.GetDay();

		if (iMonth <= pConfig->m_tEPDConfig.iLogSaveCycle) {
			iYear--;
			iMonth = (12 + iMonth - pConfig->m_tEPDConfig.iLogSaveCycle);
		}
		else
			iMonth -= pConfig->m_tEPDConfig.iLogSaveCycle;

		sprintf(szDate, "%04.04d%02.02d%02.02d",iYear,iMonth,iDay);

		rv = m_RunEPDLog_filefind.FindNextFile();
		szFileName = m_RunEPDLog_filefind.GetFileName();
		strncpy(szComp, ((LPCSTR)szFileName)+4, 8);
		szComp[8] = '\0';
		if (strncmp(szDate, szComp, 8) > 0) {
			szFileName = m_RunEPDLog_filefind.GetFilePath();
			CFile::Remove((LPCSTR)szFileName);
		}
		if (rv == FALSE) {
			m_RunEPDLog_filefind.Close();
			m_iDel_RunEPDLogStatus = 0;
		}
	}
}
void CWaveEndPointApp::CheckRunRCPLog()
{
	CString		szFileName('\0', 64);
	char		szDate[32];
	char		szComp[32];
	int			rv;
	int			iYear, iMonth, iDay;
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();


	if (m_iDel_RunRCPLogStatus == 0) {
		// Log Folder存在チェック
		szFileName = _T("RUN_RCP_LOG\\*.Log");
		if (m_RunRCPLog_filefind.FindFile((LPCSTR)szFileName, 0) == FALSE)
			return;

		m_iDel_RunRCPLogStatus++;

	}
	else {
		CTime		t = CTime::GetCurrentTime();;
		iYear = t.GetYear();
		iMonth = t.GetMonth();
		iDay = t.GetDay();

		if (iMonth <= pConfig->m_tEPDConfig.iLogSaveCycle) {
			iYear--;
			iMonth = (12 + iMonth - pConfig->m_tEPDConfig.iLogSaveCycle);
		}
		else
			iMonth -= pConfig->m_tEPDConfig.iLogSaveCycle;

		sprintf(szDate, "%04.04d%02.02d%02.02d", iYear, iMonth, iDay);

		rv = m_RunRCPLog_filefind.FindNextFile();
		szFileName = m_RunRCPLog_filefind.GetFileName();
		strncpy(szComp, ((LPCSTR)szFileName) + 4, 8);
		szComp[8] = '\0';
		if (strncmp(szDate, szComp, 8) > 0) {
			szFileName = m_RunRCPLog_filefind.GetFilePath();
			CFile::Remove((LPCSTR)szFileName);
		}
		if (rv == FALSE) {
			m_RunRCPLog_filefind.Close();
			m_iDel_RunRCPLogStatus = 0;
		}
	}
}
void CWaveEndPointApp::CheckRunCFGLog()
{
	CString		szFileName('\0', 64);
	char		szDate[32];
	char		szComp[32];
	int			rv;
	int			iYear, iMonth, iDay;
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();


	if (m_iDel_RunCFGLogStatus == 0) {
		// Log Folder存在チェック
		szFileName = _T("RUN_CFG_LOG\\*.Log");
		if (m_RunCFGLog_filefind.FindFile((LPCSTR)szFileName, 0) == FALSE)
			return;

		m_iDel_RunCFGLogStatus++;

	}
	else {
		CTime		t = CTime::GetCurrentTime();;
		iYear = t.GetYear();
		iMonth = t.GetMonth();
		iDay = t.GetDay();

		if (iMonth <= pConfig->m_tEPDConfig.iLogSaveCycle) {
			iYear--;
			iMonth = (12 + iMonth - pConfig->m_tEPDConfig.iLogSaveCycle);
		}
		else
			iMonth -= pConfig->m_tEPDConfig.iLogSaveCycle;

		sprintf(szDate, "%04.04d%02.02d%02.02d", iYear, iMonth, iDay);

		rv = m_RunCFGLog_filefind.FindNextFile();
		szFileName = m_RunCFGLog_filefind.GetFileName();
		strncpy(szComp, ((LPCSTR)szFileName) + 4, 8);
		szComp[8] = '\0';
		if (strncmp(szDate, szComp, 8) > 0) {
			szFileName = m_RunCFGLog_filefind.GetFilePath();
			CFile::Remove((LPCSTR)szFileName);
		}
		if (rv == FALSE) {
			m_RunCFGLog_filefind.Close();
			m_iDel_RunCFGLogStatus = 0;
		}
	}
}
#endif
BOOL CWaveEndPointApp::CheckAccessLevel(ACCESSLEVEL level)
{
	ACCESSLEVEL Oldlevel;
	Oldlevel = m_pAccessLevel->GetCurrLevel();
	if (Oldlevel >= level)
		return(TRUE);

	((CMainFrame*)m_pMainWnd)->m_wndView.m_pDlgHeadPage->LogIn();
	Oldlevel = m_pAccessLevel->GetCurrLevel();
	if (Oldlevel >= level)
		return(TRUE);
	return(FALSE);
}
void CWaveEndPointApp::ButtonPageUpdate()
{
	((CMainFrame*)m_pMainWnd)->m_wndView.m_pButtonPage->DisplayUpdate();
}
void CWaveEndPointApp::AutoLogOff_Start()
{
	m_iLogOn = TRUE;
	m_tLogOnTime = CTime::GetCurrentTime();
}
void CWaveEndPointApp::AutoLogOff_Stop()
{
	m_iLogOn = FALSE;
}
void CWaveEndPointApp::CheckRowLog()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch (m_RowLogDeleteFunc) {
	case FUNC_ROWLOG_CALCMONTH:
		RawLog_CalcDate();
		break;
	case FUNC_ROWLOG_FIND:
		RowLog_FindOld1stLog();
		break;
	case FUNC_ROWLOG_NEXT:
		RowLog_FindOldNextLog();
		break;
	}/* switch */
}

void CWaveEndPointApp::RawLog_CalcDate()
{
	CEPDConfig* pConfig = ((CWaveEndPointApp*)AfxGetApp())->GetEPDConfig();

	CTime		t = CTime::GetCurrentTime();
	CTimeSpan	t1((pConfig->m_tEPDConfig.iLogSaveCycle * 30), 0, 0, 0);
	t -= t1;

	m_szCalcDate.Format("%04.4d%02.2d%02.2d", t.GetYear(), t.GetMonth(), t.GetDay());

	// 古い月のサーチ処理へ
	m_RowLogDeleteFunc = FUNC_ROWLOG_FIND;
}

void CWaveEndPointApp::RowLog_FindOld1stLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	szFileName = _T("RowWaveLog\\*.*");

	if (m_RowLogfilefind.FindFile(szFileName) == FALSE) {
		m_RowLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
		return;
	}
	rv = m_RowLogfilefind.FindNextFileA();
	if (m_RowLogfilefind.IsDirectory()) {
		m_RowLogDeleteFunc = FUNC_ROWLOG_NEXT;
		return;
	}
	else {
		szFileName = m_RowLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szCalcDate)) {
			szFileName = m_RowLogfilefind.GetFilePath();
			CFile::Remove(szFileName);

		}
	}
	if (rv == FALSE) {
		m_RowLogfilefind.Close();
		m_RowLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_RowLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
void CWaveEndPointApp::RowLog_FindOldNextLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	rv = m_RowLogfilefind.FindNextFileA();
	if (m_RowLogfilefind.IsDirectory()) {
		return;
	}
	else {
		szFileName = m_RowLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szCalcDate)) {
			szFileName = m_RowLogfilefind.GetFilePath();
			CFile::Remove(szFileName);
		}
	}
	if (rv == FALSE) {
		m_RowLogfilefind.Close();
		m_RowLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_RowLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
void CWaveEndPointApp::CheckEthNetLog()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch (m_EthNetLogDeleteFunc) {
	case FUNC_ROWLOG_CALCMONTH:
		EthNetLog_CalcDate();
		break;
	case FUNC_ROWLOG_FIND:
		EthNetLog_FindOld1stLog();
		break;
	case FUNC_ROWLOG_NEXT:
		EthNetLog_FindOldNextLog();
		break;
	}/* switch */
}

void CWaveEndPointApp::EthNetLog_CalcDate()
{

	CTime	t = CTime::GetCurrentTime();
	CTimeSpan	t1(30, 0, 0, 0);
	t -= t1;

	m_szEthNetCalcDate.Format("%04.4d%02.2d%02.2d", t.GetYear(), t.GetMonth(), t.GetDay());

	// 古い月のサーチ処理へ
	m_EthNetLogDeleteFunc = FUNC_ROWLOG_FIND;
}

void CWaveEndPointApp::EthNetLog_FindOld1stLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	szFileName = _T("ETH_LOG\\*.*");

	if (m_EthNetLogfilefind.FindFile(szFileName) == FALSE) {
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
		return;
	}
	rv = m_EthNetLogfilefind.FindNextFileA();
	if (m_EthNetLogfilefind.IsDirectory()) {
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_NEXT;
		return;
	}
	else {
		szFileName = m_EthNetLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szEthNetCalcDate)) {
			szFileName = m_EthNetLogfilefind.GetFilePath();
			CFile::Remove(szFileName);

		}
	}
	if (rv == FALSE) {
		m_EthNetLogfilefind.Close();
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
void CWaveEndPointApp::EthNetLog_FindOldNextLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	rv = m_EthNetLogfilefind.FindNextFileA();
	if (m_EthNetLogfilefind.IsDirectory()) {
		return;
	}
	else {
		szFileName = m_EthNetLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szEthNetCalcDate)) {
			szFileName = m_EthNetLogfilefind.GetFilePath();
			CFile::Remove(szFileName);
		}
	}
	if (rv == FALSE) {
		m_EthNetLogfilefind.Close();
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_EthNetLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
void CWaveEndPointApp::CheckPLCLog()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch (m_PLCLogDeleteFunc) {
	case FUNC_ROWLOG_CALCMONTH:
		PLCLog_CalcDate();
		break;
	case FUNC_ROWLOG_FIND:
		PLCLog_FindOld1stLog();
		break;
	case FUNC_ROWLOG_NEXT:
		PLCLog_FindOldNextLog();
		break;
	}/* switch */
}

void CWaveEndPointApp::PLCLog_CalcDate()
{

	CTime	t = CTime::GetCurrentTime();
	CTimeSpan	t1(30, 0, 0, 0);
	t -= t1;

	m_szPLCCalcDate.Format("%04.4d%02.2d%02.2d", t.GetYear(), t.GetMonth(), t.GetDay());

	// 古い月のサーチ処理へ
	m_PLCLogDeleteFunc = FUNC_ROWLOG_FIND;
}

void CWaveEndPointApp::PLCLog_FindOld1stLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	szFileName = _T("PLC_COMM_LOG\\*.*");

	if (m_PLCLogfilefind.FindFile(szFileName) == FALSE) {
		m_PLCLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
		return;
	}
	rv = m_PLCLogfilefind.FindNextFileA();
	if (m_PLCLogfilefind.IsDirectory()) {
		m_PLCLogDeleteFunc = FUNC_ROWLOG_NEXT;
		return;
	}
	else {
		szFileName = m_PLCLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szPLCCalcDate)) {
			szFileName = m_PLCLogfilefind.GetFilePath();
			CFile::Remove(szFileName);

		}
	}
	if (rv == FALSE) {
		m_PLCLogfilefind.Close();
		m_PLCLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_PLCLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
void CWaveEndPointApp::PLCLog_FindOldNextLog()
{
	CString			szFileName('\0', 128);
	BOOL			bFound = FALSE;
	int				rv;
	CString			szDate('\0', 16);

	rv = m_PLCLogfilefind.FindNextFileA();
	if (m_PLCLogfilefind.IsDirectory()) {
		return;
	}
	else {
		szFileName = m_PLCLogfilefind.GetFileName();
		szDate = szFileName.Left(8);
		if (atoi((LPCSTR)szDate) <= atoi((LPCSTR)m_szPLCCalcDate)) {
			szFileName = m_PLCLogfilefind.GetFilePath();
			CFile::Remove(szFileName);
		}
	}
	if (rv == FALSE) {
		m_PLCLogfilefind.Close();
		m_PLCLogDeleteFunc = FUNC_ROWLOG_CALCMONTH;
	}
	else
		m_PLCLogDeleteFunc = FUNC_ROWLOG_NEXT;

	return;
}
