#pragma once


class CToolTipGraph : public CWnd
{
// �R���X�g���N�V����
public:
	CToolTipGraph(CWnd *pParent);

// �A�g���r���[�g
public:
	char	szTime[32 +1];
	char	szData[32 +1];
	char	szRow[32 + 1];
	char	szDiff[32 + 1];


// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CToolTipCTCGlass)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CToolTipGraph();
	void	Active(CPoint point);
	void	Deactive();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CToolTipGraph)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CRect	m_WinRect;
	CFont	m_Font;
	LOGFONT	m_logfont;
};

