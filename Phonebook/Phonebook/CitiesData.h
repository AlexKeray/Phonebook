#pragma once

#include "pch.h"
#include "City.h"
#include <map>

class CCitiesData
{
public:
	BOOL SelectAll(std::map<CString, City>&oCitiesMap);
};