// AlarmPopup.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "AlarmPopup.h"


/////////////////////////////////////////////////////////////////////////////
// CAlarmPopup

CAlarmPopup::CAlarmPopup()
{
	m_pDlgAlarm = NULL;
	m_bReverse = FALSE;
}

CAlarmPopup::~CAlarmPopup()
{
}


BEGIN_MESSAGE_MAP(CAlarmPopup, CButton)
	//{{AFX_MSG_MAP(CAlarmPopup)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmPopup ���b�Z�[�W �n���h��

int CAlarmPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*
	** Alarm Popup Button
	*/
	LoadBitmaps(IDB_ALARM_FOCUS,IDB_ALARM_PUSH,IDB_ALARM_FOCUS,IDB_ALARM_DISABLE);

	SetTimer(1,1000,NULL);

	return 0;
}

void CAlarmPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pDlgAlarm = ((CWaveEndPointApp*)AfxGetApp())->GetDlgAlarm();

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if(m_pDlgAlarm->IsWindowVisible() != TRUE)
		m_pDlgAlarm->ShowWindow(SW_SHOW);
	else
		m_pDlgAlarm->ShowWindow(SW_HIDE);
	CButton::OnLButtonUp(nFlags, point);
}
void CAlarmPopup::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowEnabled() == TRUE){
		if(m_bReverse){
			LoadBitmaps(IDB_ALARM_ACT,IDB_ALARM_PUSH,IDB_ALARM_ACT,IDB_ALARM_DISABLE);
			m_bReverse = FALSE;
		}
		else{
			LoadBitmaps(IDB_ALARM_FOCUS,IDB_ALARM_PUSH,IDB_ALARM_FOCUS,IDB_ALARM_DISABLE);
			m_bReverse = TRUE;
		}
		Invalidate(TRUE);
	}
	CBitmapButton::OnTimer(nIDEvent);
}
