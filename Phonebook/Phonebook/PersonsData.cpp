#include "pch.h"
#include "PersonsData.h"
#include "DBConnection.h"
#include "Log.h"
#include "CitiesTable.h"
#include "PhonenumbersTable.h"

BOOL CPersonsData::SelectAllRecordsFromDatabase(std::map<int, std::tuple<Person, City, std::list<Phonenumber>>>& oPersonsMap)
{
	CPersonsTable oPersonsTable;
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

	std::list<Person> oPersonsList;

	// Selects all Persons
	if (!oPersonsTable.SelectAllRecordsFromDatabase(oSession, oPersonsList))
	{
		oSession.Close();
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to select all Persons."));
		return FALSE;
	}

	CCitiesTable oCitiesTable;
	bool bIsRecordFound;
	CPhonenumbersTable oPhonenumbersTable;

	// Assuming oPersonsList is a std::list<Person>
	for (auto it = oPersonsList.begin(); it != oPersonsList.end(); ++it) {
		Person oPerson = *it;

		City oCity = City();

		if (!oCitiesTable.SelectWhereID(oSession, oPerson.lCityId, oCity, bIsRecordFound)) {
			oSession.Close();
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to select the person's city."));
			return FALSE;
		}

		std::list<Phonenumber> oPhonenumbersList;
		if (!oPhonenumbersTable.SelectAllRecordsFromDatabaseByPersonId(oSession, oPerson.lId, oPhonenumbersList)) {
			oSession.Close();
			CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to select the person's phonenumbers."));
			return FALSE;
		}

		// Create a tuple with the actual values using std::make_tuple and std::move
		auto oPersonsMapValue = std::make_tuple(oPerson, oCity, std::move(oPhonenumbersList));

		// Emplace the tuple into the map using the person's ID as the key
		oPersonsMap.emplace(oPerson.lId, std::move(oPersonsMapValue));
	}

	oSession.Close();
	return TRUE;
}

BOOL CPersonsData::SelectRecordFromDatabaseById(const long lId, std::tuple<Person, City, std::list<Phonenumber>>& oPersonsMapValue, bool& bIsRecordFound)
{
	CPersonsTable oPersonsTable;
	CCitiesTable oCitiesTable;
	CPhonenumbersTable oPhonenumbersTable;
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

	if (!oPersonsTable.SelectWhereID(oSession, lId, std::get<0>(oPersonsMapValue), bIsRecordFound))
	{
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to select the record with id: %ld"), lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	if (!oCitiesTable.SelectWhereID(oSession, lId, std::get<1>(oPersonsMapValue), bIsRecordFound))
	{
		oSession.Close();
		CString strMessage;
		strMessage.Format(_T("Unable to select the record with id: %ld"), lId);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		return FALSE;
	}

	if (!oPhonenumbersTable.SelectAllRecordsFromDatabaseByPersonId(oSession, lId, std::get<2>(oPersonsMapValue)))
	{
		oSession.Close();
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, _T("Unable to select the person's phonenumbers."));
		return FALSE;
	}

	oSession.Close();
	return TRUE;
}
