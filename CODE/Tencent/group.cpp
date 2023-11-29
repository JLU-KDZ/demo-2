#include "group.h"

//QQ群
QString QQGroup_KDZ::returnGroupID(){return groupID;}
QString QQGroup_KDZ::returnGroupName(){return groupName;}
QString QQGroup_KDZ::returnGroupCreater(){return groupCreater;}
vector<QString> QQGroup_KDZ::returnGroupMember(){return groupMember;}
vector<QString> QQGroup_KDZ::returnGroupAdmin(){return groupAdmin;}
vector<QString> QQGroup_KDZ::returnNewMember(){return newMember;}

int QQGroup_KDZ::changeGroupName(QString str, QString newName)
{
    if(queryAdmin(str)!=-1||str==groupCreater)
    {
        groupName=newName;
        return 0;
    }
    return 1;//无管理员或群主权限
}

int QQGroup_KDZ::queryAdmin(QString ad)
{
    for(int i=0;i<(int)groupAdmin.size();i++)
        if(ad==groupAdmin[i])
            return i;
    return -1;
}

int QQGroup_KDZ::addAdmin(QString str, QString ad)
{
    if(str==groupCreater)
    {
        if(queryAdmin(ad)==-1)
        {
            if(queryMember(ad)!=-1)
            {
                groupAdmin.push_back(ad);
                deleteMember(str,ad);
                return 0;
            }
            return 3;//不存在该成员
        }
        return 2;//已存在该管理员
    }
    return 1;//无群主权限
}

int QQGroup_KDZ::deleteAdmin(QString str, QString ad)
{
    if(str==groupCreater)
    {
        int pos=queryAdmin(ad);
        if(pos==-1)
            return 2;//不存在该管理员
        if(queryMember(ad)==-1)
        {
            groupAdmin.erase(groupAdmin.begin()+pos);
            addMember(ad);
            return 0;
        }
        return 2;//已存在该成员
    }
    return 1;//无群主权限
}

int QQGroup_KDZ::queryMember(QString mem)
{
    for(int i=0;i<(int)groupMember.size();i++)
        if(mem==groupMember[i])
            return i;
    return -1;
}

int QQGroup_KDZ::inviteMember(QString mem)
{
    if(queryMember(mem)==-1&&queryAdmin(mem)==-1&&mem!=groupCreater)
    {
        if(queryNew(mem)!=-1)
        {
            return 2;//该成员已被邀请
        }
        else
        {
            newMember.push_back(mem);
            return 0;
        }
    }
    else
    {
        return 1;//已存在该成员
    }
}

int QQGroup_KDZ::addMember(QString New)
{
    if(queryMember(New)==-1)
    {
        groupMember.push_back(New);
    }
    return 1;//已存在该成员
}

int QQGroup_KDZ::deleteMember(QString str, QString mem)
{
    int pos=queryMember(mem);
    if(pos!=-1)
    {
        if(queryAdmin(str)!=-1||str==groupCreater)
        {
            groupMember.erase(groupMember.begin()+pos);
            return 0;
        }
        return 1;//无管理员或群主权限
    }
    return 2;//不存在该成员
}

int QQGroup_KDZ::sendInvite(QString mem)
{
    QDir *list = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/List/"+mem+"";
    if(!list->exists(path))
    {
        return 1;//该账号不存在
    }

    QFile file(QCoreApplication::applicationDirPath()+"/List/"+mem+"/newGroupList.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            if(line==groupID)
            {
                file.close();
                return 2;//该成员已被邀请
            }
        }
        file.close();
    }

    if(file.open(QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<groupID<<"\r\n";
        file.close();
    }
    return 0;

}

int QQGroup_KDZ::accInvite(QString str, QString mem)
{
    if(queryNew(mem)!=-1)
    {
        if(str!=groupCreater&&queryAdmin(str)==-1)
        {
            return 2;//无群主或管理员权限
        }
        else
        {
            groupMember.push_back(mem);
            int pos=queryNew(mem);
            newMember.erase(newMember.begin()+pos);
            return 0;
        }
    }
    else
    {
        return 1;//不存在该申请成员
    }

}

int QQGroup_KDZ::queryNew(QString New)
{
    for(int i=0;i<(int)newMember.size();i++)
        if(New==newMember[i])
            return i;
    return -1;
}

int QQGroup_KDZ::initGroupFile()
{
    //创建群聊文件夹
    QDir *grouplist = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"";
    if(grouplist->exists(path))
    {
       return 1;//群号已存在
    }
    else
    {
        grouplist->mkdir(path);
    }

    //创建群聊信息文件
    QFile file1(QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"/info.txt");
    if(file1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file1);
        stream<<groupID<<"\r\n"<<groupName<<"\r\n"<<groupCreater<<"\r\n";
        stream<<"groupAdmin\r\n";
        stream<<"groupMember\r\n";
        stream<<"newMember\r\n";
        file1.close();
    }

    //创建群聊内容文件
    QFile file2(QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"/chat.txt");
    if(file2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file2.close();
    }
    return 0;
}

void QQGroup_KDZ::writeGroupFile()
{
    QFile file(QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"/info.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<groupID<<"\r\n"<<groupName<<"\r\n"<<groupCreater<<"\r\n";
        stream<<"groupAdmin\r\n";
        for(int i=0;i<(int)groupAdmin.size();i++)
        {
            stream<<groupAdmin[i]<<"\r\n";
        }
        stream<<"groupMember\r\n";
        for(int i=0;i<(int)groupMember.size();i++)
        {
            stream<<groupMember[i]<<"\r\n";
        }
        stream<<"newMember\r\n";
        for(int i=0;i<(int)newMember.size();i++)
        {
            stream<<newMember[i]<<"\r\n";
        }
        file.close();
    }
}

int QQGroup_KDZ::removeGroupFile(QString str)
{
     QString path = QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"";

     //路径是否为空
     //路径是否存在
     if(path.isEmpty()||!QDir(path).exists())
     {
         return 1;
     }
     else
     {
         if(str!=groupCreater)
         {
             return 2;//无群主权限
         }
         else
         {
             QDir qDir(path);
             qDir.removeRecursively();
         }
     }
     return 0;
}

void QQGroup_KDZ::createGroupFile()
{
    //在当前路径创建群聊文件夹
    QDir *grouplist = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/QQGroup";
    if(!grouplist->exists(path))
    {
        grouplist->mkdir(path);
    }
}

void QQGroup_KDZ::updateAdmin(vector<QString> str)
{
    groupAdmin=str;
}

void QQGroup_KDZ::updateMember(vector<QString> str)
{
    groupMember=str;
}

void QQGroup_KDZ::updateNew(vector<QString> str)
{
    newMember=str;
}

void QQGroup_KDZ::readGroupFile()
{
    vector<QString> admin;
    vector<QString> member;
    vector<QString> New;

    //读取文件
    QFile file(QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"/info.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        int flag=0;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            if(flag==0)//读取群名、群主
            {
                in.readLine();
                QString line2 = in.readLine();
                QString line3 = in.readLine();
                groupName=line2;
                groupCreater=line3;
                in.readLine();
                flag=1;
            }
            else if(flag==1)//读取群管理员
            {
                QString line = in.readLine();
                if(line!="groupMember")
                {
                    admin.push_back(line);
                }
                else
                {
                    flag=2;
                }
            }
            else if(flag==2)//读取群成员
            {
                QString line = in.readLine();
                if(line!="newMember")
                {
                    member.push_back(line);
                }
                else
                {
                    flag=3;
                }
            }
            else//读取群申请成员
            {
                QString line = in.readLine();
                New.push_back(line);
            }
        }
        file.close();
    }
    updateAdmin(admin);
    updateMember(member);
    updateNew(New);
}

int QQGroup_KDZ::searchFlag()
{
    QString path = QCoreApplication::applicationDirPath()+"/QQGroup/"+groupID+"";
    QString filePath=path+"/flag.txt";

    if(!QDir().exists(filePath))
    {
        return 1;//标记文件不存在
    }
    else
    {
        return 0;//标记文件存在
    }
}

//微信群
QString WeChatGroup_KDZ::returnGroupID(){return groupID;}
QString WeChatGroup_KDZ::returnGroupName(){return groupName;}
QString WeChatGroup_KDZ::returnGroupCreater(){return groupCreater;}
vector<QString> WeChatGroup_KDZ::returnGroupMember(){return groupMember;}
vector<QString> WeChatGroup_KDZ::returnNewMember(){return newMember;}

int WeChatGroup_KDZ::changeGroupName(QString str, QString newName)
{
    if(str==groupCreater)
    {
        groupName=newName;
        return 0;
    }
    return 1;
}

int WeChatGroup_KDZ::queryMember(QString mem)
{
    for(int i=0;i<(int)groupMember.size();i++)
        if(mem==groupMember[i])
            return i;
    return -1;
}

int WeChatGroup_KDZ::inviteMember(QString mem)
{
    if(queryMember(mem)==-1&&mem!=groupCreater)
    {
        if(queryNew(mem)!=-1)
        {
            return 2;
        }
        else
        {
            newMember.push_back(mem);
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int WeChatGroup_KDZ::addMember(QString New)
{
    if(queryMember(New)==-1)
    {
        groupMember.push_back(New);
    }
    return 1;
}

int WeChatGroup_KDZ::deleteMember(QString str, QString mem)
{
    int pos=queryMember(mem);
    if(pos!=-1)
    {
        if(str==groupCreater)
        {
            groupMember.erase(groupMember.begin()+pos);
            return 0;
        }
        return 1;
    }
    return 2;
}

int WeChatGroup_KDZ::sendInvite(QString mem)
{
    QDir *list = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/List/"+mem+"";
    if(!list->exists(path))
    {
        return 1;
    }

    QFile file(QCoreApplication::applicationDirPath()+"/List/"+mem+"/newGroupList.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line=in.readLine();
            if(line==groupID)
            {
                file.close();
                return 2;
            }
        }
        file.close();
    }

    if(file.open(QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<groupID<<"\r\n";
        file.close();
    }
    return 0;

}

int WeChatGroup_KDZ::accInvite(QString str, QString mem)
{
    if(queryNew(mem)!=-1)
    {
        if(str!=groupCreater)
        {
            return 2;
        }
        else
        {
            groupMember.push_back(mem);
            int pos=queryNew(mem);
            newMember.erase(newMember.begin()+pos);
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int WeChatGroup_KDZ::queryNew(QString New)
{
    for(int i=0;i<(int)newMember.size();i++)
        if(New==newMember[i])
            return i;
    return -1;
}

int WeChatGroup_KDZ::initGroupFile()
{
    QDir *grouplist = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"";
    if(grouplist->exists(path))
    {
        return 1;
    }
    else
    {
        grouplist->mkdir(path);
    }

    QFile file1(QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"/info.txt");
    if(file1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file1);
        stream<<groupID<<"\r\n"<<groupName<<"\r\n"<<groupCreater<<"\r\n";
        stream<<"groupMember\r\n";
        stream<<"newMember\r\n";
        file1.close();
    }

    QFile file2(QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"/chat.txt");
    if(file2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file2.close();
    }
    return 0;
}

void WeChatGroup_KDZ::writeGroupFile()
{
    QFile file(QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"/info.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<groupID<<"\r\n"<<groupName<<"\r\n"<<groupCreater<<"\r\n";
        stream<<"groupMember\r\n";
        for(int i=0;i<(int)groupMember.size();i++)
        {
            stream<<groupMember[i]<<"\r\n";
        }
        stream<<"newMember\r\n";
        for(int i=0;i<(int)newMember.size();i++)
        {
            stream<<newMember[i]<<"\r\n";
        }
        file.close();
    }
}

int WeChatGroup_KDZ::removeGroupFile(QString str)
{
    QString path = QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"";

    if(path.isEmpty()||!QDir(path).exists())
    {
        return 1;
    }
    else
    {
        if(str!=groupCreater)
        {
            return 2;
        }
        else
        {
            QDir qDir(path);
            qDir.removeRecursively();
        }
    }
    return 0;
}

void WeChatGroup_KDZ::createGroupFile()
{
    QDir *grouplist = new QDir;
    QString path = QCoreApplication::applicationDirPath()+"/WCGroup";
    if(!grouplist->exists(path))
    {
        grouplist->mkdir(path);
    }
}

void WeChatGroup_KDZ::updateMember(vector<QString> str)
{
    groupMember=str;
}

void WeChatGroup_KDZ::updateNew(vector<QString> str)
{
    newMember=str;
}

void WeChatGroup_KDZ::readGroupFile()
{
    vector<QString> member;
    vector<QString> New;

    QFile file(QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"/info.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        int flag=0;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            if(flag==0)
            {
                in.readLine();
                QString line2 = in.readLine();
                QString line3 = in.readLine();
                groupName=line2;
                groupCreater=line3;
                in.readLine();
                flag=1;
            }
            else if(flag==1)
            {
                QString line = in.readLine();
                if(line!="newMember")
                {
                    member.push_back(line);
                }
                else
                {
                    flag=2;
                }
            }
            else
            {
                QString line = in.readLine();
                New.push_back(line);
            }
        }
        file.close();
    }
    updateMember(member);
    updateNew(New);
}

int WeChatGroup_KDZ::searchFlag()
{
    QString path = QCoreApplication::applicationDirPath()+"/WCGroup/"+groupID+"";
    QString filePath=path+"/flag.txt";

    if(!QDir().exists(filePath))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
