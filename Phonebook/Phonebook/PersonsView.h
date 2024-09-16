#pragma once
#include "PersonsDoc.h"


// CPersonsView view

class CPersonsView : public CListView
{
//Enums
public:
	enum ValidateRecordVersionResult
	{
		VALIDATED,
		VALIDATION_ERROR,
		VALIDATION_CANCELED_BY_USER,
		RECORD_NOT_FOUND
	};

private:
// Members
	int m_nColumnCount = 0;
	CPersonsDoc* m_pPersonsDocument = nullptr;

// Constructors and Destructors
	DECLARE_DYNCREATE(CPersonsView)
protected:
	CPersonsView();
	virtual ~CPersonsView();

// Message map
protected:
	DECLARE_MESSAGE_MAP()

// Message handlers
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnContextmenuView();
	afx_msg void OnContextmenuAdd();
	afx_msg void OnContextmenuChange();
	afx_msg void OnContextmenuRemove();

// Overrides
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

// Methods
public:
	CPersonsDoc* GetDocument() const;
private:
	bool CreateColumns(CListCtrl& oListCtrl, int nColumnCount);
	bool SetColumnsWidth(CListCtrl& oListCtrl, int nRightPadding = 16);
	BOOL PopulateListCtrl(CListCtrl& oListCtrl, const std::map<int, std::tuple<Person, City, std::list<Phonenumber>>>& oPersonsMap);
	BOOL GetCListCtrlSelectedItemIndex(int& nCListCtrlSelectedItemIndex);
	CPersonsView::ValidateRecordVersionResult CheckAndHandleRecordState(std::tuple<Person, City, std::list<Phonenumber>>& oPersonsMapValue, int nCListCtrlSelectedItemIndex);
	bool IsRecordUpToDate(const std::tuple<Person, City, std::list<Phonenumber>>& oPersonsOldMapValue, const std::tuple<Person, City, std::list<Phonenumber>>& oPersonsNewMapValue);
	bool CPersonsView::AskUserToUpdateSelectedRecord();
	void CPersonsView::HandleValidationCanceledByUser(Person& rPerson, City& rCity, std::list<Phonenumber>& oPhonenumbersList);

// Diagnostics
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

};


