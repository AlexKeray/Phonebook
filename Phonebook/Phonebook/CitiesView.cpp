
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
#include "Log.h"
#include "Message.h"

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


	CListCtrl& oListCtrl = GetListCtrl();

	oListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CreateColumns(oListCtrl, 2);

	m_pCitiesDocument = GetDocument();
	std::map<CString, City>& oCitiesMap = m_pCitiesDocument->GetDocumentMap();

	if (!PopulateListCtrl(oListCtrl, oCitiesMap))
	{
		CMessage::ErrorMessage(_T("Loading data from the document failed."), __FILE__, __LINE__);
	}

	if (!SetColumnsWidth(oListCtrl))
	{
		CLog::LogMessage(_T("Setting the columns width of the CCitiesView failed."), __FILE__, __LINE__);
	}
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

// Methods

void CCitiesView::CreateColumns(CListCtrl& oListCtrl, int nColumnCount)
{
	m_nColumnCount = 2;

	oListCtrl.InsertColumn(0, _T("City name    "), LVCFMT_LEFT);
	oListCtrl.InsertColumn(1, _T("Area         "), LVCFMT_LEFT);
}

BOOL CCitiesView::SetColumnsWidth(CListCtrl& oListCtrl, int nRightPadding)
{
	// Autosize columns based on the longest item or the header
	for (int nColumnIndex = 0; nColumnIndex < m_nColumnCount; ++nColumnIndex)
	{
		if (!oListCtrl.SetColumnWidth(nColumnIndex, LVSCW_AUTOSIZE))
		{
			return FALSE;
		}
		int nColumnAutosizeWidth = oListCtrl.GetColumnWidth(nColumnIndex);
		if (!oListCtrl.SetColumnWidth(nColumnIndex, LVSCW_AUTOSIZE_USEHEADER))
		{
			return FALSE;
		}
		int nHeaderAutosizeWidth = oListCtrl.GetColumnWidth(nColumnIndex);
		if (nColumnAutosizeWidth > nHeaderAutosizeWidth)
		{
			if (!oListCtrl.SetColumnWidth(nColumnIndex, nColumnAutosizeWidth + nRightPadding))
			{
				return FALSE;
			}
		}
		else
		{
			if (!oListCtrl.SetColumnWidth(nColumnIndex, nHeaderAutosizeWidth + nRightPadding))
			{
				return FALSE;
			}
		}
	}
}

BOOL CCitiesView::PopulateListCtrl(CListCtrl& oListCtrl, std::map<CString, City>& oCitiesMap)
{
	int i = 0;
	int nItemIndex = 0;
	for (std::map<CString, City>::iterator it = oCitiesMap.begin(); it != oCitiesMap.end(); ++it)
	{
		nItemIndex = oListCtrl.InsertItem(i, it->second.szCityName);
		if (!oListCtrl.SetItemText(nItemIndex, 1, it->second.szCityArea))
		{
			return FALSE;
		}
		if (!oListCtrl.SetItemData(nItemIndex, static_cast<DWORD_PTR>(it->second.lId)))
		{
			return FALSE;
		}
		++i;
	}

	return TRUE;
}
