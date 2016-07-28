#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include "user.h"
#include "friendfindwidget.h"
#include <QMessageBox>
#include <QElapsedTimer>
#include "util.h"
#include "chatwidget.h"

Widget::Widget(ChatClient *client, QWidget *parent) :
    m_client(client), QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_ffWidget = NULL;
    m_curChatWidget = NULL;

    connect(m_client, SIGNAL(messageRead(string)), this, SLOT(messageProcess(string)));
}

Widget::~Widget()
{
    delete ui;
    if (m_ffWidget != NULL)
        m_ffWidget->close();
}

void Widget::initMsg(const string &userInfoMsg, const string &friendListMsg, const vector<string> newMsgVec)
{
    Message message_1(userInfoMsg);
    vector<string> vec_1 = message_1.subTextVec();
    master = User(vec_1.at(0), vec_1.at(1), vec_1.at(2), vec_1.at(3), "1");
    ui->nicknameButton->setText(QString::fromStdString(master.nickname()));

    /* init friendListWidget */
    Message message_2(friendListMsg);
    vector<string> vec_2 = message_2.subTextVec();
    if (vec_2.size() % 4 == 0)
    {
        for (int i = 0; i < vec_2.size(); i += 4)
        {
            User user(vec_2.at(i), vec_2.at(i + 1), vec_2.at(i + 2), vec_2.at(i + 3));
            string listText = "";
            if (vec_2.at(i + 1) == "")
                listText = vec_2.at(i + 2);
            else
                listText = vec_2.at(i + 1) + "(" + vec_2.at(i + 2) + ")";
            if (vec_2.at(i + 3) == "1")
                listText += " 在线";
            QListWidgetItem *item = new QListWidgetItem;
            item->setData(Qt::UserRole, QString::fromStdString(vec_2.at(i)));
            item->setText(QString::fromStdString(listText));
            ui->friendListWidget->insertItem(0, item);
            user.setFriendListItem(item);
            m_friendsVec.push_back(user);
        }
        ui->friendListWidget->sortItems(Qt::DescendingOrder);
    }
    for (int i = 0; i < newMsgVec.size(); ++i)
        messageProcess(newMsgVec.at(i));

}



void Widget::closeEvent(QCloseEvent *e)
{
    m_client->closeConnection();
    close();
}

void Widget::messageProcess(const string &msg)
{
    Message message(msg);
    message.print();
    string from = message.from();
    string type = message.type();
    string text = message.text();
    vector<string> textVec = message.subTextVec();
    if (type == "05")               /* friend's status changed */
    {
        int index = getFriendIndex(from);
        QListWidgetItem * item = m_friendsVec.at(index).friendListItem();
        if (text == "1")
        {
            m_friendsVec.at(index).setStatus("1");
            string text = "";
            User user = m_friendsVec.at(index);
            if (user.remark() == "")
                text = user.nickname() + " 在线";
            else
                text = user.remark() + "(" + user.nickname() + ") 在线";
            item->setText(QString::fromStdString(text));
        } else {
            m_friendsVec.at(index).setStatus("0");
            string text = "";
            User user = m_friendsVec.at(index);
            if (user.remark() == "")
                text = user.nickname();
            else
                text = user.remark() + "(" + user.nickname() + ")";
            item->setText(QString::fromStdString(text));
        }
    } else if (type == "07") {      /* find friend result */
        if (m_ffWidget != NULL)
            m_ffWidget->setListWidget(textVec);
    } else if (type == "08") {      /* friend application */
        int res = QMessageBox::information(NULL, "好友申请", QString::fromStdString(text) + "申请您为好友， 是否同意", "同意", "不同意", "忽略");
        if (res == 0)
        {
            vector<string> vec;
            vec.push_back("1");
            vec.push_back(master.nickname());
            m_client->sendMessage(Message(master.id(), from, "09", vec));
            m_client->sendMessage(Message(master.id(), "000", "18", from));
        } else if (res == 1) {
            vector<string> vec;
            vec.push_back("0");
            vec.push_back(master.nickname());
            m_client->sendMessage(Message(master.id(), from, "09", vec));
        }
    } else if (type == "09") {      /* friend application result */
        if (textVec.at(0) == "1")
        {
            QMessageBox::information(NULL, "好友申请回复", QString::fromStdString(textVec.at(1)) + "同意了您的好友申请", "确定");
        } else {
            QMessageBox::information(NULL, "好友申请回复", QString::fromStdString(textVec.at(1)) + "拒绝了您的好友申请", "确定");
        }
    } else if (type == "15") {      /* return user's information */
        int index = getFriendIndex(textVec.at(0));
        m_friendsVec.at(index).setInfo(textVec.at(1), textVec.at(2), textVec.at(3));
    } else if (type == "16") {      /* reveive friend's chat message */
        showChatWidget(from, message.msg());
    } else if (type == "17") {      /* return message send time */
        int index = getFriendIndex(from);
        m_friendsVec.at(index).chatWidget()->appendTime(message.sendtime());
    } else if (type == "19") {      /* return friend's nickname and status */
        User user(textVec.at(0), "", textVec.at(1), textVec.at(2));
        string listText = textVec.at(1);
        if (textVec.at(2) == "1")
            listText += " 在线";
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole, QString::fromStdString(textVec.at(0)));
        item->setText(QString::fromStdString(listText));
        ui->friendListWidget->insertItem(0, item);
        ui->friendListWidget->sortItems(Qt::DescendingOrder);
        user.setFriendListItem(item);
        m_friendsVec.push_back(user);
    }
}

void Widget::on_ffWidget_closed()
{
    m_ffWidget = NULL;
    disconnect(this, 0, m_ffWidget, 0);
}


void Widget::on_findFriendButton_clicked()
{
    if (m_ffWidget == NULL)
    {
        m_ffWidget = new FriendFindWidget(m_client);
        m_ffWidget->show();
        connect(m_ffWidget, SIGNAL(closed()), this, SLOT(on_ffWidget_closed()));
    }
}

void Widget::on_chatButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_friendListButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_friendListWidget_itemClicked(QListWidgetItem *item)
{
    string id = item->data(Qt::UserRole).toString().toStdString();
    int index = getFriendIndex(id);
    if (!m_friendsVec.at(index).isUpdateInfo())
    {
        m_client->sendMessage(Message(master.id(), "000", "14", id));
        for (int i = 0; i < 15; ++i)
        {
            if (m_friendsVec.at(index).isUpdateInfo())
                break;
            mySleep();
        }
        if (!m_friendsVec.at(index).isUpdateInfo())
        {
            QMessageBox::information(NULL, "错误", ERRORTEXT_1, "确定");
            return;
        }
    }
    User user = m_friendsVec.at(index);
    ui->friendIdLabel->setText(QString::fromStdString(user.id()));
    ui->friendNickNameLabel->setText(QString::fromStdString(user.nickname()));
    ui->friendRemarkLabel->setText(QString::fromStdString(user.remark()));
    ui->friendSexLabel->setText(QString::fromStdString(user.sex()));
    ui->friendBirthLabel->setText(QString::fromStdString(user.birthday()));
}

void Widget::on_gotoChatButton_clicked()
{
    string id = ui->friendIdLabel->text().toStdString();
    showChatWidget(id);
}

int Widget::getFriendIndex(const string &id)
{
    for (int i = 0; i < m_friendsVec.size(); ++i)
        if (m_friendsVec.at(i).id() == id)
            return i;
    return -1;
}

int Widget::showChatWidget(const string &id, const string &msg)
{
    int index = getFriendIndex(id);
    QListWidgetItem * lwi = m_friendsVec.at(index).chatItem();
    if (lwi == NULL)
    {
        lwi = new QListWidgetItem;
        lwi->setData(Qt::UserRole, QString::fromStdString(id));
        if (m_friendsVec.at(index).remark() == "")
            lwi->setText(QString::fromStdString(m_friendsVec.at(index).nickname()));
        else
            lwi->setText(QString::fromStdString(m_friendsVec.at(index).remark()));
        ui->chatListWidget->insertItem(0, lwi);
        m_friendsVec.at(index).setChatItem(lwi);
    }
    ChatWidget * cw = m_friendsVec.at(index).chatWidget();
    if (cw == NULL)
    {
        cw = new ChatWidget(m_client, id, ui->chatBlankWidget);
        ui->chatWidgetLayout->addWidget(cw);
        if (m_friendsVec.at(index).remark() == "")
            cw->setName(m_friendsVec.at(index).nickname());
        else
            cw->setName(m_friendsVec.at(index).remark());
        m_friendsVec.at(index).setChatWidget(cw);
    }
    if (msg == "" && m_curChatWidget != cw)
    {
        ui->stackedWidget->setCurrentIndex(0);
        if (m_curChatWidget != NULL)
            m_curChatWidget->hide();
        cw->show();
        m_curChatWidget = cw;
    }
    if (msg != "")
    {
        Message message(msg);
        cw->appendMsg(message.text().substr(0, message.text().size() - 14), message.sendtime());
        if (!cw->hasNewMsg())
        {
            cw->setHasNewMsg(true);
            lwi->setText(QString::fromStdString(cw->name() + " (新消息)"));
        }
    } else {
        if (cw->hasNewMsg())
        {
            cw->setHasNewMsg(false);
            lwi->setText(QString::fromStdString(cw->name()));
        }
    }
}

void Widget::mySleep()
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < 100)
        QCoreApplication::processEvents();
}
