// YearPlan.cpp: implementation of the YearPlan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YearPlanner.h"
#include "YearPlan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

YearPlan::YearPlan() : 
	m_strTitle("Year Planner"),
	m_nYear(2021)
{
}

YearPlan::~YearPlan()
{

}

BOOL YearPlan::AddEntry(int nDay, int nMonth, int nDuration, LPCTSTR lpszDescription, COLORREF clr)
{
	YearPlanEntry entry(m_nYear, nDay, nMonth, nDuration, lpszDescription, clr);

	m_entries.push_back(entry);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//

YearPlan::YearPlanEntry::YearPlanEntry()
{
	m_strDescription = "";
	m_nDuration = 0;
}

YearPlan::YearPlanEntry::YearPlanEntry(int nYear, int nDay, int nMonth, int nDuration, LPCTSTR lpszDescription, COLORREF clr)
{
	m_timeStart = CTime(nYear, nMonth, nDay, 0, 0, 0);
	m_strDescription = lpszDescription;
	m_clr = clr;
}

YearPlan::YearPlanEntry::~YearPlanEntry()
{
}

YearPlan::YearPlanEntry& YearPlan::YearPlanEntry::operator=(const YearPlan::YearPlanEntry& rhs)
{
	m_timeStart = rhs.m_timeStart;
	m_strDescription = rhs.m_strDescription;
	m_nDuration = rhs.m_nDuration;
	m_clr = rhs.m_clr;

	return *this;
}