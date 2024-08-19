#include "pch.h"
#include "DBConnection.h"
#include "Message.h"

CDBConnection::CDBConnection()
{
	// Tells if a database connection exists.
	bIsDataSourceOpened = false;

	// Database connection configuration
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("KEROSWIFT\\MSSQLSERVER2016"));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Phonebook"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	// Opens the database connection
	 HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CMessage::ErrorMessage(_T("Unable to connect to SQL server database."), hResult, __FILE__, __LINE__);
	}
	else
	{
		bIsDataSourceOpened = true;
	}
}

CDBConnection::~CDBConnection()
{
	if (bIsDataSourceOpened)
	{
		oDataSource.Close();
	}
}

/// <summary>
/// Used by the singleton to create references to the only one object instead of creting a new object.
/// </summary>
/// <returns> Reference to the singleton's object.</returns>
CDBConnection& CDBConnection::GetInstance()
{
    static CDBConnection instance = CDBConnection();
    return instance;
}


