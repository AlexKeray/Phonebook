// PersonsDlg.cpp : implementation file
//

#include "pch.h"
#include "Phonebook.h"
#include "afxdialogex.h"
#include "PersonsDlg.h"
#include "Log.h"


// CPersonsDlg dialog

IMPLEMENT_DYNAMIC(CPersonsDlg, CDialogEx)

CPersonsDlg::CPersonsDlg(Person& rPerson, int nCityIndex, std::list<City>& rCitiesList, std::list<Phonenumber>& rPhonenumbersList, CPersonsDlg::RecordUpdateStatus enRecordUpdateStatus, CPersonsDlg::DialogMode enDialogMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PERSON, pParent)
{
	m_recPerson = rPerson;
	m_nCityIndex = nCityIndex;
	m_oCitiesList = rCitiesList;
	m_oPhonenumbersList = rPhonenumbersList;
	m_enRecordUpdateStatus = enRecordUpdateStatus;
	m_enDialogMode = enDialogMode;
}

CPersonsDlg::~CPersonsDlg()
{
}

void CPersonsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_ICON, m_stIcon);
	DDX_Control(pDX, IDC_ST_FIRST_NAME, m_stFirstName);
	DDX_Control(pDX, IDC_ST_MIDDLE_NAME, m_stMiddleName);
	DDX_Control(pDX, IDC_ST_LAST_NAME, m_stLastName);
	DDX_Control(pDX, IDC_ST_UCN, m_stUcn);
	DDX_Control(pDX, IDC_ST_CITY_NAME, m_stCityName);
	DDX_Control(pDX, IDC_ST_CITY_AREA, m_stArea);
	DDX_Control(pDX, IDC_ST_ADDRESS, m_stAddress);
	DDX_Control(pDX, IDC_EDB_FIRST_NAME, m_edbFirstName);
	DDX_Control(pDX, IDC_EDB_MIDDLE_NAME, m_edbMiddleName);
	DDX_Control(pDX, IDC_EDB_LAST_NAME, m_edbLastName);
	DDX_Control(pDX, IDC_EDB_UCN, m_edbUcn);
	DDX_Control(pDX, IDC_CB_CITY_NAME, m_cbCityName);
	DDX_Control(pDX, IDC_CB_CITY_AREA, m_cbCityArea);
	DDX_Control(pDX, IDC_EDB_ADDRESS, m_edbAddress);
	DDX_Control(pDX, IDC_LC_PHONENUMBERS, m_lcPhonenumbers);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_stMessage);
}


BEGIN_MESSAGE_MAP(CPersonsDlg, CDialogEx)
//	ON_EN_CHANGE(IDC_EDB_CITY, &CPersonsDlg::OnEnChangeEdbCity)
END_MESSAGE_MAP()


// CPersonsDlg message handlers


BOOL CPersonsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	switch (m_enRecordUpdateStatus)
	{
	case CPersonsDlg::RecordUpdateStatus::UpToDate:

		LoadIcon(ID_ICON_SUCCESS);
		m_stMessage.SetWindowTextW(_T("(Up to date)"));
		break;

	case CPersonsDlg::RecordUpdateStatus::Outdated:

		LoadIcon(ID_ICON_WARNING);
		m_stMessage.SetWindowTextW(_T("(Outdated)"));
		break;

	}

	m_stFirstName.SetWindowTextW(_T("First name:"));
	m_stMiddleName.SetWindowTextW(_T("Middle name:"));
	m_stLastName.SetWindowTextW(_T("Last name:"));
	m_stUcn.SetWindowTextW(_T("UCN:"));
	m_stCityName.SetWindowTextW(_T("City:"));
	m_stArea.SetWindowTextW(_T("Area:"));
	m_stAddress.SetWindowTextW(_T("Address:"));

	int index = 0;
	CFont oFont;
	switch (m_enDialogMode)
	{
	case CPersonsDlg::DialogMode::View:

		SetWindowText(_T("View"));

		m_edbFirstName.SetWindowTextW(m_recPerson.szFirstName);
		m_edbMiddleName.SetWindowTextW(m_recPerson.szMiddleName);
		m_edbLastName.SetWindowTextW(m_recPerson.szLastName);
		m_edbUcn.SetWindowTextW(m_recPerson.szUCN);
		for (auto& oCity : m_oCitiesList)
		{
			m_cbCityName.AddString(oCity.szCityName);
			m_cbCityArea.AddString(oCity.szCityArea);
		}
		m_cbCityName.SetCurSel(m_nCityIndex);
		m_cbCityArea.SetCurSel(m_nCityIndex);
		m_edbAddress.SetWindowTextW(m_recPerson.szAddress);

		m_lcPhonenumbers.DeleteAllItems();

		// Set the columns (assuming you want to display name, age, and address)
		m_lcPhonenumbers.InsertColumn(0, _T("Phonenumber"), LVCFMT_LEFT, 4000);
		
		oFont.CreatePointFont(120, _T("MS Shell Dlg 2"));
		m_lcPhonenumbers.SetFont(&oFont);


		// Insert items
		index = 0;
		for (const auto& rPhonenumber : m_oPhonenumbersList) {
			// Insert the name
			m_lcPhonenumbers.InsertItem(index, rPhonenumber.szPhonenumber);

			++index;
		}

		m_lcPhonenumbers.SetColumnWidth(0, LVSCW_AUTOSIZE);

		m_edbFirstName.EnableWindow(FALSE);
		m_edbMiddleName.EnableWindow(FALSE);
		m_edbLastName.EnableWindow(FALSE);
		m_edbUcn.EnableWindow(FALSE);
		m_cbCityName.EnableWindow(FALSE);
		m_cbCityArea.EnableWindow(FALSE);
		m_edbAddress.EnableWindow(FALSE);


		break;

	case CPersonsDlg::DialogMode::Insert:

		SetWindowText(_T("Add"));

		m_edbFirstName.SetWindowTextW(_T(""));
		m_edbMiddleName.SetWindowTextW(_T(""));
		m_edbLastName.SetWindowTextW(_T(""));
		m_edbUcn.SetWindowTextW(_T(""));
		m_edbAddress.SetWindowTextW(_T(""));
		for (auto& oCity : m_oCitiesList)
		{
			m_cbCityName.AddString(oCity.szCityName);
			m_cbCityArea.AddString(oCity.szCityArea);
		}
		m_cbCityName.SetCurSel(-1);
		m_cbCityArea.SetCurSel(-1);
		m_edbFirstName.EnableWindow(TRUE);
		m_edbMiddleName.EnableWindow(TRUE);
		m_edbLastName.EnableWindow(TRUE);
		m_edbUcn.EnableWindow(TRUE);
		m_cbCityName.EnableWindow(TRUE);
		m_cbCityArea.EnableWindow(TRUE);
		m_edbAddress.EnableWindow(TRUE);

		break;

	case CPersonsDlg::DialogMode::Delete:

		SetWindowText(_T("Remove"));

		m_edbFirstName.SetWindowTextW(m_recPerson.szFirstName);
		m_edbMiddleName.SetWindowTextW(m_recPerson.szMiddleName);
		m_edbLastName.SetWindowTextW(m_recPerson.szLastName);
		m_edbUcn.SetWindowTextW(m_recPerson.szUCN);
		for (auto& oCity : m_oCitiesList)
		{
			m_cbCityName.AddString(oCity.szCityName);
			m_cbCityArea.AddString(oCity.szCityArea);
		}
		m_cbCityName.SetCurSel(m_nCityIndex);
		m_cbCityArea.SetCurSel(m_nCityIndex);
		m_edbAddress.SetWindowTextW(m_recPerson.szAddress);

		m_edbFirstName.EnableWindow(TRUE);
		m_edbMiddleName.EnableWindow(TRUE);
		m_edbLastName.EnableWindow(TRUE);
		m_edbUcn.EnableWindow(TRUE);
		m_cbCityName.EnableWindow(TRUE);
		m_cbCityArea.EnableWindow(TRUE);
		m_edbAddress.EnableWindow(TRUE);

		break;

	case CPersonsDlg::DialogMode::Update:

		SetWindowText(_T("Change"));

		m_edbFirstName.SetWindowTextW(m_recPerson.szFirstName);
		m_edbMiddleName.SetWindowTextW(m_recPerson.szMiddleName);
		m_edbLastName.SetWindowTextW(m_recPerson.szLastName);
		m_edbUcn.SetWindowTextW(m_recPerson.szUCN);
		for (auto& oCity : m_oCitiesList)
		{
			m_cbCityName.AddString(oCity.szCityName);
			m_cbCityArea.AddString(oCity.szCityArea);
		}
		m_cbCityName.SetCurSel(m_nCityIndex);
		m_cbCityArea.SetCurSel(m_nCityIndex);
		m_edbAddress.SetWindowTextW(m_recPerson.szAddress);

		m_edbFirstName.EnableWindow(TRUE);
		m_edbMiddleName.EnableWindow(TRUE);
		m_edbLastName.EnableWindow(TRUE);
		m_edbUcn.EnableWindow(TRUE);
		m_cbCityName.EnableWindow(TRUE);
		m_cbCityArea.EnableWindow(TRUE);
		m_edbAddress.EnableWindow(TRUE);

		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPersonsDlg::LoadIcon(int nIconId)
{
	HICON hIcon;
	CStatic* pIconControl;

	hIcon = AfxGetApp()->LoadIcon(nIconId);
	pIconControl = (CStatic*)GetDlgItem(IDC_ST_ICON);
	if (pIconControl == nullptr)
	{
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT);
		return false;
	}
	pIconControl->ModifyStyle(0, SS_ICON);
	pIconControl->SetIcon(hIcon);

	return true;
}

void CPersonsDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();

	UpdateData(TRUE);
}
