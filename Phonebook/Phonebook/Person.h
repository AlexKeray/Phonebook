#pragma once
#include "tchar.h"
#include <string>

struct Person
{
    long lId{};
    long lUpdateCounter{};
    TCHAR szFirstName[64]{};
    TCHAR szMiddleName[64]{};
    TCHAR szLastName[64]{};
    TCHAR szUCN[32]{};
    long lCityId{};
    TCHAR szAddress[512]{};

    Person& operator=(const Person& other) {
        if (this == &other)
        {
            return *this; // Self-assignment check
        }

        // Copy data members
        lId = other.lId;
        lUpdateCounter = other.lUpdateCounter;
        _tcscpy_s(szFirstName, _countof(szFirstName), other.szFirstName);
        _tcscpy_s(szMiddleName, _countof(szMiddleName), other.szMiddleName);
        _tcscpy_s(szLastName, _countof(szLastName), other.szLastName);
        _tcscpy_s(szUCN, _countof(szUCN), other.szUCN);
        lCityId = other.lCityId;
        _tcscpy_s(szAddress, _countof(szAddress), other.szAddress);

        return *this;
    }
};