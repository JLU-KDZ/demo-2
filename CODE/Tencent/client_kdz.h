#ifndef CLIENT_KDZ_H
#define CLIENT_KDZ_H

#include "getip_kdz.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QWidget>

namespace Ui {
class Client_KDZ;
}

class Client_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit Client_KDZ(QWidget *parent = 0);
    void init(QString _username,quint16 _port=8888);

    int startC();//连接客户端
    int closeC();//断开客户端
    void send(QString message);//发送消息
    ~Client_KDZ();

private slots:

    void connected_slot(); //connecedt信号对应的槽函数

    void disconnected_slot();//disconnected信号对应的槽函数

    void readyRead_slot();//readyRead信号对应的槽函数

signals:
    void sendM(QString m);

private:
    //客户端套接字指针
    QTcpSocket *socket;
    //用户名
    QString username;
    //IP
    QString ip;
    //端口
    quint16 port;
    //连接状态
    bool ok;

private:
    Ui::Client_KDZ *ui;
};

#endif // CLIENT_KDZ_H
