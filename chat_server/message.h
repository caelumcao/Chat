#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
using std::string;
using std::vector;

class Message
{
public:
    Message(const string &from, const string &to, const string &type, const string &text);
    Message(const string &from, const string &to, const string &type, const vector<string> subTextVec);
    Message(const string &msg);

    int msglen() const;
    string text() const;
    string sendtime() const;
    void setSendTime(const string &time);
    void print();

    int contentlen() const;
    void setContentlen(int contentlen);

    string from() const;
    void setFrom(const string &from);

    string to() const;
    void setTo(const string &to);

    string type() const;
    void setType(const string &type);

    int textNum() const;
    void setTextNum(int textNum);

    vector<int> textLenVec() const;
    void setTextLenVec(const vector<int> &textLenVec);

    string msg() const;
    void setMsg(const string &msg);

    vector<string> subTextVec() const;
    void setSubTextVec(const vector<string> &subTextVec);


private:


private:
    /* message structure */
    int m_contentlen;               /* 4 bytes, the value is not include the first two bytes */
    string m_from;                  /* 3 bytes */
    string m_to;                    /* 3 bytes */
    string m_type;                  /* 2 bytes */
    int m_textNum;                  /* 4 bytes */
    vector<int> m_textLenVec;  /* (m_textNum - 1) * 4 bytes */
    vector<string> m_subTextVec;
    string m_msg;                   /* less than 4096 bytes */



};

#endif // MESSAGE_H
