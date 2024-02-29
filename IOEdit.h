#pragma once
// IOEdit.h : �w�b�_�[ �t�@�C��
//
// #include "DlgNumEdit.h"
class 	CDlgNumEdit;
class 	CDlgEnumEdit;
class 	CDlgStringEdit;

/////////////////////////////////////////////////////////////////////////////
// CIOEdit �E�B���h�E

class CIOEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CIOEdit();
	void SetEditorPage();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	CDlgNumEdit	*m_DlgNumEdit;
	CDlgEnumEdit *m_DlgEnumEdit;
	CDlgStringEdit *m_DlgStringEdit;

	// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CIOEdit)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void UnselectField();
	void SelectField();
	void SetEnumValue(UINT uval);
	void SetValue(LPCSTR szEdit);
	BOOL ChkValueChange();
	void UpdateDisplay(void);
	int GetIOHdl(void)  {return(m_iIOHdl);};
	BOOL m_bUpdate;
	void SetIOHdl(int iHandle);
	void SetNumEditPage(CDlgNumEdit *pPage);
	virtual ~CIOEdit();
	void SetSpecialColor(int iSet) {m_iSpecialColor = iSet;}
	void SetPasswordFld();
	void SetHexDisplay(int iSet) {m_bHexDisplay = iSet;}

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CIOEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bSelcted;
	CIODB     *m_pIODB;
	int m_iIOHdl;

	IO_TYPE		m_iotype;
	IO_SCALE	m_ioscale;

	char			old_char_val;
	unsigned char	old_uchar_val;
	short			old_short_val;
	unsigned short  old_ushort_val;
	int				old_int_val;
	unsigned		old_uint_val;
	long			old_long_val;
	unsigned long	old_ulong_val;
	float			old_float_val;
	double			old_double_val;
	char			old_string_val[129+1];
	int				m_iSpecialColor;
	int				m_bPassword;
	int				m_bHexDisplay;
public:
	afx_msg void OnEnSetfocus();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

