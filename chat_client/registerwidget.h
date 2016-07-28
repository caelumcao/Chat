#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include "chatclient.h"

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(ChatClient *client, QWidget *parent = 0);
    ~RegisterWidget();

private:
    void setCtrlsEnabled(bool b);
    void setCtrlsVisible(bool b);

private slots:
    void messageProcess(const string &msg);

    void on_registerButton_clicked();

    void on_cancleButton_clicked();

    void on_okButton_clicked();

private:
    Ui::RegisterWidget *ui;
    ChatClient * m_client;
};

#endif // REGISTERWIDGET_H
