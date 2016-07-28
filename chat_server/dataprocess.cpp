#include "dataprocess.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "util.h"

DataProcess::DataProcess()
{
}

DataProcess::~DataProcess()
{
    m_db.CloseConnection();
}

void DataProcess::connect()
{
    m_db.Connect();
}

int DataProcess::addUser(const string &id, const string &nickname, const string &password)
{
    string sql = "insert into user(id, nickname, password) values('" + id + "', '" + nickname + "', '" + password + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::addFriend(const string &lid, const string &rid, const string &lremark)
{
    //only useful in mysql
    string sql = "insert into friends(id, friend_id, remark) values('" + lid + "', '" + rid + "', '" + lremark + "'), "
            + "('" + rid + "', '" + lid + "', '" + "" + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::updateUserStatus(const string &id, const string &status)
{
    string sql = "update user set status = '" + status + "' where id = '" + id + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::findFriends(vector<vector<std::string> > &resVec, const std::string &text)
{
    string sql = "select id, nickname from user where id = '" + text + "' or nickname = '" + text + "'";
    return m_db.DoQuery(sql, resVec);
}

int DataProcess::addMessage(const string &from_id, const string &to_id, const string &msg, const string &time, const string &status)
{
    string sql = "insert into message(from_user_id, to_user_id, msg, time, status) values('"
            + from_id + "', '" + to_id + "', '" + msg + "', '" + time + "', '" + status + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::updateMsgStatus(const std::string &id, const std::string &status)
{
    string sql = "update message set status = '" + status + "' where id = '" + id + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::checkNewMessage(vector<vector<std::string> > &resVec, const std::string &user_id)
{
    string sql = "select id, msg from message where to_user_id = '" + user_id + "' and status = '0'";
    return m_db.DoQuery(sql, resVec);
}

int DataProcess::addUserId(const std::string &id)
{
    string sql = "insert into avai_id(id) values('" + id + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::removeUserId(const std::string &id)
{
    string sql = "delete from avai_id where id = '" + id + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::chooseUserId(std::string &id)
{
    string sql_1 = "select count(id) from avai_id";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql_1.c_str(), vec) < 0)
        return -1;
    string countStr = vec.at(0).at(0);
    int count = atoi(countStr.c_str());
    int idNo = rand() % count;
    std::stringstream ss;
    ss << idNo;
    string sql_2 = "select id from avai_id limit " + ss.str() + ", 1";
    vec.clear();
    if (m_db.DoQuery(sql_2.c_str(), vec) < 0)
        return -1;
    id = vec.at(0).at(0);
    return removeUserId(id);
}

int DataProcess::checkPassword(const std::string &id, const std::string &password, int &res)
{
    string sql = "select * from user where id = '" + id + "' and password = '" + password + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    if (vec.size() > 0)
        res = 1;
    else
        res = 0;
    return 0;
}

int DataProcess::getFriendIdList(vector<std::string> &resVec, const std::string &id)
{
    string sql = "select friend_id from friends where id = '" + id + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    for (int i = 0; i < vec.size(); ++i)
        resVec.push_back(vec.at(i).at(0));
    return 0;
}

int DataProcess::getFriendList(vector<vector<std::string> > &resVec, const std::string &id)
{
    string sql_1 = "select friend_id, remark from friends where id = '" + id + "'";
    if (m_db.DoQuery(sql_1.c_str(), resVec) < 0)
        return -1;
    for (int i = 0; i < resVec.size(); ++i)
    {
        string friend_id = resVec.at(i).at(0);
        string sql_2 = "select nickname, status from user where id = '" + friend_id + "'";
        vector<vector<string> > vec;
        if (m_db.DoQuery(sql_2.c_str(), vec) < 0)
            return -1;
        resVec.at(i).push_back(vec.at(0).at(0));
        resVec.at(i).push_back(vec.at(0).at(1));
    }
    return 0;
}

int DataProcess::getUserInfo(vector<std::string> &resVec, const std::string &id)
{
    string sql = "select id, nickname, sex, birthday from user where id = '" + id + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    resVec = vec.at(0);
    return 0;
}

int DataProcess::setRemark(const std::string &id, const std::string &remark)
{
    string sql = "update friends set remark = '" + remark + "' where id = '" + id + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::setUserInfo(const std::string &id, const vector<std::string> &infoVec)
{
    string sql = "update user set nickname = '" + infoVec.at(0) + "', sex = '" + infoVec.at(1)
            + "', birthday = '" + infoVec.at(2) + "' where id = '" + id + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::getUserNickname(std::string &nickname, const std::string &id)
{
    string sql = "select nickname from user where id = '" + id + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    nickname = vec.at(0).at(0);
    return 0;
}

int DataProcess::getUserNicknameAndStatus(vector<std::string> &resVec, const std::string &id)
{
    string sql = "select id, nickname, status from user where id = '" + id + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    resVec = vec.at(0);
    return 0;
}

int DataProcess::getOnlineUserbyPid(std::string &userId, int &connfd, pid_t pid)
{
    string sql = "select user_id, connfd from online_user where pid = '" + itos(pid) + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    userId = vec.at(0).at(0);
    connfd = atoi(vec.at(0).at(1).c_str());
    return 0;
}

int DataProcess::getConnfdByUserId(int &connfd, const std::string &userId)
{
    string sql = "select connfd from online_user where user_id = '" + userId + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    connfd = atoi(vec.at(0).at(0).c_str());
    return 0;
}

int DataProcess::getPidByUserId(pid_t &pid, const std::string &userId)
{
    string sql = "select pid from online_user where user_id = '" + userId + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    if (vec.size() > 0)
        pid = atoi(vec.at(0).at(0).c_str());
    else
        pid = -1;
    return 0;
}

int DataProcess::getUserIdByPid(std::string &userId, pid_t pid)
{
    string sql = "select user_id from online_user where pid = '" + itos(pid) + "'";
    vector<vector<string> > vec;
    if (m_db.DoQuery(sql, vec) < 0)
        return -1;
    userId = vec.at(0).at(0);
    return 0;
}

int DataProcess::addUserToDB(const std::string &userId, int connfd, pid_t pid)
{
    string sql = "insert into online_user(user_id, connfd, pid) values('" + userId + "', '" + itos(connfd) + "', '" + itos(pid) + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::removeUserFromDB(pid_t pid)
{
    string sql = "delete from online_user where pid = '" + itos(pid) + "'";
    return m_db.DoUpdate(sql);
}

int DataProcess::addTempMessage(const std::string &to_id, const std::string &msg)
{
    string sql = "insert into temp_message(to_user_id, msg) values('" + to_id + "', '" + msg + "')";
    return m_db.DoUpdate(sql);
}

int DataProcess::getTempMessage(vector<vector<std::string> > &resVec, const std::string &to_id)
{
    string sql = "select id, msg from temp_message where to_user_id = '" + to_id + "'";
    return m_db.DoQuery(sql, resVec);
}

int DataProcess::removeTempMessage(const std::string &msg_id)
{
    string sql = "delete from temp_message where id = '" + msg_id + "'";
    return m_db.DoUpdate(sql);
}





