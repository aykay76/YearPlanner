// YearPlannerDoc.h : interface of the CYearPlannerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YEARPLANNERDOC_H__823538AD_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
#define AFX_YEARPLANNERDOC_H__823538AD_B371_11D5_A42C_0050BA6E72D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CYearPlannerDoc : public CDocument
{
protected: // create from serialization only
	CYearPlannerDoc();
	DECLARE_DYNCREATE(CYearPlannerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYearPlannerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CYearPlannerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CYearPlannerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YEARPLANNERDOC_H__823538AD_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
