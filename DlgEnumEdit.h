#pragma once
//
#include "DialogX.h"
#include "IOEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit �_�C�A���O

class CDlgEnumEdit : public CDialogX
{
// �R���X�g���N�V����
public:
	BOOL DlgCreate(CWnd* pParent = NULL);
	CDlgEnumEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void SetValue(unsigned uint_val,LPSTR pEnumString[],CIOEdit *pEdit);
	void GetValue(unsigned *uint_val);
	CIOEdit *GetEditFld() {return(m_pIOEdit);}

	// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgEnumEdit)
	enum { IDD = IDD_ENUMEDIT };
	CListBox	m_list_EnumStr;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgEnumEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgEnumEdit)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CIOEdit		*m_pIOEdit;
	char m_szEnumString[32][64];
	unsigned m_uint_Val;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

