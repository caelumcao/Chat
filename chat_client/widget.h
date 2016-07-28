#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chatclient.h"
#include "user.h"


namespace Ui {
class Widget;
}

class FriendFindWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(ChatClient *client, QWidget *parent = 0);
    ~Widget();

    void initMsg(const string &userInfoMsg, const string &friendListMsg, const vector<string> newMsgVec);

private slots:
    void closeEvent(QCloseEvent *e);
    void messageProcess(const string &msg);
    void on_ffWidget_closed();

    void on_findFriendButton_clicked();


    void on_chatButton_clicked();

    void on_friendListButton_clicked();

    void on_friendListWidget_itemClicked(QListWidgetItem *item);

    void on_gotoChatButton_clicked();

private:
    int getFriendIndex(const string &id);
    int showChatWidget(const string &id, const string &msg = "");
    void mySleep();

private:
    Ui::Widget *ui;
    ChatClient *m_client;
    FriendFindWidget * m_ffWidget;
    ChatWidget * m_curChatWidget;

    vector<User> m_friendsVec;
    vector<ChatWidget *> m_chatWidgetVec;
};

#endif // WIDGET_H
