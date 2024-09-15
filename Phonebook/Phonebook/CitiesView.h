
// CitiesView.h : interface of the CCitiesView class
//

#pragma once

class CCitiesView : public CListView
{
//Enums
public:

	/// <summary>
	/// Needed for the method CheckAndHandleRecordState.
	/// </summary>
	enum ValidateRecordVersionResult
	{
		VALIDATED,
		VALIDATION_ERROR,
		VALIDATION_CANCELED_BY_USER,
		RECORD_NOT_FOUND
	};

// Members
private:

	//<summary> Holds a pointer to the corresponding document. </summary>
	CCitiesDoc* m_pCitiesDocument = nullptr;

	/// <summary> Holds how many columns does the list control have. </summary>
	int m_nColumnCount = 0;

// Constructors and Destructors
protected: // create from serialization only
	CCitiesView() noexcept;
	DECLARE_DYNCREATE(CCitiesView)
public:
	virtual ~CCitiesView();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	/// <summary>
	/// Manages the update of the View after its document is updated. The method is run after the document calls UpdateAllViews.
	/// </summary>
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

protected:
	/// <summary>
	/// Configures the view. The method is called only one time, when the view is created.
	/// </summary>
	virtual void OnInitialUpdate(); // called first time after construct

// Message map
protected:
	DECLARE_MESSAGE_MAP()

// Message handlers
protected:
	/// <summary>
	/// Opens context menu at the position of the cursor.
	/// </summary>
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	/// <summary>
	/// Loads the context menu.
	/// </summary>
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
public:
	/// <summary>
	/// Opens the selected record in the dialog for viewing.
	/// </summary>
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	
	/// <summary>
	/// Updates the record if it's outdated and opens it in the dialog for viewing.
	/// </summary>
	afx_msg void OnContextOptionView();
	
	/// <summary>
	/// Updates the record if it's outdated and opens it in the dialog for editing.
	/// </summary>
	afx_msg void OnContextOptionChange();
	
	/// <summary>
	/// Opens a dialog for inserting and iserts the record.
	/// </summary>
	afx_msg void OnContextOptionAdd();
	
	/// <summary>
	// Updates the record if it's outdated and opens it in the dialog for deleting.
	/// </summary>
	afx_msg void OnContextOptionRemove();
	
	afx_msg void OnAfxIdpCommandFailure();

// Methods


private:

	/// <summary>
	/// Checks if the selected record is up to date. If not, prompts the user to update it and loads the new data in rCity if the user agrees and loads the old data if the user disagrees.
	/// </summary>
	/// <param name="rCity">: depending on the user, rCity hold the new data that will be updated in the view and shown in the dialog or the old data that just will be shown in the dialog.</param>
	/// <param name="nCListCtrlSelectedItemIndex"> is needed to update the record in the view.</param>
	/// <returns>VALIDATION_CANCELED_BY_USER, RECORD_NOT_FOUND, VALIDATION_ERROR or VALIDATED</returns>
	CCitiesView::ValidateRecordVersionResult CheckAndHandleRecordState(City& rCity, int nCListCtrlSelectedItemIndex);
	
	/// <summary>
	/// Informs the user that the record is not found in the database and prompts the user to delete the record from the view. If the user agrees, the method deletes the record from the view.
	/// </summary>
	bool HandleRecordNotFound(City& rCity, int nCListCtrlSelectedItemIndex);

	/// <summary>
	/// Informs the user that the record is not up to date and the data will be previewed in read only mode.
	/// </summary>
	void HandleValidationError(City& rCity);

	/// <summary>
	/// Informs the user that the record is not up to date and the data will be previewed in read only mode.
	/// </summary>
	void HandleValidationCanceledByUser(City& rCity);

	/// <summary>
	/// Gets the index of the last selected item in the list control.
	/// </summary>
	BOOL GetCListCtrlSelectedItemIndex(int& nSelectedItemIndex);

	/// <summary>
	/// Checks the update counter of the record in the view and the record in the database. If the record in the view is up to date, the method returns true.
	/// </summary>
	bool IsRecordUpToDate(const City& rOldCity, const City& rNewCity);

	/// <summary>
	/// Propts the user to refresh the selected record in the view.
	/// </summary>
	/// <returns> True for yes and false for no </returns>
	bool AskUserToUpdateSelectedRecord();

	/// <summary>
	/// Adds columns to the list control.
	/// </summary>
	/// <param name="nColumnCount"> Important for the method SetColumnsWidth</param>
	/// <returns></returns>
	bool CreateColumns(CListCtrl& oListCtrl, int nColumnCount);

	/// <summary>
	/// Sets the width of the columns in the list control determined by the longest record field or the length of the header.
	/// </summary>
	/// <param name="nRightPadding"> adds paddign right from the records. </param>
	/// <returns> TRUE/FALSE</returns>
	BOOL SetColumnsWidth(CListCtrl& oListCtrl, int nRightPadding = 16);

	/// <summary>
	/// Populates the list control with the data from the document.
	/// </summary>
	BOOL PopulateListCtrl(CListCtrl& oListCtrl, const std::map<CString, City>& oCitiesMap);

// CCitiesView diagnostics
// Debug
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	/// <summary>
	/// Gets pointer to the document.
	/// </summary>
	CCitiesDoc* GetDocument() const;

};

#ifndef _DEBUG  // debug version in CitiesView.cpp
inline CCitiesDoc* CCitiesView::GetDocument() const
   { return reinterpret_cast<CCitiesDoc*>(m_pDocument); }
#endif

