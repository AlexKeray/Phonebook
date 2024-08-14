#pragma once

/// <summary>
/// A struct that mimics the structure of a database table record. It is used to store a record's data, taken from the OLE DB's rowset.
/// </summary>
struct City
{
	int lId{};
	int lUpdateCounter{};
	wchar_t szCityName[64]{};
	wchar_t szCityArea[64]{};
};
