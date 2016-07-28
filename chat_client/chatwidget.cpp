#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "chatclient.h"
#include "user.h"
#include "util.h"
#include <QMessageBox>
#include <QTextCursor>
#include <QTextBlock>

ChatWidget::ChatWidget(ChatClient * client, const string &userId, QWidget *parent) :
    m_client(client), m_userId(userId), QWidget(parent), ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->textBrowser->setOverwriteMode(true);
    m_hasNewMsg = false;
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::appendMsg(const std::string &content, const std::string &time)
{
    if (time == "")
    {
        string userText = "[" + master.nickname() + "]";
        ui->textBrowser->append(QString::fromStdString(userText));
        QTextCursor cursor = ui->textBrowser->textCursor();
        m_msgPosQueue.push(cursor.position());
    } else {
        string userText = "[" + m_name + "] " + time;
        ui->textBrowser->append(QString::fromStdString(userText));
    }
    ui->textBrowser->append(QString::fromStdString(content));
}

void ChatWidget::appendTime(const std::string &time)
{
    int pos = m_msgPosQueue.front();
    m_msgPosQueue.pop();
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(pos);
    cursor.insertText(QString::fromStdString(" " + time));
}

string ChatWidget::userId() const
{
    return m_userId;
}

void ChatWidget::setUserId(const string &userId)
{
    m_userId = userId;
}
string ChatWidget::name() const
{
    return m_name;
}

void ChatWidget::setName(const string &name)
{
    m_name = name;
    ui->label->setText(QString::fromStdString(m_name));
}

void ChatWidget::on_sendButton_clicked()
{
    string text = ui->plainTextEdit->toPlainText().toStdString();
    if (text == "")
    {
        QMessageBox::information(NULL, "提示", "发送内容不能为空", "确定");
        return;
    }
    Message sMessage(master.id(), m_userId, "16", text);
    if (m_client->sendMessage(sMessage) < 0)
    {
        QMessageBox::information(NULL, "错误", ERRORTEXT_1, "确定");
        return;
    }
    ui->plainTextEdit->clear();
    appendMsg(text);
}
bool ChatWidget::hasNewMsg() const
{
    return m_hasNewMsg;
}

void ChatWidget::setHasNewMsg(bool hasNewMsg)
{
    m_hasNewMsg = hasNewMsg;
}

