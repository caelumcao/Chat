#ifndef FRIENDFINDWIDGET_H
#define FRIENDFINDWIDGET_H

#include <QWidget>
#include <string>
#include <vector>

using std::string;
using std::vector;


namespace Ui {
class FriendFindWidget;
}

class ChatClient;
class QTimer;
class QListWidgetItem;

class FriendFindWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendFindWidget(ChatClient *client, QWidget *parent = 0);
    ~FriendFindWidget();

    void setListWidget(const vector<string> &dataVec);

private:
    void setCtrlsEnabled(bool b);

signals:
    void closed();

private slots:
    void timerUpdate();

    void on_findButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void closeEvent(QCloseEvent *);

    void on_applyButton_clicked();

private:
    Ui::FriendFindWidget *ui;
    ChatClient * m_client;
    QTimer * m_timer;
    bool m_bSendMsg;
    QString m_inputText;
};

#endif // FRIENDFINDWIDGET_H
