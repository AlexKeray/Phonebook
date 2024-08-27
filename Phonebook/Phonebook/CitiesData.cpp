#include "pch.h"
#include "CitiesData.h"
#include "CitiesTable.h"
#include "atldbcli.h"
#include "DBConnection.h"
#include "Log.h"
#include "Message.h"

BOOL CCitiesData::SelectAll(std::map<CString, City>& oCitiesMap)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::LogMessage(_T("Unable to create Session."), hResult, __FILE__, __LINE__);
		return FALSE;
	}

	// Selects all Cities
	if (!oCitiesTable.SelectAll(oSession, oCitiesMap))
	{
		oSession.Close();
		CLog::LogMessage(_T("Unable to select all Cities."), __FILE__, __LINE__);
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}