#pragma once

#include "pch.h"
#include "Person.h"
#include "City.h"
#include "Phonenumber.h"
#include "PersonsTable.h"
#include <map>
#include <tuple>
#include <list>

class CPersonsData
{
public:
	BOOL SelectAllRecordsFromDatabase(std::map<int, std::tuple<Person, City, std::list<Phonenumber>>>& oPersonsMap);

	BOOL CPersonsData::SelectRecordFromDatabaseById(const long lId, std::tuple<Person, City, std::list<Phonenumber>>& oPersonsMap, bool& bIsRecordFound);

};