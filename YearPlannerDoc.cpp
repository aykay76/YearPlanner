// YearPlannerDoc.cpp : implementation of the CYearPlannerDoc class
//

#include "stdafx.h"
#include "YearPlanner.h"

#include "YearPlannerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerDoc

IMPLEMENT_DYNCREATE(CYearPlannerDoc, CDocument)

BEGIN_MESSAGE_MAP(CYearPlannerDoc, CDocument)
	//{{AFX_MSG_MAP(CYearPlannerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerDoc construction/destruction

CYearPlannerDoc::CYearPlannerDoc()
{
	// TODO: add one-time construction code here

}

CYearPlannerDoc::~CYearPlannerDoc()
{
}

BOOL CYearPlannerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CYearPlannerDoc serialization

void CYearPlannerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerDoc diagnostics

#ifdef _DEBUG
void CYearPlannerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYearPlannerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYearPlannerDoc commands
