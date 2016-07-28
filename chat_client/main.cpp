#include "widget.h"
#include "loginwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatClient chatClient;
    //chatClient.connectToServer();

    //Widget w(&chatClient);
    //w.show();

    //vector<string> vec;
    //Message message("000", "111", "99", vec);

    LoginWidget lw(&chatClient);
    lw.show();



    return a.exec();
}
