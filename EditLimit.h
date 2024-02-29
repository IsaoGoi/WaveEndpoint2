#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEditDLimit �E�B���h�E

class CEditDLimit : public CEdit
{
// �R���X�g���N�V����
public:
	CEditDLimit();

// �A�g���r���[�g
public:
	void SetSpin(CSpinButtonCtrl* pCtrl){m_pCtrl = pCtrl;};

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditLimit)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetRange(double dMin,double dMax,int iPointColumn);
	void GetRange(double *p_dMin,double *p_dMax) {*p_dMin=m_dMin;*p_dMax=m_dMax;return;}
	virtual ~CEditDLimit();

private:
	int m_iPointColumn;
	double m_dMin;
	double m_dMax;
	CSpinButtonCtrl*	m_pCtrl;

	
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditDLimit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CEditLimit : public CEdit
{
// �R���X�g���N�V����
public:
	CEditLimit();

// �A�g���r���[�g
public:
	void SetSpin(CSpinButtonCtrl* pCtrl){m_pCtrl = pCtrl;};

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditLimit)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetRange(int iMin,int iMax);
	virtual ~CEditLimit();

private:
	int m_iMin;
	int m_iMax;
		CSpinButtonCtrl*	m_pCtrl;

	
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditLimit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/*
** I/O �t�B�[���h�ݒ�p
*/
class CEdit_IO : public CEdit
{
// �R���X�g���N�V����
public:
	CEdit_IO();

// �A�g���r���[�g
public:
	void SetIOType(IO_TYPE iotype) {m_iotype = iotype;}
	void SetIOScale(IO_SCALE *pioscale) {m_ioscale = *pioscale;}

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdit_IO)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CEdit_IO();

private:
	IO_TYPE		m_iotype;
	IO_SCALE	m_ioscale;
	
	
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEdit_IO)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


