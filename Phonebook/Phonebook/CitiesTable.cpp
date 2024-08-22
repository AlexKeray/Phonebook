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
		return FALSE;
	}

	CString strQuery = _T("SELECT * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to run SQL query - \"SELECT * FROM CITIES\"."), hResult, __FILE__, __LINE__);
		oSession.Close();
		return FALSE;
	}

	// Iterating trough the rowset
	while (MoveNext() == S_OK)
	{
		oCitiesVec.push_back(m_recCity);
	}

	Close();
	oSession.Close();

	return TRUE;
}

BOOL CCitiesTable::SelectWhereID(const long lID, City& oCity)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
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
		return FALSE;
	}
	

	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CLog::LogMessage(_T("No record found - \"SELECT * FROM CITIES WHERE ID=%d\"."), hResult, __FILE__, __LINE__);
		Close();
		oSession.Close();
		return FALSE;
	}

	oCity = m_recCity;

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		oCity = City();
		CLog::LogMessage(_T("Extra records found. 1 was expected - \"SELECT * FROM CITIES WHERE ID=%d\"."), __FILE__, __LINE__);
		Close();
		oSession.Close();
		return FALSE;
	}

	Close();
	oSession.Close();

	return TRUE;
}

BOOL CCitiesTable::UpdateWhereID(const long lID, const City& recNewRecord)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		return FALSE;
	}

	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::LogMessage(_T("Unable to configure the DBPropSet."), __FILE__, __LINE__);
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WITH(UPDLOCK) WHERE ID=%ld"), lID); // UPDLOCK ensures that only this session can modify or lock the table, while other session can only read the the table. That said, other sessions that just select data will succseed, others that select with some lock will wait until the session that locks the table finishes its execution. The lock's lifetime is from the time when the query is ran, to the end of the session.

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to start a transaction. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Close();
		return FALSE;
	}

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		oSession.Close();
		return FALSE;
	}


	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("No record found. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	if (m_recCity.lUpdateCounter != recNewRecord.lUpdateCounter)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("The record with ID: %d is outdated. Please refresh to view the latest changes before attempting to update it."), m_recCity.lId);
		CMessage::Message(strFormattedMessage);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	// Set the new values for the record
	m_recCity = recNewRecord;

	m_recCity.lUpdateCounter++;

	// Update the record in the database
	hResult = SetData(1);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to update the record with ID: %d"), m_recCity.lId);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 1 was expected. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	hResult = oSession.Commit();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to commit the transaction. It is rollbacked instead. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	Close();
	oSession.Close();

	return TRUE;
}

BOOL CCitiesTable::Insert(City& recNewRecord)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		return FALSE;
	}

	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::LogMessage(_T("Unable to configure the DBPropSet."), __FILE__, __LINE__);
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery = _T("SELECT TOP 0 * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Close();
		return FALSE;
	}

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 0 was expected - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		Close();
		oSession.Close();
		return FALSE;
	}

	// Set the new values for the record
	m_recCity = recNewRecord;

	m_recCity.lUpdateCounter = 0;

	// Update the record in the database
	hResult = CCommand::Insert(1);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to insert the record."), hResult, __FILE__, __LINE__);
		Close();
		oSession.Close();
		return FALSE;
	}


	Close();
	oSession.Close();

	return TRUE;
}

BOOL CCitiesTable::DeleteWhereID(const long lID, const City& recNewRecord)
{
	HRESULT hResult;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		return FALSE;
	}

	// Configures the rowset for updating the database.
	CDBPropSet oUpdateDBPropSet;
	if (!ConfigureRowsetForDBModification(oUpdateDBPropSet))
	{
		CLog::LogMessage(_T("Unable to configure the DBPropSet."), __FILE__, __LINE__);
		return FALSE;
	}

	// Configuring the SQL query
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WITH(UPDLOCK) WHERE ID=%ld"), lID);

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to start a transaction. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Close();
		return FALSE;
	}

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to run SQL query. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		oSession.Close();
		return FALSE;
	}

	// Iterating trough the rowset
	if (MoveFirst() != S_OK)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("No record found. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	if (m_recCity.lUpdateCounter != recNewRecord.lUpdateCounter)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("The record with ID: %d is outdated. Please refresh to view the latest changes before attempting to delete it."), m_recCity.lId);
		CMessage::Message(strFormattedMessage);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	hResult = Delete();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to delete the record with ID: %d"), m_recCity.lId);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	// Checking for excess records.
	if (MoveNext() != DB_S_ENDOFROWSET)
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Extra records found. 1 was expected. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	hResult = oSession.Commit();
	if (FAILED(hResult))
	{
		CString strFormattedMessage;
		strFormattedMessage.Format(_T("Unable to commit the transaction. It is rollbacked instead. - \"%s\"."), strQuery);
		CLog::LogMessage(strFormattedMessage, hResult, __FILE__, __LINE__);
		oSession.Abort();
		Close();
		oSession.Close();
		return FALSE;
	}

	Close();
	oSession.Close();

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