#ifndef ACCOUNT_H
#define ACCOUNT_H

#include<QCoreApplication>
#include<QString>
#include<Qdebug>
#include<QDir>
#include<QFile>
#include<vector>
#include<stdbool.h>

using namespace std;

class List_KDZ//列表类
{
public:
    List_KDZ()
    {}
    virtual ~List_KDZ(){}
    //返回信息
    int returnFriendNumber();//返回好友数量
    int returnGroupNumber();//返回群数量
    vector<QString> returnFriendList();//返回好友列表
    vector<QString> returnGroupList();//返回群列表
    vector<QString> returnNewFriendList();//返回好友申请列表
    vector<QString> returnNewGroupList();//返回群申请列表
    //修改信息
    int queryFriendList(QString str);//查询好友列表
    int addFriendList(QString str);//增加好友列表
    int deleteFriendList(QString str);//删除好友列表
    int queryGroupList(QString str);//查询群列表
    int addGroupList(QString str);//增加群列表
    int deleteGroupList(QString str);//删除群列表
    int queryNewFriendList(QString str);//查询好友申请列表
    int addNewFriendList(QString str);//增加好友申请列表
    int deleteNewFriendList(QString str);//删除好友申请列表
    int queryNewGroupList(QString str);//查询群申请列表
    int addNewGroupList(QString str);//增加群申请列表
    int deleteNewGroupList(QString str);//删除群申请列表
    //列表文件
    static void createListFile();//创建列表文件
    static int initListFile(QString id);//初始化列表文件
    static int removeListFile(QString id);//删除列表文件
    void readListFile(QString id);//读列表文件
    void writeListFile(QString id);//写列表文件
protected:
    vector<QString> friendList;//好友列表
    vector<QString> groupList;//群列表
    vector<QString> newFriendList;//好友申请列表
    vector<QString> newGroupList;//群申请列表
};

class Account_KDZ //帐号基类
{
public:
    Account_KDZ(QString _ID,QString _email,  QString _password, QString _name, int _sex = 0, QString _area = "", QString _birthday = "2000/1/1", QString _signature = "", QString _applyDate = "",int _status=0,QString _hostName="")
    :list()
    {
        ID = _ID;
        email = _email;
        password = _password;
        name = _name;
        sex = _sex;
        area = _area;
        birthday = _birthday;
        signature = _signature;
        applyDate = _applyDate;
        status=_status;
        hostName=_hostName;
    }
    virtual ~Account_KDZ() {}
    //返回账号信息
    virtual QString returnEmail() = 0;//返回账号的邮箱
    virtual QString returnID() = 0;//返回账号的ID
    virtual QString returnPassword() = 0;//返回账号的密码
    virtual QString returnName() = 0;//返回昵称
    virtual int returnSex() = 0;//返回性别
    virtual QString returnArea() = 0;//返回地区
    virtual QString returnBirthday() = 0;//返回生日
    virtual QString returnSignature() = 0;//返回个性签名
    virtual QString returnApplyDate() = 0;//返回注册时间
    virtual int returnStatus() = 0;//返回登陆状态
    virtual QString returnHostName() = 0;//返回主机名
    virtual List_KDZ* returnList() = 0;//返回列表的指针
    //修改账号信息
    virtual void changePassWord(QString newPassword) = 0;//修改密码
    virtual void changeName(QString newName) = 0;//修改昵称
    virtual void changeSex(int newSex) = 0;//修改性别
    virtual void changeArea(QString newArea) = 0;//修改地区
    virtual void changeBirthday(QString newBirthday) = 0;//修改生日
    virtual void changeSignature(QString newSignature) = 0;//修改个性签名
    virtual void changeStatus(int newStatus) = 0;//修改登陆状态
    virtual void changeHostName(QString newName) = 0;//修改主机名
protected:
    QString ID;//ID
    QString email;//邮箱
    QString password;//密码
    QString name;//昵称
    int sex;//性别:(0,女),(1,男),(3,其他)
    QString area;//地区
    QString birthday;//生日
    QString signature;//个性签名
    QString applyDate;//注册日期
    int status;//登录状态:(0,退出),(1，登录)
    QString hostName;//主机名
    List_KDZ list;//好友&群列表
};

class QQ_KDZ : public Account_KDZ //QQ类
{
public:
    QQ_KDZ(QString _id, QString _email, QString _password, QString _name, int _sex = 0, QString _area = "", QString _birthday = "2000/1/1", QString _signature = "", QString _applyDate = "", QString _linkedWechat = "", QString _sharedWeibo = "",int _status=0,QString _hostName="")
        :Account_KDZ( _id,_email, _password, _name, _sex, _area, _birthday, _signature, _applyDate,_status,_hostName)
    {
        linkedWeChat = _linkedWechat;
        sharedWeibo = _sharedWeibo;
    }
    virtual ~QQ_KDZ() {}
    //返回QQ账号信息
    QString returnEmail();
    QString returnID();
    QString returnPassword();
    QString returnName();
    int returnSex();
    QString returnArea();
    QString returnBirthday();
    QString returnSignature();
    QString returnApplyDate();
    int returnStatus();
    QString returnHostName();
    List_KDZ* returnList();
    QString returnLinkedWeChat();//返回绑定微信
    QString returnSharedWeibo();//返回共享微博
    //修改QQ账号信息
    void changePassWord(QString newPassword);
    void changeName(QString newName);
    void changeSex(int newSex);
    void changeArea(QString newArea);
    void changeBirthday(QString newBirthday);
    void changeSignature(QString newSignature);
    void changeStatus(int newStatus);
    void changeHostName(QString newName);
    void changeLinkedWeChat(QString newLinkedWeChat);//修改绑定微信
protected:
    QString linkedWeChat;//绑定微信
    QString sharedWeibo;//共享微博
};

class WeChat_KDZ : public Account_KDZ //微信类
{
public:
    WeChat_KDZ(QString _id, QString _email, QString _password, QString _name, int _sex = 0, QString _area = "", QString _birthday = "2000/1/1", QString _signature = "", QString _applyDate = "", QString _linkedQQ = "",int _status=0,QString _hostName="")
        :Account_KDZ( _id,_email, _password, _name, _sex, _area, _birthday, _signature, _applyDate,_status,_hostName)
    {
        linkedQQ = _linkedQQ;
    }
    virtual ~WeChat_KDZ(){}
    //返回微信账号信息
    QString returnEmail();
    QString returnID();
    QString returnPassword();
    QString returnName();
    int returnSex();
    QString returnArea();
    QString returnBirthday();
    QString returnSignature();
    QString returnApplyDate();
    int returnStatus();
    QString returnHostName();
    List_KDZ* returnList();
    QString returnLinkedQQ();//返回绑定QQ
    //修改微信账号信息
    void changePassWord(QString newPassword);
    void changeName(QString newName);
    void changeSex(int newSex);
    void changeArea(QString newArea);
    void changeBirthday(QString newBirthday);
    void changeSignature(QString newSignature);
    void changeStatus(int newStatus);
    void changeHostName(QString newName);
    void changeLinkedQQ(QString newLinkedQQ);//修改绑定QQ
protected:
    QString linkedQQ;//绑定QQ
};

class Weibo_KDZ : public Account_KDZ //微博类
{
public:
    Weibo_KDZ( QString _id,QString _email, QString _password, QString _name, int _sex = 0, QString _area = "", QString _birthday = "2000/1/1", QString _signature = "", QString _applyDate = "",QString _sharedQQ = "",int _status=0,QString _hostName="")
        :Account_KDZ(_id,_email,  _password, _name, _sex, _area, _birthday, _signature, _applyDate,_status,_hostName)
    {
        sharedQQ=_sharedQQ;
    }
    virtual ~Weibo_KDZ() {}
    //返回微博账号信息
    QString returnEmail();
    QString returnID();
    QString returnPassword();
    QString returnName();
    int returnSex();
    QString returnArea();
    QString returnBirthday();
    QString returnSignature();
    QString returnApplyDate();
    QString returnSharedQQ();
    int returnStatus();
    QString returnHostName();
    List_KDZ* returnList();
    void changeLinkedWeChat(QString newSharedQQ);//修改共享QQ
    //修改微博账号信息
    void changePassWord(QString newPassword);
    void changeName(QString newName);
    void changeSex(int newSex);
    void changeArea(QString newArea);
    void changeBirthday(QString newBirthday);
    void changeSignature(QString newSignature);
    void changeStatus(int newStatus);
    void changeHostName(QString newName);
protected:
    QString sharedQQ;//共享QQ
};

#endif // ACCOUNT_H
