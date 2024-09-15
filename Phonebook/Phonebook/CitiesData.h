#pragma once

#include "pch.h"
#include "City.h"
#include <map>

class CCitiesData
{
public:
	/// <summary>
	/// Gets all records from the database table as City objects and stores them into a map. The key is the city name + city area and the value is the whole record.
	/// </summary>
	BOOL SelectAllRecordsFromDatabase(std::map<CString, City>&oCitiesMap);
	/// <summary>
	/// Selects a record from the database table by id and stores it into City object.
	/// </summary>
	BOOL SelectRecordFromDatabaseById(const long lId, City& rCity, bool& bIsRecordFound);
	/// <summary>
	/// Inserts a record into the database table.
	/// </summary>
	BOOL InsertRecordInDatabase(City& rCity);
	/// <summary>
	/// Deletes a record from the database table by id.
	/// </summary>
	BOOL DeleteRecordFromDatabaseById(long lId, long lUpdateCounter);
	/// <summary>
	/// Updates a record in the database table by id.
	/// </summary>
	BOOL UpdateRecordInDatabaseById(City& rCity);
};