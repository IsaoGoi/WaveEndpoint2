#pragma once
// AlarmPopup.h : �w�b�_�[ �t�@�C��
//
/////////////////////////////////////////////////////////////////////////////
// CAlarmPopup �E�B���h�E

class CAlarmPopup : public CBitmapButton
{
// �R���X�g���N�V����
public:
	CAlarmPopup();

	CDlgAlarm	*m_pDlgAlarm;
// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAlarmPopup)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CAlarmPopup();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CAlarmPopup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int	m_bReverse;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

