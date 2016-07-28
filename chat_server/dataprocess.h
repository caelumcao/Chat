#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include "dbconnector.h"

class DataProcess
{
public:
    DataProcess();
    ~DataProcess();
    void connect();
    int addUser(const string &id, const string &nickname, const string &password);
    int addFriend(const string &lid, const string &rid, const string &lremark = "");
    int updateUserStatus(const string &id, const string &status);
    int findFriends(vector<vector<string> > & resVec, const string &text);
    int addMessage(const string &from_id, const string &to_id, const string &msg, const string &time, const string &status);
    int updateMsgStatus(const string &id, const string &status);
    int checkNewMessage(vector<vector<string> > & resVec, const string &user_id);
    int addUserId(const string &id);
    int removeUserId(const string &id);
    int chooseUserId(string &id);
    int checkPassword(const string &id, const string &password, int &res);
    int getFriendIdList(vector<string> &resVec, const string &id);
    int getFriendList(vector<vector<string> > &resVec, const string &id);
    int getUserInfo(vector<string> &resVec, const string &id);
    int setRemark(const string &id, const string &remark);
    int setUserInfo(const string &id, const vector<string> &infoVec);
    int getUserNickname(string &nickname, const string &id);
    int getUserNicknameAndStatus(vector<string> &resVec, const string &id);
    int getOnlineUserbyPid(string &userId, int &connfd, pid_t pid);
    int getConnfdByUserId(int &connfd, const string &userId);
    int getPidByUserId(pid_t &pid, const string &userId);
    int getUserIdByPid(string &userId, pid_t pid);
    int addUserToDB(const string &userId, int connfd, pid_t pid);
    int removeUserFromDB(pid_t pid);
    int addTempMessage(const string &to_id, const string &msg);
    int getTempMessage(vector<vector<string> > &resVec, const string &to_id);
    int removeTempMessage(const string &msg_id);
private:
    DBConnector m_db;

};

#endif // DATAPROCESS_H
