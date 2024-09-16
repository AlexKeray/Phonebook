#pragma once
#include "tchar.h"
#include <string>

struct Phonenumber
{
    long lId{};
    long lUpdateCounter{};
    long lPersonId{};
    long lPhoneTypeId{};
    TCHAR szPhonenumber[32]{};

    Phonenumber& operator=(const Phonenumber& other) {
        if (this == &other)
        {
            return *this; // Self-assignment check
        }

        // Copy data members
        lId = other.lId;
        lUpdateCounter = other.lUpdateCounter;
        lPersonId = other.lPersonId;
        lPhoneTypeId = other.lPhoneTypeId;
        _tcscpy_s(szPhonenumber, _countof(szPhonenumber), other.szPhonenumber);

        return *this;
    }
};