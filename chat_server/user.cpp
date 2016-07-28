#include "user.h"

User::User()
{
    m_userId = "---";
    m_connfd = -1;
    m_pid = -1;
}

User::User(const std::string userId, int connfd, pid_t pid)
    : m_userId(userId), m_connfd(connfd), m_pid(pid)
{
}

string User::userId() const
{
    return m_userId;
}

void User::setUserId(const string &userId)
{
    m_userId = userId;
}

int User::connfd() const
{
    return m_connfd;
}

void User::setConnfd(int connfd)
{
    m_connfd = connfd;
}

pid_t User::pid() const
{
    return m_pid;
}

void User::setPid(const pid_t &pid)
{
    m_pid = pid;
}



