#pragma once
#include "CitiesAccessor.h"
#include <vector>

class CCitiesTable : private CCommand<CAccessor<CCitiiesAccessor>>
{
public:
	// Methods -----------------------------------------

	/// <summary>
	/// Loads all records from the Cities database table into a collection.
	/// </summary>
	/// <param name="oCitiesArray"> Collection from the document class</param>
	/// <returns> Integer that tells if the method was successfull. </returns>
	BOOL loadCities(std::vector<City>&);
};