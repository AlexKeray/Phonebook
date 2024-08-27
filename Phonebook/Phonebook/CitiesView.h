
// CitiesView.h : interface of the CCitiesView class
//

#pragma once

class CCitiesView : public CListView
{
protected: // create from serialization only
	CCitiesView() noexcept;
	DECLARE_DYNCREATE(CCitiesView)

// Attributes
public:
	CCitiesDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CCitiesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

// Members
private:
	//<summary> Holds a pointer to the corresponding document. </summary>
	CCitiesDoc* m_pCitiesDocument = nullptr;
	/// <summary> Holds how many columns does the list control have. </summary>
	int m_nColumnCount = 0;

// Methods
private:
	void CreateColumns(CListCtrl& oListCtrl, int nColumnCount);
	BOOL SetColumnsWidth(CListCtrl& oListCtrl, int nRightPadding = 16);
	BOOL PopulateListCtrl(CListCtrl& oListCtrl, std::map<CString, City>& oCitiesMap);
};

#ifndef _DEBUG  // debug version in CitiesView.cpp
inline CCitiesDoc* CCitiesView::GetDocument() const
   { return reinterpret_cast<CCitiesDoc*>(m_pDocument); }
#endif

