#pragma once
// EditDouble.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// Dummy �E�B���h�E

class CEditDouble : public CEdit
{
// �R���X�g���N�V����
public:
	CEditDouble();
	virtual void SetScale(double dMin,double dMax);

// �A�g���r���[�g
public:
	int		m_bSelect;
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	int iDecPt;
	int iMinusFlg;
	int iMinusChk;
	int iPaintFlg;
	CString stDt;
	double	m_dMin;
	double	m_dMax;


	LOGFONT m_logfont;
	CFont * m_pFont;


// �C���v�������e�[�V����
public:
	virtual ~CEditDouble();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
};

class CEditInteger : public CEditDouble
{
// �R���X�g���N�V����
public:
	CEditInteger();
	virtual void SetScale(int iMin,int iMax);

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	int iDecPt;
	int iMinusFlg;
	int iMinusChk;
	int iPaintFlg;
	CString stDt;
	int	m_iMin;
	int	m_iMax;


// �C���v�������e�[�V����
public:
	virtual ~CEditInteger();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};
class CEditEnum : public CEditDouble
{
// �R���X�g���N�V����
public:
	CEditEnum();
	virtual void SetScale(int iMin,int iMax,char *pEnum[]);

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	CString stDt;
	int	m_iMin;
	int	m_iMax;
	char **m_pszEnumStr;


// �C���v�������e�[�V����
public:
	virtual ~CEditEnum();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditDouble)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};
class CEditString : public CEditDouble
{
// �R���X�g���N�V����
public:
	CEditString();
	virtual void SetScale(int iMaxLen);

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDouble)
	//}}AFX_VIRTUAL

private:
	CString stDt;
	int	m_iMaxLen;


// �C���v�������e�[�V����
public:
	virtual ~CEditString();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditString)
	virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnEnKillfocus();
	virtual afx_msg void OnEnSetfocus();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

