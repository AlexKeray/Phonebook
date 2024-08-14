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
	CLog& logger = CLog::GetInstance();
	const char* szFileName = getFileNameFromPath(filePath);
	logger.LogMessage(strMessage, szFileName, line);
	delete[] szFileName;
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
	CLog& logger = CLog::GetInstance();
	const char* szFileName = getFileNameFromPath(filePath);
	logger.LogMessage(strMessage, hResult, szFileName, line);
	delete[] szFileName;
}

/// <summary>
/// Extracts only the file name from a file path.
/// </summary>
/// <param name="filePath"> Stores the file path. </param>
/// <returns> Returns the file name. </returns>
const char* CMessage::getFileNameFromPath(const char* szFilePath)
{
	int iFileNameLastIndex = 0;
	while (szFilePath[iFileNameLastIndex] != '\0')
	{
		iFileNameLastIndex++;
	}
	iFileNameLastIndex--;
	int iFileNameFirstIndex = iFileNameLastIndex;
	while (szFilePath[iFileNameFirstIndex] != '\\')
	{
		iFileNameFirstIndex--;
	}
	iFileNameFirstIndex++;
	
	int nFileNameLength = iFileNameLastIndex - iFileNameFirstIndex + 1;
	
	char* szFileName = new char[nFileNameLength + 1];

	for (int i = 0; i < nFileNameLength; i++)
	{
		szFileName[i] = szFilePath[iFileNameFirstIndex + i];
	}
	szFileName[nFileNameLength] = '\0';

	return szFileName;
}