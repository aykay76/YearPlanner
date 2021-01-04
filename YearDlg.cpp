// YearDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yearplanner.h"
#include "YearDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYearDlg dialog


CYearDlg::CYearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYearDlg)
	m_nYear = 0;
	//}}AFX_DATA_INIT
}


void CYearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYearDlg)
	DDX_Text(pDX, IDC_YEAR, m_nYear);
	DDV_MinMaxInt(pDX, m_nYear, 1970, 2038);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYearDlg, CDialog)
	//{{AFX_MSG_MAP(CYearDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYearDlg message handlers
