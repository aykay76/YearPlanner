// YearPlannerView.cpp : implementation of the CYearPlannerView class
//

#include "stdafx.h"
#include "YearPlanner.h"

#include "YearPlan.h"

#include "YearPlannerDoc.h"
#include "YearPlannerView.h"

#include "YearDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *month[13] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec","Jan"
};
char firstdays[13];

char *days[7] = {
	"Mo",
	"Tu",
	"We",
	"Th",
	"Fr",
	"Sa",
	"Su"
};

int dim[13] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31
};

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView

IMPLEMENT_DYNCREATE(CYearPlannerView, CView)

BEGIN_MESSAGE_MAP(CYearPlannerView, CView)
	//{{AFX_MSG_MAP(CYearPlannerView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_YEAR, OnEditYear)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView construction/destruction

CYearPlannerView::CYearPlannerView() : 
	m_nOffsetX(0),
	m_nOffsetY(0),
	m_nCellWidth(100),
	m_nCellHeight(150),
	m_dScale(1.0),
	m_fYearPlanIsDirty(true),
	m_nLeftMargin(20),
	m_nTopMargin(150),
	m_nMonthWidth(50),
	m_nDayHeight(50)
{
}

CYearPlannerView::~CYearPlannerView()
{
}

BOOL CYearPlannerView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView drawing

void CYearPlannerView::OnDraw(CDC* pDC)
{
	CYearPlannerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// All the device specific stuff and DC preparation is done out here, that way
	// the actual drawing functions won't get confused - they will always be in logical
	// space
	if (pDC->IsPrinting()) {
		int nOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
	
		// calculate the scale so that the planner sita in the full width of the page
		int pagewidth = pDC->GetDeviceCaps(HORZSIZE) * 10;
		double dOldScale = m_dScale;
		m_dScale = pagewidth / (80. + 37. * m_nCellWidth);

		DrawYearPlanner(pDC);
		pDC->SetMapMode(nOldMapMode);

		m_dScale = dOldScale;
	} else {
		CBitmap bmp;
		CBitmap *pOldBitmap;
		CRect rc;

		GetClientRect(rc);

		bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
		CDC mdc;
		mdc.CreateCompatibleDC(pDC);
		pOldBitmap = mdc.SelectObject(&bmp);
		mdc.Rectangle(rc.left - 1, -1, rc.Width() + 2, rc.Height() + 2);
		int nOldMapMode = mdc.SetMapMode(MM_LOMETRIC);
		mdc.DPtoLP(rc);

		DrawYearPlanner(&mdc);

		mdc.SetMapMode(nOldMapMode);
		GetClientRect(rc);
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
		mdc.SelectObject(pOldBitmap);
		mdc.DeleteDC();
		bmp.DeleteObject();
	}
}

void CYearPlannerView::DrawYearPlanner(CDC *pDC)
{
	if (m_fYearPlanIsDirty) {
		CalculateFirstDayPositions();
	}

	// we are going to do everything in real measurements ie mm.
	CRect rc;
	GetClientRect(rc);

	pDC->DPtoLP(rc);

	// setup the device context
	pDC->SetBkMode(TRANSPARENT);

	CSize szScrollpad(27, 27);
	pDC->DPtoLP(&szScrollpad);

	CPen penLines(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&penLines);

	// first of all, calculate the display regions and 'stuff'
	int nLeftMargin = m_nLeftMargin * m_dScale;
	int nTopMargin = m_nTopMargin * m_dScale;
	int nMonthWidth = m_nMonthWidth * m_dScale;
	int nBottomMargin = szScrollpad.cy;
	int nRightMargin = szScrollpad.cx;
	int nDayHeight = m_nDayHeight * m_dScale;

	// precalculate
	m_nCellWidth *= m_dScale;
	m_nCellHeight *= m_dScale;

	// Draw the title of the year planner
	{
		CFont fnt;
		CFont *pOldFont;
		CSize sz;
		int nChartWidth = nLeftMargin + nMonthWidth + (37 * m_nCellWidth);

		fnt.CreatePointFont(m_nCellHeight * 2, "Arial");
		pOldFont = pDC->SelectObject(&fnt);
		sz = pDC->GetTextExtent(m_YearPlan.m_strTitle);
		pDC->TextOut(nChartWidth / 2 - sz.cx / 2 - m_nOffsetX, -sz.cy / 2 + m_nOffsetY, m_YearPlan.m_strTitle);
		pDC->SelectObject(pOldFont);
	}

	// draw the month labels section
	{
		CRect rcMonths;
		rcMonths.left = nLeftMargin;
		rcMonths.top = -nTopMargin;
		rcMonths.right = rcMonths.left + nMonthWidth;
		rcMonths.bottom = rc.bottom + nBottomMargin;

		// we only clip when drawing to the screen
		if (!pDC->IsPrinting()) {
			CRgn rgn;
			pDC->LPtoDP(rcMonths);
			rgn.CreateRectRgnIndirect(rcMonths);
			pDC->SelectClipRgn(&rgn);
			rgn.DeleteObject();
		}

		CFont fnt;
		CFont *pOldFont;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfEscapement = -900;
		strcpy(lf.lfFaceName, "Tahoma");
		lf.lfHeight = m_nCellHeight;
		fnt.CreatePointFontIndirect(&lf);
		pOldFont = pDC->SelectObject(&fnt);

		int x = nLeftMargin;
		int y = -nTopMargin + m_nOffsetY;
		CSize sz;

		for (int i = 0; i < 13; i++) {
			pDC->MoveTo(x, y);
			pDC->LineTo(x + nMonthWidth, y);
			sz = pDC->GetTextExtent(month[i]);
			pDC->TextOut(x + nMonthWidth / 2 - sz.cy / 2, y - m_nCellHeight / 2 - sz.cx / 2, month[i]);

			y -= m_nCellHeight;
		}
		// finish off
		pDC->MoveTo(x, y);
		pDC->LineTo(x + nMonthWidth, y);

		pDC->SelectObject(pOldFont);

		pDC->MoveTo(nLeftMargin, rc.top + m_nOffsetY - nTopMargin);
		pDC->LineTo(nLeftMargin, rc.top + m_nOffsetY - nTopMargin - (m_nCellHeight * 13));

		pDC->MoveTo(nLeftMargin + nMonthWidth, rc.top + m_nOffsetY - nTopMargin);
		pDC->LineTo(nLeftMargin + nMonthWidth, rc.top + m_nOffsetY - nTopMargin - (m_nCellHeight * 13));
	}

	// draw some vertical line and put in the day titles while we're there
	{
		CRect rcDays;
		rcDays.left = nLeftMargin + nMonthWidth;
		rcDays.top = -nTopMargin + nDayHeight;
		rcDays.right = rc.right - nRightMargin;
		rcDays.bottom = -nTopMargin;

		// we only clip when drawing to the screen
		if (!pDC->IsPrinting()) {
			CRgn rgn;
			pDC->LPtoDP(rcDays);
			rgn.CreateRectRgnIndirect(rcDays);
			pDC->SelectClipRgn(&rgn);
			rgn.DeleteObject();
		}

		CFont fnt;
		CFont *pOldFont;
		fnt.CreatePointFont(m_nCellHeight, "Tahoma");
		pOldFont = pDC->SelectObject(&fnt);

		int x = nLeftMargin + nMonthWidth - m_nOffsetX;
		int y = -nTopMargin;
		CSize sz;

		for (int i = 0; i < 37; i++) {
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y + nDayHeight);

			sz = pDC->GetTextExtent(days[i % 7]);
			pDC->TextOut(x + m_nCellWidth / 2 - sz.cx / 2, y + nDayHeight / 2 + sz.cy / 2, days[i % 7]);

			x += m_nCellWidth;
		}
		// finish off with a 'closing' line on the right hand end
		pDC->MoveTo(x, y);
		pDC->LineTo(x, y + nDayHeight);

		pDC->MoveTo(nLeftMargin + nMonthWidth, -nTopMargin + nDayHeight);
		pDC->LineTo(nLeftMargin + nMonthWidth + (37 * m_nCellWidth) - m_nOffsetX, -nTopMargin + nDayHeight);

		pDC->MoveTo(nLeftMargin + nMonthWidth, -nTopMargin);
		pDC->LineTo(nLeftMargin + nMonthWidth + (37 * m_nCellWidth) - m_nOffsetX, -nTopMargin);

		pDC->SelectObject(pOldFont);
	}

	// Fill in the coloured blocks for the weekends
	{
		// fill in the weekend bits
		CBrush brWeekend(RGB(235, 205, 205));
		CPen penWeekend(PS_SOLID, 1, RGB(235, 205, 205));
		CPen *pOldPen;
		CBrush *pOldBrush;

		pOldPen = pDC->SelectObject(&penWeekend);
		pOldBrush = pDC->SelectObject(&brWeekend);

		m_rcPlanner.left = nLeftMargin + nMonthWidth;
		m_rcPlanner.top = -nTopMargin;
		m_rcPlanner.right = rc.right - nRightMargin;
		m_rcPlanner.bottom = rc.bottom + nBottomMargin;

		// we only clip when drawing to the screen
		if (!pDC->IsPrinting()) {
			CRgn rgn;
			pDC->LPtoDP(m_rcPlanner);
			rgn.CreateRectRgnIndirect(m_rcPlanner);
			pDC->DPtoLP(m_rcPlanner);
			pDC->SelectClipRgn(&rgn);
			rgn.DeleteObject();
		}

		// draw the vertical lines
		int x = nLeftMargin + nMonthWidth;
		int d = 0;
		for (int i = 0; i < 37; i++) {
			if (d && ((d % 5) == 0 || (d % 6) == 0)) {
				pDC->Rectangle(x - m_nOffsetX, -nTopMargin + m_nOffsetY, x - m_nOffsetX + m_nCellWidth, -nTopMargin + m_nOffsetY - 13 * m_nCellHeight);
			}

			x += m_nCellWidth;
			d = (d + 1) % 7;
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	///////////////////////////////////////////////////////////////////////////
	// Now to calculate which cells the dates will fall in. The first thing
	// is to find Jaunary 1st (naturally) - then it all leads on from there.
	{
		CBrush brBlank(RGB(234, 173, 173));
		CPen penBlank(PS_SOLID, 1, RGB(234, 173, 173));
		CPen *pOldPen;
		CBrush *pOldBrush;
		int dow = firstdays[0];
		int x;
		int y;
		CFont fnt;
		CFont *pOldFont;
		fnt.CreatePointFont(m_nCellHeight, "Tahoma");
		pOldFont = pDC->SelectObject(&fnt);
		pOldBrush = pDC->SelectObject(&brBlank);

		x = nLeftMargin + nMonthWidth - m_nOffsetX + dow * m_nCellWidth;
		y = -nTopMargin + m_nOffsetY;
		for (int m = 0; m < 13; m++) {
			pOldPen = pDC->SelectObject(&penBlank);
			pDC->Rectangle(nLeftMargin + nMonthWidth - m_nOffsetX, y, nLeftMargin + nMonthWidth - m_nOffsetX + m_nCellWidth * dow, y - m_nCellHeight);
			pDC->SelectObject(pOldPen);

			for (int d = 0; d < dim[m % 12]; d++) {
				CString str;
				str.Format("%d", d + 1);
				pDC->TextOut(x + 5, y, str);
				x += m_nCellWidth;
			}

			pOldPen = pDC->SelectObject(&penBlank);
			pDC->Rectangle(x, y, nLeftMargin + nMonthWidth - m_nOffsetX + m_nCellWidth * 37, y - m_nCellHeight);
			pDC->SelectObject(pOldPen);

			dow = (dow + dim[m]) % 7;

			x = nLeftMargin + nMonthWidth - m_nOffsetX + dow * m_nCellWidth;
			y -= m_nCellHeight;
		}

		pDC->SelectObject(pOldFont);
	}

	// overplot the lines for the cells to make sure they are black
	{
		// draw the vertical lines
		int x = nLeftMargin + nMonthWidth;
		int d = 0;
		for (int i = 0; i < 37; i++) {
			pDC->MoveTo(x - m_nOffsetX, -nTopMargin + m_nOffsetY);
			pDC->LineTo(x - m_nOffsetX, -nTopMargin + m_nOffsetY - (13 * m_nCellHeight));

			x += m_nCellWidth;
			d = (d + 1) % 7;
		}
		pDC->MoveTo(x - m_nOffsetX, -nTopMargin + m_nOffsetY);
		pDC->LineTo(x - m_nOffsetX, -nTopMargin + m_nOffsetY - (13 * m_nCellHeight));

		// draw the horizontal
		int y = -nTopMargin;
		for (i = 0; i < 13; i++) {
			pDC->MoveTo(nLeftMargin - m_nOffsetX + nMonthWidth, y + m_nOffsetY);
			pDC->LineTo(nLeftMargin - m_nOffsetX + nMonthWidth + (37 * m_nCellWidth), y + m_nOffsetY);

			y -= m_nCellHeight;
		}
		pDC->MoveTo(nLeftMargin - m_nOffsetX + nMonthWidth, y + m_nOffsetY);
		pDC->LineTo(nLeftMargin - m_nOffsetX + nMonthWidth + (37 * m_nCellWidth), y + m_nOffsetY);
	}

	// now draw the entries over the top.
	{
		CPen penEntry(PS_SOLID, 1, RGB(255, 0, 255));
		CPen *pOldPen;
		CBrush brEntry(RGB(255, 0, 255));
		CBrush *pOldBrush;

		pOldPen = pDC->SelectObject(&penEntry);
		pOldBrush = pDC->SelectObject(&brEntry);

		int y = -nTopMargin + m_nOffsetY - 30 * m_dScale;
		for (int i = 0; i < 6; i++) {
//			pDC->Rectangle(nLeftMargin - m_nOffsetX + nMonthWidth, y, nLeftMargin - m_nOffsetX + nMonthWidth + m_nCellWidth * 5, y - (20 * m_dScale));

			y -= 20 * m_dScale;
		}

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

	pDC->SelectObject(pOldPen);

	m_nCellWidth /= m_dScale;
	m_nCellHeight /= m_dScale;
}

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView printing

BOOL CYearPlannerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CYearPlannerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CYearPlannerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView diagnostics

#ifdef _DEBUG
void CYearPlannerView::AssertValid() const
{
	CView::AssertValid();
}

void CYearPlannerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYearPlannerDoc* CYearPlannerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYearPlannerDoc)));
	return (CYearPlannerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerView message handlers

void CYearPlannerView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Get the minimum and maximum scroll-bar positions.
//	pScrollBar->GetScrollRange(&minpos, &maxpos); 
//	maxpos = pScrollBar->GetScrollLimit();
	
	// Get the current position of scroll box.
//	int curpos = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
//		curpos = minpos;
		break;
		
	case SB_RIGHT:      // Scroll to far right.
//		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINELEFT:      // Scroll left.
		m_nOffsetX -= m_nCellWidth * m_dScale;
		if (m_nOffsetX < 0) m_nOffsetX = 0;
//		if (curpos > minpos)
//			curpos--;
		break;
		
	case SB_LINERIGHT:   // Scroll right.
		m_nOffsetX += m_nCellWidth * m_dScale;
		if (m_nOffsetX > 36 * m_nCellWidth * m_dScale) m_nOffsetX = 36 * m_nCellWidth * m_dScale;
//		if (curpos < maxpos)
//			curpos++;
		break;
		
	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
//			SCROLLINFO   info;
//			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
//			if (curpos > minpos)
//				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
//			SCROLLINFO   info;
//			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
//			if (curpos < maxpos)
//				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
//		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
//		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	
	// Set the new position of the thumb (scroll box).
//	pScrollBar->SetScrollPos(curpos);
	RedrawWindow();

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CYearPlannerView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Get the minimum and maximum scroll-bar positions.
//	pScrollBar->GetScrollRange(&minpos, &maxpos); 
//	maxpos = pScrollBar->GetScrollLimit();
	
	// Get the current position of scroll box.
//	int curpos = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_TOP:      // Scroll to far left.
//		curpos = minpos;
		break;
		
	case SB_BOTTOM:      // Scroll to far right.
//		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINEUP:      // Scroll left.
		m_nOffsetY -= m_nCellHeight * m_dScale;
		if (m_nOffsetY < 0) m_nOffsetY = 0;
//		if (curpos > minpos)
//			curpos--;
		break;
		
	case SB_LINEDOWN:   // Scroll right.
		m_nOffsetY += m_nCellHeight * m_dScale;
		if (m_nOffsetY > (12 * m_nCellHeight * m_dScale)) m_nOffsetY = 12 * m_nCellHeight * m_dScale;
//		if (curpos < maxpos)
//			curpos++;
		break;
		
	case SB_PAGEUP:    // Scroll one page left.
		{
			// Get the page size. 
//			SCROLLINFO   info;
//			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
//			if (curpos > minpos)
//				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGEDOWN:      // Scroll one page right.
		{
			// Get the page size. 
//			SCROLLINFO   info;
//			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
//			if (curpos < maxpos)
//				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
//		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
//		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	
	// Set the new position of the thumb (scroll box).
//	pScrollBar->SetScrollPos(curpos);
	RedrawWindow();

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CYearPlannerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);
	m_wndScrollpad.Create(WS_CHILD | WS_VISIBLE, CRect(rcClient.right - 27, rcClient.bottom - 27, rcClient.right, rcClient.bottom), this, 1);

	return 0;
}

void CYearPlannerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	CRect rcClient;
	GetClientRect(rcClient);
	m_wndScrollpad.MoveWindow(CRect(rcClient.right - 27, rcClient.bottom - 27, rcClient.right, rcClient.bottom));	
}

void CYearPlannerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CView::OnLButtonDown(nFlags, point);
}

void CYearPlannerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int m, d;

	if (DateFromPoint(point, m, d)) {
		
	}
	
	CView::OnRButtonDown(nFlags, point);
}

BOOL CYearPlannerView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;//CView::OnEraseBkgnd(pDC);
}

void CYearPlannerView::OnEditYear() 
{
	CYearDlg dlg;

	dlg.m_nYear = m_YearPlan.m_nYear;

	if (dlg.DoModal() == IDOK) {
		m_YearPlan.m_nYear = dlg.m_nYear;
		m_fYearPlanIsDirty = true;
		Invalidate();
	}
}

void CYearPlannerView::OnZoomIn() 
{
	m_nOffsetX /= m_dScale;
	m_nOffsetY /= m_dScale;
	m_dScale += 0.25;
	m_nOffsetX *= m_dScale;
	m_nOffsetY *= m_dScale;
	RedrawWindow();
}

void CYearPlannerView::OnZoomOut() 
{
	if (m_dScale - 0.25 > 0) {
		m_nOffsetX /= m_dScale;
		m_nOffsetY /= m_dScale;
		m_dScale -= 0.25;
		m_nOffsetX *= m_dScale;
		m_nOffsetY *= m_dScale;
		RedrawWindow();
	}
}

// Calculates the 'x' coordinates of the cells for the first day of the month.
// This will help us to find the day of the month that has been clicked in
void CYearPlannerView::CalculateFirstDayPositions()
{
	// get the day of the week for 1/1/year
	CTime janfirst(m_YearPlan.m_nYear, 1, 1, 0, 0, 0);
	int dow = janfirst.GetDayOfWeek();
	if (dow == 1) dow = 8;
	dow -= 2;

	firstdays[0] = dow;

	// first of all, check leap year and adjust Feb days accordingly
	bool fIsLeapYear = false;
	if (m_YearPlan.m_nYear % 4 == 0) {
		fIsLeapYear = true;
		if (m_YearPlan.m_nYear % 100 == 0) {
			fIsLeapYear = false;
			if (m_YearPlan.m_nYear % 400 == 0) {
				fIsLeapYear = true;
			}
		}
	}
	if (fIsLeapYear) {
		dim[1] = 29;
	}

	for (int m = 0; m < 12; m++) {
		dow = (dow + dim[m]) % 7;
		firstdays[m + 1] = dow;
	}

	m_fYearPlanIsDirty = false;
}

bool CYearPlannerView::DateFromPoint(CPoint &point, int &month, int &day)
{
	// convert the cursor position point into logical points based on the DC setup
	CDC *pDC = GetDC();
	int nOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
	bool fInPlanner = false;
	bool fReturn = false;

	pDC->LPtoDP(m_rcPlanner);
	if (m_rcPlanner.PtInRect(point)) {
		fInPlanner = true;
	} else {
		fInPlanner = false;
	}
	pDC->DPtoLP(m_rcPlanner);
	pDC->DPtoLP(&point);

	// before dealing with offsets etc. we need to ensure that the mouse has actually been clicked
	// in a date cell and not in the space around the year planner
	if (fInPlanner) {
		int x, y;
		double px, py;

		// add the offsets and scale the position to find which cell we are in.
		px = (point.x + m_nOffsetX) / m_dScale;
		py = (point.y - m_nOffsetY) / m_dScale;
		px -= (m_nLeftMargin + m_nMonthWidth);
		py += m_nTopMargin;

		x = px / m_nCellWidth;
		y = abs(py / m_nCellHeight);

		// then we can get the date from the cell position
		if (x >= firstdays[y] && x < firstdays[y] + dim[y]) {
			fReturn = true;
			month = y;
			day = x - firstdays[y] + 1;
		}
	}

	ReleaseDC(pDC);

	return fReturn;
}

void CYearPlannerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	int m, d;

/*	if (DateFromPoint(point, m, d)) {
		CDC *pDC = GetDC();
		int nOldMapMode = pDC->SetMapMode(MM_LOMETRIC);
		CPen penDotted(PS_DOT, 1, RGB(0, 0, 0));
		CPen *pOldPen;

		CRgn rgn;
		pDC->LPtoDP(m_rcPlanner);
		rgn.CreateRectRgnIndirect(m_rcPlanner);
		pDC->DPtoLP(m_rcPlanner);
		pDC->SelectClipRgn(&rgn);
		rgn.DeleteObject();
		
		pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&penDotted);

		int x, y;
		x = (m_nLeftMargin + m_nMonthWidth + (firstdays[m] + d - 1) * m_nCellWidth) * m_dScale - m_nOffsetX;
		y = (-m_nTopMargin - m * m_nCellHeight) * m_dScale + m_nOffsetY;
		pDC->Rectangle(x, y, x + m_nCellWidth * m_dScale, y - m_nCellHeight * m_dScale);

		pDC->SelectObject(pOldPen);

		ReleaseDC(pDC);
	}*/

	CView::OnMouseMove(nFlags, point);
}
