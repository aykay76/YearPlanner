// YearPlannerView.h : interface of the CYearPlannerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YEARPLANNERVIEW_H__823538AF_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
#define AFX_YEARPLANNERVIEW_H__823538AF_B371_11D5_A42C_0050BA6E72D8__INCLUDED_

#include "ScrollPad.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class YearPlan;

class CYearPlannerView : public CView
{
protected: // create from serialization only
	CYearPlannerView();
	DECLARE_DYNCREATE(CYearPlannerView)

// Attributes
public:
	CYearPlannerDoc* GetDocument();

// Operations
public:
//	void PrintYearPlanner(CDC *pDC);
	void DrawYearPlanner(CDC *pDC);
	void CalculateFirstDayPositions();
	bool DateFromPoint(CPoint &point, int &month, int &day);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYearPlannerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CYearPlannerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CYearPlannerView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditYear();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// Data members
protected:
	YearPlan m_YearPlan;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nCellWidth;
	int m_nCellHeight;
	CScrollBar m_wndHScroll;
	CScrollBar m_wndVScroll;
	CScrollPad m_wndScrollpad;
	double m_dScale;
	bool m_fYearPlanIsDirty;

	// some variables for the margins etc.
	int m_nMonthWidth;
	int m_nLeftMargin;
	int m_nTopMargin;
	int m_nDayHeight;

	CRect m_rcPlanner;
};

#ifndef _DEBUG  // debug version in YearPlannerView.cpp
inline CYearPlannerDoc* CYearPlannerView::GetDocument()
   { return (CYearPlannerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YEARPLANNERVIEW_H__823538AF_B371_11D5_A42C_0050BA6E72D8__INCLUDED_)
