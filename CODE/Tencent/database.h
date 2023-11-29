#ifndef DATABASE_H
#define DATABASE_H

#include<QtSql/QSql>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QtSql/QSqlTableModel>
#include<QtSql/QSqlDatabase>
#include<QCoreApplication>
#include<QString>
#include<vector>
#include<QDebug>
#include<QDate>

using namespace std;

class DB_KDZ//数据库类
{
public:
    DB_KDZ(){}
    virtual ~DB_KDZ(){}
    virtual void init() = 0;//数据库初始化
    virtual void close() = 0;//数据库关闭
    virtual void exit(QString id) = 0;//退出账号
    virtual int login(QString id,QString pwd,QString _hostName) = 0;//登录账号
    virtual int logon(QString id,QString pwd,QString email,QString name) = 0;//注册账号
    virtual int find(QString id,QString newpwd,QString email) = 0;//找回密码
    virtual vector<QString> info(QString id) = 0;//返回账号信息
    virtual int query(QString id) = 0;//查询账号是否存在
    virtual vector<QString> allID() = 0;//返回所有账号
};

class DBQQ_KDZ: public DB_KDZ//QQ数据库
{
public:
    DBQQ_KDZ(){}
    virtual ~DBQQ_KDZ(){}
    void init();
    void close();
    void exit(QString id);
    int login(QString id,QString pwd,QString _hostName);
    int QLogin(QString id,QString _hostName);
    int wbLogin(QString id,QString pwd);
    int logon(QString id,QString pwd,QString email,QString name);
    int find(QString id,QString newpwd,QString email);
    QString selectLWC(QString id);//查找绑定微信
    QString selectLWB(QString id);//查找共享微博
    vector<QString> info(QString id);
    int update(QString id,QString name,int sex,QString area,QString birthday,QString signature);//更新账号信息
    int uniqueWC(QString id, QString wc);//判断绑定微信是否唯一
    int check(QString id,QString pwd);//判断账号ID与密码是否正确
    int query(QString id);
    int linkWC(QString id,QString wc);//绑定微信
    vector<QString> allID();
protected:
    QSqlDatabase db1;
};

class DBWC_KDZ: public DB_KDZ//微信数据库
{
public:
    DBWC_KDZ(){}
    virtual ~DBWC_KDZ(){}
    void init();
    void close();
    void exit(QString id);
    int login(QString id,QString pwd,QString _hostName);
    int logon(QString id,QString pwd,QString email,QString name);
    int find(QString id,QString newpwd,QString email);
    QString selectLQQ(QString id);//查找绑定QQ
    vector<QString> info(QString id);
    vector<QString> qLogin(QString _hostName);//微信快捷登录
    int update(QString id,QString name,int sex,QString area,QString birthday,QString signature);//更新账号信息
    int uniqueQQ(QString id, QString qq);//判断绑定QQ是否唯一
    int check(QString id,QString pwd);//判断账号ID与密码是否正确
    int query(QString id);
    int linkQQ(QString id,QString qq);//绑定QQ
    vector<QString> allID();
protected:
    QSqlDatabase db2;
};

class DBWB_KDZ: public DB_KDZ//微博数据库
{
public:
    DBWB_KDZ(){}
    virtual ~DBWB_KDZ(){}
    void init();
    void close();
    void exit(QString id);
    int login(QString id,QString pwd,QString _hostName);
    int logon(QString id,QString pwd,QString email,QString name);
    int find(QString id,QString newpwd,QString email);
    vector<QString> info(QString id);
    vector<QString> qLogin(QString _hostName);//微博快捷登录
    int update(QString id,QString name,int sex,QString area,QString birthday,QString signature);//更新账号信息
    int query(QString id);
    vector<QString> allID();
protected:
    QSqlDatabase db3;
};

#endif // DATABASE_H
