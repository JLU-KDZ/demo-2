#include "account.h"

//List_KDZ
int List_KDZ::returnFriendNumber(){return (int)friendList.size();}
int List_KDZ::returnGroupNumber(){return (int)groupList.size();}
vector<QString> List_KDZ::returnFriendList(){return friendList;}
vector<QString> List_KDZ::returnGroupList(){return groupList;}
vector<QString> List_KDZ::returnNewFriendList(){return newFriendList;}
vector<QString> List_KDZ::returnNewGroupList(){return newGroupList;}

int List_KDZ::queryFriendList(QString str)
{
    for(int i=0;i<(int)friendList.size();i++)
        if(friendList[i]==str)
            return i;
    return -1;
}

int List_KDZ::addFriendList(QString str)
{
    if(queryFriendList(str)==-1)
    {
        friendList.push_back(str);
        return 0;
    }
    return -1;//成员已存在
}

int List_KDZ::deleteFriendList(QString str)
{
    int pos=queryFriendList(str);
    if(pos!=-1)
    {
        friendList.erase(friendList.begin()+pos);
        return 0;
    }
    return -1;//成员不存在
}

int List_KDZ::queryGroupList(QString str)
{
    for(int i=0;i<(int)groupList.size();i++)
        if(groupList[i]==str)
            return i;
    return -1;
}

int List_KDZ::addGroupList(QString str)
{
    if(queryGroupList(str)==-1)
    {
        groupList.push_back(str);
        return 0;
    }
    return -1;//群已存在
}

int List_KDZ::deleteGroupList(QString str)
{
    int pos=queryGroupList(str);
    if(pos!=-1)
    {
        groupList.erase(groupList.begin()+pos);
        return 0;
    }
    return -1;//群不存在
}

int List_KDZ::queryNewFriendList(QString str)
{
    for(int i=0;i<(int)newFriendList.size();i++)
        if(newFriendList[i]==str)
            return i;
    return -1;
}
int List_KDZ::addNewFriendList(QString str)
{
    if(queryNewFriendList(str)==-1)
    {
        newFriendList.push_back(str);
        return 0;
    }
    return -1;
}

int List_KDZ::deleteNewFriendList(QString str)
{
    int pos=queryNewFriendList(str);
    if(pos!=-1)
    {
        newFriendList.erase(newFriendList.begin()+pos);
        return 0;
    }
    return -1;
}

int List_KDZ::queryNewGroupList(QString str)
{
    for(int i=0;i<(int)newGroupList.size();i++)
        if(newGroupList[i]==str)
            return i;
    return -1;
}

int List_KDZ::addNewGroupList(QString str)
{
    if(queryNewGroupList(str)==-1)
    {
        newGroupList.push_back(str);
        return 0;
    }
    return -1;
}

int List_KDZ::deleteNewGroupList(QString str)
{
    int pos=queryNewGroupList(str);
    if(pos!=-1)
    {
        newGroupList.erase(newGroupList.begin()+pos);
        return 0;
    }
    return -1;
}

void List_KDZ::createListFile()
{
    QDir *list = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/List";
    if(!list->exists(path))
    {
        list->mkdir(path);
    }
}

int List_KDZ::initListFile(QString id)
{
    QDir *list = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/List/"+id+"";
    if(list->exists(path))
    {
        return 1;//文件已存在
    }
    else
    {
        list->mkdir(path);
    }

    QFile file1(QCoreApplication::applicationDirPath()+"/List/"+id+"/friendList.txt");
    if(file1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file1.close();
    }

    QFile file2(QCoreApplication::applicationDirPath()+"/List/"+id+"/groupList.txt");
    if(file2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file2.close();
    }

    QFile file3(QCoreApplication::applicationDirPath()+"/List/"+id+"/newFriendList.txt");
    if(file3.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file3.close();
    }

    QFile file4(QCoreApplication::applicationDirPath()+"/List/"+id+"/newGroupList.txt");
    if(file4.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file4.close();
    }
    return 0;
}

int List_KDZ::removeListFile(QString id)
{
    QString path = QCoreApplication::applicationDirPath()+"/List/"+id+"";

    //路径是否为空
    //路径是否存在
    if(path.isEmpty()||!QDir(path).exists())
    {
        return 1;
    }
    else
    {
        QDir qDir1(path+"/friendList.txt");
        qDir1.removeRecursively();
        QDir qDir2(path+"/groupList.txt");
        qDir2.removeRecursively();
        QDir qDir3(path+"/newFriendList.txt");
        qDir3.removeRecursively();
        QDir qDir4(path+"/newGroupList.txt");
        qDir4.removeRecursively();
    }
    return 0;
}

void List_KDZ::readListFile(QString id)
{
    vector<QString> friendL;
    vector<QString> groupL;
    vector<QString> newFriendL;
    vector<QString> newGroupL;

    //读取文件
    QFile file1(QCoreApplication::applicationDirPath()+"/List/"+id+"/friendList.txt");
    if(file1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file1);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            friendL.push_back(line);
        }
        file1.close();
    }

    QFile file2(QCoreApplication::applicationDirPath()+"/List/"+id+"/groupList.txt");
    if(file2.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file2);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            groupL.push_back(line);
        }
        file2.close();
    }

    QFile file3(QCoreApplication::applicationDirPath()+"/List/"+id+"/newFriendList.txt");
    if(file3.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file3);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            newFriendL.push_back(line);
        }
        file3.close();
    }

    QFile file4(QCoreApplication::applicationDirPath()+"/List/"+id+"/newGroupList.txt");
    if(file4.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file4);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            newGroupL.push_back(line);
        }
        file4.close();
    }
    friendList=friendL;
    groupList=groupL;
    newFriendList=newFriendL;
    newGroupList=newGroupL;
}

void List_KDZ::writeListFile(QString id)
{
    //写入文件
    QFile file1(QCoreApplication::applicationDirPath()+"/List/"+id+"/friendList.txt");
    if(file1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file1);
        for(int i=0;i<(int)friendList.size();i++)
        {
            stream<<friendList[i]<<"\r\n";
        }
        file1.close();
    }

    QFile file2(QCoreApplication::applicationDirPath()+"/List/"+id+"/groupList.txt");
    if(file2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file2);
        for(int i=0;i<(int)groupList.size();i++)
        {
            stream<<groupList[i]<<"\r\n";
        }
        file2.close();
    }

    QFile file3(QCoreApplication::applicationDirPath()+"/List/"+id+"/newFriendList.txt");
    if(file3.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file3);
        for(int i=0;i<(int)newFriendList.size();i++)
        {
            stream<<newFriendList[i]<<"\r\n";
        }
        file3.close();
    }

    QFile file4(QCoreApplication::applicationDirPath()+"/List/"+id+"/newGroupList.txt");
    if(file4.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file4);
        for(int i=0;i<(int)newGroupList.size();i++)
        {
            stream<<newGroupList[i]<<"\r\n";
        }
        file4.close();
    }
}

//QQ_KDZ
QString QQ_KDZ::returnEmail(){return email;}
QString QQ_KDZ::returnID(){return ID;}
QString QQ_KDZ::returnPassword(){return password;}
QString QQ_KDZ::returnName(){return name;}
int QQ_KDZ::returnSex(){return sex;}
QString QQ_KDZ::returnArea(){return area;}
QString QQ_KDZ::returnBirthday(){return birthday;}
QString QQ_KDZ::returnSignature(){return signature;}
QString QQ_KDZ::returnApplyDate(){return applyDate;}
int QQ_KDZ::returnStatus(){return status;}
QString QQ_KDZ::returnHostName(){return hostName;}

List_KDZ *QQ_KDZ::returnList()
{
    return &list;
}
QString QQ_KDZ::returnLinkedWeChat(){return linkedWeChat;}
QString QQ_KDZ::returnSharedWeibo(){return sharedWeibo;}

void QQ_KDZ::changePassWord(QString newPassword)
{
    password=newPassword;
}
void QQ_KDZ::changeName(QString newName)
{
    name=newName;
}
void QQ_KDZ::changeSex(int newSex)
{
    sex=newSex;
}
void QQ_KDZ::changeArea(QString newArea)
{
    area=newArea;
}
void QQ_KDZ::changeBirthday(QString newBirthday)
{
    birthday=newBirthday;
}
void QQ_KDZ::changeSignature(QString newSignature)
{
    signature=newSignature;
}

void QQ_KDZ::changeStatus(int newStatus)
{
    status=newStatus;
}

void QQ_KDZ::changeHostName(QString newName)
{
    hostName=newName;
}
void QQ_KDZ::changeLinkedWeChat(QString newLinkedWeChat)
{
    linkedWeChat=newLinkedWeChat;
}

//WeChat_KDZ
QString WeChat_KDZ::returnEmail(){return email;}
QString WeChat_KDZ::returnID(){return ID;}
QString WeChat_KDZ::returnPassword(){return password;}
QString WeChat_KDZ::returnName(){return name;}
int WeChat_KDZ::returnSex(){return sex;}
QString WeChat_KDZ::returnArea(){return area;}
QString WeChat_KDZ::returnBirthday(){return birthday;}
QString WeChat_KDZ::returnSignature(){return signature;}
QString WeChat_KDZ::returnApplyDate(){return applyDate;}
int WeChat_KDZ::returnStatus(){return status;}
QString WeChat_KDZ::returnHostName(){return hostName;}

List_KDZ *WeChat_KDZ::returnList()
{
    return &list;
}
QString WeChat_KDZ::returnLinkedQQ(){return linkedQQ;}

void WeChat_KDZ::changePassWord(QString newPassword)
{
    password=newPassword;
}
void WeChat_KDZ::changeName(QString newName)
{
    name=newName;
}
void WeChat_KDZ::changeSex(int newSex)
{
    sex=newSex;
}
void WeChat_KDZ::changeArea(QString newArea)
{
    area=newArea;
}
void WeChat_KDZ::changeBirthday(QString newBirthday)
{
    birthday=newBirthday;
}
void WeChat_KDZ::changeSignature(QString newSignature)
{
    signature=newSignature;
}

void WeChat_KDZ::changeStatus(int newStatus)
{
    status=newStatus;
}

void WeChat_KDZ::changeHostName(QString newName)
{
    hostName=newName;
}
void WeChat_KDZ::changeLinkedQQ(QString newLinkedQQ)
{
    linkedQQ=newLinkedQQ;
}

//Weibo_KDZ
QString Weibo_KDZ::returnEmail(){return email;}
QString Weibo_KDZ::returnID(){return ID;}
QString Weibo_KDZ::returnPassword(){return password;}
QString Weibo_KDZ::returnName(){return name;}
int Weibo_KDZ::returnSex(){return sex;}
QString Weibo_KDZ::returnArea(){return area;}
QString Weibo_KDZ::returnBirthday(){return birthday;}
QString Weibo_KDZ::returnSignature(){return signature;}
QString Weibo_KDZ::returnApplyDate(){return applyDate;}
QString Weibo_KDZ::returnSharedQQ(){return sharedQQ;}
int Weibo_KDZ::returnStatus(){return status;}
QString Weibo_KDZ::returnHostName(){return hostName;}

List_KDZ *Weibo_KDZ::returnList()
{
    return &list;
}
void Weibo_KDZ::changePassWord(QString newPassword)
{
    password=newPassword;
}
void Weibo_KDZ::changeName(QString newName)
{
    name=newName;
}
void Weibo_KDZ::changeSex(int newSex)
{
    sex=newSex;
}
void Weibo_KDZ::changeArea(QString newArea)
{
    area=newArea;
}
void Weibo_KDZ::changeBirthday(QString newBirthday)
{
    birthday=newBirthday;
}
void Weibo_KDZ::changeSignature(QString newSignature)
{
    signature=newSignature;
}

void Weibo_KDZ::changeStatus(int newStatus)
{
    status=newStatus;
}

void Weibo_KDZ::changeHostName(QString newName)
{
    hostName=newName;
}
