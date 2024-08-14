#include "pch.h" // The precompiled header
#include "CitiesTable.h" 
#include "atldbcli.h" // From "atldbcli.h" we get the declaration of OLE DB classes, functions and macros.
#include<iostream>
#include "Message.h" // For displaying Messages to the GUI

/// <summary>
/// Loads all records from the Cities database table into a collection.
/// </summary>
/// <param name="oCitiesArray"> Collection from the document class</param>
/// <returns> Integer that tells if the method was successfull. </returns>
BOOL CCitiesTable::loadCities(std::vector<City>& oCitiesArray)
{
	HRESULT hResult;
	CDataSource oDataSource;
	CSession oSession;

	// Database connection configuration
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("KEROSWIFT\\MSSQLSERVER2016"));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Phonebook"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	// Opens the database connection
	hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CMessage::ErrorMessage(_T("Unable to connect to SQL server database."), hResult, __FILE__, __LINE__);
		return FALSE;
	}

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CMessage::ErrorMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		oDataSource.Close();
		return FALSE;
	}

	CString strQuery = _T("SELECT * FROM CITIES");

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CMessage::ErrorMessage(_T("Unable to run SQL query - \"SELECT * FROM CITIES\"."), hResult, __FILE__, __LINE__);
		oSession.Close();
		oDataSource.Close();
		return FALSE;
	}

	// Iterating trough the rowset
	while (MoveNext() == S_OK)
	{
		oCitiesArray.push_back(m_recCity);
	}

	Close();
	oSession.Close();
	oDataSource.Close();

	return TRUE;
}