// YearPlanner.h : main header file for the YEARPLANNER application
//

#if !defined(AFX_YEARPLANNER_H__823538A7_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
#define AFX_YEARPLANNER_H__823538A7_B371_11D5_A42C_0050BA6E72D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerApp:
// See YearPlanner.cpp for the implementation of this class
//

class CYearPlannerApp : public CWinApp
{
public:
	CYearPlannerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYearPlannerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CYearPlannerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YEARPLANNER_H__823538A7_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
