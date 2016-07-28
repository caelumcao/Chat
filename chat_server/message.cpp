#include "message.h"
#include "util.h"


Message::Message(const std::string &from, const std::string &to, const std::string &type, const std::string &text)
    : m_from(from), m_to(to), m_type(type)
{
    m_textNum = 1;
    m_contentlen = IDSIZE * 2 + MSGTYPESIZE + INTSIZE * m_textNum + text.size();
    m_subTextVec.push_back(text);
    m_msg = "";
    numWriteToStr(m_msg, m_contentlen);
    m_msg = m_msg + m_from + m_to + m_type;
    numWriteToStr(m_msg, m_textNum);
    m_msg = m_msg + text;
}

Message::Message(const std::string &from, const std::string &to, const std::string &type, const vector<std::string> subTextVec)
    : m_from(from), m_to(to), m_type(type), m_subTextVec(subTextVec)
{
    m_textNum = m_subTextVec.size();
    string text = "";
    for (int i = 0; i < m_textNum; ++i)
    {
        if (i < m_textNum - 1)
            m_textLenVec.push_back(m_subTextVec.at(i).size());
        text += m_subTextVec.at(i);
    }
    m_contentlen = IDSIZE * 2 + MSGTYPESIZE + (m_textNum == 0 ? INTSIZE : INTSIZE * m_textNum) + text.size();
    m_msg = "";
    numWriteToStr(m_msg, m_contentlen);
    m_msg = m_msg + m_from + m_to + m_type;
    numWriteToStr(m_msg, m_textNum);
    for (int i = 0; i < m_textLenVec.size(); ++i)
        numWriteToStr(m_msg, m_textLenVec.at(i));
    m_msg = m_msg + text;
}

Message::Message(const std::string &msg) : m_msg(msg)
{
    int index = 0;
    numReadFromStr(m_contentlen, m_msg.substr(index, INTSIZE));
    index += INTSIZE;
    m_from = m_msg.substr(index, IDSIZE);
    index += IDSIZE;
    m_to = m_msg.substr(index, IDSIZE);
    index += IDSIZE;
    m_type = m_msg.substr(index, MSGTYPESIZE);
    index += MSGTYPESIZE;
    numReadFromStr(m_textNum, m_msg.substr(index, INTSIZE));
    index += INTSIZE;
    for (int i = 0; i < m_textNum - 1; ++i)
    {
        int temp;
        numReadFromStr(temp, m_msg.substr(index, INTSIZE));
        m_textLenVec.push_back(temp);
        index += INTSIZE;
    }
    for (int i = 0; i < m_textLenVec.size(); ++i)
    {
        string temp = m_msg.substr(index, m_textLenVec.at(i));
        m_subTextVec.push_back(temp);
        index += m_textLenVec.at(i);
    }
    m_subTextVec.push_back(m_msg.substr(index, m_msg.size() - index));
}

int Message::msglen() const
{
    return m_msg.size();
}

std::string Message::text() const
{
    string temp = "";
    for (int i = 0; i < m_subTextVec.size(); ++i)
        temp += m_subTextVec.at(i);
    return temp;
}

std::string Message::sendtime() const
{
    string str = text();
    string timeStr = str.substr(str.size() - 14, 14);
    return timeStr.substr(0, 4) + "-" + timeStr.substr(4, 2) + "-" + timeStr.substr(6, 2)
            + " " + timeStr.substr(8, 2) + ":" + timeStr.substr(10, 2) + ":" + timeStr.substr(12, 2);
}

void Message::setSendTime(const std::string &time)
{
    int len = m_subTextVec.size();
    m_subTextVec.at(len - 1) = m_subTextVec.at(len - 1) + time;
    m_contentlen += 14;
    string str;
    numWriteToStr(str, m_contentlen);
    m_msg.replace(0, 4, str);
    m_msg += time;
}

void Message::print()
{
    std::cout << "len = " << m_contentlen << std::endl
              << "from = " << m_from << std::endl
              << "to = " << m_to << std::endl
              << "type = " << m_type << std::endl
              << "textNum = " << m_textNum << std::endl;
    for (int i = 0; i < m_textLenVec.size(); ++i)
        std::cout << "text " << (i + 1) << " len: " << m_textLenVec.at(i) << std::endl;
    for (int i = 0; i < m_subTextVec.size(); ++i)
        std::cout << "text " << (i + 1) << ": " << m_subTextVec.at(i) << std::endl;

}

int Message::contentlen() const
{
    return m_contentlen;
}

void Message::setContentlen(int contentlen)
{
    m_contentlen = contentlen;
}

string Message::from() const
{
    return m_from;
}

void Message::setFrom(const string &from)
{
    m_from = from;
}

string Message::to() const
{
    return m_to;
}

void Message::setTo(const string &to)
{
    m_to = to;
}

string Message::type() const
{
    return m_type;
}

void Message::setType(const string &type)
{
    m_type = type;
}

int Message::textNum() const
{
    return m_textNum;
}

void Message::setTextNum(int textNum)
{
    m_textNum = textNum;
}

vector<int> Message::textLenVec() const
{
    return m_textLenVec;
}

void Message::setTextLenVec(const vector<int> &textLenVec)
{
    m_textLenVec = textLenVec;
}

string Message::msg() const
{
    return m_msg;
}

void Message::setMsg(const string &msg)
{
    m_msg = msg;
}

vector<string> Message::subTextVec() const
{
    return m_subTextVec;
}

void Message::setSubTextVec(const vector<string> &subTextVec)
{
    m_subTextVec = subTextVec;
}



