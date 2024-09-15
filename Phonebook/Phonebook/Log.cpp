#include "pch.h"
#include "Log.h"
#include <iostream>
#include <string>
#include <atlbase.h>
#include <iomanip>

CLog::CLog()
{
    InitLogFile();
    InitializeHResultMap();
}

/// <summary>
/// Creates and opens the log.txt file and provides the path to it.
/// </summary>
void CLog::InitLogFile()
{
    CString strPath;

    GetProjectRootDir(strPath, 2);

    if (strPath.IsEmpty())
    {
        return;
    }

    if (CreateLogDir(strPath) == false)
    {
        strPath.Empty();
        return;
    }

    strPath += _T("\\log.txt");

    if (OpenLogFile(strPath) == false)
    {
        return;
    }

    m_strLogFilePath = strPath;

}

/// <summary>
/// Provides the path to the application's executable (.exe) file.
/// </summary>
/// <param name="strPath"> Reference to variable which will hold the executable path.</param>
bool CLog::GetExecutablePath(CString& strPath)
{
    TCHAR szPathBuffer[MAX_PATH]{};
    if (GetModuleFileName(NULL, szPathBuffer, MAX_PATH) == 0)
    {
        return false;
    }

    strPath = static_cast<CString>(szPathBuffer);
    return true;
}

/// <summary>
/// Provides the root diretory of the project. Its extracted from the executable path.
/// </summary>
/// <param name="strPath"> Reference to variable which holds the executable path.</param>
/// <param name="nExeFolderDepth"> A constant that specifies the number of folders between the project root and the executable.</param>
bool CLog::GetProjectRootDir(CString& strPath, int nExeFolderDepth)
{
    nExeFolderDepth++;

    if (GetExecutablePath(strPath) == false)
    {
        strPath.Empty();
        return false;
    }

    for (int i = 0; i < nExeFolderDepth; i++)
    {
        if (!GetParentDir(strPath))
        {
            strPath.Empty();
            return false;
        }
    }

    return true;
}

/// <summary>
/// Provides the path to the parent directory.
/// </summary>
/// <param name="strPath"> Holds tha path to the current directory.</param>
bool CLog::GetParentDir(CString& strPath)
{
    int pos = strPath.ReverseFind(_T('\\'));
    if (pos == -1)
    {
        return false;
    }
    strPath = strPath.Left(pos);
    return true;
}

/// <summary>
/// Creates the directory which will hold the log file.
/// </summary>
/// <param name="strPath"> Holds the current path.</param>
bool CLog::CreateLogDir(CString& strPath)
{
    strPath += _T("\\log");
    if (PathFileExists(strPath) == false)
    {
        if (CreateDirectory(strPath, NULL) == 0)
        {
            return false;
        }
    }
    return true;
}

/// <summary>
/// Creates and opens the log file in mode truncate. Every time the application restarts, the log file will be erased before the new information is stored.
/// </summary>
/// <param name="strPath"> Holds the path to the log file.</param>
bool CLog::OpenLogFile(const CString& strPath)
{
    m_oLogFile.open(strPath, std::ios::out | std::ios::trunc);
    if (!m_oLogFile.is_open())
    {
        return false;
    }
    return true;
}

/// <summary>
/// Used by the singleton to create references to the only one object instead of creting a new object.
/// </summary>
/// <returns> Reference to the singleton's object.</returns>
CLog& CLog::GetInstance() {
    static CLog instance = CLog();
    return instance;
}

/// <summary>
/// Closes the file stream.
/// </summary>
CLog::~CLog() {
    if (m_oLogFile.is_open()) 
    {
        m_oLogFile.close();
    }
}

/// <summary>
/// Initializes the hResult map. The keys are HResults codes in hexadecimal, the values are the macro names provided by Microsoft.
/// </summary>
void CLog::InitializeHResultMap()
{
    // Data Source Opening Errors
    oHResultMap[0x80040E21] = "DB_E_ERRORSOCCURRED";
    oHResultMap[0x80040E4D] = "DB_SEC_E_AUTH_FAILED";
    oHResultMap[0x80040E19] = "DB_E_NOTFOUND";

    // Session Opening Errors
    oHResultMap[0x80004005] = "E_FAIL";
    oHResultMap[0x8007000E] = "E_OUTOFMEMORY";
    oHResultMap[0x80070057] = "E_INVALIDARG";

    // Query Execution Errors
    oHResultMap[0x80040E14] = "DB_E_ERRORSINCOMMAND";
    oHResultMap[0x80040E0C] = "DB_E_NOCOMMAND";
    oHResultMap[0x80040E37] = "DB_E_NOTABLE";
}

/// <summary>
/// Stores error information in the log file. Also stores information about the file and the nLine at which the error occured.
/// </summary>
/// <param name="strMessage"> The error message.</param>
/// <param name="file"> The name of the file where the error occured.</param>
/// <param name="nLine"> The nLine at which the error occured.</param>
void CLog::Message(CLog::Mode enMode, const char* pszFunctionName, const char* pszFilePath, int nLine, const CString& strMessage)
{
    CLog& logger = CLog::GetInstance();
    char* pszMode = nullptr;
    switch (enMode)
    {
        case CLog::Mode::Error:
			pszMode = "Error";
			break;
        case CLog::Mode::Warning:
            pszMode = "Warning";
            break;
        case CLog::Mode::Info:
            pszMode = "Info";
            break;
    }

    const char* pszFileName = getFileNameFromPath(pszFilePath);
    logger.m_oLogFile << "Type: " << pszMode << "\t" << "Function: " << pszFunctionName << "\t" << "File: " << pszFileName << "\t" << "Line: " << nLine << "\t";
    
    if (!strMessage.IsEmpty())
    {
        CW2A utf8Message(strMessage, CP_UTF8);
        logger.m_oLogFile << "Message: " << static_cast<const char*>(utf8Message);
    }

    logger.m_oLogFile << "\n";

    logger.m_oLogFile.flush();
    delete[] pszFileName;
}

/// <summary>
/// /// Stores error information in the log file. Also stores information about the hResult error code, the file and the nLine at which the error occured.
/// </summary>
/// <param name="strMessage"> The error message.</param>
/// <param name="hResult"> The error code.</param>
/// <param name="file"> The name of the file where the error occured.</param>
/// <param name="nLine"> The nLine at which the error occured.</param>
void CLog::Message(CLog::Mode enMode, const char* pszFunctionName, const char* pszFilePath, int nLine, const HRESULT& hResult, const CString& strMessage)
{
    CLog& logger = CLog::GetInstance();
    const char* pszMode = nullptr;
    switch (enMode)
    {
    case CLog::Mode::Error:
        pszMode = "Error";
        break;
    case CLog::Mode::Warning:
        pszMode = "Warning";
        break;
    case CLog::Mode::Info:
        pszMode = "Info";
        break;
    }
    const char* pszFileName = getFileNameFromPath(pszFilePath);

    std::string pszHResultDescription = "";
    auto it = logger.oHResultMap.find(hResult);
    if (it != logger.oHResultMap.end())
    {
        pszHResultDescription = "(" + it->second + ")";
    }
    else
    {
        pszHResultDescription = "(Unknown error)";
    }

    if (!strMessage.IsEmpty())
    {
        CW2A utf8Message(strMessage, CP_UTF8);
        logger.m_oLogFile << std::left 
            << std::setw(6) << "Type: " << std::setw(8) << pszMode
            << std::setw(10) << "Function: " << std::setw(30) << pszFunctionName
            << std::setw(6) << "File: " << std::setw(16) << pszFileName
            << std::setw(6) << "Line: " << std::setw(6) << nLine
            << std::setw(12) << "Error code: " << std::setw(2)<< "0x" <<std::setw(10)<< std::hex << hResult << std::setw(20) << pszHResultDescription 
            << std::setw(9)<< "Message: " << std::setw(36)<< static_cast<const char*>(utf8Message);
    }
    else
    {
        logger.m_oLogFile << std::left << std::setw(10) << "Type: " << std::setw(10) << pszMode
            << std::setw(15) << "Function: " << std::setw(30) << pszFunctionName
            << std::setw(10) << "File: " << std::setw(20) << pszFileName
            << std::setw(10) << "Line: " << std::setw(5) << nLine
            << std::setw(15) << "Error code: " << std::setw(2) << "0x" << std::setw(10) << std::hex << hResult << std::setw(20) << pszHResultDescription;
    }

    logger.m_oLogFile << "\n";
    logger.m_oLogFile.flush();
    delete[] pszFileName;
}

/// <summary>
/// Extracts only the file name from a file path.
/// </summary>
/// <param name="pszFilePath"> Stores the file path. </param>
/// <returns> Returns the file name. </returns>
const char* CLog::getFileNameFromPath(const char* szFilePath)
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