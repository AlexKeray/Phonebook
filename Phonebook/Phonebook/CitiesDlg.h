#pragma once
#include "afxdialogex.h"
#include "City.h"


// CCityDetailsDlg dialog

class CCitiesDlg : public CDialogEx
{
// Enums
public:
	enum class DialogMode { View, Update, Insert, Delete };

	enum class RecordUpdateStatus { UpToDate, Outdated};

// Members
public:
	City m_recCity;
	CEdit m_edbCityName;
	CEdit m_edbCityArea;
	CStatic m_stIcon;
	CStatic m_stMessage;
	CStatic m_stCityName;
	CStatic m_stCityArea;
private:
	DialogMode m_enDialogMode;
	RecordUpdateStatus m_enRecordUpdateStatus;
	CString m_strName;
	CString m_strArea;

// Constructors and destructors
private:
	DECLARE_DYNAMIC(CCitiesDlg)
public:
	CCitiesDlg(City& rCity, CCitiesDlg::RecordUpdateStatus enRecordUpdateStatus, CCitiesDlg::DialogMode oDialogMode, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCitiesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIG_CITY };
#endif

// Overrides
private:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message map
	DECLARE_MESSAGE_MAP()

// Mesage handlers
private:
	afx_msg void OnBnClickedOk();

// Methods
private:
	BOOL CCitiesDlg::LoadIcon(int nIconId);
	
};
