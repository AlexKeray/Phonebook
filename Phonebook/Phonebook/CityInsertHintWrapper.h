// CityInsertHintWrapper.h
#pragma once
#include <afx.h> // For CObject
#include "City.h"

class CityInsertHintWrapper : public CObject
{
public:
    CityInsertHintWrapper(int nPosition, const City& rCity)
        : m_nPosition(nPosition), m_rCity(rCity) {}

    int GetPosition() const { return m_nPosition; }
    const City& GetRecord() const { return m_rCity; }

private:
    int m_nPosition;
    City m_rCity;
};