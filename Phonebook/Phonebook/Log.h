#pragma once
#include <afxwin.h>  // For MFC core and standard components
#include <afxstr.h>  // For CString
#include <fstream>   // For file I/O
#include <windows.h> // For GetModuleFileName
#include <unordered_map> // For linking hResult error codes to their names

/// <summary>
/// Stores information about errors in a txt file. The class usess the Singleton design pattern. Lazy initialization is used, which means a instance will be created not when the application starts but when GetInstance() is clled for the first time.
/// </summary>
class CLog
{
    // Members ---------------------------------------------------------------
private:

    /// <summary>
    /// Stores the file path of the log file.
    /// </summary>
    CString m_strLogFilePath{};

    /// <summary>
    /// Stores the instance of the file stream that reads and writes information from and to a text file.
    /// </summary>
    std::ofstream m_oLogFile{};

    /// <summary>
    /// Unordered map that links hResult codes to human-readable messages. These messages are the hResult macros provided by Microsoft.
    /// </summary>
    std::unordered_map<HRESULT, std::string> oHResultMap;

private:
    // Constructors ---------------------------------------------------------------

    CLog();

    // Methods ---------------------------------------------------------------

    /// <summary>
    /// Creates and opens the log.txt file and provides the path to it.
    /// </summary>
    void InitLogFile();

    /// <summary>
    /// Initializes the hResult map. The keys are HResults codes in hexadecimal, the values are the macro names provided by Microsoft.
    /// </summary>
    void InitializeHResultMap();

    /// <summary>
    /// Provides the path to the application's executable (.exe) file.
    /// </summary>
    /// <param name="strPath"> Reference to variable which will hold the executable path.</param>
    bool GetExecutablePath(CString& strPath);

    /// <summary>
    /// Provides the root diretory of the project. Its extracted from the executable path.
    /// </summary>
    /// <param name="strPath"> Reference to variable which holds the executable path.</param>
    /// <param name="nExeFolderDepth"> A constant that specifies the number of folders between the project root and the executable.</param>
    bool GetProjectRootDir(CString& strPath, int nExeFolderDepth);

    /// <summary>
    /// Provides the path to the parent directory.
    /// </summary>
    /// <param name="strPath"> Holds tha path to the current directory.</param>
    bool GetParentDir(CString& strPath);

    /// <summary>
    /// Creates the directory which will hold the log file.
    /// </summary>
    /// <param name="strPath"> Holds the current path.</param>
    bool CreateLogDir(CString& strPath);

    /// <summary>
    /// Creates and opens the log file in mode truncate. Every time the application restarts, the log file will be erased before the new information is stored.
    /// </summary>
    /// <param name="strPath"> Holds the path to the log file.</param>
    bool OpenLogFile(const CString& strPath);

    /// <summary>
    /// Extracts only the file name from a file path.
    /// </summary>
    /// <param name="filePath"> Stores the file path. </param>
    /// <returns> Returns the file name. </returns>
    static const char* getFileNameFromPath(const char* szFilePath);

public:

    // Copy constructor ---------------------------------------------------------------
    CLog(const CLog&) = delete;

    // Assignment operator ---------------------------------------------------------------
    CLog& operator=(const CLog&) = delete;

    // Destructor ---------------------------------------------------------------

    /// <summary>
    /// Closes the file stream.
    /// </summary>
    ~CLog();

    // Methods ---------------------------------------------------------------

    /// <summary>
    /// Used by the singleton to create references to the only one object instead of creting a new object.
    /// </summary>
    /// <returns> Reference to the singleton's object.</returns>
    static CLog& GetInstance();


    /// <summary>
    /// Stores error information in the log file. Also stores information about the file and the line at whuch the error occured.
    /// </summary>
    /// <param name="strMessage"> The error message.</param>
    /// <param name="file"> The name of the file where the error occured.</param>
    /// <param name="line"> The line at which the error occured.</param>
    static void LogMessage(const CString&, const char* file, int line);

    /// <summary>
    /// /// Stores error information in the log file. Also stores information about the hResult error code, the file and the line at whuch the error occured.
    /// </summary>
    /// <param name="strMessage"> The error message.</param>
    /// <param name="hResult"> The error code.</param>
    /// <param name="file"> The name of the file where the error occured.</param>
    /// <param name="line"> The line at which the error occured.</param>
    static void LogMessage(const CString&, const HRESULT& hResult, const char* file, int line);

};

