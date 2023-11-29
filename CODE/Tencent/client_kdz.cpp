#include "client_kdz.h"
#include "ui_client_kdz.h"

Client_KDZ::Client_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client_KDZ)
{
    ui->setupUi(this);
}

void Client_KDZ::init(QString _username, quint16 _port)
{
    username=_username;
    //获取本机正在使用的IP
    ip=QString::fromStdString(GetIP_KDZ::GetSystemIpAddress());
    qDebug()<<"IP地址:"+ip;
    port=_port;
    //套接字初始化
    socket = new QTcpSocket(this);
    //连接状态初始化
    ok = false;
    //连接信号与槽
    connect(socket,&QTcpSocket::connected,this,&Client_KDZ::connected_slot);
    //连接信号与槽
    connect(socket,&QTcpSocket::disconnected,this,&Client_KDZ::disconnected_slot);
    //当客户端接收到服务器发的消息后，触发readyRead信号
    connect(socket,&QTcpSocket::readyRead,this,&Client_KDZ::readyRead_slot);
}

Client_KDZ::~Client_KDZ()
{
    delete ui;
}

int Client_KDZ::startC()
{
    if(ok == false)
    {
        //连接服务器
        socket->connectToHost(QHostAddress(ip),port);
        //成功连后触发connected的信号
        ok=true;
        return 0;
    }
    else
    {
        return 1;
    }
}

int Client_KDZ::closeC()
{
    if(ok == true)
    {
        QString mes=username+"退出群聊";
        //告诉服务器客户端断开
        socket->write(mes.toLocal8Bit());
        //断开服务器
        socket->disconnectFromHost();
        //成功断开后触发disconnected的信号
        ok=false;
        return 0;
    }
    else
    {
        return 1;
    }
}

void Client_KDZ::send(QString message)
{
    QString mes=username+":"+message;
    //将数据发送给服务器
    socket->write(mes.toLocal8Bit());
}

//connect信号对应的槽函数
void Client_KDZ::connected_slot()
{
    QString mes=username+"加入群聊";
    //告诉服务器客户端接入
    socket->write(mes.toLocal8Bit());
}

//disconnect信号对应的槽函数的声明
void Client_KDZ::disconnected_slot()
{

}

//readyRead信号对应的槽函数
void Client_KDZ::readyRead_slot()
{
    //获取套接字中的数据
    QString mes=QString::fromLocal8Bit(socket->readAll());
    //将数据显示再listwidget中
    emit sendM(mes);
}
