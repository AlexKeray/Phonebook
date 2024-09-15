
// CitiesView.cpp : implementation of the CCitiesView class

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
#include "CitiesDlg.h"
#include "Hints.h"
#include <cstring>  // Necessary for _tcscpy

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Constructors and destructors
CCitiesView::CCitiesView() noexcept {}

CCitiesView::~CCitiesView() {}

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

// Overrides

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& rListCtrl = GetListCtrl();
	CityInsertHintWrapper* pInsertWrapper;
	CityDeleteHintWrapper* pDeleteWrapper;
	CityUpdateHintWrapper* pUpdateWrapper;
	int nItemIndex;

	switch (lHint)
	{
	case Hint::HINT_INSERT_ITEM:

		// Holds the hint data (position in the container and object the record)
		pInsertWrapper = dynamic_cast<CityInsertHintWrapper*>(pHint);
		if (pInsertWrapper == nullptr)
		{
			CMessage::Message(_T("Can't add the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate-Insert failed"));
			return;
		}

		// Inserts the item
		nItemIndex = pInsertWrapper->GetPosition();
		nItemIndex = rListCtrl.InsertItem(nItemIndex, pInsertWrapper->GetRecord().szCityName);
		if (nItemIndex == -1)
		{
			CMessage::Message(_T("Can't add the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: InsertItem failed"));
			return;
		}
		if (!rListCtrl.SetItemText(nItemIndex, 1, pInsertWrapper->GetRecord().szCityArea))
		{
			CMessage::Message(_T("Can't add the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemText failed"));
			return;
		}
		if (!rListCtrl.SetItemData(nItemIndex, static_cast<DWORD_PTR>(pInsertWrapper->GetRecord().lId)))
		{
			CMessage::Message(_T("Can't add the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemData failed"));
			return;
		}

		// Handles which item appears selected (have blue background).
		if (!rListCtrl.SetItemState(nItemIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemState failed"));
		}

		break;

	case Hint::HINT_DELETE_ITEM:

		// Holds the hint data (the position).
		pDeleteWrapper = dynamic_cast<CityDeleteHintWrapper*>(pHint);
		if (pDeleteWrapper == nullptr)
		{
			CMessage::Message(_T("Can't delete the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: delete failed"));
			return;
		}

		// Deletes the item from the list control
		nItemIndex = pDeleteWrapper->GetPosition();
		if (!rListCtrl.DeleteItem(nItemIndex))
		{
			CMessage::Message(_T("Can't delete the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: DeleteItem failed"));
			return;
		}

		// Handles which item appears selected (have blue background).
		if (rListCtrl.SetItemState(nItemIndex, 0, LVIS_SELECTED | LVIS_FOCUSED))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemState"));
		}

		break;

	case Hint::HINT_UPDATE_ITEM:

		// Holds the hint data (the remove position, the insert positon and the object of the record).
		pUpdateWrapper = dynamic_cast<CityUpdateHintWrapper*>(pHint);
		if (pUpdateWrapper == nullptr)
		{
			CMessage::Message(_T("Can't Update the city"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: update failed"));
			return;
		}

		// Deletes the old item
		nItemIndex = pUpdateWrapper->GetRemovePosition();
		if (!rListCtrl.DeleteItem(nItemIndex))
		{
			CMessage::Message(_T("Can't delete the city when updating"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: DeleteItem failed"));
			return;
		}

		// Inserts the new item
		nItemIndex = pUpdateWrapper->GetInsertPosition();
		nItemIndex = rListCtrl.InsertItem(nItemIndex, pUpdateWrapper->GetRecord().szCityName);
		if (!rListCtrl.SetItemText(nItemIndex, 1, pUpdateWrapper->GetRecord().szCityArea))
		{
			CMessage::Message(_T("Can't delete the city when updating"), CMessageDlg::Mode::Error);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemText failed"));
			return;
		}
		if (!rListCtrl.SetItemData(nItemIndex, static_cast<DWORD_PTR>(pUpdateWrapper->GetRecord().lId)))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("OnUpdate: SetItemData"));
			return;
		}

		// Handles which item appears selected (have blue background).
		rListCtrl.SetItemState(nItemIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	}

	return;
}

void CCitiesView::OnInitialUpdate()
{
	// Parent class initialization
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	// Gets reference to the list control
	CListCtrl& rListCtrl = GetListCtrl();

	// Sets the style of the list control
	rListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	rListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (!CreateColumns(rListCtrl, 2))
	{
		CMessage::Message(_T("Cant load the view."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("CreateColumns failed"));
		return;
	}

	// Gets reference to the document
	m_pCitiesDocument = GetDocument();
	if (m_pCitiesDocument == nullptr)
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("GetDocument failed"));
		return;
	}

	// Gets reference to the map of the document
	const std::map<CString, City>& oCitiesMap = m_pCitiesDocument->GetDocumentMap();
	if (oCitiesMap.empty())
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("The document's map is empty."));
		return;
	}

	// Populates the list control
	if (!PopulateListCtrl(rListCtrl, oCitiesMap))
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("PopulateListCtrl failed."));
		return;
	}

	// Sets the width of the comuns of the list control
	if (!SetColumnsWidth(rListCtrl))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Setting the columns width of the CCitiesView failed."));
	}

	return;
}

// Message map

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CCitiesView::OnNMDblclk)
	ON_COMMAND(AFX_IDP_COMMAND_FAILURE, &CCitiesView::OnAfxIdpCommandFailure)
	ON_COMMAND(ID_CONTEXTMENU_VIEW, &CCitiesView::OnContextOptionView)
	ON_COMMAND(ID_CONTEXTMENU_CHANGE, &CCitiesView::OnContextOptionChange)
	ON_COMMAND(ID_CONTEXTMENU_ADD, &CCitiesView::OnContextOptionAdd)
	ON_COMMAND(ID_CONTEXTMENU_REMOVE, &CCitiesView::OnContextOptionRemove)
END_MESSAGE_MAP()

// Message handlers
void CCitiesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCitiesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	CMenu menu;

	// Load the resource of the context menu
	menu.LoadMenu(IDR_CONTEXT_MENU); // IDR_CONTEXT_MENU is the resource ID of the context menu

	// Get the pointer of the submenu to edit the options
	CMenu* pSubMenu = menu.GetSubMenu(0);
	if (pSubMenu == nullptr)
	{
		CMessage::Message(_T("Can't load the context menu"), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}
	
	// If no item is selected, disable the view, change and remove options
	CListCtrl& rListCtrl = GetListCtrl();
	if (rListCtrl.GetNextItem(-1, LVNI_SELECTED) == -1)
	{
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_VIEW, MF_BYCOMMAND | MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_CHANGE, MF_BYCOMMAND | MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_REMOVE, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		// Enable the view, change, add and remove options if an item is selected
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_VIEW, MF_BYCOMMAND | MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_CHANGE, MF_BYCOMMAND | MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_ADD, MF_BYCOMMAND | MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_CONTEXTMENU_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}

	// Display the context menu
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CCitiesView::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	// If no item is selected, do nothing
	CListCtrl& rListCtrl = GetListCtrl();
	if (rListCtrl.GetNextItem(-1, LVNI_SELECTED) == -1)
	{
		*pResult = 0;
		return;
	}

	// If an item is selected, send message to call OnContextOptionView
	SendMessage(WM_COMMAND, ID_CONTEXTMENU_VIEW);

	*pResult = 0;
	return;
}

void CCitiesView::OnContextOptionView()
{
	City oCity;
	int nCListCtrlSelectedItemIndex = 0;
	if (!GetCListCtrlSelectedItemIndex(nCListCtrlSelectedItemIndex))
	{
		CMessage::Message(_T("Can't open item for view. No item is selected."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	if (m_pCitiesDocument->SelectContainerItemByIndex(nCListCtrlSelectedItemIndex, oCity))
	{
		CMessage::Message(_T("Can't open item for view. The record is missing."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	ValidateRecordVersionResult enResult = CheckAndHandleRecordState(oCity, nCListCtrlSelectedItemIndex);

	if (enResult == ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER)
	{
		HandleValidationCanceledByUser(oCity);
		return;
	}

	if (enResult == ValidateRecordVersionResult::RECORD_NOT_FOUND)
	{
		if (HandleRecordNotFound(oCity, nCListCtrlSelectedItemIndex))
		{
			CMessage::Message(_T("Can't remove the record."), CMessageDlg::Mode::Information);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		}
		return;
	}

	else if (enResult == ValidateRecordVersionResult::VALIDATION_ERROR)
	{
		CMessage::Message(_T("Can't find the latest version of the record."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		HandleValidationError(oCity);
		return;
	}


	CCitiesDlg oCitiesDlg = CCitiesDlg(oCity, CCitiesDlg::RecordUpdateStatus::UpToDate, CCitiesDlg::DialogMode::View);

	oCitiesDlg.DoModal();

	return;

}

void CCitiesView::OnContextOptionChange()
{
	City oCity;
	int nCListCtrlSelectedItemIndex = 0;
	if (!GetCListCtrlSelectedItemIndex(nCListCtrlSelectedItemIndex))
	{
		CMessage::Message(_T("Can't open item for change. No item is selected."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}
	if (m_pCitiesDocument->SelectContainerItemByIndex(nCListCtrlSelectedItemIndex, oCity))
	{
		CMessage::Message(_T("Can't open item for change. The record is missing."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	ValidateRecordVersionResult enResult = CheckAndHandleRecordState(oCity, nCListCtrlSelectedItemIndex);

	if (enResult == ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER)
	{
		HandleValidationCanceledByUser(oCity);
		return;
	}

	if (enResult == ValidateRecordVersionResult::RECORD_NOT_FOUND)
	{
		if (HandleRecordNotFound(oCity, nCListCtrlSelectedItemIndex))
		{
			CMessage::Message(_T("Can't remove the record."), CMessageDlg::Mode::Information);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		}
		return;
	}

	else if (enResult == ValidateRecordVersionResult::VALIDATION_ERROR)
	{
		CMessage::Message(_T("Can't find the latest version of the record."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		HandleValidationError(oCity);
		return;
	}

	CCitiesDlg oCitiesDlg = CCitiesDlg(oCity, CCitiesDlg::RecordUpdateStatus::UpToDate, CCitiesDlg::DialogMode::Update);

	if (oCitiesDlg.DoModal() != IDOK)
	{
		return;
	}

	oCity = oCitiesDlg.m_recCity;

	if (!m_pCitiesDocument->UpdateRecordInDatabaseAndContainer(nCListCtrlSelectedItemIndex, oCity))
	{
		CMessage::Message(_T("Can't update the record"), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't update the record"));
		return;
	}

}

void CCitiesView::OnContextOptionAdd()
{
	City oCity;
	CCitiesDlg oCitiesDlg = CCitiesDlg(oCity,CCitiesDlg::RecordUpdateStatus::UpToDate, CCitiesDlg::DialogMode::Insert);

	if (oCitiesDlg.DoModal() != IDOK)
	{
		return;
	}

	oCity = oCitiesDlg.m_recCity;

	if (!m_pCitiesDocument->InsertRecordInDatabaseAndContainer(oCity))
	{
		CString strMessage;
		strMessage.Format(_T("Can't add the City %s %s"), oCity.szCityName, oCity.szCityArea);
		CMessage::Message(strMessage, CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return;
	}

	return;
}

void CCitiesView::OnContextOptionRemove()
{

	City oCity;
	int nCListCtrlSelectedItemIndex = 0;

	if (!GetCListCtrlSelectedItemIndex(nCListCtrlSelectedItemIndex))
	{
		CMessage::Message(_T("Can't open item for change. No item is selected."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	if (m_pCitiesDocument->SelectContainerItemByIndex(nCListCtrlSelectedItemIndex, oCity))
	{
		CMessage::Message(_T("Can't open item for change. The record is missing."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	ValidateRecordVersionResult enResult = CheckAndHandleRecordState(oCity, nCListCtrlSelectedItemIndex);

	if (enResult == ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER)
	{
		HandleValidationCanceledByUser(oCity);
		return;
	}

	if (enResult == ValidateRecordVersionResult::RECORD_NOT_FOUND)
	{
		if (HandleRecordNotFound(oCity, nCListCtrlSelectedItemIndex))
		{
			CMessage::Message(_T("Can't remove the record."), CMessageDlg::Mode::Information);
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		}
		return;
	}

	else if (enResult == ValidateRecordVersionResult::VALIDATION_ERROR)
	{
		CMessage::Message(_T("Can't find the latest version of the record."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		HandleValidationError(oCity);
		return;
	}

	CCitiesDlg oCitiesDlg = CCitiesDlg(oCity, CCitiesDlg::RecordUpdateStatus::UpToDate, CCitiesDlg::DialogMode::Delete);

	if (oCitiesDlg.DoModal() != IDOK)
	{
		return;
	}

	CString strMessage;
	strMessage.Format(_T("Are you sure you want to delete the city: %s %s"), oCity.szCityName, oCity.szCityArea);

	if (CMessage::Message(strMessage, CMessageDlg::Mode::Question) == IDNO)
	{
		return;
	}

	if (m_pCitiesDocument->DeleteRecordInDatabaseAndContainer(nCListCtrlSelectedItemIndex, oCity))
	{
		CMessage::Message(_T("Can't remove the record."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

}

void CCitiesView::OnAfxIdpCommandFailure()
{
}

// Methods

CCitiesView::ValidateRecordVersionResult CCitiesView::CheckAndHandleRecordState(City& rCity, int nCListCtrlSelectedItemIndex)
{
	City oNewCity;
	bool bIsRecordFound = false;
	bool bIsRecordUpToDate = false;
	if (m_pCitiesDocument->SelectDatabaseRecordById(rCity.lId, oNewCity, bIsRecordFound) && bIsRecordFound)
	{
		bIsRecordUpToDate = IsRecordUpToDate(rCity, oNewCity);
	}

	bool bUpdateConfirmed = false;
	if (!bIsRecordUpToDate)
	{
		if (!AskUserToUpdateSelectedRecord())
		{
			return ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER;
		}
		else
		{
			if (!bIsRecordFound)
			{
				return ValidateRecordVersionResult::RECORD_NOT_FOUND;
			}
			if (!m_pCitiesDocument->UpdateContainerItemByIndex(nCListCtrlSelectedItemIndex, oNewCity))
			{
				return ValidateRecordVersionResult::VALIDATION_ERROR;
			}

			rCity = oNewCity;
			return ValidateRecordVersionResult::VALIDATED;
		}
	}

	return ValidateRecordVersionResult::VALIDATED;
}

bool CCitiesView::HandleRecordNotFound(City& rCity, int nCListCtrlSelectedItemIndex)
{
	CMessage::Message(_T("No version of this record can be found!"), CMessageDlg::Mode::Information);
	CString strMessage;
	strMessage.Format(_T("Do you want to hide the record: %s %s"), rCity.szCityName, rCity.szCityArea);

	CMessageDlg oMessageDlg = CMessageDlg(strMessage, CMessageDlg::Mode::Question);
	auto result = oMessageDlg.DoModal();

	if (result == IDCANCEL)
	{
		CCitiesDlg oCitiesDlg = CCitiesDlg(rCity, CCitiesDlg::RecordUpdateStatus::Outdated, CCitiesDlg::DialogMode::View);
		oCitiesDlg.DoModal();
		return true;
	}

	if (m_pCitiesDocument->DeleteContainerItemByIndex(nCListCtrlSelectedItemIndex))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}

	return true;
}

void CCitiesView::HandleValidationError(City& rCity)
{
	CCitiesDlg oCitiesDlg = CCitiesDlg(rCity, CCitiesDlg::RecordUpdateStatus::Outdated, CCitiesDlg::DialogMode::View);
	oCitiesDlg.DoModal();
	return;
}

void CCitiesView::HandleValidationCanceledByUser(City& rCity)
{
	CCitiesDlg oCitiesDlg = CCitiesDlg(rCity, CCitiesDlg::RecordUpdateStatus::Outdated, CCitiesDlg::DialogMode::View);
	oCitiesDlg.DoModal();
	return;
}

BOOL CCitiesView::GetCListCtrlSelectedItemIndex(int& nCListCtrlSelectedItemIndex)
{
	CListCtrl& rListCtrl = GetListCtrl();
	nCListCtrlSelectedItemIndex = rListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nCListCtrlSelectedItemIndex == -1)
	{
		return FALSE;
	}

	return TRUE;
}

bool CCitiesView::IsRecordUpToDate(const City& rOldCity, const City& rNewCity)
{
	if (rOldCity.lUpdateCounter != rNewCity.lUpdateCounter)
	{
		return false;
	}

	return true;
}

bool CCitiesView::AskUserToUpdateSelectedRecord()
{
	CMessageDlg oMessageDlg = CMessageDlg(_T("This record is outdated.\nWould you like to load the latest version?\nOutdated records can't be modified or deleted."), CMessageDlg::Mode::Question);

	int nResult = oMessageDlg.DoModal();

	if (nResult == IDCANCEL)
	{
		return false;
	}

	return true;
}

bool CCitiesView::CreateColumns(CListCtrl& oListCtrl, int nColumnCount)
{
	// Important for the method SetColumnsWidth
	m_nColumnCount = nColumnCount;

	if (oListCtrl.InsertColumn(0, _T("City name    "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}
	if (oListCtrl.InsertColumn(1, _T("Area         "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}

	return true;
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
	return TRUE;
}

BOOL CCitiesView::PopulateListCtrl(CListCtrl& oListCtrl, const std::map<CString, City>& oCitiesMap)
{
	int nItemIndex = 0;
	for (std::map<CString, City>::const_iterator it = oCitiesMap.begin(); it != oCitiesMap.end(); ++it)
	{
		nItemIndex = oListCtrl.InsertItem(nItemIndex, it->second.szCityName);
		if (nItemIndex == -1)
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't insert the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemText(nItemIndex, 1, it->second.szCityArea))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemData(nItemIndex, static_cast<DWORD_PTR>(it->second.lId)))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		nItemIndex++;
	}

	return TRUE;
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






