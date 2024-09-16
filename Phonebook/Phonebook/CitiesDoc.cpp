
// CitiesDoc.cpp : implementation of the CCitiesDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "CitiesDoc.h"
#include "CitiesTable.h"
#include "Message.h"
#include "CitiesData.h"
#include "Log.h"
#include "Hints.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Constructors and Destructors

IMPLEMENT_DYNCREATE(CCitiesDoc, CDocument)

CCitiesDoc::CCitiesDoc() noexcept
{
	// TODO: add one-time construction code here

}

CCitiesDoc::~CCitiesDoc()
{
}

// Message map

BEGIN_MESSAGE_MAP(CCitiesDoc, CDocument)
END_MESSAGE_MAP()

// Methods

const std::map<CString, City>& CCitiesDoc::GetDocumentMap()
{
	return m_CitiesMap;
}

BOOL CCitiesDoc::SelectDatabaseRecordById(long lRecordId, City& rCity, bool& bIsRecordFound)
{
	CCitiesData oCitiesData;
	if (!oCitiesData.SelectRecordFromDatabaseById(lRecordId, rCity, bIsRecordFound))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to select the record with id: %ld"), lRecordId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}
	return TRUE;
}

BOOL CCitiesDoc::InsertRecordInDatabaseAndContainer(City& rCity)
{
	CCitiesData oCitiesData;
	if (!oCitiesData.InsertRecordInDatabase(rCity))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to insert the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}

	if (!InsertContainerItem(rCity))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return FALSE;
	}

	return TRUE;

}

BOOL CCitiesDoc::InsertContainerItem(City& rCity)
{
	CString key = CString(rCity.szCityName) + CString(rCity.szCityArea);
	auto result = m_CitiesMap.insert(std::make_pair(key, rCity));
	if (!result.second)
	{
		CString strMessage;
		strMessage.Format(_T("Unable to insert the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}

	int nInsertItemIndex = std::distance(m_CitiesMap.begin(), result.first);

	CityInsertHintWrapper* pWrapper = new CityInsertHintWrapper(nInsertItemIndex, rCity);
	UpdateAllViews(NULL, Hint::HINT_INSERT_ITEM, pWrapper);

	return TRUE;
}

BOOL CCitiesDoc::DeleteRecordInDatabaseAndContainer(int nItemIndex, City& rCity)
{
	CCitiesData oCitiesData;
	if (!oCitiesData.DeleteRecordFromDatabaseById(rCity.lId, rCity.lUpdateCounter))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to delete the record with id %ld"), rCity.lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}

	if (!DeleteContainerItemByIndex(nItemIndex))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return FALSE;
	}

	return TRUE;
}

bool CCitiesDoc::DeleteContainerItemByIndex(int nDeleteItemIndex) 
{
	if (nDeleteItemIndex < 0 || nDeleteItemIndex >= m_CitiesMap.size())
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Invalid index"));
		return false;
	}

	auto it = m_CitiesMap.begin();
	std::advance(it, nDeleteItemIndex);
	m_CitiesMap.erase(it);

	CityDeleteHintWrapper* pWrapper = new CityDeleteHintWrapper(nDeleteItemIndex);
	UpdateAllViews(NULL, Hint::HINT_DELETE_ITEM, pWrapper);

	return true;
}

BOOL CCitiesDoc::UpdateRecordInDatabaseAndContainer(int nDeleteItemIndex, City& rCity)
{
	CCitiesData oCitiesData;
	if (!oCitiesData.UpdateRecordInDatabaseById(rCity))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to update the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}

	if (!UpdateContainerItemByIndex(nDeleteItemIndex, rCity))
	{
		return FALSE;
	}
}

BOOL CCitiesDoc::UpdateContainerItemByIndex(int& nDeleteItemIndex, City& rCity)
{
	auto it = m_CitiesMap.begin();
	std::advance(it, nDeleteItemIndex);
	m_CitiesMap.erase(it);

	CString key = CString(rCity.szCityName) + CString(rCity.szCityArea);
	auto result = m_CitiesMap.insert(std::make_pair(key, rCity));
	if (!result.second)
	{
		CString strMessage;
		strMessage.Format(_T("Unable to insert the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}
	auto insertIt = result.first;
	int nInsertIndex = std::distance(m_CitiesMap.begin(), result.first);

	CityUpdateHintWrapper* pWrapper = new CityUpdateHintWrapper(nDeleteItemIndex, nInsertIndex, rCity);
	UpdateAllViews(NULL, Hint::HINT_UPDATE_ITEM, pWrapper);

	return TRUE;
}

bool CCitiesDoc::SelectContainerItemByIndex(int nSelectedItemIndex, City& rCity)
{
	if (nSelectedItemIndex < 0 || nSelectedItemIndex >= m_CitiesMap.size())
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Invalid index"));
		return false;
	}

	auto it = m_CitiesMap.begin();
	std::advance(it, nSelectedItemIndex);
	rCity = it->second;

	return true;
}

// Overrides
BOOL CCitiesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CCitiesData oCitiesData;
	if (!oCitiesData.SelectAllRecordsFromDatabase(m_CitiesMap))
	{
		return FALSE;
	}

	return TRUE;
}

// CCitiesDoc serialization

void CCitiesDoc::Serialize(CArchive& ar)
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

// Diagnostics

#ifdef _DEBUG
void CCitiesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCitiesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCitiesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCitiesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS
