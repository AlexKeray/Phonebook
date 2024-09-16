// PersonsDoc.cpp : implementation file
//

#include "pch.h"
#include "Phonebook.h"
#include "PersonsDoc.h"
#include "PersonsData.h"
#include "Log.h"


// CPersonsDoc

// Constructors and Destructors

IMPLEMENT_DYNCREATE(CPersonsDoc, CDocument)

CPersonsDoc::CPersonsDoc()
{
}

CPersonsDoc::~CPersonsDoc()
{
}

// Message map

BEGIN_MESSAGE_MAP(CPersonsDoc, CDocument)
END_MESSAGE_MAP()

// Overrides

BOOL CPersonsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CPersonsData oPersonsData;
	if (!oPersonsData.SelectAllRecordsFromDatabase(m_oPersonsMap))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return FALSE;
	}

	return TRUE;
}

// Methods
const std::map<int, std::tuple < Person, City, std::list<Phonenumber>>>& CPersonsDoc::GetDocumentMap()
{
	return m_oPersonsMap;
}

bool CPersonsDoc::SelectContainerItemByIndex(int nSelectedItemIndex, std::tuple < Person, City, std::list<Phonenumber>>& rMapValue)
{
	if (nSelectedItemIndex < 0 || nSelectedItemIndex >= m_oPersonsMap.size())
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Invalid index"));
		return false;
	}

	auto it = m_oPersonsMap.begin();
	std::advance(it, nSelectedItemIndex);
	rMapValue = it->second;
	//rPerson = std::get<0>(it->second);
	//rCity = std::get<1>(it->second);

	return true;
}

BOOL CPersonsDoc::SelectDatabaseRecordById(long lRecordId, std::tuple < Person, City, std::list<Phonenumber>>& rMapValue, bool& bIsRecordFound)
{
	CPersonsData oPersonsData;
	if (!oPersonsData.SelectRecordFromDatabaseById(lRecordId, rMapValue, bIsRecordFound))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to select the record with id: %ld"), lRecordId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strMessage);
		return FALSE;
	}

	return TRUE;
}

// CPersonsDoc diagnostics

#ifdef _DEBUG
void CPersonsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPersonsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPersonsDoc serialization

void CPersonsDoc::Serialize(CArchive& ar)
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
#endif



