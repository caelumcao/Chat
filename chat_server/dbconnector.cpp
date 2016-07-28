#include "dbconnector.h"
#include "util.h"

DBConnector::DBConnector()
{
    m_bConnected = false;
    m_result = NULL;
    //mysql_init(m_connection);
    m_connection = mysql_init(NULL);
}

int DBConnector::Connect()
{
    if (m_bConnected)
        return 0;
    //if (mysql_real_connect(m_connection, HOSTADRESS, USERNAME, PASSWORD, DBNAME, 0, NULL, 0) == NULL)

    if ((m_connection = mysql_real_connect(m_connection, "localhost", "caelum", "88888888", "chat_server", 3306, NULL, CLIENT_MULTI_RESULTS)) == NULL)
    {
        log_test(mysql_error(m_connection));
        return -1;
    }
    m_bConnected = true;
    return 0;
}

void DBConnector::CloseConnection()
{
    mysql_close(m_connection);
    m_bConnected = false;
}

int DBConnector::DoQuery(const string &sql, vector<vector<string> > &dataVec)
{
    if (!m_bConnected)
    {
        log_test("database: not connected");
        return -1;
    }
    try
    {
        if (mysql_real_query(m_connection, sql.c_str(), sql.size()) != 0)
        {
            log_test(mysql_error(m_connection));
            return -1;
        }
        if ((m_result = mysql_store_result(m_connection)) == NULL)
        {
            log_test(mysql_error(m_connection));
            return -1;
        }
    } catch (...) {
        ReConnect();
        return -1;
    }
    int fieldCount = mysql_num_fields(m_result);
    MYSQL_ROW row = NULL;
    while ((row = mysql_fetch_row(m_result)) != NULL)
    {
        vector<string> list;
        for (int i = 0; i < fieldCount; ++i)
        {
            if (row[i])
                list.push_back(row[i]);
            else
                list.push_back("");
        }
        dataVec.push_back(list);
    }
    FreePreResult();
    return 0;
}

int DBConnector::DoUpdate(const string &sql)
{
    if (!m_bConnected)
    {
        log_test("database: not connected");
        return -1;
    }
    try
    {
        if (mysql_real_query(m_connection, sql.c_str(), sql.size()) != 0)
        {
            log_test(mysql_error(m_connection));
            return -1;
        }
    } catch (...) {
        ReConnect();
        return -1;
    }
    return 0;
}

int DBConnector::StartTransaction()
{
    if (mysql_autocommit(m_connection, 0) != 0)
    {
        log_test(mysql_error(m_connection));
        return -1;
    }
    return 0;
}

int DBConnector::Commit()
{
    if (mysql_commit(m_connection) != 0)
    {
        Rollback();
        EndTransaction();
        log_test(mysql_error(m_connection));
        return -1;
    }
    EndTransaction();
    return 0;
}

void DBConnector::FreePreResult()
{
//    if (m_result != NULL)
//    {
//        mysql_free_result(m_result);
//        m_result = NULL;
//    }
    mysql_free_result(m_result);
    while (!mysql_next_result(m_connection))
    {
        m_result = mysql_store_result(m_connection);
        mysql_free_result(m_result);
    }
}

int DBConnector::ReConnect()
{
    CloseConnection();
    return Connect();

}

int DBConnector::Rollback()
{
    if (mysql_rollback(m_connection) != 0)
    {
        log_test(mysql_error(m_connection));
        return -1;
    }
    return 0;
}

int DBConnector::EndTransaction()
{
    if (mysql_autocommit(m_connection, 1) != 0)
    {
        log_test(mysql_error(m_connection));
        return -1;
    }
    return 0;
}









