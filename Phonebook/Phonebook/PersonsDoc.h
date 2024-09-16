#pragma once
#include <map>
#include <list>
#include <tuple>
#include "Person.h"
#include "City.h"
#include "Phonenumber.h"

// CPersonsDoc document

class CPersonsDoc : public CDocument
{
// Members
private:
	std::map<int, std::tuple<Person, City, std::list<Phonenumber>>> m_oPersonsMap;

// Constructors and Destructors
private:
	DECLARE_DYNCREATE(CPersonsDoc)
public:
	CPersonsDoc();
	virtual ~CPersonsDoc();

// Message map
protected:
	DECLARE_MESSAGE_MAP()

// Overrides
protected:
	virtual BOOL OnNewDocument();

// Methods
public:
	const std::map<int, std::tuple <Person, City, std::list<Phonenumber> >> &GetDocumentMap();
	bool SelectContainerItemByIndex(int nSelectedItemIndex, std::tuple < Person, City, std::list<Phonenumber>>& rMapValue);
	BOOL SelectDatabaseRecordById(long lRecordId, std::tuple < Person, City, std::list<Phonenumber>>& rMapValue, bool& bIsRecordFound);


#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};
