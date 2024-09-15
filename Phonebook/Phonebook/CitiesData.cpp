#include "pch.h"
#include "CitiesData.h"
#include "CitiesTable.h"
#include "atldbcli.h"
#include "DBConnection.h"
#include "Log.h"
#include "Message.h"

BOOL CCitiesData::SelectAllRecordsFromDatabase(std::map<CString, City>& oCitiesMap)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to create Session."));
		return FALSE;
	}

	// Selects all Cities
	if (!oCitiesTable.SelectAllRecordsFromDatabase(oSession, oCitiesMap))
	{
		oSession.Close();
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to select all Cities."));
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}

BOOL CCitiesData::SelectRecordFromDatabaseById(const long lId, City& rCity, bool& bIsRecordFound)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to create Session."));
		return FALSE;
	}

	// Selects City by id
	if (!oCitiesTable.SelectWhereID(oSession, lId, rCity, bIsRecordFound))
	{
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to select the record with id: %ld"), lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}

BOOL CCitiesData::InsertRecordInDatabase(City& rCity)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to create Session."));
		return FALSE;
	}

	// InsertRecordInDatabase City
	if (!oCitiesTable.InsertRecordInDatabase(oSession, rCity))
	{
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to insert the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}

BOOL CCitiesData::DeleteRecordFromDatabaseById(long lId, long lUpdateCounter)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to create Session."));
		return FALSE;
	}

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to start a transaction.."));
		oSession.Close();
		return FALSE;
	}

	// InsertRecordInDatabase City
	if (!oCitiesTable.DeleteWhereID(oSession, lId, lUpdateCounter))
	{
		oSession.Abort();
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to delete the record with id: %ld"), lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	hResult = oSession.Commit();
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to commit the transaction. It is rollbacked instead."));
		oSession.Abort();
		oSession.Close();
		return FALSE;
	}

	oSession.Close();
	return TRUE;

}

BOOL CCitiesData::UpdateRecordInDatabaseById(City& rCity)
{
	CCitiesTable oCitiesTable;
	CSession oSession;
	CDataSource& oDataSource = CDBConnection::GetInstance().GetDataSource();
	HRESULT hResult;

	// Opens a session.
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to create Session."));
		return FALSE;
	}

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to start a transaction."));
		oSession.Close();
		return FALSE;
	}

	// InsertRecordInDatabase City
	if (!oCitiesTable.UpdateWhereID(oSession, rCity.lId, rCity))
	{
		oSession.Abort();
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to insert the record %s %s"), rCity.szCityName, rCity.szCityArea);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	hResult = oSession.Commit();
	if (FAILED(hResult))
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to commit the transaction. It is rollbacked instead."));
		oSession.Abort();
		oSession.Close();
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}
