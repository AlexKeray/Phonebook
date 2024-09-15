// CityDeleteHintWrapper.h
#pragma once
#include <afx.h> // For CObject
#include "City.h"

class CityDeleteHintWrapper : public CObject
{
public:
    CityDeleteHintWrapper(int nPosition)
        : m_nPosition(nPosition) {}

    int GetPosition() const { return m_nPosition; }

private:
    int m_nPosition;
};