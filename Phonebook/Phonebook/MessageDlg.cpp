// MessageDlg.cpp : implementation file
//

#include "pch.h"
#include "Phonebook.h"
#include "afxdialogex.h"
#include "MessageDlg.h"
#include "Resource.h"


// CMessageDlg dialog

IMPLEMENT_DYNAMIC(CMessageDlg, CDialogEx)

CMessageDlg::CMessageDlg(CString strMessage, CMessageDlg::Mode enMode, CString strTitle, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESSAGE, pParent)
{
	m_strMessage = strMessage;
    m_enMode = enMode;
    m_strTitle = strTitle;
}

CMessageDlg::~CMessageDlg()
{
}

BOOL CMessageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_stMessage.SetWindowTextW(m_strMessage);

	AdjustDialogSize(m_strMessage);

    

	switch (m_enMode)
	{
	case CMessageDlg::Mode::Information:

        SetDefaultDlgTitle(_T("Notification"));
		LoadIcon(ID_ICON_INFO);
		break;

	case CMessageDlg::Mode::Question:

        SetDefaultDlgTitle(_T("Choose action"));
		LoadIcon(ID_ICON_QUESTION);
		break;

	case CMessageDlg::Mode::Warning:

        SetDefaultDlgTitle(_T("Warning"));
		LoadIcon(ID_ICON_WARNING);
		break;

	case CMessageDlg::Mode::Error:

        SetDefaultDlgTitle(_T("Error"));
		LoadIcon(ID_ICON_ERROR);
		break;
	}
	return TRUE;
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_ICON, m_stIcon);
	DDX_Control(pDX, IDC_ST_MESSAGE, m_stMessage);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialogEx)
END_MESSAGE_MAP()


BOOL CMessageDlg::LoadIcon(int nIconId)
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
	return TRUE;
}

void CMessageDlg::AdjustDialogSize(CString& text)
{
     // Get the static text control
    CWnd* pStaticText = GetDlgItem(IDC_ST_MESSAGE);
    if (pStaticText)
    {
        // Get the device context (DC) of the static text control
        CDC* pDC = pStaticText->GetDC();
        // Get the font of the static text control
        CFont* pFont = pStaticText->GetFont();
        CFont* pOldFont = pDC->SelectObject(pFont);

        // Get the original size of the static text control
        CRect originalRect;
        pStaticText->GetWindowRect(&originalRect);
        ScreenToClient(&originalRect);  // Convert to client coordinates

        // Store the original width and height of the static text control
        int originalStaticTextWidth = originalRect.Width();
        int originalStaticTextHeight = originalRect.Height();

        // Measure text without wrapping
        CSize unwrappedTextSize = pDC->GetTextExtent(text);

        // Check if the text fits within the static control's width
        if (unwrappedTextSize.cx <= originalStaticTextWidth)
        {
            // The text fits within the static control's width
            // Resize the static control and dialog to be more narrow
            int newWidth = unwrappedTextSize.cx;
            pStaticText->MoveWindow(originalRect.left, originalRect.top, newWidth, originalStaticTextHeight);

            // Resize the dialog to accommodate the narrower control
            CRect dlgRect;
            GetWindowRect(&dlgRect);
            int widthDiff = originalStaticTextWidth - newWidth;
            dlgRect.right -= widthDiff;
            MoveWindow(dlgRect);

            // Move the OK and Cancel buttons left by the same width difference
            CWnd* pOKButton = GetDlgItem(IDOK);
            CWnd* pCancelButton = GetDlgItem(IDCANCEL);
            if (pOKButton && pCancelButton)
            {
                CRect okRect;
                pOKButton->GetWindowRect(&okRect);
                ScreenToClient(&okRect);  // Convert to client coordinates
                pOKButton->MoveWindow(okRect.left - widthDiff/2, okRect.top, okRect.Width(), okRect.Height());

                CRect cancelRect;
                pCancelButton->GetWindowRect(&cancelRect);
                ScreenToClient(&cancelRect);  // Convert to client coordinates
                pCancelButton->MoveWindow(cancelRect.left - widthDiff/2, cancelRect.top, cancelRect.Width(), cancelRect.Height());
            }
        }
        else
        {
            // The text width exceeds the static control's width, need to wrap text
            CRect textRect = originalRect;
            textRect.right = textRect.left + originalStaticTextWidth; // Set the max width for wrapping

            // Measure the wrapped text
            pDC->DrawText(text, &textRect, DT_CALCRECT | DT_WORDBREAK);

            // Calculate the height of one line of text (line buffer)
            TEXTMETRIC tm;
            pDC->GetTextMetrics(&tm);
            int lineHeight = tm.tmHeight;  // Height of one line
            int wrappedTextHeight = textRect.Height() + lineHeight; // Add buffer space for one more line

            // Restore the old font
            pDC->SelectObject(pOldFont);
            pStaticText->ReleaseDC(pDC);

            // Resize the static control based on the new calculated height and original width
            pStaticText->MoveWindow(originalRect.left, originalRect.top, originalStaticTextWidth, wrappedTextHeight);

            // Resize the dialog to accommodate the new static text control size
            CRect dlgRect;
            GetWindowRect(&dlgRect);
            int heightDiff = wrappedTextHeight - originalStaticTextHeight - lineHeight;
            dlgRect.bottom += heightDiff;
            MoveWindow(dlgRect);

            // Move the OK and Cancel buttons down by the same height difference
            CWnd* pOKButton = GetDlgItem(IDOK);
            CWnd* pCancelButton = GetDlgItem(IDCANCEL);
            if (pOKButton && pCancelButton)
            {
                CRect okRect;
                pOKButton->GetWindowRect(&okRect);
                ScreenToClient(&okRect);
                pOKButton->MoveWindow(okRect.left, okRect.top + heightDiff, okRect.Width(), okRect.Height());

                CRect cancelRect;
                pCancelButton->GetWindowRect(&cancelRect);
                ScreenToClient(&cancelRect);
                pCancelButton->MoveWindow(cancelRect.left, cancelRect.top + heightDiff, cancelRect.Width(), cancelRect.Height());
            }
        }
    }
}

void CMessageDlg::SetDefaultDlgTitle(const CString& strTitle)
{
    if (m_strTitle == _T(""))
    {
        SetWindowText(strTitle);
        return;
    }
    SetWindowText(m_strTitle);
}
