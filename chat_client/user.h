#ifndef USER_H
#define USER_H

#include <string>
using std::string;

class ChatWidget;
class QListWidgetItem;

class User
{
public:
    User();
    User(const string &id, const string &nickname, const string &sex, const string &birthday, const string &status);  /* for master */
    User(const string &id, const string &remark, const string &nickname, const string &status); /* for friends */

    void setInfo(const string &nickname, const string &sex, const string &birthday);

    string id() const;
    void setId(const string &id);

    string nickname() const;
    void setNickname(const string &nickname);

    string sex() const;
    void setSex(const string &sex);

    string birthday() const;
    void setBirthday(const string &birthday);

    string status() const;
    void setStatus(const string &status);

    string remark() const;
    void setRemark(const string &remark);

    bool isUpdateInfo() const;
    void setIsUpdateInfo(bool isUpdateInfo);

    ChatWidget *chatWidget() const;
    void setChatWidget(ChatWidget *chatWidget);

    QListWidgetItem *chatItem() const;
    void setChatItem(QListWidgetItem *chatItem);

    QListWidgetItem *friendListItem() const;
    void setFriendListItem(QListWidgetItem *friendListItem);

private:
    string m_id;
    string m_nickname;
    string m_remark;
    string m_sex;
    string m_birthday;
    string m_status;
    bool m_isUpdateInfo;

    ChatWidget * m_chatWidget;
    QListWidgetItem * m_chatItem;
    QListWidgetItem * m_friendListItem;

};

extern User master;

/*
 * error (why?)
User & master()
{
    static User u;
    return u;
}
*/


#endif // USER_H
