// CCityDetailsDlg.cpp : implementation file
//

#include "pch.h"
#include "Phonebook.h"
#include "afxdialogex.h"
#include "CitiesDlg.h"
#include "City.h"


// CCityDetailsDlg dialog

IMPLEMENT_DYNAMIC(CCitiesDlg, CDialogEx)

CCitiesDlg::CCitiesDlg(City& rCity, CCitiesDlg::RecordUpdateStatus enRecordUpdateStatus, CCitiesDlg::DialogMode enDialogMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIG_CITIES_DETAILS, pParent)
{
	m_recCity = rCity;
	m_enDialogMode = enDialogMode;
	m_enRecordUpdateStatus = enRecordUpdateStatus;
}

CCitiesDlg::~CCitiesDlg()
{
}

void CCitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_edbCityName);
	DDX_Control(pDX, IDC_EDB_CITIES_AREA, m_edbCityArea);
	DDX_Text(pDX, IDC_EDB_CITIES_NAME, m_strName);
	DDX_Text(pDX, IDC_EDB_CITIES_AREA, m_strArea);

	DDX_Control(pDX, IDC_ST_ICON, m_stIcon);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_stMessage);
	DDX_Control(pDX, IDC_ST_CITY_NAME, m_stCityName);
	DDX_Control(pDX, IDC_ST_CITY_AREA, m_stCityArea);
	DDX_Control(pDX, IDC_ST_ICON, m_stIcon);
}


BEGIN_MESSAGE_MAP(CCitiesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCitiesDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCityDetailsDlg message handlers


BOOL CCitiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	switch (m_enRecordUpdateStatus)
	{
	case CCitiesDlg::RecordUpdateStatus::UpToDate:

		LoadIcon(ID_ICON_SUCCESS);
		m_stMessage.SetWindowTextW(_T("(Up to date)"));
		break;

	case CCitiesDlg::RecordUpdateStatus::Outdated:

		LoadIcon(ID_ICON_WARNING);
		m_stMessage.SetWindowTextW(_T("(Outdated)"));
		break;

	}

	switch (m_enDialogMode)
	{
	case CCitiesDlg::DialogMode::View:

		SetWindowText(_T("View"));

		m_strName = m_recCity.szCityName;
		m_strArea = m_recCity.szCityArea;

		m_edbCityName.EnableWindow(FALSE);
		m_edbCityArea.EnableWindow(FALSE);

		break;

	case CCitiesDlg::DialogMode::Insert:

		SetWindowText(_T("Add"));

		m_edbCityName.EnableWindow(TRUE);
		m_edbCityArea.EnableWindow(TRUE);

		m_strName = _T("");
		m_strArea = _T("");

		break;

	case CCitiesDlg::DialogMode::Delete:

		SetWindowText(_T("Remove"));

		m_strName = m_recCity.szCityName;
		m_strArea = m_recCity.szCityArea;

		m_edbCityName.EnableWindow(FALSE);
		m_edbCityArea.EnableWindow(FALSE);

		break;

	case CCitiesDlg::DialogMode::Update:

		SetWindowText(_T("Change"));

		m_strName = m_recCity.szCityName;
		m_strArea = m_recCity.szCityArea;

		m_edbCityName.EnableWindow(TRUE);
		m_edbCityArea.EnableWindow(TRUE);

		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCitiesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	UpdateData(TRUE);

	wcscpy_s(m_recCity.szCityName, m_strName.GetLength() + 1, m_strName);
	wcscpy_s(m_recCity.szCityArea, m_strArea.GetLength() + 1, m_strArea);
}

BOOL CCitiesDlg::LoadIcon(int nIconId)
{
	HICON hIcon;
	CStatic* pIconControl;

	hIcon = AfxGetApp()->LoadIcon(nIconId);
	pIconControl = (CStatic*)GetDlgItem(IDC_ST_ICON);
	if (pIconControl == nullptr)
	{
		AfxMessageBox(_T("Failed to get static control!"));
		return FALSE;  // Return FALSE to indicate failure
	}
	pIconControl->ModifyStyle(0, SS_ICON);
	pIconControl->SetIcon(hIcon);
}