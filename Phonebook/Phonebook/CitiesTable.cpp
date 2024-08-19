#include "pch.h" // The precompiled header
#include "CitiesTable.h" 
#include "atldbcli.h" // From "atldbcli.h" we get the declaration of OLE DB classes, functions and macros.
#include<iostream>
#include "Message.h" // For displaying Messages to the GUI
#include "Log.h" // For logging error info into a log file
#include "DBConnection.h"

/// <summary>
/// Loads all records from the Cities database table into a collection.
/// </summary>
/// <param name="oCitiesArray"> Collection from the document class</param>
/// <returns> Integer that tells if the method was successfull. </returns>
BOOL CCitiesTable::SelectAll(std::vector<City>& oCitiesVec)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		oDataSource.Close();
		return FALSE;
	}

	CString strQuery = _T("SELECT * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to run SQL query - \"SELECT * FROM CITIES\"."), hResult, __FILE__, __LINE__);
		oSession.Close();
		oDataSource.Close();
		return FALSE;
	}

	// Iterating trough the rowset
	while (MoveNext() == S_OK)
	{
		oCitiesVec.push_back(m_recCity);
	}

	Close();
	oSession.Close();
	oDataSource.Close();

	return TRUE;
}

BOOL CCitiesTable::SelectWhereID(const long lID, std::vector<City>& oCitiesVec)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		oDataSource.Close();
		return FALSE;
	}

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE ID=%ld"), lID);

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to run SQL query - \"SELECT * FROM CITIES WHERE ID=%d\"."), hResult, __FILE__, __LINE__);
		oSession.Close();
		oDataSource.Close();
		return FALSE;
	}
	

	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CLog::LogMessage(_T("No record found - \"SELECT * FROM CITIES WHERE ID=%d\"."), hResult, __FILE__, __LINE__);
		Close();
		oSession.Close();
		oDataSource.Close();
		return FALSE;
	}

	oCitiesVec.push_back(m_recCity);

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CLog::LogMessage(_T("Extra records found. 1 was expected - \"SELECT * FROM CITIES WHERE ID=%d\"."), __FILE__, __LINE__);
		Close();
		oSession.Close();
		oDataSource.Close();
		return FALSE;
	}

	Close();
	oSession.Close();
	oDataSource.Close();

	return TRUE;
}
