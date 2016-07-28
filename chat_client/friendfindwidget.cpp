#include "friendfindwidget.h"
#include "ui_friendfindwidget.h"
#include "chatclient.h"
#include "message.h"
#include "user.h"
#include <QTimer>
#include <QListWidget>


FriendFindWidget::FriendFindWidget(ChatClient *client, QWidget *parent) :
    m_client(client), QWidget(parent),
    ui(new Ui::FriendFindWidget)
{
    ui->setupUi(this);
    ui->applyButton->setEnabled(true);

    m_bSendMsg = false;
    m_inputText = "";
    m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

    //m_timer->start(100);
}

FriendFindWidget::~FriendFindWidget()
{
    delete ui;
}

void FriendFindWidget::setListWidget(const vector<string> &dataVec)
{
    ui->listWidget->clear();
    int len = dataVec.size();
    if (len % 2 != 0)       /* message error */
    {
        setCtrlsEnabled(true);
        return;
    }
    for (int i = 0; i < len; i += 2)
    {
        string listText = dataVec.at(i + 1);
        if (dataVec.at(i) == m_inputText.toStdString())
            listText = listText + "(" + dataVec.at(i) + ")";
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole, QString::fromStdString(dataVec.at(i)));
        item->setText(QString::fromStdString(listText));
        ui->listWidget->insertItem(0, item);
    }
    ui->listWidget->sortItems(Qt::DescendingOrder);
    setCtrlsEnabled(true);
}

void FriendFindWidget::setCtrlsEnabled(bool b)
{
    m_bSendMsg = !b;
    ui->lineEdit->setEnabled(b);
    ui->findButton->setEnabled(b);
}

void FriendFindWidget::timerUpdate()
{
    if (!m_bSendMsg)
    {
        if (ui->listWidget->hasFocus() && ui->listWidget->selectedItems().size() > 0)
        {
            if (!ui->applyButton->isEnabled())
                ui->applyButton->setEnabled(true);
        } else {
            if (ui->applyButton->isEnabled())
                ui->applyButton->setEnabled(false);
        }
    } else {
        if (ui->applyButton->isEnabled())
            ui->applyButton->setEnabled(false);
    }
}

void FriendFindWidget::on_findButton_clicked()
{
    setCtrlsEnabled(false);
    m_inputText = ui->lineEdit->text();
    Message sMessage(master.id(), "000", "06", m_inputText.toStdString());
    if (m_client->sendMessage(sMessage) < 0)        /* write error */
    {
        setCtrlsEnabled(true);
        return;
    }
}

void FriendFindWidget::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == "")
        ui->findButton->setEnabled(false);
    else
        ui->findButton->setEnabled(true);
}

void FriendFindWidget::closeEvent(QCloseEvent *)
{
    emit closed();
}

void FriendFindWidget::on_applyButton_clicked()
{
    setCtrlsEnabled(false);
    QList<QListWidgetItem *> itemList = ui->listWidget->selectedItems();
    for (int i = 0; i < itemList.size(); ++i)
    {
        QListWidgetItem * item = itemList.at(i);
        std::string toId = item->data(Qt::UserRole).toString().toStdString();
        Message sMessage(master.id(), toId, "08", master.nickname());
        if (m_client->sendMessage(sMessage) < 0)      /* write error */
        {
            setCtrlsEnabled(true);
            return;
        }
        item->setText(item->text() + " 已发送请求");
    }
    setCtrlsEnabled(true);
}
