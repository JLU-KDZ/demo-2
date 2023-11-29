#include "database.h"

using namespace std;

void DBQQ_KDZ::init()
{
    //创建数据库
    db1 = QSqlDatabase::addDatabase("QSQLITE","connection1");
    QString dataPath1 = QCoreApplication::applicationDirPath()+"/QQ.db";
    db1.setDatabaseName(dataPath1);
    db1.open();//打开
    if(db1.open())
    {
       qDebug()<<"open QQ suc";
    }else
    {
       qDebug()<<"open QQ fail";
    }

    //创建数据表
    QString cmd="create table if not exists QQ(ID varchar(64) unique,password varchar(64),email varchar(64),name varchar(64),sex int,area varchar(64),birthday varchar(64),signature varchar(128),applyDate varchar(64),status int,hostName varchar(64),linkedWeChat varchar(64),sharedWeibo varchar(64))";
    QSqlQuery query(db1);
    query.prepare(cmd);
    if (query.exec(cmd)){//运行
        qDebug()<<"exec QQ suc";
    }
    else{
        qDebug()<<"exec QQ fail";
    }
}

void DBQQ_KDZ::close()
{
    db1.close();
}

void DBQQ_KDZ::exit(QString id)
{
    //登录状态设置为0
    QString cmd=QString("update QQ set status = '%1' where ID = '%2'").arg(0).arg(id);
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return;
    }
    else
    {
        qDebug()<<query.lastError();
        return;
    }
}

int DBQQ_KDZ::logon(QString id,QString pwd,QString email,QString name)
{
    //查找ID是否已存在
    QString select1=QString ("select* from QQ where ID = '%1'").arg(id);
    QSqlQuery query1(db1);
    query1.prepare(select1);
    if(!query1.exec())
    {
        qDebug()<<query1.lastError();
        return 2;
    }
    else if(query1.next())
    {
        return 1;
    }

    //将注册信息插入数据库
    QDate date=QDate::currentDate();
    QString dateStr=date.toString("yyyy/M/d");
    QString cmd=QString("insert into QQ values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13')").arg(id).arg(pwd).arg(email).arg(name).arg(0).arg("").arg("2000/1/1").arg("").arg(dateStr).arg(0).arg("").arg("").arg("");
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return 0;
    }
    else
    {
        qDebug()<<query.lastError();
        return 2;
    }
}

int DBQQ_KDZ::find(QString id, QString newpwd, QString email)
{
    //查找ID是否已存在
    QString selectSql=QString ("select* from QQ where ID = '%1'").arg(id);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 3;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            //判断email是否一致
            QString _email=querySql.value(2).toString();
            if(_email!=email)
            {
                return 2;
            }
            else
            {
                //更新密码
                QString cmd=QString("update QQ set password = '%1' where ID = '%2' and email = '%3'").arg(newpwd).arg(id).arg(email);
                QSqlQuery query(db1);
                query.prepare(cmd);
                if(!query.exec(cmd))
                {
                    qDebug()<<querySql.lastError();
                    return 3;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

QString DBQQ_KDZ::selectLWC(QString id)
{
    //查找绑定微信并返回
    QString ans;
    QString cmd=QString ("select linkedWeChat from QQ where ID = '%1'").arg(id);
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        if(!query.next())
        {
            return ans;
        }
        else
        {
            ans=query.value(0).toString();
            return ans;
        }
    }
}

QString DBQQ_KDZ::selectLWB(QString id)
{
    //查找共享微博并返回
    QString ans;
    QString cmd=QString ("select sharedWeiBo from QQ where ID = '%1'").arg(id);
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        if(!query.next())
        {
            return ans;
        }
        else
        {
            ans=query.value(0).toString();
            return ans;
        }
    }
}

vector<QString> DBQQ_KDZ::info(QString id)
{
    //查找账号信息
    vector<QString> ans;
    QString selectSql=QString ("select* from QQ where ID = '%1'").arg(id);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return ans;
    }
    else
    {
        if(!querySql.next())
        {
            return ans;
        }
        else
        {
            for(int i=0;i<13;i++)
            {
                //性别整型转化为字符串
                if(i==4)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("女"));
                    }
                    else if(querySql.value(i).toInt()==1)
                    {
                        ans.push_back(QString("男"));
                    }
                    else
                    {
                        ans.push_back(QString("其他"));
                    }
                }
                //登录状态整型转化为字符串
                else if(i==9)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("退出"));
                    }
                    else
                    {
                        ans.push_back(QString("登录"));
                    }
                }
                else
                {
                    ans.push_back(querySql.value(i).toString());
                }
            }
            return ans;
        }
    }
}

int DBQQ_KDZ::update(QString id,QString name, int sex, QString area, QString birthday, QString signature)
{
    //更新账号信息
    QString selectSql=QString ("select* from QQ where ID = '%1'").arg(id);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString cmd=QString("update QQ set name = '%1' , sex = '%2' , area = '%3' , birthday = '%4' , signature = '%5' where ID = '%6'").arg(name).arg(sex).arg(area).arg(birthday).arg(signature).arg(id);
            QSqlQuery query(db1);
            query.prepare(cmd);
            if(!query.exec(cmd))
            {
                qDebug()<<querySql.lastError();
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
}

int DBQQ_KDZ::uniqueWC(QString id ,QString wc)
{
    //查找绑定微信
    QString selectSql=QString ("select ID from QQ where linkedWeChat = '%1'").arg(wc);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 3;
    }
    else
    {
        if(!querySql.next())
        {
            return 0;//未绑定
        }
        else
        {
            if(querySql.value(0).toString()==id)
                return 1;//已与当前id绑定
            else
                return 2;//已与其他id绑定
        }
    }
}

int DBQQ_KDZ::check(QString id, QString pwd)
{
    //检查账号和密码是否正确
    QString selectSql=QString ("select* from QQ where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
            return 0;
    }
}

int DBQQ_KDZ::query(QString id)
{
    QString selectSql=QString ("select* from QQ where ID = '%1'").arg(id);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;//数据库运行失败
    }
    else
    {
        if(!querySql.next())
        {
            return 1;//账号不存在
        }
        else
            return 0;
    }
}

int DBQQ_KDZ::linkWC(QString id, QString wc)
{
    //设置绑定微信
    QString cmd=QString("update QQ set linkedWeChat = '%1' where ID = '%2'").arg(wc).arg(id);
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(!query.exec(cmd))
    {
        qDebug()<<query.lastError();
        return 1;
    }
    else
    {
        return 0;
    }
}

vector<QString> DBQQ_KDZ::allID()
{
    vector<QString> ans;
    QString cmd=QString("select ID from QQ");
    QSqlQuery query(db1);
    query.prepare(cmd);
    if(!query.exec(cmd))
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        while(query.next())
        {
            ans.push_back(query.value(0).toString());
        }
        return ans;
    }
}

int DBQQ_KDZ::login(QString id, QString pwd,QString _hostName)
{
    //检查账号和密码是否正确
    QString selectSql=QString ("select* from QQ where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {

        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            //检查是否已登录
            QString sta=QString ("select status from QQ where ID = '%1'").arg(id);
            QSqlQuery querysta(db1);
            querysta.prepare(sta);
            if(!querysta.exec())
            {
                qDebug()<<querysta.lastError();
                return 2;
            }
            else
            {
                if(querysta.next())
                {
                    if(querysta.value(0).toInt()==1)
                    {
                        return 3;//已登陆
                    }
                    else
                    {
                        //设置登录状态并绑定主机名
                        QString cmd=QString("update QQ set status = '%1' , hostName = '%2' where ID = '%3'").arg(1).arg(_hostName).arg(id);
                        QSqlQuery query(db1);
                        query.prepare(cmd);
                        if(!query.exec(cmd))
                        {
                            qDebug()<<query.lastError();
                            return 2;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }

        }
    }
}

int DBQQ_KDZ::QLogin(QString id, QString _hostName)
{
    //检查是否已登录
    QString sta=QString ("select status from QQ where ID = '%1'").arg(id);
    QSqlQuery querysta(db1);
    querysta.prepare(sta);
    if(!querysta.exec())
    {
        qDebug()<<querysta.lastError();
        return 2;
    }
    else
    {
        if(querysta.next())
        {
            if(querysta.value(0).toInt()==1)
            {
                return 3;//已登陆
            }
            else
            {
                //设置登录状态并绑定主机名
                QString cmd=QString("update QQ set status = '%1' , hostName = '%2' where ID = '%3'").arg(1).arg(_hostName).arg(id);
                QSqlQuery query(db1);
                query.prepare(cmd);
                if(!query.exec(cmd))
                {
                    qDebug()<<query.lastError();
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 1;//账号不存在
        }
    }
}

int DBQQ_KDZ::wbLogin(QString id, QString pwd)
{
    //检查账号和密码是否正确
    QString selectSql=QString("select* from QQ where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db1);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;//密码错误
        }
        else
        {
            return 0;//密码正确
        }
    }
}

void DBWC_KDZ::init()
{
    db2 = QSqlDatabase::addDatabase("QSQLITE","connection2");
    QString dataPath2 = QCoreApplication::applicationDirPath()+"/WC.db";
    db2.setDatabaseName(dataPath2);
    db2.open();//打开
    if(db2.open())
    {
       qDebug()<<"open WC suc";

    }else
    {
       qDebug()<<"open WC fail";
    }

    QString cmd="create table if not exists WC(ID varchar(64) unique,password varchar(64),email varchar(64),name varchar(64),sex int,area varchar(64),birthday varchar(64),signature varchar(128),applyDate varchar(64),status int,hostName varchar(64),linkedQQ varchar(64))";
    QSqlQuery query(db2);
    if (query.exec(cmd)){//运行
        qDebug()<<"exec WC suc";
    }
    else{
        qDebug()<<"exec WC fail";
    }
}

void DBWC_KDZ::close()
{
    db2.close();
}

void DBWC_KDZ::exit(QString id)
{
    QString cmd=QString("update WC set status = '%1' where ID = '%2'").arg(0).arg(id);
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return;
    }
    else
    {
        qDebug()<<query.lastError();
        return;
    }
}

int DBWC_KDZ::login(QString id, QString pwd, QString _hostName)
{
    QString selectSql=QString ("select* from WC where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {

        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString sta=QString ("select status from WC where ID = '%1'").arg(id);
            QSqlQuery querysta(db2);
            querysta.prepare(sta);
            if(!querysta.exec())
            {
                qDebug()<<querysta.lastError();
                return 2;
            }
            else
            {
                if(querysta.next())
                {
                    if(querysta.value(0).toInt()==1)
                    {
                        return 3;
                    }
                    else
                    {
                        QString cmd=QString("update WC set status = '%1' , hostName = '%2' where ID = '%3'").arg(1).arg(_hostName).arg(id);
                        QSqlQuery query(db2);
                        query.prepare(cmd);
                        if(!query.exec(cmd))
                        {
                            qDebug()<<query.lastError();
                            return 2;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

int DBWC_KDZ::logon(QString id, QString pwd, QString email, QString name)
{
    QString select1=QString ("select* from WC where ID = '%1'").arg(id);
    QSqlQuery query1(db2);
    query1.prepare(select1);
    if(!query1.exec())
    {
        qDebug()<<query1.lastError();
        return 2;
    }
    else if(query1.next())
    {
        return 1;
    }

    QDate date=QDate::currentDate();
    QString dateStr=date.toString("yyyy/M/d");
    QString cmd=QString("insert into WC values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12')").arg(id).arg(pwd).arg(email).arg(name).arg(0).arg("").arg("2000/1/1").arg("").arg(dateStr).arg(0).arg("").arg("");
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return 0;
    }
    else
    {
        qDebug()<<query.lastError();
        return 2;
    }
}

int DBWC_KDZ::find(QString id, QString newpwd, QString email)
{
    QString selectSql=QString ("select* from WC where ID = '%1'").arg(id);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 3;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString _email=querySql.value(2).toString();
            if(_email!=email)
            {
                return 2;
            }
            else
            {
                QString cmd=QString("update WC set password = '%1' where ID = '%2' and email = '%3'").arg(newpwd).arg(id).arg(email);
                QSqlQuery query(db2);
                query.prepare(cmd);
                if(!query.exec(cmd))
                {
                    qDebug()<<querySql.lastError();
                    return 3;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

QString DBWC_KDZ::selectLQQ(QString id)
{
    QString ans;
    QString cmd=QString ("select linkedQQ from WC where ID = '%1'").arg(id);
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        if(!query.next())
        {
            return ans;
        }
        else
        {
            ans=query.value(0).toString();
            return ans;
        }
    }
}

vector<QString> DBWC_KDZ::info(QString id)
{
    vector<QString> ans;
    QString selectSql=QString ("select* from WC where ID = '%1'").arg(id);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return ans;
    }
    else
    {
        if(!querySql.next())
        {
            return ans;
        }
        else
        {
            for(int i=0;i<12;i++)
            {
                if(i==4)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("女"));
                    }
                    else if(querySql.value(i).toInt()==1)
                    {
                        ans.push_back(QString("男"));
                    }
                    else
                    {
                        ans.push_back(QString("其他"));
                    }
                }
                else if(i==9)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("退出"));
                    }
                    else
                    {
                        ans.push_back(QString("登录"));
                    }
                }
                else
                {
                    ans.push_back(querySql.value(i).toString());
                }
            }
            return ans;
        }
    }
}

vector<QString> DBWC_KDZ::qLogin(QString _hostName)
{
    vector<QString> ans;
    QString cmd=QString ("select ID from WC where status = '%1' and hostName = '%2'").arg(1).arg(_hostName);
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        while(query.next())
        {
            ans.push_back(query.value(0).toString());
        }
        return ans;
    }
}

int DBWC_KDZ::update(QString id, QString name, int sex, QString area, QString birthday, QString signature)
{
    QString selectSql=QString ("select* from WC where ID = '%1'").arg(id);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString cmd=QString("update WC set name = '%1' , sex = '%2' , area = '%3' , birthday = '%4' , signature = '%5' where ID = '%6'").arg(name).arg(sex).arg(area).arg(birthday).arg(signature).arg(id);
            QSqlQuery query(db2);
            query.prepare(cmd);
            if(!query.exec(cmd))
            {
                qDebug()<<querySql.lastError();
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
}

int DBWC_KDZ::uniqueQQ(QString id,QString qq)
{
    QString selectSql=QString ("select ID from WC where linkedQQ = '%1'").arg(qq);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 3;
    }
    else
    {
        if(!querySql.next())
        {
            return 0;
        }
        else
        {
            if(querySql.value(0).toString()==id)
                return 1;
            else
                return 2;
        }
    }
}

int DBWC_KDZ::check(QString id, QString pwd)
{
    QString selectSql=QString ("select* from WC where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
            return 0;
    }
}

int DBWC_KDZ::query(QString id)
{
    QString selectSql=QString ("select* from WC where ID = '%1'").arg(id);
    QSqlQuery querySql(db2);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
            return 0;
    }
}

int DBWC_KDZ::linkQQ(QString id, QString qq)
{
    QString cmd=QString("update WC set linkedQQ = '%1' where ID = '%2'").arg(qq).arg(id);
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(!query.exec(cmd))
    {
        qDebug()<<query.lastError();
        return 1;
    }
    else
    {
        return 0;
    }
}

vector<QString> DBWC_KDZ::allID()
{
    vector<QString> ans;
    QString cmd=QString("select ID from WC");
    QSqlQuery query(db2);
    query.prepare(cmd);
    if(!query.exec(cmd))
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        while(query.next())
        {
            ans.push_back(query.value(0).toString());
        }
        return ans;
    }
}

void DBWB_KDZ::init()
{
    db3 = QSqlDatabase::addDatabase("QSQLITE","connection3");
    QString dataPath3 = QCoreApplication::applicationDirPath()+"/WB.db";
    db3.setDatabaseName(dataPath3);
    db3.open();//打开
    if(db3.open())
    {
       qDebug()<<"open WB suc";

    }else
    {
       qDebug()<<"open WB fail";
    }

    QString cmd="create table if not exists WB(ID varchar(64) unique,password varchar(64),email varchar(64),name varchar(64),sex int,area varchar(64),birthday varchar(64),signature varchar(128),applyDate varchar(64),status int,hostName varchar(64),sharedQQ varchar(64))";
    QSqlQuery query(db3);
    query.prepare(cmd);
    if (query.exec(cmd)){//运行
        qDebug()<<"exec WB suc";
    }
    else{
        qDebug()<<"exec WB fail";
    }
}

void DBWB_KDZ::close()
{
    db3.close();
}

void DBWB_KDZ::exit(QString id)
{
    QString cmd=QString("update WB set status = '%1' where ID = '%2'").arg(0).arg(id);
    QSqlQuery query(db3);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return;
    }
    else
    {
        qDebug()<<query.lastError();
        return;
    }
}

int DBWB_KDZ::login(QString id, QString pwd,QString _hostName)
{
    QString selectSql=QString ("select* from WB where ID = '%1' and password = '%2'").arg(id).arg(pwd);
    QSqlQuery querySql(db3);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {

        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString sta=QString ("select status from WB where ID = '%1'").arg(id);
            QSqlQuery querysta(db3);
            querysta.prepare(sta);
            if(!querysta.exec())
            {
                qDebug()<<querysta.lastError();
                return 2;
            }
            else
            {
                if(querysta.next())
                {
                    if(querysta.value(0).toInt()==1)
                    {
                        return 3;
                    }
                    else
                    {
                        QString cmd=QString("update WB set status = '%1' , hostName = '%2' where ID = '%3'").arg(1).arg(_hostName).arg(id);
                        QSqlQuery query(db3);
                        query.prepare(cmd);
                        if(!query.exec(cmd))
                        {
                            qDebug()<<query.lastError();
                            return 2;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

int DBWB_KDZ::logon(QString id, QString pwd, QString email, QString name)
{
    QString select1=QString ("select* from WB where ID = '%1'").arg(id);
    QSqlQuery query1(db3);
    query1.prepare(select1);
    if(!query1.exec())
    {
        qDebug()<<query1.lastError();
        return 2;
    }
    else if(query1.next())
    {
        return 1;
    }

    QDate date=QDate::currentDate();
    QString dateStr=date.toString("yyyy/M/d");

    QString cmd=QString("insert into WB values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12')").arg(id).arg(pwd).arg(email).arg(name).arg(0).arg("").arg("2000/1/1").arg("").arg(dateStr).arg(0).arg("").arg("");
    QSqlQuery query(db3);
    query.prepare(cmd);
    if(query.exec(cmd))
    {
        return 0;
    }
    else
    {
        qDebug()<<query.lastError();
        return 2;
    }
}

int DBWB_KDZ::find(QString id, QString newpwd, QString email)
{
    QString selectSql=QString ("select* from WB where ID = '%1'").arg(id);
    QSqlQuery querySql(db3);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 3;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString _email=querySql.value(2).toString();
            if(_email!=email)
            {
                return 2;
            }
            else
            {
                QString cmd=QString("update WB set password = '%1' where ID = '%2' and email = '%3'").arg(newpwd).arg(id).arg(email);
                QSqlQuery query(db3);
                query.prepare(cmd);
                if(!query.exec(cmd))
                {
                    qDebug()<<querySql.lastError();
                    return 3;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

vector<QString> DBWB_KDZ::info(QString id)
{
    vector<QString> ans;
    QString selectSql=QString ("select* from WB where ID = '%1'").arg(id);
    QSqlQuery querySql(db3);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return ans;
    }
    else
    {
        if(!querySql.next())
        {
            return ans;
        }
        else
        {
            for(int i=0;i<10;i++)
            {
                if(i==4)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("女"));
                    }
                    else if(querySql.value(i).toInt()==1)
                    {
                        ans.push_back(QString("男"));
                    }
                    else
                    {
                        ans.push_back(QString("其他"));
                    }
                }
                else if(i==9)
                {
                    if(querySql.value(i).toInt()==0)
                    {
                        ans.push_back(QString("退出"));
                    }
                    else
                    {
                        ans.push_back(QString("登录"));
                    }
                }
                else
                {
                    ans.push_back(querySql.value(i).toString());
                }
            }
            return ans;
        }
    }
}

vector<QString> DBWB_KDZ::qLogin(QString _hostName)
{
    vector<QString> ans;
    QString cmd=QString ("select ID from WB where status = '%1' and hostName = '%2'").arg(1).arg(_hostName);
    QSqlQuery query(db3);
    query.prepare(cmd);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        while(query.next())
        {
            ans.push_back(query.value(0).toString());
        }
        return ans;
    }
}

int DBWB_KDZ::update(QString id, QString name, int sex, QString area, QString birthday, QString signature)
{
    QString selectSql=QString ("select* from WB where ID = '%1'").arg(id);
    QSqlQuery querySql(db3);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
        {
            QString cmd=QString("update WB set name = '%1' , sex = '%2' , area = '%3' , birthday = '%4' , signature = '%5' where ID = '%6'").arg(name).arg(sex).arg(area).arg(birthday).arg(signature).arg(id);
            QSqlQuery query(db3);
            query.prepare(cmd);
            if(!query.exec(cmd))
            {
                qDebug()<<querySql.lastError();
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
}

int DBWB_KDZ::query(QString id)
{
    QString selectSql=QString ("select* from WB where ID = '%1'").arg(id);
    QSqlQuery querySql(db3);
    querySql.prepare(selectSql);
    if(!querySql.exec())
    {
        qDebug()<<querySql.lastError();
        return 2;
    }
    else
    {
        if(!querySql.next())
        {
            return 1;
        }
        else
            return 0;
    }
}

vector<QString> DBWB_KDZ::allID()
{
    vector<QString> ans;
    QString cmd=QString("select ID from WB");
    QSqlQuery query(db3);
    query.prepare(cmd);
    if(!query.exec(cmd))
    {
        qDebug()<<query.lastError();
        return ans;
    }
    else
    {
        while(query.next())
        {
            ans.push_back(query.value(0).toString());
        }
        return ans;
    }
}
