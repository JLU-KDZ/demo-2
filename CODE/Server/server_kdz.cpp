#include "server_kdz.h"
#include "ui_server_kdz.h"
#include <QDebug>

Server_KDZ::Server_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server_KDZ)
{
    ui->setupUi(this);
    //实例化服务器对象
    server = new QTcpServer(this);

    setDir();
}

int Server_KDZ::searchFile(QString gid)
{
    QDir *dir = new QDir;
    QString path = pqth0+QString("/QQGroup/"+gid+"");

    if(!dir->exists(path))
    {
       return 1;//群号不存在
    }
    else
    {
        return 0;//群号存在
    }

}

void Server_KDZ::setFile(QString gid)
{
    QDir *dir = new QDir;
    QString path = pqth0+QString("/QQGroup/"+gid+"");

    if(!dir->exists(path))
    {
       return;//群号不存在
    }
    else
    {
        QFile file(path+"/flag.txt");
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            file.close();
        }
        return;//添加标记文件
    }
}

void Server_KDZ::removeFile(QString gid)
{
    QDir *dir = new QDir;
    QString path = pqth0+QString("/QQGroup/"+gid+"");

    if(!dir->exists(path))
    {
       return;//群号不存在
    }
    else
    {
        QString filePath=path+"/flag.txt";
        if(!QDir().exists(filePath))
        {
            return;//标记文件不存在
        }
        else
        {
            QFile::remove(filePath);
            return;//删除标记文件
        }
    }
}

void Server_KDZ::refresh()
{
    server->close();
    delete server;
    server=new QTcpServer(this);
    socketList.clear();
}

void Server_KDZ::setDir()
{
    /*###*/
    //设置群号路径
    //调试路径
    pqth0=QString("E:/qtcode/build-Tencent-Desktop_Qt_5_9_4_MinGW_32bit-Debug/debug");

    //发布路径
    /*
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    pqth0=dir.absolutePath()+QString("/Tencent");
    */

}

void Server_KDZ::historyMes(QString mes)
{
    QString gid=ui->label_cgid->text();

    QFile file(pqth0+QString("/QQGroup/"+gid+"/chat.txt"));
    if(file.open(QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<mes<<"\r\n";
        file.close();
        return;
    }
}

void Server_KDZ::closeEvent(QCloseEvent *e)
{
    removeFile(ui->label_cgid->text());

    server->close();
    delete server;
    socketList.clear();
    e->accept();
}

Server_KDZ::~Server_KDZ()
{
    delete ui;
}

void Server_KDZ::readyRead_slot()
{
    //判断客户端套接字是否有效,无效则移除
    for(int i=0;i<socketList.count();i++)
    {
        if(socketList.at(i)->state()==false)
        {
            socketList.removeAt(i);
        }
    }

    //遍历客户端的链表，判断是哪一个客户端发送的消息
   for(quint16 i=0;i<socketList.count();i++)
   {
       //获取客户端中待读数据的个数
       if(socketList.at(i)->bytesAvailable())
       {
            //获取客户端传过来的待读数据
            QString mes=QString::fromLocal8Bit(socketList.at(i)->readAll());
            ui->listWidget->addItem(mes);
            //写入历史消息文件
            historyMes(mes);
            //广播消息
            send(mes);
       }
   }
}

void Server_KDZ::send(QString mes)
{
    for(int i=0;i<socketList.count();i++)
    {
        socketList.at(i)->write(mes.toLocal8Bit());
    }
}

void Server_KDZ::on_pushButton_clicked()
{
    if(ui->label_cgid->text()==ui->lineEdit->text())
    {
        QMessageBox::information(this,"启动提示","服务器已启动！");
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        return;
    }

    QString gid=ui->lineEdit->text();
    QString cgid=ui->label_cgid->text();
    if(gid.length()==0)
    {
        QMessageBox::warning(this,tr("启动失败"),tr("群号为空！"),QMessageBox::Ok);
        return;
    }

    if(searchFile(gid)==1)
    {
        QMessageBox::warning(this,tr("启动失败"),tr("群号不存在！"),QMessageBox::Ok);
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        return;
    }

    if(searchFile(cgid)==0)
    {
        removeFile(cgid);
    }

    if(searchFile(gid)==0)
    {
        setFile(gid);
    }

    //设置端口
    quint16 port=8888;

    //服务器监听
    if(cgid!=QString("无"))
    {
        refresh();
    }

    if(server->listen(QHostAddress::Any,port))
    {
        QMessageBox::information(this,"启动提示","服务器启动成功！");
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        ui->label_cgid->setText(gid);
    }
    else
    {
        QMessageBox::warning(this,tr("启动失败"),tr("服务器启动失败！"),QMessageBox::Ok);
        return;
    }

    //当有新的客户端连接到服务器后，触发newConnection信号
    connect(server,&QTcpServer::newConnection,this,&Server_KDZ::newConnection_slot);
}

void Server_KDZ::newConnection_slot()
{
    //获取最新连接的客户端的套接字
    QTcpSocket *socket=server->nextPendingConnection();
    socketList.append(socket);

    //当服务器接受到客户端传过来的消息，触发readyRead信号
    connect(socket,&QTcpSocket::readyRead,this,&Server_KDZ::readyRead_slot);
}
