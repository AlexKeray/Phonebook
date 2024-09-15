
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

// Constructors and Destructors
protected:
	DECLARE_DYNCREATE(CCitiesDoc) // supports dynamic creation
	CCitiesDoc() noexcept;
	
public:
	virtual ~CCitiesDoc();

// Message map
protected:
	DECLARE_MESSAGE_MAP()

// Methods
public:
	/// <summary>
	/// Gets reference of the map of the document.
	/// </summary>
	/// <returns> const reference</returns>
	const std::map<CString, City>& GetDocumentMap();
	/// <summary>
	/// Selects a record from the database table by id and stores it into City object.
	/// </summary>
	BOOL SelectDatabaseRecordById(long lRecordId, City& rCity, bool& bIsRecordFound);
	/// <summary>
	/// Inserts a record into the database table and the document container.
	/// </summary>
	BOOL InsertRecordInDatabaseAndContainer(City& rCity);
	/// <summary>
	/// Inserts a record into the document container.
	/// </summary>
	BOOL InsertContainerItem(City& rCity);
	/// <summary>
	/// Deletes a record from the database table by id and the document container.
	/// </summary>
	BOOL DeleteRecordInDatabaseAndContainer(int nItemIndex, City& rCity);
	/// <summary>
	/// Deletes the selected record from the document container.
	/// </summary>
	bool DeleteContainerItemByIndex(int nDeleteItemIndex);
	/// <summary>
	/// Updates a record in the database table and the document container.
	/// </summary>
	BOOL UpdateRecordInDatabaseAndContainer(int nItemIndex, City& rCity);
	/// <summary>
	/// Updates a record in the document container.
	/// </summary>
	BOOL UpdateContainerItemByIndex(int& nDeleteItemIndex, City& rCity);

	/// <summary>
	/// Gets the data of the selected record from the document container.
	/// </summary>
	bool SelectContainerItemByIndex(int nSelectedItemIndex, City& rCity);


// Overrides
public:
	/// <summary>
	/// Initializes the document.
	/// </summary>
	virtual BOOL OnNewDocument();

	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Diagnostics
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
