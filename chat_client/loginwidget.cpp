#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "registerwidget.h"
#include "util.h"

LoginWidget::LoginWidget(ChatClient *client, QWidget *parent) :
    m_client(client), QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    m_userInfoMsg = m_friendListMsg = "";

    connect(client, SIGNAL(messageRead(string)), this, SLOT(messageProcess(string)));
}


LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::setCtrlsEnabled(bool b)
{
    ui->idLineEdit->setEnabled(b);
    ui->psdLineEdit->setEnabled(b);
    ui->loginButton->setEnabled(b);
    ui->registerButton->setEnabled(b);
}

void LoginWidget::messageProcess(const std::string &msg)
{
    Message rMessage(msg);
    rMessage.print();
    if (rMessage.type() == "02")        /* register result ->ignore */
    {

    } else if (rMessage.type() == "04") {       /* login result */
        setCtrlsEnabled(true);
        if (rMessage.text() == "1" && m_friendListMsg != "" && m_userInfoMsg != "")   /* login success */
        {
            Widget * w = new Widget(m_client);
            w->initMsg(m_userInfoMsg, m_friendListMsg, m_newMsgVec);
            w->show();
            close();
        } else {    /* login fail */
            m_client->closeConnection();
            ui->statusLabel->setText(ERRORTEXT_2);
            ui->psdLineEdit->clear();
        }
    } else if (rMessage.type() == "11") {       /* friend list */
        m_friendListMsg = msg;
    } else if (rMessage.type() == "15") {       /* user's information */
        m_userInfoMsg = msg;
    } else {        /* some other messages */
        m_newMsgVec.push_back(msg);
    }
}

void LoginWidget::on_registerButton_clicked()
{
    RegisterWidget * rw = new RegisterWidget(m_client);
    rw->show();
}

void LoginWidget::on_loginButton_clicked()
{
    setCtrlsEnabled(false);
    m_userInfoMsg = m_friendListMsg = "";
    m_newMsgVec.clear();
    ui->statusLabel->setText("");
    m_client->connectToServer();
    string id = ui->idLineEdit->text().toStdString();
    if (id.size() != 3)
    {
        ui->statusLabel->setText(ERRORTEXT_2);
    }
    string password = ui->psdLineEdit->text().toStdString();
    if (m_client->sendMessage(Message(id, "000", "03", password)) < 0)  /* write error */
    {
        setCtrlsEnabled(true);
        ui->statusLabel->setText(ERRORTEXT_1);
        m_client->closeConnection();
    }
}
