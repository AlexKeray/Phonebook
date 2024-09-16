#pragma once
#include "Person.h"
#include <atldbcli.h>

class CPersonsAccessor
{
protected:
	// Members -----------------------------------

	Person m_recPerson;

	// Accessors ---------------------------------

	BEGIN_ACCESSOR_MAP(CPersonsAccessor, 2)

		// When the application writes in the database it uses accessor 0 and 1
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recPerson.lId)
		END_ACCESSOR()

		// When the application only reads from the database it uses only accessor 1
		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recPerson.lUpdateCounter)
		COLUMN_ENTRY(3, m_recPerson.szFirstName)
		COLUMN_ENTRY(4, m_recPerson.szMiddleName)
		COLUMN_ENTRY(5, m_recPerson.szLastName)
		COLUMN_ENTRY(6, m_recPerson.szUCN)
		COLUMN_ENTRY(7, m_recPerson.lCityId)
		COLUMN_ENTRY(8, m_recPerson.szAddress)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};