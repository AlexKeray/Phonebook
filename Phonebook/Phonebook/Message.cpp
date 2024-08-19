#include "pch.h"
#include "Message.h"
#include "Log.h"

/// <summary>
/// Just displays a message to the GUI.
/// </summary>
/// <param name="strMessage"> The message text. </param>
void CMessage::Message(const CString& strMessage)
{
	AfxMessageBox(strMessage, MB_OK | MB_ICONINFORMATION, 0);
}

/// <summary>
/// Displays a message in the GUI and logs error info in a log file.
/// </summary>
/// <param name="strMessage"> The error text. </param>
/// <param name="filePath"> The path of the file in which the error occured. </param>
/// <param name="line"> The line in which the error occured. </param>
void CMessage::ErrorMessage(const CString& strMessage, const char* filePath, int line)
{
	AfxMessageBox(strMessage, MB_OK | MB_ICONERROR, 0);
	CLog::LogMessage(strMessage, filePath, line);
}

/// <summary>
/// Displays a message in the GUI and logs error info in a log file.
/// </summary>
/// <param name="strMessage"> The error text. </param>
/// <param name="filePath"> The path of the file in which the error occured. </param>
/// <param name="line"> The line in which the error occured. </param>
/// <param name="hResult"> The OLE DB error code. </param>
void CMessage::ErrorMessage(const CString& strMessage, HRESULT hResult, const char* filePath, int line)
{
	AfxMessageBox(strMessage, MB_OK | MB_ICONERROR, 0);
	CLog::LogMessage(strMessage, hResult, filePath, line);
}