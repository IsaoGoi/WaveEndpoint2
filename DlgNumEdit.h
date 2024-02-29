#if !defined(AFX_DLGNUMEDIT_H__CC8FF1E4_B89F_11D5_9985_0090CC08163E__INCLUDED_)
#define AFX_DLGNUMEDIT_H__CC8FF1E4_B89F_11D5_9985_0090CC08163E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNumEdit.h : �w�b�_�[ �t�@�C��
//
#include "DialogX.h"
#include "EditLimit.h"
#include "UpButton.h"
#include "IOEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNumEdit �_�C�A���O

class CDlgNumEdit : public CDialogX
{
// �R���X�g���N�V����
public:
	BOOL DlgCreate(CWnd *pParent = NULL);
	void StopUpDown();
	void StartUpDown(int iUpDpwn);
	void SetValue(LPCSTR pszVal,IO_TYPE iotype,IO_SCALE *pioscale,CIOEdit *pEdit);
	void GetValue(LPSTR pszVal);
	CDlgNumEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	CIOEdit *GetEditFld() {return(m_pIOEdit);}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgNumEdit)
	enum { IDD = IDD_NUMEDIT };
	CEdit_IO		m_edit_fld;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgNumEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgNumEdit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtn0();
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	afx_msg void OnBtn3();
	afx_msg void OnBtn4();
	afx_msg void OnBtn5();
	afx_msg void OnBtn6();
	afx_msg void OnBtn7();
	afx_msg void OnBtn8();
	afx_msg void OnBtn9();
	afx_msg void OnBtnColon();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnMinus();
	afx_msg void OnBtnMax();
	afx_msg void OnBtnMin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CIOEdit		*m_pIOEdit;
	int			m_iValUpDown;
	int			m_iUpDownCycle;
	IO_TYPE		m_iotype;
	IO_SCALE	m_ioscale;
	CString		m_szEdit;
	void		SpinUp();
	void		SpinDown();
	CUpButton	m_btn_up;
	CDownButton	m_btn_down;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


#endif // !defined(AFX_DLGNUMEDIT_H__CC8FF1E4_B89F_11D5_9985_0090CC08163E__INCLUDED_)

