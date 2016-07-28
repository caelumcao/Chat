#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <string>
#include <queue>
using std::string;
using std::queue;

namespace Ui {
class ChatWidget;
}

class ChatClient;

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(ChatClient * client, const string &userId, QWidget *parent = 0);
    ~ChatWidget();

    void appendMsg(const string &content, const string &time = "");
    void appendTime(const string &time);

    string userId() const;
    void setUserId(const string &userId);

    string name() const;
    void setName(const string &name);

    bool hasNewMsg() const;
    void setHasNewMsg(bool hasNewMsg);

private slots:
    void on_sendButton_clicked();

private:
    Ui::ChatWidget *ui;
    string m_userId;
    string m_name;
    ChatClient * m_client;
    queue<int> m_msgPosQueue;
    bool m_hasNewMsg;
};

#endif // CHATWIDGET_H
