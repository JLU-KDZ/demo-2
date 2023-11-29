#include "server_kdz.h"

#include <QApplication>
#include <QHostInfo>

QHostAddress hostIP();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server_KDZ s;
    s.show();

    return a.exec();
}
