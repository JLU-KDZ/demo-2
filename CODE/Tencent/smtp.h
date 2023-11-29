#ifndef SMTP_H
#define SMTP_H

#include<QByteArray>
#include<QString>
#include<QDebug>
#include<QTcpSocket>
#include<QTcpServer>
#include<QHostAddress>

class Smtp_KDZ
{
public:
    Smtp_KDZ(QByteArray username,QByteArray password);
    ~Smtp_KDZ();
    int SendData(QByteArray sendIp,QByteArray s_Title,QByteArray s_Content);//发送数据
    QString WaitAndReadData();//等待发送
    static int getRand(int min, int max);//获取随机数

private:
    QByteArray m_UserName="";
    QByteArray m_Password="";
    QTcpSocket * m_pSocket=nullptr;
    QString m_ReceiverData="";

};

#endif // SMTP_H
