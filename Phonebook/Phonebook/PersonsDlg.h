#pragma once
#include "afxdialogex.h"
#include "Person.h"
#include "City.h"
#include "Phonenumber.h"
#include <list>


// CPersonsDlg dialog

class CPersonsDlg : public CDialogEx
{
// Enums
public:
	enum class DialogMode { View, Update, Insert, Delete };

	enum class RecordUpdateStatus { UpToDate, Outdated };

// Members
public:
	CStatic m_stIcon;
	CStatic m_stMessage;
	CStatic m_stFirstName;
	CStatic m_stMiddleName;
	CStatic m_stLastName;
	CStatic m_stUcn;
	CStatic m_stCityName;
	CStatic m_stArea;
	CStatic m_stAddress;
	CEdit m_edbFirstName;
	CEdit m_edbMiddleName;
	CEdit m_edbLastName;
	CEdit m_edbUcn;
	CComboBox m_cbCityName;
	CComboBox m_cbCityArea;
	CEdit m_edbAddress;
	CListCtrl m_lcPhonenumbers;

	Person m_recPerson;
	int m_nCityIndex;
	std::list<City> m_oCitiesList;
	std::list<Phonenumber> m_oPhonenumbersList;
	CPersonsDlg::RecordUpdateStatus m_enRecordUpdateStatus;
	CPersonsDlg::DialogMode m_enDialogMode;

// Constructors and destructors

	DECLARE_DYNAMIC(CPersonsDlg)
public:
	CPersonsDlg(Person& rPerson, int nCityIndex, std::list<City>& rCitiesList, std::list<Phonenumber>& rPhonenumbersList, CPersonsDlg::RecordUpdateStatus enRecordUpdateStatus, CPersonsDlg::DialogMode oDialogMode, CWnd* pParent = nullptr);
	virtual ~CPersonsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PERSON };
#endif

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


// Message map
	DECLARE_MESSAGE_MAP()

// Methods
private:
	BOOL LoadIcon(int nIconId);
	virtual void OnOK();
};
