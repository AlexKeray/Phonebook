#pragma once

/// <summary>
/// A struct that mimics the structure of a database table record. It is used to store a record's data, taken from the OLE DB's rowset.
/// </summary>
struct City
{
	int lId{};
	int lUpdateCounter{};
	TCHAR szCityName[64]{}; // TCHAR is a typedef that resolves to char in ANSI builds and wchar_t in Unicode builds.
	TCHAR szCityArea[64]{};
};
