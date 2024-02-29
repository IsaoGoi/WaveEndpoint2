// DlgEnumEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "pch.h"
#include "WaveEndPoint.h"
#include "DlgEnumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit �_�C�A���O


CDlgEnumEdit::CDlgEnumEdit(CWnd* pParent /*=NULL*/)
	: CDialogX(CDlgEnumEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnumEdit)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDlgEnumEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnumEdit)
	DDX_Control(pDX, IDC_LST_ENUMSTR, m_list_EnumStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnumEdit, CDialogX)
	//{{AFX_MSG_MAP(CDlgEnumEdit)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnumEdit ���b�Z�[�W �n���h��
BOOL CDlgEnumEdit::DlgCreate(CWnd* pParent)
{
	SetBackColor(64,64,64);
	return CDialogX::Create(IDD, pParent);
}
int CDlgEnumEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogX::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// �\�����W�␳
	CRect rcWindow;
	CRect rcParent;
	CRect rcEdit;
	int	x,y;

	GetClientRect(rcWindow);

	GetParent()->GetWindowRect(rcParent);
	m_pIOEdit->GetWindowRect(rcEdit);

	if(rcParent.Height() < rcWindow.Height() || rcParent.Width() < (rcWindow.Width()*3))
		GetParent()->GetParent()->GetWindowRect(rcParent);

	if(rcParent.right >= (rcEdit.right + rcWindow.Width()))
		x = rcEdit.right;
	else{
		x = rcEdit.left - rcWindow.Width();
		if(x < 0)
			x = 0;
	}
	if(rcParent.bottom>= (rcEdit.bottom + rcWindow.Height()))
		y = rcEdit.bottom;
	else{
		y = rcEdit.top - rcWindow.Height();
		if(y < 0)
			y = 0;
	}

	/*
	** Recipe�\��
	*/
	rcWindow.OffsetRect(x,y);
	MoveWindow(rcWindow);
	return 0;
}

void CDlgEnumEdit::SetValue(unsigned uint_val,LPSTR pEnumString[],CIOEdit *pEdit)
{
	for(int i=0;i<32;i++){
		if(pEnumString[i] == NULL){
			m_szEnumString[i][0] = '\0';
			break;
		}
// 2012/03/13		sprintf_s(m_szEnumString[i],sizeof(m_szEnumString[i]),"%d:%s",i,pEnumString[i]);
		sprintf_s(m_szEnumString[i],sizeof(m_szEnumString[i]),"%s",pEnumString[i]);
		m_szEnumString[i][64]='\0';
	}/* for */
	m_uint_Val = uint_val;
	m_pIOEdit = pEdit;
	return;
}
void CDlgEnumEdit::GetValue(unsigned *uint_val)
{
	*uint_val = m_uint_Val;
}

BOOL CDlgEnumEdit::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	for(int i=0;i<32;i++){
		if(m_szEnumString[i][0] == '\0')
			break;

		m_list_EnumStr.InsertString(i,m_szEnumString[i]);
		m_list_EnumStr.SetItemData(i,i);
	}/* for */
	m_list_EnumStr.SetCurSel(m_uint_Val);

	// �Ăяo����I/O�t�B�[���h��I����Ԃ�
	m_pIOEdit->SelectField();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
void CDlgEnumEdit::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	m_pIOEdit->UnselectField();
	m_pIOEdit = NULL;
	if(::IsWindow(m_hWnd) != 0){
		CDialogX::OnClose();
		DestroyWindow();
	}
}
void CDlgEnumEdit::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	int item = m_list_EnumStr.GetCurSel();
	if (item == LB_ERR){
		return;
	}	
	m_uint_Val = item;
	m_pIOEdit->SetEnumValue(m_uint_Val);
	CDialogX::OnOK();
	SendMessage(WM_CLOSE);
}
void CDlgEnumEdit::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	CDialogX::OnCancel();
	SendMessage(WM_CLOSE);
}
