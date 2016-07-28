#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "util.h"

RegisterWidget::RegisterWidget(ChatClient *client, QWidget *parent) :
    m_client(client), QWidget(parent), ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    ui->okButton->hide();
    ui->sucLabel->hide();

    connect(client, SIGNAL(messageRead(string)), this, SLOT(messageProcess(string)));
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::setCtrlsEnabled(bool b)
{
    ui->nicknameLineEdit->setEnabled(b);
    ui->psdLineEdit->setEnabled(b);
    ui->confirmLineEdit->setEnabled(b);
    ui->registerButton->setEnabled(b);
    ui->cancleButton->setEnabled(b);
}

void RegisterWidget::setCtrlsVisible(bool b)
{
    ui->nicknameLineEdit->setVisible(b);
    ui->psdLineEdit->setVisible(b);
    ui->confirmLineEdit->setVisible(b);
    ui->registerButton->setVisible(b);
    ui->cancleButton->setVisible(b);
    ui->label->setVisible(b);
    ui->label_2->setVisible(b);
    ui->label_3->setVisible(b);
    ui->statusLabel->setVisible(b);

    ui->sucLabel->setVisible(!b);
    ui->okButton->setVisible(!b);
}

void RegisterWidget::messageProcess(const string &msg)
{
    Message rMessage(msg);
    if (rMessage.type() == "02")        /* register result */
    {
        rMessage.print();
        m_client->closeConnection();
        setCtrlsEnabled(true);
        string text = rMessage.text();  /* register success */
        if (text.size() == 4 && text.substr(0, 1) == "1")
        {
            string id = text.substr(1, 3);
            setCtrlsVisible(false);
            ui->sucLabel->setText(TEXT_1 + QString::fromStdString(id));
        } else if (text == "0") {   /* register fail */
            ui->statusLabel->setText(ERRORTEXT_3);
            ui->nicknameLineEdit->clear();
            ui->psdLineEdit->clear();
            ui->confirmLineEdit->clear();
        }
    }
}

void RegisterWidget::on_registerButton_clicked()
{
    if (ui->psdLineEdit->text() != ui->confirmLineEdit->text())
    {
        ui->statusLabel->setText(ERRORTEXT_4);
        ui->psdLineEdit->clear();
        ui->confirmLineEdit->clear();
        return;
    }
    setCtrlsEnabled(false);
    ui->statusLabel->setText("");
    m_client->connectToServer();
    string nickname = ui->nicknameLineEdit->text().toStdString();
    string password = ui->psdLineEdit->text().toStdString();
    vector<string> textVec;
    textVec.push_back(nickname);
    textVec.push_back(password);
    if (m_client->sendMessage(Message("???", "000", "01", textVec)) < 0)
    {
        setCtrlsEnabled(true);
        ui->statusLabel->setText(ERRORTEXT_1);
        m_client->closeConnection();
    }
}

void RegisterWidget::on_cancleButton_clicked()
{
    deleteLater();
}

void RegisterWidget::on_okButton_clicked()
{
    deleteLater();
}
