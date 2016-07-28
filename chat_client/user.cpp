#include "user.h"

User::User()
{
    m_id = "???";
    m_nickname = "";
    m_remark = "";
    m_sex = m_birthday = "";
    m_status = "1";
    m_isUpdateInfo = false;

    m_chatWidget = NULL;
    m_chatItem = NULL;
    m_friendListItem = NULL;
}

User::User(const std::string &id, const std::string &nickname, const std::string &sex, const std::string &birthday, const std::string &status)
    : m_id(id), m_nickname(nickname), m_sex(sex), m_birthday(birthday), m_status(status)
{
    m_remark = "";
    m_isUpdateInfo = false;
    m_chatWidget = NULL;
    m_chatItem = NULL;
    m_friendListItem = NULL;
}

User::User(const std::string &id, const std::string &remark, const std::string &nickname, const std::string &status)
    : m_id(id), m_remark(remark), m_nickname(nickname), m_status(status)
{
    m_sex = m_birthday = "";
    m_isUpdateInfo = false;
    m_chatWidget = NULL;
    m_chatItem = NULL;
    m_friendListItem = NULL;
}

void User::setInfo(const std::string &nickname, const std::string &sex, const std::string &birthday)
{
    m_nickname = nickname;
    m_sex = sex;
    m_birthday = birthday;
    m_isUpdateInfo = true;
}

string User::id() const
{
    return m_id;
}

void User::setId(const string &id)
{
    m_id = id;
}
string User::nickname() const
{
    return m_nickname;
}

void User::setNickname(const string &nickname)
{
    m_nickname = nickname;
}
string User::sex() const
{
    return m_sex;
}

void User::setSex(const string &sex)
{
    m_sex = sex;
}
string User::birthday() const
{
    return m_birthday;
}

void User::setBirthday(const string &birthday)
{
    m_birthday = birthday;
}
string User::status() const
{
    return m_status;
}

void User::setStatus(const string &status)
{
    m_status = status;
}
string User::remark() const
{
    return m_remark;
}

void User::setRemark(const string &remark)
{
    m_remark = remark;
}
bool User::isUpdateInfo() const
{
    return m_isUpdateInfo;
}

void User::setIsUpdateInfo(bool isUpdateInfo)
{
    m_isUpdateInfo = isUpdateInfo;
}
ChatWidget *User::chatWidget() const
{
    return m_chatWidget;
}

void User::setChatWidget(ChatWidget *chatWidget)
{
    m_chatWidget = chatWidget;
}
QListWidgetItem *User::chatItem() const
{
    return m_chatItem;
}

void User::setChatItem(QListWidgetItem *chatItem)
{
    m_chatItem = chatItem;
}
QListWidgetItem *User::friendListItem() const
{
    return m_friendListItem;
}

void User::setFriendListItem(QListWidgetItem *friendListItem)
{
    m_friendListItem = friendListItem;
}







User master;


