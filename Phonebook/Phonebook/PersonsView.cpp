// PersonsView.cpp : implementation file
//

#include "pch.h"
#include "Phonebook.h"
#include "PersonsView.h"
#include "Log.h"
#include "Message.h"
#include "PersonsDlg.h"
#include "PersonsDoc.h"


// CPersonsView

// Constructors and Destructors

IMPLEMENT_DYNCREATE(CPersonsView, CListView)

CPersonsView::CPersonsView()
{

}

CPersonsView::~CPersonsView()
{
}

// Message map

BEGIN_MESSAGE_MAP(CPersonsView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CPersonsView::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CPersonsView::OnNMRClick)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTMENU_VIEW, &CPersonsView::OnContextmenuView)
	ON_COMMAND(ID_CONTEXTMENU_ADD, &CPersonsView::OnContextmenuAdd)
	ON_COMMAND(ID_CONTEXTMENU_CHANGE, &CPersonsView::OnContextmenuChange)
	ON_COMMAND(ID_CONTEXTMENU_REMOVE, &CPersonsView::OnContextmenuRemove)
END_MESSAGE_MAP()

// Message handlers
void CPersonsView::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
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

void CPersonsView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	CPoint point = pNMItemActivate->ptAction;

	// Convert the point to screen coordinates
	ClientToScreen(&point);

	// Call the OnContextMenu method with the converted point
	OnContextMenu(this, point);

	*pResult = 0;
}

void CPersonsView::OnContextmenuView()
{
	int nCListCtrlSelectedItemIndex = 0;
	if (!GetCListCtrlSelectedItemIndex(nCListCtrlSelectedItemIndex))
	{
		CMessage::Message(_T("Can't open item for view. No item is selected."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}
	std::tuple<Person, City, std::list<Phonenumber>> oPersonsMapValue;
	if (!m_pPersonsDocument->SelectContainerItemByIndex(nCListCtrlSelectedItemIndex, oPersonsMapValue))
	{
		CMessage::Message(_T("Can't open item for view. The record is missing."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	ValidateRecordVersionResult enResult = CheckAndHandleRecordState(oPersonsMapValue, nCListCtrlSelectedItemIndex);

	if (enResult == ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER)
	{
		HandleValidationCanceledByUser(std::get<0>(oPersonsMapValue), std::get<1>(oPersonsMapValue), std::get<2>(oPersonsMapValue));
		return;
	}

	//if (enResult == ValidateRecordVersionResult::RECORD_NOT_FOUND)
	//{
	//	if (!HandleRecordNotFound(oCity, nCListCtrlSelectedItemIndex))
	//	{
	//		CMessage::Message(_T("Can't remove the record."), CMessageDlg::Mode::Information);
	//		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
	//	}
	//	return;
	//}

	//else if (enResult == ValidateRecordVersionResult::VALIDATION_ERROR)
	//{
	//	CMessage::Message(_T("Can't find the latest version of the record."), CMessageDlg::Mode::Error);
	//	CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
	//	HandleValidationError(oCity);
	//	return;
	//}

	std::list<City> oCitiesList;
	oCitiesList.push_back(std::get<1>(oPersonsMapValue));
	CPersonsDlg oPersonsDlg = CPersonsDlg(std::get<0>(oPersonsMapValue), 0, oCitiesList, std::get<2>(oPersonsMapValue), CPersonsDlg::RecordUpdateStatus::UpToDate, CPersonsDlg::DialogMode::View);

	oPersonsDlg.DoModal();

	return;

}

void CPersonsView::OnContextmenuAdd()
{
	// TODO: Add your command handler code here
}

void CPersonsView::OnContextmenuChange()
{
	// TODO: Add your command handler code here
}

void CPersonsView::OnContextmenuRemove()
{
	// TODO: Add your command handler code here
}

// Overrides

void CPersonsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	CListCtrl& rListCtrl = GetListCtrl();

	rListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	rListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (!CreateColumns(rListCtrl, 4))
	{
		CMessage::Message(_T("Can't load the view."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	// Gets reference to the document
	m_pPersonsDocument = GetDocument();
	if (m_pPersonsDocument == nullptr)
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("GetDocument failed"));
		return;
	}

	// Gets reference to the map of the document
	auto& oPersonsMap = m_pPersonsDocument->GetDocumentMap();
	if (oPersonsMap.empty())
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("The document's map is empty."));
		return;
	}

	// Populates the list control
	if (!PopulateListCtrl(rListCtrl, oPersonsMap))
	{
		CMessage::Message(_T("Loading data from the document failed."), CMessageDlg::Mode::Error);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return;
	}

	// Sets the width of the comuns of the list control
	if (!SetColumnsWidth(rListCtrl))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Setting the columns width of the CCitiesView failed."));
	}
}

void CPersonsView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
}

void CPersonsView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
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
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//CListCtrl& rListCtrl = GetListCtrl();
	//if (rListCtrl.GetNextItem(-1, LVNI_SELECTED) == -1)
	//{
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_VIEW, MF_BYCOMMAND | MF_GRAYED);
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_CHANGE, MF_BYCOMMAND | MF_GRAYED);
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_REMOVE, MF_BYCOMMAND | MF_GRAYED);
	//}
	//else
	//{
	//	// Enable the view, change, add and remove options if an item is selected
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_VIEW, MF_BYCOMMAND | MF_ENABLED);
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_CHANGE, MF_BYCOMMAND | MF_ENABLED);
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_ADD, MF_BYCOMMAND | MF_ENABLED);
	//	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//}

	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_VIEW, MF_BYCOMMAND | MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_CHANGE, MF_BYCOMMAND | MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_REMOVE, MF_BYCOMMAND | MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_CONTEXTMENU_ADD, MF_BYCOMMAND | MF_GRAYED);

	// Display the context menu
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

// Methods

bool CPersonsView::CreateColumns(CListCtrl& oListCtrl, int nColumnCount)
{
	// Important for the method SetColumnsWidth
	m_nColumnCount = nColumnCount;

	if (oListCtrl.InsertColumn(0, _T("First name    "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}
	if (oListCtrl.InsertColumn(1, _T("Second name    "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}
	if (oListCtrl.InsertColumn(2, _T("Last name    "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}
	if (oListCtrl.InsertColumn(3, _T("City    "), LVCFMT_LEFT) == -1)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}

	return true;
}

bool CPersonsView::SetColumnsWidth(CListCtrl& oListCtrl, int nRightPadding)
{
	// Autosize columns based on the longest item or the header
	for (int nColumnIndex = 0; nColumnIndex < m_nColumnCount; ++nColumnIndex)
	{
		if (!oListCtrl.SetColumnWidth(nColumnIndex, LVSCW_AUTOSIZE))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
			return false;
		}
		int nColumnAutosizeWidth = oListCtrl.GetColumnWidth(nColumnIndex);
		if (!oListCtrl.SetColumnWidth(nColumnIndex, LVSCW_AUTOSIZE_USEHEADER))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
			return false;
		}
		int nHeaderAutosizeWidth = oListCtrl.GetColumnWidth(nColumnIndex);
		if (nColumnAutosizeWidth > nHeaderAutosizeWidth)
		{
			if (!oListCtrl.SetColumnWidth(nColumnIndex, nColumnAutosizeWidth + nRightPadding))
			{
				CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
				return false;
			}
		}
		else
		{
			if (!oListCtrl.SetColumnWidth(nColumnIndex, nHeaderAutosizeWidth + nRightPadding))
			{
				CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
				return false;
			}
		}
	}
	return true;
}

CPersonsDoc* CPersonsView::GetDocument() const
{
	return reinterpret_cast<CPersonsDoc*>(m_pDocument);
}

BOOL CPersonsView::PopulateListCtrl(CListCtrl& oListCtrl, const std::map<int, std::tuple<Person, City, std::list<Phonenumber>>>& oPersonsMap)
{
	int nItemIndex = 0;
	for (auto it = oPersonsMap.begin(); it != oPersonsMap.end(); ++it)
	{
		nItemIndex = oListCtrl.InsertItem(nItemIndex, std::get<0>(it->second).szFirstName);
		if (nItemIndex == -1)
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't insert the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemText(nItemIndex, 1, std::get<0>(it->second).szMiddleName))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemText(nItemIndex, 2, std::get<0>(it->second).szLastName))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemText(nItemIndex, 3, std::get<1>(it->second).szCityName))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		if (!oListCtrl.SetItemData(nItemIndex, static_cast<DWORD_PTR>(std::get<0>(it->second).lId)))
		{
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Can't set the data of the item in the List control."));
			return FALSE;
		}
		nItemIndex++;
	}

	return TRUE;
}

BOOL CPersonsView::GetCListCtrlSelectedItemIndex(int& nCListCtrlSelectedItemIndex)
{
	CListCtrl& rListCtrl = GetListCtrl();
	nCListCtrlSelectedItemIndex = rListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nCListCtrlSelectedItemIndex == -1)
	{
		return FALSE;
	}

	return TRUE;
}

CPersonsView::ValidateRecordVersionResult CPersonsView::CheckAndHandleRecordState(std::tuple<Person, City, std::list<Phonenumber>>& oPersonsOldMapValue, int nCListCtrlSelectedItemIndex)
{
	Person oNewPerson;
	City oNewCity;
	bool bIsRecordFound = false;
	bool bIsRecordUpToDate = false;
	std::tuple<Person, City, std::list<Phonenumber>> oPersonsNewMapValue;
	if (m_pPersonsDocument->SelectDatabaseRecordById(std::get<0>(oPersonsOldMapValue).lId, oPersonsNewMapValue, bIsRecordFound) && bIsRecordFound)
	{
		bIsRecordUpToDate = IsRecordUpToDate(oPersonsOldMapValue, oPersonsNewMapValue);
	}

	bool bUpdateConfirmed = false;
	if (!bIsRecordUpToDate)
	{
		return ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER;
		//if (!AskUserToUpdateSelectedRecord())
		//{
		//	return ValidateRecordVersionResult::VALIDATION_CANCELED_BY_USER;
		//}
		//else
		//{
		//	if (!bIsRecordFound)
		//	{
		//		return ValidateRecordVersionResult::RECORD_NOT_FOUND;
		//	}
		//	//if (!m_pPersonsDocument->UpdateContainerItemByIndex(nCListCtrlSelectedItemIndex, oNewCity))
		//	//{
		//	//	return ValidateRecordVersionResult::VALIDATION_ERROR;
		//	//}

		//	std::get<0>(oPersonsOldMapValue) = std::get<0>(oPersonsNewMapValue);
		//	std::get<1>(oPersonsOldMapValue) = std::get<1>(oPersonsNewMapValue);
		//	std::get<2>(oPersonsOldMapValue).clear();
		//	for (auto& oNewPhonenumber : std::get<2>(oPersonsNewMapValue))
		//	{
		//		std::get<2>(oPersonsOldMapValue).push_back(oNewPhonenumber);
		//	}
		//	return ValidateRecordVersionResult::VALIDATED;
		//}
	}

	return ValidateRecordVersionResult::VALIDATED;
}

bool CPersonsView::IsRecordUpToDate(const std::tuple<Person, City, std::list<Phonenumber>>& oPersonsOldMapValue, const std::tuple<Person, City, std::list<Phonenumber>>& oPersonsNewMapValue)
{
	if (std::get<0>(oPersonsOldMapValue).lUpdateCounter != std::get<0>(oPersonsNewMapValue).lUpdateCounter)
	{
		return false;
	}
	if (std::get<1>(oPersonsOldMapValue).lUpdateCounter != std::get<1>(oPersonsNewMapValue).lUpdateCounter)
	{
		return false;
	}

	// Compare lists of Phonenumbers
	const std::list<Phonenumber>& oldPhoneNumbers = std::get<2>(oPersonsOldMapValue);
	const std::list<Phonenumber>& newPhoneNumbers = std::get<2>(oPersonsNewMapValue);

	if (oldPhoneNumbers.size() != newPhoneNumbers.size())
	{
		return false;
	}

	auto oldIt = oldPhoneNumbers.begin();
	auto newIt = newPhoneNumbers.begin();

	while (oldIt != oldPhoneNumbers.end() && newIt != newPhoneNumbers.end())
	{
		if (oldIt->lUpdateCounter != newIt->lUpdateCounter)
		{
			return false;
		}
		++oldIt;
		++newIt;
	}

	return true;
}

bool CPersonsView::AskUserToUpdateSelectedRecord()
{
	CMessageDlg oMessageDlg = CMessageDlg(_T("This record is outdated.\nWould you like to load the latest version?\nOutdated records can't be modified or deleted."), CMessageDlg::Mode::Question);

	int nResult = oMessageDlg.DoModal();

	if (nResult == IDCANCEL)
	{
		return false;
	}

	return true;
}

void CPersonsView::HandleValidationCanceledByUser(Person& rPerson, City& rCity, std::list<Phonenumber>& oPhonenumbersList)
{
	std::list<City> oCitiesList;
	oCitiesList.push_back(rCity);
	CPersonsDlg oPersonsDlg = CPersonsDlg(rPerson, 0, oCitiesList, oPhonenumbersList, CPersonsDlg::RecordUpdateStatus::Outdated, CPersonsDlg::DialogMode::View);
	oPersonsDlg.DoModal();
	return;
}

// CPersonsView diagnostics

#ifdef _DEBUG
void CPersonsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CPersonsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


