#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>
#include <vector>
#include "mysql/mysql.h"

using std::string;
using std::vector;

static const char * HOSTADRESS = "127.0.0.1";
static const char * USERNAME = "caelum";
static const char * PASSWORD = "88888888";
static const char * DBNAME = "chat_server";
static const int DBPORT = 3306;

class DBConnector
{
public:
    DBConnector();
    int Connect();
    void CloseConnection();
    int DoQuery(const string &sql, vector<vector<string> > &dataVec);
    int DoUpdate(const string &sql);
    int StartTransaction();
    int Commit();


private:
    void FreePreResult();
    int ReConnect();
    int Rollback();
    int EndTransaction();

private:
    bool m_bConnected;
    MYSQL * m_connection;
    MYSQL_RES * m_result;
};

#endif // DBCONNECTOR_H
