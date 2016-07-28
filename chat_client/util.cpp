#include "util.h"
#include <sstream>

void numWriteToStr(std::string &str, int num)
{
    std::stringstream ss;
    if (num >= 0 && num < 9)
        ss << "000" << num;
    else if (num >= 10 && num < 99)
        ss << "00" << num;
    else if (num >= 100 && num < 999)
        ss << "0" << num;
    else if (num > 1000 && num <= 9999)
        ss << num;
    str = str + ss.str();
}

void numReadFromStr(int &num, const std::string &str)
{
    num = atoi(str.substr(0, INTSIZE).c_str());
}

