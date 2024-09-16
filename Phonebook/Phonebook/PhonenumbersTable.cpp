#include "pch.h"
#include "PhonenumbersTable.h"
#include "list"

//BOOL CPhonenumbersTable::SelectAllRecordsFromDatabase(CSession& oSession, CList<Phonenumber, Phonenumber>& oPhonenumbersList)
//{
//	HRESULT hResult;
//
//	CString strQuery = _T("SELECT * FROM PHONE_NUMBERS");
//
//	// Executes a query and opens a rowset.
//	hResult = Open(oSession, strQuery);
//	if (FAILED(hResult))
//	{
//		CString strMessage;
//		strMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
//		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
//		oSession.Close();
//		return FALSE;
//	}
//
//	// Iterating trough the rowset
//	while (MoveNext() == S_OK)
//	{
//		oPhonenumbersList.AddTail(m_recPhonenumber);
//	}
//
//	Close();
//	oSession.Close();
//
//	return TRUE;
//}

BOOL CPhonenumbersTable::SelectAllRecordsFromDatabaseByPersonId(CSession& oSession, long lPersonId, std::list<Phonenumber>& oPhonenumbersList)
{
	HRESULT hResult;

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE PERSON_ID = %ld"), lPersonId);

	// Executes a query and opens a rowset.
	hResult = Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to run SQL query - \"%s\"."), strQuery);
		CLog::Message(CLog::Mode::Error, LOG_CONTEXT, hResult, strMessage);
		oSession.Close();
		return FALSE;
	}

	while (MoveNext() == S_OK)
	{
		oPhonenumbersList.push_back(m_recPhonenumber);
	}
	
	Close();

	return TRUE;
}