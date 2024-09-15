#include "MessageDlg.h"

/// <summary>
/// It displays messages to the user in the GUI and logs error information in a log file.
/// </summary>
class CMessage
{
// Methods -----------------------------------------

public:

	/// <summary>
	/// Displays a message to the user.
	/// </summary>
	/// <param name="strMessage"> is the message text. </param>
	/// <param name="enMode"> determines the appropriate icon to display </param>
	/// <param name="strTitle"> determines the title of the message dialog. If its empty, the default value is determined by the enMode. </param>
	static int Message(const CString& strMessage, CMessageDlg::Mode enMode, CString strTitle = _T(""));
};

