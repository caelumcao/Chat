#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <string>

using std::string;

const unsigned int MAXLINE = 4096;
const unsigned int INTSIZE = 4;
const unsigned int IDSIZE = 3;
const unsigned int MSGTYPESIZE = 2;
const unsigned int MSGTIMESIZE = 12;
const unsigned int MSGHEADERSIZE = INTSIZE + IDSIZE * 2 + MSGTYPESIZE + INTSIZE;

static const QString ERRORTEXT_1 = "网络异常";
static const QString ERRORTEXT_2 = "帐号不存在或密码错误";
static const QString ERRORTEXT_3 = "注册失败";
static const QString ERRORTEXT_4 = "密码不一致，请重新输入";

static const QString TEXT_1 = "恭喜您，注册成功！\n\n请牢记您的账号：";

void numWriteToStr(string &str, int num);

void numReadFromStr(int &num, const string &str);

#endif // UTIL_H
