#pragma once
#include "PersonsAccessor.h"
#include <map>
#include <set>
#include "atldbcli.h"
#include <list>


class CPersonsTable : private CCommand<CAccessor<CPersonsAccessor>>
{

public:

	BOOL SelectAllRecordsFromDatabase(CSession& oSession, std::list<Person>& oPersonsList);

	BOOL SelectWhereID(CSession& oSession, const long lID, Person& rPerson, bool& bIsRecordFound);

private:
	BOOL ConfigureRowsetForDBModification(CDBPropSet & oUpdateDBPropSet);
};