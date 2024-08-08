
// CitiesView.cpp : implementation of the CCitiesView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "CitiesDoc.h"
#include "CitiesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCitiesView construction/destruction

CCitiesView::CCitiesView() noexcept
{
	// TODO: add construction code here

}

CCitiesView::~CCitiesView()
{
}

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

void CCitiesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCitiesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCitiesView diagnostics

#ifdef _DEBUG
void CCitiesView::AssertValid() const
{
	CListView::AssertValid();
}

void CCitiesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCitiesDoc* CCitiesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDoc)));
	return (CCitiesDoc*)m_pDocument;
}
#endif //_DEBUG


// CCitiesView message handlers
