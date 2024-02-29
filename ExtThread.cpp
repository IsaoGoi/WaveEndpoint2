
#include	"pch.h"
#include	<afxmt.h>
#include	<process.h>
#include	"Extthread.h"

/*
**==============================================================================**
**																				**
**		File Name	:	ExtThread.CPP											**
**																				**
**		Function	:	���[�J�[�X���b�hClass (�ȈՃX���b�h)					**
**																				**
**		�N���X��	:	Class CExtThread											**
**																				**
**		�����o�֐�	:	CExtThread();				 �ݽ�׸�					**
**						virtual	~CExtThread();		 �޸׽�						**
**						BOOL	CreateThread(); 	 �گ��Create				**
**													 �گ�ނ̋N��				**
**						void	Stop();				 �گ�ޒ�~�v��				**
**						virtual	void StartWork()	 Start Up(Over ride)		**
**						virtual	void DoWork();		 �گ�ޏ���(Over ride)		**
**						virtual	void EndWork();		 �㏈��  (Over ride)		**
**						void	KillThread();		 �گ�ޏI���v��				**
**						int		Run();				 �گ�ޏ���					**
**																				**
**==============================================================================**
*/

static	unsigned int __stdcall Start(void *pv);


/*
**------------------------------------------------------------**
**
**	Module		:	CExtThread();
**
**	Function	:	CExtThread�׽ �ݽ�׸�
**
**	����		:	�Ȃ�
**
**	�o��		:	�Ȃ�
**
**------------------------------------------------------------**
*/
CExtThread::CExtThread()
{
	/*
	** �گ�ޏI����������č쐬
	*/
	m_pExitEvent = new CEvent(TRUE,TRUE);
//	m_pExitEvent = new CEvent();
	ASSERT(m_pExitEvent);

	/*
	** �گ�ގ����N���p����č쐬
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
**	Function	:	CExtThread�׽ �޸׸�
**
**	����		:	�Ȃ�
**
**	�o��		:	�Ȃ�
**
**------------------------------------------------------------**
*/
CExtThread::~CExtThread()
{
	// <#990407>
//	Sleep(5000);

	/*
	** �گ�ގ����N���p����č폜
	*/
	delete m_pWorkEvent;

	/*
	** �گ�ޏI����������č폜
	*/
	delete m_pExitEvent;

}
/*
**------------------------------------------------------------**
**
**	Module		:	BOOL CExtThread::CreateThread(UINT StackSize,
**												 UINT nMilliSecs)
**
**	Function	:	ܰ��گ�ސ�������
**
**	����		:	UINT	StackSize	... ��������
**					UINT	nMilliSecs	... �����N������
**
**	�o��		:	TRUE  = ����I��
**					FALSE = �ُ�I��
**
**------------------------------------------------------------**
*/
BOOL CExtThread::CreateThread(UINT StackSize,UINT nMilliSecs,int nPriority)
{
	/*
	** �گ�ޏI���v���׸�Reset
	*/
	m_bEndThread =FALSE;

	/*
	** �گ�ގ����N�����ԑҔ�
	*/
	m_nCycleTime = nMilliSecs;

	/*
	** �گ�ޏI������� Reset
	*/
	m_pExitEvent->ResetEvent();


	/*
	** �گ��Create
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
**	Function	:	�گ�ޏI������
**
**	����		:	�Ȃ�
**
**	�o��		:	�Ȃ�
**
**------------------------------------------------------------**
*/
void CExtThread::KillThread()
{
	/*
	** �گ�ޏI���v���׸�Set
	*/
	m_bEndThread =TRUE;

	/*
	** �گ�ދN��Event Set
	*/
	m_pWorkEvent->SetEvent();

	/*
	** �گ�ޏI���҂�
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
**	Function	:	�گ�ޏ���Ӽޭ��
**
**	����		:	�Ȃ�
**
**	�o��		:	0 Fixed
**
**------------------------------------------------------------**
*/
int CExtThread::Run()
{

	/*
	** �گ�ގ����N������� Setup
	*/
	CSingleLock csl(m_pWorkEvent);


	/*
	** �گ��Start Up ����
	*/
	Sleep(200);
	StartWork();

	/*
	** �گ��Loop
	*/
	while(!m_bEndThread)
	{
		/*
		** ��������Sleep
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
	** �گ�ތ㏈��
	*/
	EndWork();
	m_bEnded = TRUE;

	/*
	** �گ������ى��
	*/
	CloseHandle(m_hThread);

	/*
	** �گ�ޏI������Ēʒm
	*/
	m_pExitEvent->SetEvent();

	/*
	** �گ�ޏI��
	*/
	_endthreadex(0);
	return 0;
}



/*
**------------------------------------------------------------**
**
**	Module		:	unsigned int __stdcall Start(void *pv)
**
**	Function	:	�گ�ދN��Ӽޭ��
**
**	����		:	void *pv  ... CExtThread Class�߲��
**
**	�o��		:	0 Fixed
**
**------------------------------------------------------------**
*/
static unsigned int __stdcall Start(void *pv)
{
	CExtThread* pMT = static_cast<CExtThread*>(pv);
	return pMT->Run();
}


