#pragma once
#include "City.h" // From here we get the struct that represents a record from the database table
#include <atldbcli.h> // From here get the Accessor macros definition

/// <summary>
/// Maps every field of a record to every field of a c++ custom structure that mimics the record structure.
/// </summary>
class CCitiiesAccessor
{
protected:
	// Members -----------------------------------

	City m_recCity;

	// Accessors ---------------------------------

	BEGIN_ACCESSOR_MAP(CCitiiesAccessor, 2)

		// When the application writes in the database it uses accessor 0 and 1
		BEGIN_ACCESSOR(0, TRUE)
			COLUMN_ENTRY(1, m_recCity.lId)
		END_ACCESSOR()

		// When the application only reads from the database it uses only accessor 1
		BEGIN_ACCESSOR(1, TRUE)
			COLUMN_ENTRY(2, m_recCity.lUpdateCounter)
			COLUMN_ENTRY(3, m_recCity.szCityName)
			COLUMN_ENTRY(4, m_recCity.szCityArea)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};