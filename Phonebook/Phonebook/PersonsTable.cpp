#include "pch.h"
#include "PersonsTable.h"
#include "atldbcli.h"
#include <afx.h>
#include "Log.h"

BOOL CPersonsTable::SelectAllRecordsFromDatabase(CSession& oSession, std::list<Person>& oPersonsList)
{
	HRESULT hResult;

	CString strQuery = _T("SELECT * FROM PERSONS");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	// Iterating trough the rowset
	while (MoveNext() == S_OK)
	{
		oPersonsList.push_back(m_recPerson);
	}

	Close();

	return TRUE;
}

BOOL CPersonsTable::SelectWhereID(CSession& oSession, const long lID, Person& rPerson, bool& bIsRecordFound)
{
	HRESULT hResult;

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM PERSONS WHERE ID=%ld"), lID);

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		bIsRecordFound = false;
		CString strMessage;
		strMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strQuery);
		return FALSE;
	}

	// Iterating trough the rowset
	hResult = MoveFirst();
	if (hResult != S_OK)
	{

		if (hResult == DB_S_ENDOFROWSET)
		{
			bIsRecordFound = false;
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("No record found - \"SELECT * FROM CITIES WHERE ID=%d\"."));
			Close();
			return TRUE;
		}

		bIsRecordFound = false;
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Problem with the rowset - \"SELECT * FROM CITIES WHERE ID=%d\"."));
		Close();
		return FALSE;
	}

	rPerson = m_recPerson;

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		bIsRecordFound = false;
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Extra records found. 1 was expected - \"SELECT * FROM CITIES WHERE ID=%d\"."));
		Close();
		return FALSE;
	}

	bIsRecordFound = true;
	Close();
	return TRUE;
}