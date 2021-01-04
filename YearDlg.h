#if !defined(AFX_YEARDLG_H__2BE9BF7A_834F_49E5_BDDE_66AC0B2ECE3A__INCLUDED_)
#define AFX_YEARDLG_H__2BE9BF7A_834F_49E5_BDDE_66AC0B2ECE3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YearDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYearDlg dialog

class CYearDlg : public CDialog
{
// Construction
public:
	CYearDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYearDlg)
	enum { IDD = IDD_YEAR };
	int		m_nYear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYearDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YEARDLG_H__2BE9BF7A_834F_49E5_BDDE_66AC0B2ECE3A__INCLUDED_)
