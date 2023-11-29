#ifndef GROUP_H
#define GROUP_H

#include "account.h"

#include <QCoreApplication>
#include <QString>
#include <Qdebug>
#include <QDir>
#include <QFile>
#include <vector>

class Group_KDZ //群
{
public:
    Group_KDZ(QString _groupID, QString _groupName, QString _groupCreater)
    {
        groupID=_groupID;
        groupName=_groupName;
        groupCreater=_groupCreater;
    }
    virtual ~Group_KDZ(){}
    virtual QString returnGroupID() = 0;//返回群号
    virtual QString returnGroupName() = 0;//返回群名
    virtual QString returnGroupCreater() = 0;//返回群主
    virtual vector<QString> returnGroupMember() = 0;//返回群成员列表
    virtual vector<QString> returnNewMember() = 0;//返回申请成员列表
    virtual int changeGroupName(QString str,QString newName) = 0;//修改群名
    virtual int queryMember(QString mem) = 0;//查询成员
    virtual int inviteMember(QString mem) = 0;//邀请成员
    virtual int addMember(QString mem) = 0;//添加成员
    virtual int deleteMember(QString str,QString mem) = 0;//删除成员
    virtual int sendInvite(QString mem) = 0;//发送邀请
    virtual int accInvite(QString str,QString mem) = 0;//处理邀请
    virtual int queryNew(QString New) = 0;//查询申请成员
    virtual int initGroupFile() = 0;//初始化群文件
    virtual void writeGroupFile() = 0;//更新群文件
    virtual int removeGroupFile(QString str) = 0;//删除群文件
    virtual void readGroupFile() = 0;//读取文件
    virtual int searchFlag() = 0;//查找标记文件
protected:
    QString groupID;//群号
    QString groupName;//群名称
    QString groupCreater;//群主
    vector<QString> groupMember;//群成员列表
    vector<QString> newMember;//申请成员列表
};

class QQGroup_KDZ : public Group_KDZ //QQ群
{
public:
    QQGroup_KDZ(QString _groupID, QString _groupName, QString _groupCreater):Group_KDZ(_groupID,_groupName,_groupCreater)
    {}
    virtual ~QQGroup_KDZ(){}
    QString returnGroupID();
    QString returnGroupName();
    QString returnGroupCreater();
    vector<QString> returnGroupMember();
    vector<QString> returnGroupAdmin();
    vector<QString> returnNewMember();
    int changeGroupName(QString str,QString newName);
    int queryAdmin(QString ad);//查询管理员
    int addAdmin(QString str,QString ad);//添加管理员
    int deleteAdmin(QString str,QString ad);//删除管理员
    int queryMember(QString mem);
    int inviteMember(QString mem);
    int addMember(QString New);
    int deleteMember(QString str,QString mem);
    int sendInvite(QString mem);
    int accInvite(QString str,QString mem);
    int queryNew(QString New);
    int initGroupFile();
    void writeGroupFile();
    int removeGroupFile(QString str);
    static void createGroupFile();//创建群文件
    void updateAdmin(vector<QString> str);//更新管理员列表
    void updateMember(vector<QString> str);//更新成员列表
    void updateNew(vector<QString> str);//更新申请成员列表
    void readGroupFile();
    int searchFlag();
protected:
    vector<QString> groupAdmin;//管理员
};

class WeChatGroup_KDZ : public Group_KDZ //微信群
{
public:
    WeChatGroup_KDZ(QString _groupID, QString _groupName, QString _groupCreater):Group_KDZ(_groupID,_groupName,_groupCreater)
    {}
    virtual ~WeChatGroup_KDZ(){}
    QString returnGroupID();
    QString returnGroupName();
    QString returnGroupCreater();
    vector<QString> returnGroupMember();
    vector<QString> returnNewMember();
    int changeGroupName(QString str,QString newName);
    int queryMember(QString mem);
    int inviteMember(QString mem);
    int addMember(QString New);
    int deleteMember(QString str,QString mem);
    int sendInvite(QString mem);
    int accInvite(QString str,QString mem);
    int queryNew(QString New);
    int initGroupFile();
    void writeGroupFile();
    int removeGroupFile(QString str);
    static void createGroupFile();//创建群文件
    void updateMember(vector<QString> str);
    void updateNew(vector<QString> str);
    void readGroupFile();
    int searchFlag();
};

#endif // GROUP_H
