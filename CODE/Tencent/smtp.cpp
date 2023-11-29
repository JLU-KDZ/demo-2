#include "smtp.h"

Smtp_KDZ::Smtp_KDZ(QByteArray username,QByteArray password)
{
    if(username.contains("@qq.com"))
    {
        m_UserName= username;
        m_Password = password;
    }
    else
    {
        qDebug()<<"Error";//若邮箱格式错误，在控制台输出“Error”
    }
}
int Smtp_KDZ::SendData(QByteArray sendIp, QByteArray s_Title, QByteArray s_Content)
{
    m_pSocket=new QTcpSocket();
    m_pSocket->connectToHost("smtp.qq.com",25,QTcpSocket::ReadWrite);  //连接qq邮箱
    m_pSocket->waitForConnected(1000);
    WaitAndReadData();
    m_pSocket->write("helo localhost\r\n");
    WaitAndReadData();
    m_pSocket->write("auth login\r\n");
    WaitAndReadData();
    m_pSocket->write(m_UserName.toBase64()+"\r\n");  //写入用户名
    WaitAndReadData();
    m_pSocket->write(m_Password.toBase64()+"\r\n");  //写入密码
    WaitAndReadData();
    m_pSocket->write("mail from: <"+m_UserName+">\r\n"); //发送的邮箱
    WaitAndReadData();
    m_pSocket->write("rcpt to: <"+sendIp+">\r\n"); //接收的邮箱
    WaitAndReadData();
    m_pSocket->write("data\r\n");  //开始写入
    WaitAndReadData();
    m_pSocket->write("from:<"+m_UserName+">\r\n");  //发送名称
    WaitAndReadData();
    m_pSocket->write("to:<"+sendIp+">");  //接受名称
    WaitAndReadData();
    m_pSocket->write("data\r\n");
    WaitAndReadData();
    m_pSocket->write("Subject:"+s_Title+"\r\n");  //标题
    m_pSocket->write("\r\n");
    m_pSocket->write(s_Content.append("\r\n")); //内容
    m_pSocket->write(".\r\n");
    WaitAndReadData();
    m_pSocket->write("quit\r\n");
    m_pSocket->disconnect();
    return 0;
}

QString Smtp_KDZ::WaitAndReadData()
{
    m_pSocket->waitForReadyRead(1000);//等待读取
    m_ReceiverData = m_pSocket->readAll();//开始读取
    return m_ReceiverData;
}

int Smtp_KDZ::getRand(int min, int max)
{
    return ( rand() % (max - min + 1) ) + min ;
}

Smtp_KDZ::~Smtp_KDZ()
{
    delete m_pSocket;
}
