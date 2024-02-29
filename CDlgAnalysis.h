#pragma once


#include	"VListct.h"

// CDlgAnalysis ダイアログ
#if 0
typedef struct {
	unsigned short usData[EPD_WAVE_TOTAL];
}SAMPLE_WAVE;
#endif


class CDlgAnalysis : public CDialogX
{
	DECLARE_DYNAMIC(CDlgAnalysis)

public:
	CDlgAnalysis(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgAnalysis();

	void	DisplaySamplingList();

	CImageList* m_pImage;
	CEPDConfig* m_pConfig;
	CIODB*		m_pIODB;
	SCROLLINFO	m_tScrollInfo;
	CRect		m_Rect_WaveGraph;
	POINT		m_pGraph[EPD_WAVE_TOTAL + 3];
	int			m_iSamplingTime;
	int			m_iSerchMaxCnt;
	int			m_iSerchDiffCnt;
	unsigned short	m_usMaxData[EPD_WAVE_TOTAL + 1];
	unsigned short	m_usMinData[EPD_WAVE_TOTAL + 1];
	unsigned short	m_usDiffData[EPD_WAVE_TOTAL + 1];
	int				m_iMax_Best10[EPD_WAVE_TOTAL + 1];
	int				m_iDiff_Best10[EPD_WAVE_TOTAL + 1];

	CRect		m_Rect_MaxDiffGraph;
	POINT		m_pWaveGraph[6000+10];

	int			m_iSelectWaveNo;
	int			m_iSelectWaveNo2;

	int			m_iPMNo;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALYSIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK()		{ return; }
	virtual void OnCancel() { return; }
	CTreeCtrl m_Tree_Sampling;
	CScrollBar m_Scroll_WaveLength;
	afx_msg void OnTvnSelchangedTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_szPC;
	CString m_szSamplingTM;
	CString m_szFileName;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	CIOEdit m_io_SerchStartTime;
	CIOEdit m_io_SerchEndTime;
	afx_msg void OnBnClickedBtnSerch();
	afx_msg void OnBnClickedChkMax1()	{ InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkMax2() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkMax3() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkMax4() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkMax5() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkDiff1() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkDiff2() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkDiff3() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkDiff4() { InvalidateRect(m_Rect_MaxDiffGraph); }
	afx_msg void OnBnClickedChkDiff5() { InvalidateRect(m_Rect_MaxDiffGraph); }
	CVListCtrl m_list_wave;
	afx_msg void OnLvnItemchangedListWave(NMHDR* pNMHDR, LRESULT* pResult);
	CVListCtrl m_list_wave2;
	afx_msg void OnLvnItemchangedListWave2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnCsv();
	CIOEdit m_io_SerchStartWave;
	CIOEdit m_io_SerchEndWave;
	CString m_szWaveMin;
	CString m_szWaveMax;
};
