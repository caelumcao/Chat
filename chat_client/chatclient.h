#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QAbstractSocket>
#include <QTcpSocket>
#include <string>
#include "message.h"
using std::string;

class ChatClient : public QTcpSocket
{
    Q_OBJECT
public:
    ChatClient();

    bool bConnected() const;
    void setBConnected(bool bConnected);

signals:
    void messageRead(const string &msg);

public slots:
    void connectToServer();
    void closeConnection();
    //void sendRequest();

    void readMessage();
    int sendMessage(const Message &message);
    void connectionEstablished();
    void connectionClosedByServer();
    void connectionError();

private:
    qint64 readn(char *buf, qint64 n);
    qint64 writen(const char *buf, qint64 n);

    void messageProcess(const string &msg);

private:
    bool m_bConnected;
};

#endif // CHATCLIENT_H
