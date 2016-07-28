#include "chatserver.h"
#include "util.h"
#include <iostream>
#include <time.h>
#include <pthread.h>

bool ChatServer::m_bTempMessage = false;

ChatServer::ChatServer(int port) : m_port(port)
{

}

void ChatServer::run()
{
    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_chld);

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd;
    pid_t childpid;
    while (true)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
                log_err("accept error");
        }

        if ((childpid = Fork()) == 0)
        {
            std::cout << "fork ok!\n";
            Close(listenfd);
            m_dp.connect();
            Signal(SIGALRM, sigAlarm);
            readMessage(connfd);
            exit(0);
        }
    }
}

void ChatServer::readMessage(int sockfd)
{
    char lenbuf[INTSIZE + 1];
    lenbuf[INTSIZE] = '\0';
    int len;

    while (true)
    {
        string msg = "";
        readn(sockfd, lenbuf, INTSIZE);
        msg.append(lenbuf, INTSIZE);
        numReadFromStr(len, msg);
        char buf[MAXLINE];
        readn(sockfd, buf, len);
        msg.append(buf, len);
        messageProcess(sockfd, msg);
    }
}

ssize_t	ChatServer::readn(int fd, void *vptr, size_t n) /* Read "n" bytes from a descriptor. */
{
    size_t	nleft;
    ssize_t	nread;
    char	*ptr;

    ptr = (char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
            {
                nread = 0;		/* and call read() again */
                if (m_bTempMessage)
                    tempMessageProcess();
            } else {
                log_err("read message: read error");
            }
        } else if (nread == 0) {
            //off-line
            string userId;
            m_dp.getUserIdByPid(userId, getpid());
            sendMsgToOnLineFriends(userId, "05", "0");
            m_dp.updateUserStatus(userId, "0");
            m_dp.removeUserFromDB(getpid());
            std::cout << "client closed\n";
            exit(1);
        }

        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);		/* return >= 0 */
}

void ChatServer::messageProcess(int sockfd, const string &msg)
{
    Message message(msg);
    message.print();
    string from = message.from();
    string to = message.to();
    string type = message.type();
    string text = message.text();
    vector<string> textVec = message.subTextVec();
    if (type == "00")
    {

    } else if (type == "01") {      /* register */
        string newid, sText = "";
        if (m_dp.chooseUserId(newid) < 0)
            sText = "0";
        else
            sText = "1" + newid;
        Message sMessage("000", from, "02", sText);
        Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
        m_dp.addUser(newid, textVec.at(0), textVec.at(1));
    } else if (type == "03") {      /* login */
        int res = 0;
        m_dp.checkPassword(from, text, res);
        if (res == 1)
        {
            m_dp.addUserToDB(from, sockfd, getpid());
            m_dp.updateUserStatus(from, "1");

            vector<string> userInfoVec;
            if (m_dp.getUserInfo(userInfoVec, from) < 0)
                std::cout << "getUserInfo error\n";
            Message userInfoMsg("000", from, "15", userInfoVec);
            Writen(sockfd, userInfoMsg.msg().c_str(), userInfoMsg.msglen());

            vector<vector<string> > friendListVec;
            if (m_dp.getFriendList(friendListVec, from) < 0)
                std::cout << "getFriendList error\n";
            Message friendListMsg("000", from, "11", asVector(friendListVec));
            Writen(sockfd, friendListMsg.msg().c_str(), friendListMsg.msglen());

            vector<vector<string> > newMessageVec;
            if (m_dp.checkNewMessage(newMessageVec, from) < 0)
                std::cout << "checkNewMessage error\n";
            for (int i = 0; i < newMessageVec.size(); ++i)
            {
                Writen(sockfd, newMessageVec.at(i).at(1).c_str(), newMessageVec.at(i).at(1).size());
                m_dp.updateMsgStatus(newMessageVec.at(i).at(0), "1");
            }

            Message sMessage("000", from, "04", "1");
            Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());

            sendMsgToOnLineFriends(from, "05", "1");

        } else {
            Message sMessage("000", from, "04", "0");
            Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
        }

    } else if (type == "06") {      /* find friends */
        vector<vector<string> > resVec;
        m_dp.findFriends(resVec, text);
        Message sMessage("000", from, "07", asVector(resVec));
        Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
    } else if (type == "08") {      /* friend application */
        sendMsgToUser(message, curTime());
    } else if (type == "09") {      /* friend application result */
        if (textVec.at(0) == "1")    /* agree */
        {
            m_dp.addFriend(from, to);
            sendMsgToUser(message, curTime());
        }
    } else if (type == "10") {      /* set remark */
        m_dp.setRemark(from, text);
    } else if (type == "12") {      /* set user's information */
        string nickname = "";
        m_dp.getUserNickname(nickname, from);
        m_dp.setUserInfo(from, textVec);
        if (nickname != textVec.at(0))  /* nickname changed */
            sendMsgToOnLineFriends(from, "13", textVec.at(0));
    } else if (type == "14") {      /* request user's information */
        vector<string> userInfoVec;
        m_dp.getUserInfo(userInfoVec, text);
        Message sMessage("000", from, "15", userInfoVec);
        Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
    } else if (type == "16") {      /* send chat message */
        string time = curTime();
        string sendTime = time.substr(0, 4) + time.substr(5, 2) + time.substr(8, 2)
                + time.substr(11, 2) + time.substr(14, 2) + time.substr(17, 2);
        Message sMessage(to, from, "17", sendTime);
        Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
        message.setSendTime(sendTime);
        sendMsgToUser(message, time);
    } else if (type == "18") {      /* request friend's status */
        vector<string> vec;
        m_dp.getUserNicknameAndStatus(vec, text);
        Message sMessage("000", from, "19", vec);
        Writen(sockfd, sMessage.msg().c_str(), sMessage.msglen());
    }

    if (m_bTempMessage)
        tempMessageProcess();
}

std::string ChatServer::curTime()
{
    time_t now;
    time(&now);
    struct tm *w = localtime(&now);
    char ch[20];
    sprintf(ch, "%04d-%02d-%02d %02d:%02d:%02d", w->tm_year + 1900, w->tm_mon + 1, w->tm_mday, w->tm_hour, w->tm_min, w->tm_sec);
    string timeStr = ch;
    return timeStr;
}

vector<std::string> ChatServer::asVector(const vector<vector<std::string> > &vec)
{
    vector<string> resVec;
    for (int i = 0; i < vec.size(); ++i)
    {
        vector<string> tempVec = vec.at(i);
        for (int j = 0; j < tempVec.size(); ++j)
            resVec.push_back(tempVec.at(j));
    }
    return resVec;
}

int ChatServer::sendMsgToUser(const Message &message, const std::string &time)
{
    string toId = message.to();
    pid_t toPid;
    m_dp.getPidByUserId(toPid, toId);
    if (toPid > 0)
    {
        m_dp.addMessage(message.from(), toId, message.msg(), time, "1");
        m_dp.addTempMessage(toId, message.msg());
        kill(toPid, SIGALRM);
    } else {
        m_dp.addMessage(message.from(), toId, message.msg(), time, "0");
    }
    return 0;
}

int ChatServer::sendMsgToOnLineFriends(const std::string &userId, const std::string &type, const std::string &text)
{
    vector<string> toIdVec;
    m_dp.getFriendIdList(toIdVec, userId);
    for (int i = 0; i < toIdVec.size(); ++i)
    {
        string toId = toIdVec.at(i);
        pid_t pid;
        m_dp.getPidByUserId(pid, toId);
        if (pid > 0)
        {
            Message sMessage(userId, toId, type, text);
            m_dp.addTempMessage(toId, sMessage.msg());
            kill(pid, SIGALRM);
        }
    }
    return 0;
}

void ChatServer::sigAlarm(int sig)
{
    m_bTempMessage = true;
}

void ChatServer::tempMessageProcess()
{
    string userId;
    m_dp.getUserIdByPid(userId, getpid());
    int connfd;
    m_dp.getConnfdByUserId(connfd, userId);
    while (m_bTempMessage)
    {
        m_bTempMessage = false;
        vector<vector<string> > vec;
        m_dp.getTempMessage(vec, userId);
        for (int i = 0; i < vec.size(); ++i)
        {
            Writen(connfd, vec.at(i).at(1).c_str(), vec.at(i).at(1).size());
            m_dp.removeTempMessage(vec.at(i).at(0));
        }
    }
}



