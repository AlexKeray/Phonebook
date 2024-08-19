#pragma once
#include "atldbcli.h" // From "atldbcli.h" we get the declaration of OLE DB classes, functions and macros.

class CDBConnection
{
    //Members ---------------------------------------------------------------------
private:
    CDataSource oDataSource;
    bool bIsDataSourceOpened;

private:
    // Constructor ---------------------------------------------------------------

    CDBConnection();
public:
    // Copy constructor ---------------------------------------------------------------
    CDBConnection(const CDBConnection&) = delete;

    // Assignment operator ---------------------------------------------------------------
    CDBConnection& operator=(const CDBConnection&) = delete;

    // Destructor ---------------------------------------------------------------

    /// <summary>
    /// Closes the databse connection.
    /// </summary>
    ~CDBConnection();

    // Methods ---------------------------------------------------------------

    /// <summary>
    /// Used by the singleton to create references to the only one object instead of creting a new object.
    /// </summary>
    /// <returns> Reference to the singleton's object.</returns>
    static CDBConnection& GetInstance();

    CDataSource& GetDataSource()
    {
        return oDataSource;
    }
};