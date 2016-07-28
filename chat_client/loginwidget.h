#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "chatclient.h"
#include "widget.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(ChatClient *client, QWidget *parent = 0);
    ~LoginWidget();

private:
    void setCtrlsEnabled(bool b);

signals:
    void loginSuccess();

private slots:
    void messageProcess(const string &msg);

    void on_registerButton_clicked();

    void on_loginButton_clicked();


private:
    Ui::LoginWidget *ui;
    ChatClient * m_client;

    string m_userInfoMsg;
    string m_friendListMsg;
    vector<string> m_newMsgVec;

};

#endif // LOGINWIDGET_H
