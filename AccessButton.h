#pragma once
// AccessButton.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CAccessButton �E�B���h�E

class CAccessButton : public CBitmapButton
{
// �R���X�g���N�V����
public:
	CAccessButton();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
		void PasswordDraw();
		void InputLogOn();

private:
	CStatic	*m_strPassword;								// �߽ܰ������
	CBitmap	*m_passBitmap;								
	CStatic	*m_passString;								
	CString strUserID;
	CString strUserFullName;
	int		iUserLevel;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAccessButton)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CAccessButton();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CAccessButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

