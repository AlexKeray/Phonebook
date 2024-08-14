/// <summary>
/// It displays messages to the user in the GUI and logs error information in a log file.
/// </summary>
class CMessage
{
// Methods -----------------------------------------

private:
	/// <summary>
	/// Extracts only the file name from a file path.
	/// </summary>
	/// <param name="filePath"> Stores the file path. </param>
	/// <returns> Returns the file name. </returns>
	static const char* getFileNameFromPath(const char* filePath);
public:

	/// <summary>
	/// Just displays a message to the GUI.
	/// </summary>
	/// <param name="strMessage"> The message text. </param>
	static void Message(const CString& strMessage);

	/// <summary>
	/// Displays a message in the GUI and logs error info in a log file.
	/// </summary>
	/// <param name="strMessage"> The error text. </param>
	/// <param name="filePath"> The path of the file in which the error occured. </param>
	/// <param name="line"> The line in which the error occured. </param>
	static void ErrorMessage(const CString& strMessage, const char* filePath, int line);

	/// <summary>
	/// Displays a message in the GUI and logs error info in a log file.
	/// </summary>
	/// <param name="strMessage"> The error text. </param>
	/// <param name="filePath"> The path of the file in which the error occured. </param>
	/// <param name="line"> The line in which the error occured. </param>
	/// <param name="hResult"> The OLE DB error code. </param>
	static void ErrorMessage(const CString& strMessage, HRESULT hResult, const char* file, int line);
};

