// YearPlan.h: interface for the YearPlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YEARPLAN_H__46A27908_EF9C_4C50_A503_8F7C95E1B9C0__INCLUDED_)
#define AFX_YEARPLAN_H__46A27908_EF9C_4C50_A503_8F7C95E1B9C0__INCLUDED_

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class YearPlan  
{
	class YearPlanEntry {
	// Construction/destruction
	public:
		YearPlanEntry();
		YearPlanEntry(int nYear, int nDay, int nMonth, int nDuration, LPCTSTR lpszDescription, COLORREF clr);
		virtual ~YearPlanEntry();

		YearPlanEntry& operator=(const YearPlanEntry& rhs);

	// Implementation
	public:

	// Data members
	public:
		COLORREF m_clr;
		CTime m_timeStart;
		int m_nDuration;
		CString	m_strDescription;
		int m_nSlot;
	};

// Construction/destruction
public:
	YearPlan();
	virtual ~YearPlan();

// Implementation
public:
	BOOL AddEntry(int nDay, int nMonth, int nDuration, LPCTSTR lpszDescription, COLORREF clr);

// Data members
public:
	CString m_strTitle;
	int m_nYear;
	std::vector<YearPlanEntry> m_entries;
	char m_entrymap[400]; // enough for one byte per day for a year + Jan
};

#endif // !defined(AFX_YEARPLAN_H__46A27908_EF9C_4C50_A503_8F7C95E1B9C0__INCLUDED_)
