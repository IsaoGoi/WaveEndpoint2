#pragma once


// CDlgWaveRef ダイアログ

class CDlgWaveRef : public CDialogX
{
	DECLARE_DYNAMIC(CDlgWaveRef)

public:
	CDlgWaveRef(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgWaveRef();

	void ShowDlgWaveRef();
	void HideDlgWaveRef();
	CIODB* m_pIODB;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVEREFERENCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK() { return; }
	virtual void OnCancel();
	CIOEdit m_io_RefStartLevel;
	CIOEdit m_io_RefStartTime;
	CIOEdit m_io_RefEndLevel;
	CIOEdit m_io_RefEndTime;
	CIOEdit m_io_RefPeakLevel;
	CIOEdit m_io_RefPeakTime;
	CIOEdit m_io_RefMinLevel;
	CIOEdit m_io_RefMinTime;
	CIOEdit m_io_RefDiffLevel;
};
