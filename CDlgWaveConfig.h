#pragma once
#include	"EditDouble.h"


#define	EDITFLD_Y_MAX	25
#define	EDITFLD_X_MAX	6

#define	PAGE_DATA_MAX	75

// CDlgWaveConfig ダイアログ

class CDlgWaveConfig : public CDialogX
{
	DECLARE_DYNAMIC(CDlgWaveConfig)

public:
	CDlgWaveConfig(CWnd* pParent = nullptr,int iPM=PM1);   // 標準コンストラクター
	virtual ~CDlgWaveConfig();

	void	ModifyCheck();

	
	int		m_iRightTopMax;

	CBrush		m_brushBack;
	COLORREF	m_colText;
	CEPDConfig* m_pConfig;
	int			m_iPMNo;

	void	SetTitle();
	void	SetData();
	void	GetData();
	int		GetFldXY(int* px, int* py);

	BOOL	m_bFldEnable[PAGE_DATA_MAX + 1];
	int		m_iFldCnt;
	int		m_iDataMax;
	int		m_iPageTopNo;
	WAVE_CONFIG		m_tEditWaveData;
	CEditDouble* m_pEditData[2][PAGE_DATA_MAX];

	int		m_iCurrX;
	int		m_iCurrY;


	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVECONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnKeyUp();
	afx_msg void OnKeyDown();
	afx_msg void OnKeyLeft();
	afx_msg void OnKeyRight();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel() { return; }
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedBtnLoadwave();
	afx_msg void OnBnClickedBtnCreate();
};
