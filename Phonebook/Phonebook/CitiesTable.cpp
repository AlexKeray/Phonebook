#include "pch.h" // The precompiled header
#include "CitiesTable.h" 
#include "atldbcli.h" // From "atldbcli.h" we get the declaration of OLE DB classes, functions and macros.
#include <iostream>
#include "Message.h" // For displaying Messages to the GUI
#include "Log.h" // For logging error info into a log file
#include "DBConnection.h"
#include <map>

/// <summary>
/// Loads all records from the Cities database table into a collection.
/// </summary>
/// <param name="oCitiesArray"> Collection from the document class</param>
/// <returns> Integer that tells if the method was successfull. </returns>
BOOL CCitiesTable::SelectAllRecordsFromDatabase(CSession& oSession, std::map<CString, City>& oCitiesMap)
{
	HRESULT hResult;

	CString strQuery = _T("SELECT * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to run SQL query - \"SELECT * FROM CITIES\"."));
		oSession.Close();
		return FALSE;
	}

	// Iterating trough the rowset
	while (MoveNext() == S_OK)
	{
		CString key = CString(m_recCity.szCityName) + CString(m_recCity.szCityArea);
		oCitiesMap[key] = m_recCity;
	}

	Close();
	oSession.Close();

	return TRUE;
}

BOOL CCitiesTable::SelectWhereID(CSession& oSession, const long lID, City& oCity, bool& bIsRecordFound)
{
	HRESULT hResult;

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE ID=%ld"), lID);

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		bIsRecordFound = false;
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to run SQL query - \"SELECT * FROM CITIES WHERE ID=%d\"."));
		return FALSE;
	}
	

	// Iterating trough the rowset
	hResult = MoveFirst();
	if (hResult != S_OK)
	{

		if (hResult == DB_S_ENDOFROWSET)
		{
			oCity = City();
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

	oCity = m_recCity;

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		oCity = City();
		bIsRecordFound = false;
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Extra records found. 1 was expected - \"SELECT * FROM CITIES WHERE ID=%d\"."));
		Close();
		return FALSE;
	}

	bIsRecordFound = true;
	Close();
	return TRUE;
}

BOOL CCitiesTable::UpdateWhereID(CSession& oSession, const long lID, City& recNewRecord)
{
	HRESULT hResult;

	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Unable to configure the DBPropSet."));
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WITH(UPDLOCK) WHERE ID=%ld"), lID); // UPDLOCK ensures that only this session can modify or lock the table, while other session can only read the the table. That said, other sessions that just select data will succseed, others that select with some lock will wait until the session that locks the table finishes its execution. The lock's lifetime is from the time when the query is ran, to the end of the session.


	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		return FALSE;
	}


	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("No record found. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	if (m_recCity.lUpdateCounter != recNewRecord.lUpdateCounter)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("The record with ID: %d is outdated. Please refresh to view the latest changes before attempting to update it."), m_recCity.lId);
		//CMessage::Message(strFormattedMessage);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, strFormattedMessage);
		Close();
		return FALSE;
	}

	// Set the new values for the record
	m_recCity = recNewRecord;

	m_recCity.lUpdateCounter++;

	// UpdateRecordInDatabaseById the record in the database
	hResult = SetData(1);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to update the record with ID: %d"), m_recCity.lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	recNewRecord.lUpdateCounter = m_recCity.lUpdateCounter;

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 1 was expected. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}


	Close();

	return TRUE;
}

BOOL CCitiesTable::InsertRecordInDatabase(CSession& rSession, City& recNewRecord)
{
	HRESULT hResult;
	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Unable to configure the DBPropSet."));
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery = _T("SELECT TOP 0 * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(rSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		return FALSE;
	}

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 0 was expected - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	// Set the new values for the record
	m_recCity = recNewRecord;

	m_recCity.lUpdateCounter = 0;

	// UpdateRecordInDatabaseById the record in the database
	hResult = CCommand::Insert(1);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to insert the record."));
		Close();
		return FALSE;
	}

	Close();

	strQuery = _T("SELECT TOP 1 * FROM CITIES ORDER BY ID DESC");

	// Executes a query and opens a rowset.
	hResult = Open(rSession, strQuery);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		return FALSE;
	}

	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("No record found. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	recNewRecord.lId = m_recCity.lId;
	recNewRecord.lUpdateCounter = m_recCity.lUpdateCounter;
	_tcscpy_s(recNewRecord.szCityName, sizeof(recNewRecord.szCityName) / sizeof(TCHAR), m_recCity.szCityName);
	_tcscpy_s(recNewRecord.szCityArea, sizeof(recNewRecord.szCityArea) / sizeof(TCHAR), m_recCity.szCityArea);

	Close();

	return TRUE;
}

BOOL CCitiesTable::DeleteWhereID(CSession& rSession, const long lID, long lUpdateCounter)
{
	HRESULT hResult;

	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, _T("Unable to configure the DBPropSet."));
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WITH(UPDLOCK) WHERE ID=%ld"), lID);

	// Executes a query and opens a rowset.
	hResult = Open(rSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		return FALSE;
	}

	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("No record found. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	if (m_recCity.lUpdateCounter != lUpdateCounter)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("The record with ID: %d is outdated. Please refresh to view the latest changes before attempting to delete it."), m_recCity.lId);
		//CMessage::Message(strFormattedMessage);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	hResult = Delete();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to delete the record with ID: %d"), m_recCity.lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 1 was expected. - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strFormattedMessage);
		Close();
		return FALSE;
	}

	Close();
	return TRUE;
}

BOOL CCitiesTable::ConfigureRowsetForDBModification(CDBPropSet& oUpdateDBPropSet)
{
	oUpdateDBPropSet.SetGUID(DBPROPSET_ROWSET);
	if (!oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true))
	{
		return FALSE;
	}
	if (!oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true))
	{
		return FALSE;
	}
	if (!oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true))
	{
		return FALSE;
	}
	if (!oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE))
	{
		return FALSE;
	}
	if (!oUpdateDBPropSet.AddProperty(DBPROP_CANHOLDROWS, true))
	{
		return FALSE;
	}

	return TRUE;
}