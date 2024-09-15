// CityUpdateHintWrapper.h
#pragma once
#include <afx.h> // For CObject
#include "City.h"

class CityUpdateHintWrapper : public CObject
{
public:
    CityUpdateHintWrapper(int nRemovePosition, int nInsertPosition, const City& rCity)
        : m_nRemovePosition(nRemovePosition), m_nInsertPosition(nInsertPosition), m_rCity(rCity) {}

    int GetRemovePosition() const { return m_nRemovePosition; }
    int GetInsertPosition() const { return m_nInsertPosition; }
    const City& GetRecord() const { return m_rCity; }

private:
    int m_nRemovePosition;
    int m_nInsertPosition;
    City m_rCity;
};