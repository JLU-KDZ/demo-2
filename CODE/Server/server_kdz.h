#ifndef SERVER_KDZ_H
#define SERVER_KDZ_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class Server_KDZ;
}

class Server_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit Server_KDZ(QWidget *parent = 0);
    int searchFile(QString gid);
    void setFile(QString gid);
    void removeFile(QString gid);
    void refresh();
    void setDir();
    void historyMes(QString mes);
    void closeEvent(QCloseEvent* e);
    ~Server_KDZ();

private slots:

    void newConnection_slot();//新连接

    void readyRead_slot();//准备读取

    void send(QString mes);//发送信息

    void on_pushButton_clicked();//启动服务器

private:
    Ui::Server_KDZ *ui;
    //定义服务器指针
    QTcpServer *server;
    //定义客户端指针列表
    QList<QTcpSocket*> socketList;
    //群号路径
    QString pqth0;
};

#endif // SERVER_KDZ_H
