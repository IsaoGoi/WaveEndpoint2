#include "pch.h"
#include "WaveEndPoint.h"
#include "PLCComLog.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPLCComLogThread::CPLCComLogThread()
{
	m_bInitialized = FALSE;
}
/*
**=======================**
**      初期処理
**=======================**
*/
void CPLCComLogThread::StartWork()
{
	int		i,j;
	
	m_iWritePt = 0;
	m_iReadPt  = 0;

	for(i=0;i<PLC_LOG_TBL_MAX;i++){
		m_MsgLog[i].bSet = FALSE;

		m_MsgLog[i].HdlMsg = GlobalAlloc(GPTR,PLC_LOG_BUF_SIZE);
		if(m_MsgLog[i].HdlMsg == NULL){
			// メモリ獲得エラー
			AfxMessageBox(_T("PLC Com Log Buffer Table Memory Allocate Error"));
			for(j=0;j<i;j++){
				GlobalUnlock(m_MsgLog[j].HdlMsg);
				GlobalFree(m_MsgLog[j].HdlMsg);
			}
			return;
		}
		m_MsgLog[i].pMsg = (unsigned char *)GlobalLock(m_MsgLog[i].HdlMsg);
		if(m_MsgLog[i].pMsg == NULL){
			// メモリ獲得エラー
			AfxMessageBox(_T("PLC Com Log Buffer Table Memory Allocate Error"));
			for(j=0;j<i;j++){
				GlobalUnlock(m_MsgLog[j].HdlMsg);
				GlobalFree(m_MsgLog[j].HdlMsg);
			}
			return;
		}
	}/* for */
	m_bInitialized = TRUE;
	InitializeCriticalSection(&m_LogSetInterlock);

}
/*
**=======================**
**  スレッド終了処理
**=======================**
*/
void CPLCComLogThread::EndWork()
{
	int	i;

	if(m_bInitialized){
		for(i=0;i<PLC_LOG_TBL_MAX;i++){
			GlobalUnlock(m_MsgLog[i].HdlMsg);
			GlobalFree(m_MsgLog[i].HdlMsg);
		}/* for */
	}
	m_bInitialized = FALSE;
	DeleteCriticalSection(&m_LogSetInterlock);

}
/*
**=======================**
**  スレッドメイン処理
**=======================**
*/
void CPLCComLogThread::DoWork()
{
	int	i;
	for(i=0;i<2;i++){
		if(m_MsgLog[m_iReadPt].bSet == TRUE){
			OutPutLog();
			m_MsgLog[m_iReadPt].bSet= FALSE;
			if(++m_iReadPt == PLC_LOG_TBL_MAX)
				m_iReadPt = 0;
		}
		else
			break;
	}/*　for */
}
/*
**=======================**
**  ログバッファキューイン
**=======================**
*/
void CPLCComLogThread::SetLogQueue(int iPC,BOOL bRecv,PLCMSGTYPE Type,int iLen,unsigned char *pMsg)
{
	int		iLength;

	SYSTEMTIME	t_time;
	
	if(m_bInitialized == FALSE)
		return;


	// Log Message Full
	if(m_MsgLog[m_iWritePt].bSet == TRUE){
		return;
	}

	EnterCriticalSection(&m_LogSetInterlock);

	if(iLen >= PLC_LOG_BUF_SIZE)
		iLength = PLC_LOG_BUF_SIZE-1;
	else
		iLength = iLen;
		
	m_MsgLog[m_iWritePt].iPC  = iPC;
	m_MsgLog[m_iWritePt].bRecv  = bRecv;
	m_MsgLog[m_iWritePt].MsgType= Type;
	m_MsgLog[m_iWritePt].iLen   = iLength;
	memcpy((char *)m_MsgLog[m_iWritePt].pMsg,(char *)pMsg,iLength);

	//　日付時刻をセット
	GetLocalTime(&t_time);
	sprintf_s(m_MsgLog[m_iWritePt].szDateTime,sizeof(m_MsgLog[m_iWritePt].szDateTime),"%04.04d/%02.02d/%02.02d-%02.02d:%02.02d:%02.02d",
			t_time.wYear,
			t_time.wMonth,
			t_time.wDay,
			t_time.wHour,
			t_time.wMinute,
			t_time.wSecond);

	m_MsgLog[m_iWritePt].bSet= TRUE;

	// 書き込みﾎﾟｲﾝﾀ更新
	if(++m_iWritePt == PLC_LOG_TBL_MAX)
		m_iWritePt = 0;

	LeaveCriticalSection(&m_LogSetInterlock);

}

static char *g_szSendRecv[]={
/* 0 */ "Send",
/* 1 */ "Recv",
/* 2 */	"Unknown",
};
static char *g_szMsgType[]={
/* 0 */ "Command",
/* 1 */ "Reply",
/* 2 */ "Event",
/* 3 */ "E-Reply",
/* 4 */	"Unknown",
};


void CPLCComLogThread::OutPutLog()
{
	char		chData[16];
	int			i;
	int			iLen;

	if(CFile::GetStatus(PLC_LOG_FILE_NAME,m_FileStatus) == TRUE){
		// Size Check
		if(m_FileStatus.m_size > PLC_LOG_FILE_SIZE){
			ChangeLogFile();
		}
	}

	if(m_stdFile.Open(PLC_LOG_FILE_NAME,CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText) == 0){
		return;
	}
	m_stdFile.SeekToEnd();

	m_szOutLogBuf.Format("%s,PC%d,%s,%s,%s\n",
						m_MsgLog[m_iReadPt].szDateTime,
						(m_MsgLog[m_iReadPt].iPC+1),
						g_szSendRecv[m_MsgLog[m_iReadPt].bRecv],
						g_szMsgType[m_MsgLog[m_iReadPt].MsgType],
						m_MsgLog[m_iReadPt].pMsg);
	m_stdFile.WriteString((LPCSTR)m_szOutLogBuf);
	m_stdFile.Close();
}
void CPLCComLogThread::ChangeLogFile()
{
	BOOL		bFolder = TRUE;
	CString		szFileName('\0',64);
	SYSTEMTIME	t_time;

	CString		szFileName1('\0',16);
	CString		szFileName2('\0',64);

	// Log Folder存在チェック
	CFileFind	filefind;
	szFileName.Format("%s",PLC_LOG_FILE_SAVEPATH);
	if(filefind.FindFile((LPCSTR)szFileName)== FALSE){
		if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE)
			bFolder = FALSE;
	}
	else{
		filefind.FindNextFile();
		if(filefind.IsDirectory()==FALSE){
			if(CreateDirectory((LPCSTR)szFileName,NULL) == FALSE)
				bFolder = FALSE;
		}
		filefind.Close();
	}
	szFileName1.Format("%s",PLC_LOG_FILE_NAME);
	if(bFolder == TRUE){
		//　日付時刻をセット
		GetLocalTime(&t_time);
		szFileName2.Format("%s\\%04.04d%02.02d%02.02d%02.02d%02.02d%02.02d.TXT",
			PLC_LOG_FILE_SAVEPATH,
			t_time.wYear,
			t_time.wMonth,
			t_time.wDay,
			t_time.wHour,
			t_time.wMinute,
			t_time.wSecond);				// 2005/09/22 秒を追加(Applicaton Error不具合の対応)

		CFile::Rename((LPCSTR)szFileName1,(LPCSTR)szFileName2);
	}
	else
		CFile::Remove((LPCSTR)szFileName1);
}

