#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "util.h"
#include "chatserver.h"
#include "dataprocess.h"
#include "message.h"

using namespace std;

int main()
{
    //log_test("hello world1");
    //log_err("hello world2");
    ChatServer server(8888);
    server.run();

//    Message message("000", "111", "16", "hello");
//    message.setSendTime("10101010101010");
//    message.print();

//    DataProcess dp;
//    Message sMessage("525", "164", "08", "aaa");
//    if (dp.addMessage("525", "164", sMessage.msg(), "2012-10-10 10:10:10", "0") < 0)
//        cout << "fail\n";
//    else
//        cout << "success\n";
    //srand(time(0));
    //DataProcess dp;
//    vector<vector<string> > vec;
//    if (dp.checkNewMessage(vec, "222")< 0)
//        cout << "fail\n";
//    else {
//        cout << "success\n";
//        for (int i = 0; i < vec.size(); ++i)
//            cout << vec.at(i).at(0) << ", " << vec.at(i).at(1) << endl;
//    }
//    vector<string> vec;
//    vec.push_back("hello");
//    vec.push_back("world");
//    Message m("111", "000", "01", vec);
//    m.print();
//    Message m2(m.msg());
//    m2.print();
//    std::cout << m2.text() << endl;


//    for (int i = 1; i < 1000; ++i)
//    {
//        stringstream ss;
//        ss << i;
//        string str = ss.str();
//        if (str.size() == 1)
//            str = "00" + str;
//        else if (str.size() == 2)
//            str = "0" + str;
//        dp.addUserId(str);
//    }

//    string id;
//    if (dp.chooseUserId(id) < 0)
//        cout << "fail\n";
//    else
//        cout << "id = " << id << endl;

    return 0;
}

