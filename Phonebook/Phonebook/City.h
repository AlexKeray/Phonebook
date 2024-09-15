#pragma once

/// <summary>
/// A struct that mimics the structure of a database table record. It is used to store a record's data, taken from the OLE DB's rowset.
/// </summary>
struct City
{
	long lId{};
	long lUpdateCounter{};
	TCHAR szCityName[64]{}; // TCHAR is a typedef that resolves to char in ANSI builds and wchar_t in Unicode builds.
	TCHAR szCityArea[64]{};

    City& operator=(const City& other) {
        if (this == &other)
        {
            return *this; // Self-assignment check
        }

        // Copy data members
        lId = other.lId;
        lUpdateCounter = other.lUpdateCounter;
        _tcscpy_s(szCityName, _countof(szCityName), other.szCityName);
        _tcscpy_s(szCityArea, _countof(szCityArea), other.szCityArea);

        return *this;
    }
};
