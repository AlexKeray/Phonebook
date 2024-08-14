#include "pch.h"
#include "Log.h"
#include <iostream>
#include <string>
#include <atlbase.h>

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
/// Stores error information in the log file. Also stores information about the file and the line at whuch the error occured.
/// </summary>
/// <param name="strMessage"> The error message.</param>
/// <param name="file"> The name of the file where the error occured.</param>
/// <param name="line"> The line at which the error occured.</param>
void CLog::LogMessage(const CString& strMessage, const char* file, int line)
{
    CW2A utf8Message(strMessage, CP_UTF8);
    m_oLogFile << static_cast<const char*>(utf8Message) << "\n";
    m_oLogFile << "File: " << file << "\t\tLine: " << line << "\n\n";
    m_oLogFile.flush();
}

/// <summary>
/// /// Stores error information in the log file. Also stores information about the hResult error code, the file and the line at whuch the error occured.
/// </summary>
/// <param name="strMessage"> The error message.</param>
/// <param name="hResult"> The error code.</param>
/// <param name="file"> The name of the file where the error occured.</param>
/// <param name="line"> The line at which the error occured.</param>
void CLog::LogMessage(const CString& strMessage, const HRESULT& hResult, const char* filePath, int line)
{
    CW2A utf8Message(strMessage, CP_UTF8);
    m_oLogFile << static_cast<const char*>(utf8Message) << "\n";
    auto it = oHResultMap.find(hResult);
    if (it != oHResultMap.end())
    {
        m_oLogFile << "Error code: 0x" << std::hex << hResult << " (" << it->second << ")\n";
    }
    else
    {
        m_oLogFile << "Error code: 0x" << std::hex << hResult << " (Unknown error)\n";
    }
    m_oLogFile << "File: " << filePath << "\t\tLine: " << std::dec << line << "\n\n";
    m_oLogFile.flush();
}
