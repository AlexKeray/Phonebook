#include "pch.h"
#include "Message.h"
#include "Log.h"


/// <summary>
/// Displays a message to the user.
/// </summary>
/// <param name="strMessage"> The message text. </param>
/// <param name="enMode"> Determines the appropriate icon to display </param>
/// <param name="strTitle"> Determines the title of the message dialog. If its empty, the default value is determined by the enMode. </param>
int CMessage::Message(const CString& strMessage, CMessageDlg::Mode enMode, CString strTitle)
{
	CMessageDlg oMessageDlg = CMessageDlg(strMessage, enMode, strTitle);

	return oMessageDlg.DoModal();
}