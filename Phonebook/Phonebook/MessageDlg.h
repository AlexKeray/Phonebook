#pragma once
#include "afxdialogex.h"


// CMessageDlg dialog

class CMessageDlg : public CDialogEx
{
// Enums
public:
	enum class Mode { Information, Question, Warning, Error };

// Members
public:
	CStatic m_stIcon;
	CStatic m_stMessage;
	CMessageDlg::Mode m_enMode;
	CString m_strMessage;
	CString m_strTitle;

//Constructors and destructors
private:
	DECLARE_DYNAMIC(CMessageDlg)
public:
	CMessageDlg(CString strMessage, CMessageDlg::Mode enMode, CString strTitle = _T(""), CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMessageDlg();

// Dialog data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MESSAGE };
#endif

// Overrides
private:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()

// Message handdlers

// Methods
	BOOL LoadIcon(int nIconId);
	void CMessageDlg::AdjustDialogSize(CString& text);
	void SetDefaultDlgTitle(const CString& strTitle);
};
