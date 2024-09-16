#pragma once
#include "Phonenumber.h"
#include <atldbcli.h>

class CPhonenumbersAccessor
{
protected:
	// Members -----------------------------------

	Phonenumber m_recPhonenumber;

	// Accessors ---------------------------------

	BEGIN_ACCESSOR_MAP(CPhonenumbersAccessor, 2)

		// When the application writes in the database it uses accessor 0 and 1
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recPhonenumber.lId)
		END_ACCESSOR()

		// When the application only reads from the database it uses only accessor 1
		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recPhonenumber.lUpdateCounter)
		COLUMN_ENTRY(3, m_recPhonenumber.lPersonId)
		COLUMN_ENTRY(4, m_recPhonenumber.lPhoneTypeId)
		COLUMN_ENTRY(5, m_recPhonenumber.szPhonenumber)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};