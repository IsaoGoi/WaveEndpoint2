#pragma once

// DlgUserList.h : �w�b�_�[ �t�@�C��
//
#include "DialogX.h"
#include "VListCt.h"
#include "ioedit.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgUserList �_�C�A���O

class CDlgUserList : public CDialogX
{
// �R���X�g���N�V����
public:
	CDlgUserList(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDlgUserList();
	BOOL ModifyCheck();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgUserList)
	enum { IDD = IDD_USERLIST };
	CVListCtrl	m_list_user;
	CComboBox	m_cmb_level;
	CIOEdit	m_io_ConfirmPassword;
	CIOEdit	m_io_Password;
	CIOEdit	m_io_UserName;
	CIOEdit	m_io_GlsPassword;
	CIOEdit	m_ioAutoLogOff;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgUserList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgUserList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {return;}
	virtual void OnOK();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList * m_img_Status;
	CAccessLevel *m_pAccessLevel;
	CIODB		*m_pIODB;
	int			m_iEditItem;

	void	SetUserList();
public:
//	afx_msg void OnNMClickUselist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedUselist(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
