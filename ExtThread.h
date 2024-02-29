#ifndef EXTTHREAD_H
#define EXTTHREAD_H
#include <AFXMT.H>

/*
** ���[�J�[�X���b�h(�ȈՔ�)
*/
class  CExtThread
{
public:
	CExtThread();
	virtual	~CExtThread();

	BOOL	CreateThread(UINT StackSize,UINT nMilliSecs,int nPriority = THREAD_PRIORITY_NORMAL);	/* �گ�ނ̋N��		*/
	void	Stop()	{m_bEndThread=TRUE;};					/* �گ�ޒ�~�v��	*/

	int		m_iMemError;
	int		m_bEnded;

protected:
	CEvent*	m_pWorkEvent;									/* �����N���p�����	*/
	CEvent*	m_pExitEvent;									/* �گ�ޏI�������	*/
	int		m_nCycleTime;									/* �گ�ގ�������	*/
	int		m_bEndThread;									/* �گ�ޏI���v���׸�*/
	HANDLE  m_hThread;										/* �گ�������		*/

	virtual	void StartWork() {}								/* Start Up(Over ride)*/
	virtual	void DoWork() {}								/* �گ�ޏ���(Over ride)*/
	virtual	void EndWork() {}								/* �㏈��  (Over ride)*/

public:
	void	KillThread();									/* �گ�ޏI���v��	*/
	int		Run();											/* �گ�ޏ���		*/
};
#endif

