#if !defined(AFX_SCROLLPAD_H__1C045FBC_0E82_4A55_9EB2_739BB8969334__INCLUDED_)
#define AFX_SCROLLPAD_H__1C045FBC_0E82_4A55_9EB2_739BB8969334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollPad.h : header file
//

#define SPS_TRACKMOVE		0x00000001

/////////////////////////////////////////////////////////////////////////////
// CScrollPad window

class CScrollPad : public CWnd
{
// Construction
public:
	CScrollPad();

// Attributes
public:

// Operations
public:
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParent, UINT id);
	void SetPadStyle(DWORD dwPadStyle) { m_dwPadStyle = dwPadStyle; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollPad)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScrollPad();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollPad)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Data members
protected:
	BYTE m_nActive;  // bit field for which sections are active
	CRgn m_TopRegion;
	CRgn m_LeftRegion;
	CRgn m_RightRegion;
	CRgn m_BottomRegion;
	CRgn *m_pInvertedRgn;
	bool	m_fMouseButtonDown;
	DWORD m_dwPadStyle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLPAD_H__1C045FBC_0E82_4A55_9EB2_739BB8969334__INCLUDED_)
