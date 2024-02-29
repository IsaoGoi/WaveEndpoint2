#ifndef EXTTHREAD_H
#define EXTTHREAD_H
#include <AFXMT.H>

/*
** ワーカースレッド(簡易版)
*/
class  CExtThread
{
public:
	CExtThread();
	virtual	~CExtThread();

	BOOL	CreateThread(UINT StackSize,UINT nMilliSecs,int nPriority = THREAD_PRIORITY_NORMAL);	/* ｽﾚｯﾄﾞの起動		*/
	void	Stop()	{m_bEndThread=TRUE;};					/* ｽﾚｯﾄﾞ停止要求	*/

	int		m_iMemError;
	int		m_bEnded;

protected:
	CEvent*	m_pWorkEvent;									/* 周期起動用ｲﾍﾞﾝﾄ	*/
	CEvent*	m_pExitEvent;									/* ｽﾚｯﾄﾞ終了ｲﾍﾞﾝﾄ	*/
	int		m_nCycleTime;									/* ｽﾚｯﾄﾞ周期時間	*/
	int		m_bEndThread;									/* ｽﾚｯﾄﾞ終了要求ﾌﾗｸﾞ*/
	HANDLE  m_hThread;										/* ｽﾚｯﾄﾞﾊﾝﾄﾞﾙ		*/

	virtual	void StartWork() {}								/* Start Up(Over ride)*/
	virtual	void DoWork() {}								/* ｽﾚｯﾄﾞ処理(Over ride)*/
	virtual	void EndWork() {}								/* 後処理  (Over ride)*/

public:
	void	KillThread();									/* ｽﾚｯﾄﾞ終了要求	*/
	int		Run();											/* ｽﾚｯﾄﾞ処理		*/
};
#endif

