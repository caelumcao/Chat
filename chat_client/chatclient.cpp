#include "chatclient.h"
#include "message.h"
#include <iostream>
#include "util.h"


ChatClient::ChatClient()
{
    m_bConnected = false;
    connect(this, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(this, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

void ChatClient::connectToServer()
{
    if (!m_bConnected)
        connectToHost("192.168.23.160", 8888);
    //connectToHost("139.129.20.14", 8888);
}

void ChatClient::closeConnection()
{
    std::cout << "close connection\n";
    m_bConnected = false;
    close();
}

void ChatClient::readMessage()
{
    char lenbuf[INTSIZE + 1];
    lenbuf[INTSIZE] = '\0';
    int len;
    while (bytesAvailable() > 0)
    {
        string msg = "";
        if (readn(lenbuf, INTSIZE) <= 0)
        {
            std::cout << "read error or connection closed\n";
            return;
        }
        msg.append(lenbuf, INTSIZE);
        numReadFromStr(len, msg);
        char buf[MAXLINE];
        if (readn(buf, len) <= 0)
        {
            std::cout << "read error or connection closed\n";
            return;
        }
        msg.append(buf, len);
        messageProcess(msg);
    }
}

int ChatClient::sendMessage(const Message &message)
{
    return writen(message.msg().c_str(), message.msglen());
}

void ChatClient::connectionEstablished()
{
    m_bConnected = true;
    std::cout << "connection established\n";
}

void ChatClient::connectionClosedByServer()
{
    std::cout << "connection closed by server\n";
    m_bConnected = false;
    close();
}

void ChatClient::connectionError()
{
    std::cout << "connection error\n";
    close();
}

qint64 ChatClient::readn(char *buf, qint64 n)
{
    qint64	nleft, nread;
    char	*ptr;

    ptr = buf;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(ptr, nleft)) < 0)
        {
            //std::cout << "read meesage: read error\n";
            return -1;
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        ptr   += nread;
    }
    return (n - nleft);		/* return >= 0 */
}

qint64 ChatClient::writen(const char *buf, qint64 n)
{
    qint64		nleft, nwritten;
    const char	*ptr;

    ptr = buf;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(ptr, nleft)) <= 0)
        {
            std::cout << "write error\n";
            return -1;			/* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return n;
}

void ChatClient::messageProcess(const std::string &msg)
{
    emit messageRead(msg);
}
bool ChatClient::bConnected() const
{
    return m_bConnected;
}

void ChatClient::setBConnected(bool bConnected)
{
    m_bConnected = bConnected;
}





