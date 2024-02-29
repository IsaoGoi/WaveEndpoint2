
#include	"pch.h"
#include	<afxmt.h>
#include	<process.h>
#include	"Extthread.h"

/*
**==============================================================================**
**																				**
**		File Name	:	ExtThread.CPP											**
**																				**
**		Function	:	ワーカースレッドClass (簡易スレッド)					**
**																				**
**		クラス名	:	Class CExtThread											**
**																				**
**		メンバ関数	:	CExtThread();				 ｺﾝｽﾄﾗｸﾀ					**
**						virtual	~CExtThread();		 ﾃﾞｸﾗｽﾀ						**
**						BOOL	CreateThread(); 	 ｽﾚｯﾄﾞCreate				**
**													 ｽﾚｯﾄﾞの起動				**
**						void	Stop();				 ｽﾚｯﾄﾞ停止要求				**
**						virtual	void StartWork()	 Start Up(Over ride)		**
**						virtual	void DoWork();		 ｽﾚｯﾄﾞ処理(Over ride)		**
**						virtual	void EndWork();		 後処理  (Over ride)		**
**						void	KillThread();		 ｽﾚｯﾄﾞ終了要求				**
**						int		Run();				 ｽﾚｯﾄﾞ処理					**
**																				**
**==============================================================================**
*/

static	unsigned int __stdcall Start(void *pv);


/*
**------------------------------------------------------------**
**
**	Module		:	CExtThread();
**
**	Function	:	CExtThreadｸﾗｽ ｺﾝｽﾄﾗｸﾀ
**
**	入力		:	なし
**
**	出力		:	なし
**
**------------------------------------------------------------**
*/
CExtThread::CExtThread()
{
	/*
	** ｽﾚｯﾄﾞ終了同期ｲﾍﾞﾝﾄ作成
	*/
	m_pExitEvent = new CEvent(TRUE,TRUE);
//	m_pExitEvent = new CEvent();
	ASSERT(m_pExitEvent);

	/*
	** ｽﾚｯﾄﾞ周期起動用ｲﾍﾞﾝﾄ作成
	*/
	m_pWorkEvent = new CEvent();
	ASSERT(m_pWorkEvent);

	m_iMemError = FALSE;
	m_bEnded = FALSE;
}
/*
**------------------------------------------------------------**
**
**	Module		:	~CExtThread();
**
**	Function	:	CExtThreadｸﾗｽ ﾃﾞｸﾗｸﾀ
**
**	入力		:	なし
**
**	出力		:	なし
**
**------------------------------------------------------------**
*/
CExtThread::~CExtThread()
{
	// <#990407>
//	Sleep(5000);

	/*
	** ｽﾚｯﾄﾞ周期起動用ｲﾍﾞﾝﾄ削除
	*/
	delete m_pWorkEvent;

	/*
	** ｽﾚｯﾄﾞ終了同期ｲﾍﾞﾝﾄ削除
	*/
	delete m_pExitEvent;

}
/*
**------------------------------------------------------------**
**
**	Module		:	BOOL CExtThread::CreateThread(UINT StackSize,
**												 UINT nMilliSecs)
**
**	Function	:	ﾜｰｶｽﾚｯﾄﾞ生成処理
**
**	入力		:	UINT	StackSize	... ｽﾀｯｸｻｲｽﾞ
**					UINT	nMilliSecs	... 周期起動時間
**
**	出力		:	TRUE  = 正常終了
**					FALSE = 異常終了
**
**------------------------------------------------------------**
*/
BOOL CExtThread::CreateThread(UINT StackSize,UINT nMilliSecs,int nPriority)
{
	/*
	** ｽﾚｯﾄﾞ終了要求ﾌﾗｸﾞReset
	*/
	m_bEndThread =FALSE;

	/*
	** ｽﾚｯﾄﾞ周期起動時間待避
	*/
	m_nCycleTime = nMilliSecs;

	/*
	** ｽﾚｯﾄﾞ終了ｲﾍﾞﾝﾄ Reset
	*/
	m_pExitEvent->ResetEvent();


	/*
	** ｽﾚｯﾄﾞCreate
	*/
	unsigned usThreadAddr;
	m_hThread =	reinterpret_cast<HANDLE>
		(_beginthreadex(NULL,StackSize,Start,this,1,&usThreadAddr));

	if(nPriority != THREAD_PRIORITY_NORMAL)
	{
		SetThreadPriority(m_hThread,nPriority);
	}
	return reinterpret_cast<unsigned long>(m_hThread);
	
}
/*
**------------------------------------------------------------**
**
**	Module		:	void CExtThread::KillThread()
**
**	Function	:	ｽﾚｯﾄﾞ終了処理
**
**	入力		:	なし
**
**	出力		:	なし
**
**------------------------------------------------------------**
*/
void CExtThread::KillThread()
{
	/*
	** ｽﾚｯﾄﾞ終了要求ﾌﾗｸﾞSet
	*/
	m_bEndThread =TRUE;

	/*
	** ｽﾚｯﾄﾞ起動Event Set
	*/
	m_pWorkEvent->SetEvent();

	/*
	** ｽﾚｯﾄﾞ終了待ち
	*/
	CSingleLock csl(m_pExitEvent);
	csl.Lock();
	csl.Unlock();
	Sleep(100);
}
/*
**------------------------------------------------------------**
**
**	Module		:	int CExtThread::Run()
**
**	Function	:	ｽﾚｯﾄﾞ処理ﾓｼﾞｭｰﾙ
**
**	入力		:	なし
**
**	出力		:	0 Fixed
**
**------------------------------------------------------------**
*/
int CExtThread::Run()
{

	/*
	** ｽﾚｯﾄﾞ周期起動ｲﾍﾞﾝﾄ Setup
	*/
	CSingleLock csl(m_pWorkEvent);


	/*
	** ｽﾚｯﾄﾞStart Up 処理
	*/
	Sleep(200);
	StartWork();

	/*
	** ｽﾚｯﾄﾞLoop
	*/
	while(!m_bEndThread)
	{
		/*
		** 周期時間Sleep
		*/
		csl.Lock(m_nCycleTime);
		csl.Unlock();
		if(!m_bEndThread){

#if 0 //_DEBUG    
	CMemoryState oldMemState, newMemState, diffMemState;
    oldMemState.Checkpoint();
#endif

		DoWork();


#if 0 //_DEBUG
    newMemState.Checkpoint();
    if( diffMemState.Difference( oldMemState, newMemState ) )    {
        TRACE( "SaveProcessData Memory leaked!\n" );    
		diffMemState.DumpStatistics();
		oldMemState.DumpAllObjectsSince();
	}
#endif
		}
	}/* while */
	
	/*
	** ｽﾚｯﾄﾞ後処理
	*/
	EndWork();
	m_bEnded = TRUE;

	/*
	** ｽﾚｯﾄﾞﾊﾝﾄﾞﾙ解放
	*/
	CloseHandle(m_hThread);

	/*
	** ｽﾚｯﾄﾞ終了ｲﾍﾞﾝﾄ通知
	*/
	m_pExitEvent->SetEvent();

	/*
	** ｽﾚｯﾄﾞ終了
	*/
	_endthreadex(0);
	return 0;
}



/*
**------------------------------------------------------------**
**
**	Module		:	unsigned int __stdcall Start(void *pv)
**
**	Function	:	ｽﾚｯﾄﾞ起動ﾓｼﾞｭｰﾙ
**
**	入力		:	void *pv  ... CExtThread Classﾎﾟｲﾝﾀ
**
**	出力		:	0 Fixed
**
**------------------------------------------------------------**
*/
static unsigned int __stdcall Start(void *pv)
{
	CExtThread* pMT = static_cast<CExtThread*>(pv);
	return pMT->Run();
}


