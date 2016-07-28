#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "user.h"
#include "dataprocess.h"
#include "message.h"
using std::string;

class ChatServer
{
public:
    ChatServer(int port);
    void run();     //server entry
    void readMessage(int sockfd);
    ssize_t	readn(int fd, void *vptr, size_t n);
    void messageProcess(int sockfd, const string &msg);

private:
    string curTime();
    vector<string> asVector(const vector<vector<string> > &vec);
    int sendMsgToUser(const Message &message, const string &time);
    int sendMsgToOnLineFriends(const string  &userId, const string &type, const string &text);
    static void sigAlarm(int sig);
    void tempMessageProcess();


private:
    unsigned short m_port;
    DataProcess m_dp;
    static bool m_bTempMessage;
};

#endif // CHATSERVER_H
