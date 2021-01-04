// ScrollPad.cpp : implementation file
//

#include "stdafx.h"
#include "YearPlanner.h"
#include "ScrollPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollPad

CScrollPad::CScrollPad()
{
}

CScrollPad::~CScrollPad()
{
}


BEGIN_MESSAGE_MAP(CScrollPad, CWnd)
	//{{AFX_MSG_MAP(CScrollPad)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollPad operations

BOOL CScrollPad::Create(DWORD dwStyle, const RECT &rect, CWnd *pParent, UINT id)
{
	BOOL bResult = CWnd::Create("STATIC", NULL, dwStyle | SS_NOTIFY, rect, pParent, id);

	DWORD dwClassStyle = GetClassLong(m_hWnd, GCL_STYLE);
	dwClassStyle &= ~CS_DBLCLKS;
	SetClassLong(m_hWnd, GCL_STYLE, dwClassStyle);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CScrollPad message handlers

int CScrollPad::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pInvertedRgn = NULL;
	m_fMouseButtonDown = false;
	m_dwPadStyle = 0;

	return 0;
}

void CScrollPad::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;

	GetClientRect(rc);

	// draw the background
	{
		CBrush bgBrush(GetSysColor(COLOR_3DFACE));
		CBrush *pOldBrush = dc.SelectObject(&bgBrush);
		dc.SelectStockObject(NULL_PEN);
		dc.Rectangle(rc);
		dc.SelectObject(pOldBrush);
	}

	// draw the outside line
	{
		COLORREF clrOld = dc.GetTextColor();
		dc.Draw3dRect(rc, GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
		dc.SetTextColor(clrOld);
	}

	// draw the diagonal lines
	{
		CPen pen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen;

		pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(1, 1);
		dc.LineTo(rc.Width(), rc.Height());

		dc.MoveTo(rc.Width() - 2, 1);
		dc.LineTo(0, rc.Height() - 1);
		dc.SelectObject(pOldPen);
	}

	// draw the hilight lines
	{
		CPen pen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
		CPen *pOldPen;

		pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(2, 1);
		dc.LineTo(rc.Width() / 2 + 2, rc.Height() / 2 + 1);

		dc.MoveTo(1, 2);
		dc.LineTo(rc.Width() / 2 + 1, rc.Height() / 2 + 2);

		dc.MoveTo(2, rc.Height() - 2);
		dc.LineTo(rc.Width() - 1, 1);

		dc.SelectObject(pOldPen);
	}

	// draw the arrows
	{
		CBrush brArrow(RGB(0, 0, 0));
		CBrush *pOldBrush;
		CPen penArrow(PS_SOLID, 1, RGB(0, 0, 0));
		CPen *pOldPen;
		POINT points[3];

		pOldBrush = dc.SelectObject(&brArrow);
		pOldPen = dc.SelectObject(&penArrow);

		// top
		points[0].x = rc.Width() / 2;
		points[0].y = 3;
		points[1].x = points[0].x + 2;
		points[1].y = points[0].y + 2;
		points[2].x = points[1].x - 4;
		points[2].y = points[1].y;
		dc.Polygon(points, 3);

		// bottom
		points[0].x = rc.Width() / 2;
		points[0].y = rc.Height() - 4;
		points[1].x = points[0].x - 2;
		points[1].y = points[0].y - 2;
		points[2].x = points[1].x + 4;
		points[2].y = points[1].y;
		dc.Polygon(points, 3);

		// right
		points[0].x = rc.Width() - 4;
		points[0].y = rc.Height() / 2;
		points[1].x = points[0].x - 2;
		points[1].y = points[0].y + 2;
		points[2].x = points[1].x;
		points[2].y = points[1].y - 4;
		dc.Polygon(points, 3);

		// left
		points[0].x = 3;
		points[0].y = rc.Height() / 2;
		points[1].x = points[0].x + 2;
		points[1].y = points[0].y + 2;
		points[2].x = points[1].x;
		points[2].y = points[1].y - 4;
		dc.Polygon(points, 3);

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}

	// last but not least - if we have an inverted region, invert it now
	if (m_pInvertedRgn) {
		dc.InvertRgn(m_pInvertedRgn);
	}
}

void CScrollPad::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDC *pDC = GetDC();

	SetCapture();

	if (m_TopRegion.PtInRegion(point)) {
		pDC->InvertRgn(&m_TopRegion);
		m_pInvertedRgn = &m_TopRegion;

		GetParent()->SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	} else if (m_LeftRegion.PtInRegion(point)) {
		pDC->InvertRgn(&m_LeftRegion);
		m_pInvertedRgn = &m_LeftRegion;

		GetParent()->SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
	} else if (m_BottomRegion.PtInRegion(point)) {
		pDC->InvertRgn(&m_BottomRegion);
		m_pInvertedRgn = &m_BottomRegion;

		GetParent()->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	} else if (m_RightRegion.PtInRegion(point)) {
		pDC->InvertRgn(&m_RightRegion);
		m_pInvertedRgn = &m_RightRegion;

		GetParent()->SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
	} else {
		// should never happen!
	}

	m_fMouseButtonDown = true;

	ReleaseDC(pDC);

	CWnd::OnLButtonDown(nFlags, point);
}

void CScrollPad::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDC *pDC = GetDC();

	ReleaseCapture();

	if (m_fMouseButtonDown) {
		if (m_pInvertedRgn) {
			pDC->InvertRgn(m_pInvertedRgn);
			m_pInvertedRgn = NULL;
		}
		m_fMouseButtonDown = false;
	}

	ReleaseDC(pDC);

	CWnd::OnLButtonUp(nFlags, point);
}

void CScrollPad::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDC *pDC = GetDC();

	CRect rc;
	GetClientRect(rc);

	if (m_fMouseButtonDown && m_dwPadStyle & SPS_TRACKMOVE) {
		if (rc.PtInRect(point)) {
			if (m_pInvertedRgn) {
				if (m_pInvertedRgn->PtInRegion(point)) {
					goto done;
				} else {
					pDC->InvertRgn(m_pInvertedRgn);
					m_pInvertedRgn = NULL;
				}
			}

			if (m_TopRegion.PtInRegion(point)) {
				pDC->InvertRgn(&m_TopRegion);
				m_pInvertedRgn = &m_TopRegion;

				GetParent()->SendMessage(WM_VSCROLL, SB_LINEUP, 0);
			} else if (m_LeftRegion.PtInRegion(point)) {
				pDC->InvertRgn(&m_LeftRegion);
				m_pInvertedRgn = &m_LeftRegion;

				GetParent()->SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
			} else if (m_BottomRegion.PtInRegion(point)) {
				pDC->InvertRgn(&m_BottomRegion);
				m_pInvertedRgn = &m_BottomRegion;

				GetParent()->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			} else if (m_RightRegion.PtInRegion(point)) {
				pDC->InvertRgn(&m_RightRegion);
				m_pInvertedRgn = &m_RightRegion;

				GetParent()->SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
			} else {
				// should never happen!
			}
		} else {
			// if the point is outside the window
			if (m_pInvertedRgn) {
				pDC->InvertRgn(m_pInvertedRgn);
				m_pInvertedRgn = NULL;
			}
		}
	}

done:
	ReleaseDC(pDC);

	CWnd::OnMouseMove(nFlags, point);
}

void CScrollPad::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx && cy) {
		// create the regions
		CRect rc;
		GetClientRect(rc);
		POINT points[3];

		points[0].x = 2;
		points[0].y = 1;
		points[1].x = rc.Width() - 2;
		points[1].y = 1;
		points[2].x = rc.Width() / 2;
		points[2].y = rc.Height() / 2;
		if (m_TopRegion.m_hObject) m_TopRegion.DeleteObject();
		m_TopRegion.CreatePolygonRgn(points, 3, WINDING);

		points[0].x = 1;
		points[0].y = 1;
		points[1].x = 1;
		points[1].y = rc.Height() - 2;
		points[2].x = rc.Width() / 2;
		points[2].y = rc.Height() / 2;
		if (m_LeftRegion.m_hObject) m_LeftRegion.DeleteObject();
		m_LeftRegion.CreatePolygonRgn(points, 3, WINDING);

		points[0].x = rc.Width() - 1;
		points[0].y = 1;
		points[1].x = rc.Width() - 1;
		points[1].y = rc.Height() - 2;
		points[2].x = rc.Width() / 2;
		points[2].y = rc.Height() / 2;
		if (m_RightRegion.m_hObject) m_RightRegion.DeleteObject();
		m_RightRegion.CreatePolygonRgn(points, 3, WINDING);

		points[0].x = 1;
		points[0].y = rc.Height() - 1;
		points[1].x = rc.Width() - 2;
		points[1].y = rc.Height() - 2;
		points[2].x = rc.Width() / 2;
		points[2].y = rc.Height() / 2;
		if (m_BottomRegion.m_hObject) m_BottomRegion.DeleteObject();
		m_BottomRegion.CreatePolygonRgn(points, 3, WINDING);
	}
}
