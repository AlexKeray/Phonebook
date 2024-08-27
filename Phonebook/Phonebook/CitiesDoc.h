
// CitiesDoc.h : interface of the CCitiesDoc class
//


#pragma once
#include <vector>
#include "City.h"
#include <map>

class CCitiesDoc : public CDocument
{
// Members
private:
	std::map<CString, City> m_CitiesMap;

// Methods
public:
	std::map<CString, City>& GetDocumentMap();

protected: // create from serialization only
// Constructors
	CCitiesDoc() noexcept;
	DECLARE_DYNCREATE(CCitiesDoc) // supports dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CCitiesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
