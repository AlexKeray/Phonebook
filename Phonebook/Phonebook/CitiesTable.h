#pragma once
#include "CitiesAccessor.h"
#include <vector>
#include <map>

class CCitiesTable : private CCommand<CAccessor<CCitiiesAccessor>>
{
public:
	// Methods -----------------------------------------

	/// <summary>
	/// Loads all records from the Cities database table into a collection.
	/// </summary>
	/// <param name="oCitiesArray"> Collection from the document class</param>
	/// <returns> Integer that tells if the method was successfull. </returns>
	BOOL SelectAll(CSession& oSession, std::map<CString, City>& oCitiesMap);

	/// <summary>
	/// Selects one record by ID from the City table.
	/// </summary>
	/// <param name="lID">The id of the record we want to select.</param>
	/// <param name="oCity">Holds the selected record.</param>
	BOOL SelectWhereID(const long lID, City& oCity);

	/// <summary>
	/// Updates a record from the CIty table.
	/// </summary>
	/// <param name="lID">The ID of the record</param>
	/// <param name="recNewRecord">The new version of the record</param>
	BOOL UpdateWhereID(const long lID, const City& recNewRecord);

	/// <summary>
	/// Inserts a new record into the City table
	/// </summary>
	/// <param name="recNewRecord">The data of the new record.</param>
	BOOL Insert(City& recNewRecord);

	/// <summary>
	/// Deletes a record.
	/// </summary>
	/// <param name="lID">The ID of the record prepared for deletion.</param>
	/// <param name="recNewRecord">The latest version of the record.</param>
	BOOL DeleteWhereID(const long lID, const City& recNewRecord);

private:
	/// <summary>
	/// Configures a DBPropSet for modification.
	/// </summary>
	/// <param name="oUpdateDBPropSet">The DBPropSet object.</param>
	BOOL ConfigureRowsetForDBModification(CDBPropSet& oUpdateDBPropSet);

};