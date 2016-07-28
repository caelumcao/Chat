#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
using std::string;
using std::vector;

class User
{
public:
    User();
    User(const string userId, int connfd, pid_t pid);

    string userId() const;
    void setUserId(const string &userId);

    int connfd() const;
    void setConnfd(int connfd);

    pid_t pid() const;
    void setPid(const pid_t &pid);

private:
    string m_userId;
    int m_connfd;
    pid_t m_pid;
};

#endif // USER_H
