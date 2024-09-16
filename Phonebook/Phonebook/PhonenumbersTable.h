#pragma once
#include "PersonsAccessor.h"
#include <map>
#include <set>
#include "atldbcli.h"
#include "PhonenumbersAccessor.h"
#include <afx.h>
#include "Log.h"


class CPhonenumbersTable : private CCommand<CAccessor<CPhonenumbersAccessor>>
{

public:

	//BOOL SelectAllRecordsFromDatabase(CSession& oSession, CList<Phonenumber, Phonenumber>& oPhonenumbersList);

	BOOL SelectAllRecordsFromDatabaseByPersonId(CSession& oSession, long lPersonId, std::list<Phonenumber>& oPhonenumbersList);

	//BOOL SelectWhereID(CSession & oSession, const long lID, City & oCity, bool& bIsRecordFound);


	//BOOL UpdateWhereID(CSession & oSession, const long lID, City & recNewRecord);


	//BOOL InsertRecordInDatabase(CSession & rSession, City & recNewRecord);

	//BOOL DeleteWhereID(CSession & rSession, const long lID, long lUpdateCounter);

private:
	BOOL ConfigureRowsetForDBModification(CDBPropSet& oUpdateDBPropSet);
};