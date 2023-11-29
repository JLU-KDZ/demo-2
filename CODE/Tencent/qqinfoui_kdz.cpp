#include "qqinfoui_kdz.h"
#include "ui_qqinfoui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqui_kdz.h"
#include "qqexpui_kdz.h"
#include "qqchatui_kdz.h"
#include "widget.h"
#include "database.h"
#include "group.h"

#include <QDate>
#include <QCloseEvent>
#include <QMessageBox>

extern Widget* pw;
extern QQUI_KDZ* pq;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;
extern DBQQ_KDZ* dbqq;

QQInfoUI_KDZ::QQInfoUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQInfoUI_KDZ)
{
    ui->setupUi(this);
}

void QQInfoUI_KDZ::toPage1()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQInfoUI_KDZ::toPage2()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void QQInfoUI_KDZ::toPage3()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void QQInfoUI_KDZ::myInfo(QString id)
{
    //设置个人资料
    ui->label_myIDOUT->setText(id);
    ui->lineEdit_myname->setText(pql->returnPQQ()->returnName());
    ui->lineEdit_mysignature->setText(pql->returnPQQ()->returnSignature());

    if(pql->returnPQQ()->returnSex()==0)
    {
        ui->lineEdit_mysex->setText(QString("女"));
    }
    else if(pql->returnPQQ()->returnSex()==1)
    {
        ui->lineEdit_mysex->setText(QString("男"));
    }
    else
    {
        ui->lineEdit_mysex->setText(QString("其他"));
    }
    ui->lineEdit_myarea->setText(pql->returnPQQ()->returnArea());

    QString birthday=pql->returnPQQ()->returnBirthday();
    QDate birthdate=QDate::fromString(birthday,"yyyy/M/d");
    ui->dateEdit_mybirthday->setDate(birthdate);

    QString applydate=pql->returnPQQ()->returnApplyDate();
    QDate mydate=QDate::fromString(applydate,"yyyy/M/d");
    QDate current=QDate::currentDate();
    QString qqyear=QString::number((int)(mydate.daysTo(current))/365);
    ui->label_myQQyearOUT->setText(qqyear+" 年");
}

void QQInfoUI_KDZ::friendInfo(QString id)
{
    //设置好友资料
    ui->label_FIDOUT->setText(id);
    vector<QString> ans=dbqq->info(id);
    ui->label_FnameOUT->setText(ans[3]);
    ui->label_FsexOUT->setText(ans[4]);
    ui->label_FareaOUT->setText(ans[5]);
    ui->label_FbirthdayOUT->setText(ans[6]);
    ui->label_FsignatureOUT->setText(ans[7]);

    QDate date=QDate::fromString(ans[8],"yyyy/M/d");
    QDate current=QDate::currentDate();
    QString qqyear=QString::number((int)(date.daysTo(current))/365);
    ui->label_FQQyearOUT->setText(qqyear+" 年");
}

void QQInfoUI_KDZ::groupInfo(QString id)
{
    //设置群聊资料
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(id,"###","###");
    pgroup->readGroupFile();
    QString creater=pgroup->returnGroupCreater();
    QString name=pgroup->returnGroupName();

    ui->label_GIDOUT->setText(id);
    ui->label_GcreaterOUT->setText(creater);
    ui->lineEdit_GnameOUT->setText(name);

    delete pgroup;
}

void QQInfoUI_KDZ::closeEvent(QCloseEvent* e)
{
    e->accept();
    delete pqi;
    pqi=nullptr;
}

QQInfoUI_KDZ::~QQInfoUI_KDZ()
{
    delete ui;
}

void QQInfoUI_KDZ::on_pushButton_save_clicked()
{
    //保存个人资料
    QString name=ui->lineEdit_myname->text();
    QString sex=ui->lineEdit_mysex->text();
    QString signature=ui->lineEdit_mysignature->text();
    QString area=ui->lineEdit_myarea->text();
    QString birthday=ui->dateEdit_mybirthday->date().toString("yyyy/M/d");

    if(name.length()==0)
    {
        QMessageBox::warning(this,tr("保存失败"),tr("昵称为空！"),QMessageBox::Ok);
        ui->lineEdit_myname->setFocus();
        return;
    }
    if(sex.length()==0)
    {
        QMessageBox::warning(this,tr("保存失败"),tr("性别为空！"),QMessageBox::Ok);
        ui->lineEdit_mysex->setFocus();
        return;
    }

    int sexInt;
    if(sex=="女")
    {
        sexInt=0;
    }
    else if(sex=="男")
    {
        sexInt=1;
    }
    else if(sex=="其他")
    {
        sexInt=2;
    }
    else
    {
        QMessageBox::warning(this,tr("保存失败"),tr("性别不合法！"),QMessageBox::Ok);
        ui->lineEdit_mysex->clear();
        ui->lineEdit_mysex->setFocus();
        return;
    }

    pql->returnPQQ()->changeName(name);
    pql->returnPQQ()->changeSignature(signature);
    pql->returnPQQ()->changeArea(area);
    pql->returnPQQ()->changeSex(sexInt);
    pql->returnPQQ()->changeBirthday(birthday);

    //更新数据库
    int flag=dbqq->update(pql->returnPQQ()->returnID(),name,sexInt,area,birthday,signature);
    if(flag==0)
    {
        //刷新界面
        pql->refresh();
        QMessageBox::information(this,"保存提示","保存成功！");
        return;
    }
    if(flag==1)
    {
        QMessageBox::warning(this,tr("保存失败"),tr("该账号不存在！"),QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox::warning(this,tr("保存失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        return;
    }
}

void QQInfoUI_KDZ::on_pushButton_Fdelete_clicked()
{
    //删除好友
    QMessageBox::StandardButton btn=QMessageBox::question(this, "提示", "确认删除?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes)
    {
        QString id=ui->label_FIDOUT->text();
        pql->returnPQQ()->returnList()->readListFile(pql->returnPQQ()->returnID());
        pql->returnPQQ()->returnList()->deleteFriendList(id);
        pql->returnPQQ()->returnList()->writeListFile(pql->returnPQQ()->returnID());

        List_KDZ* plist=new List_KDZ();
        plist->readListFile(id);
        plist->deleteFriendList(pql->returnPQQ()->returnID());
        plist->writeListFile(id);
        delete plist;

        pqi->close();
        delete pqi;
        pqi=nullptr;

        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
    }
}

void QQInfoUI_KDZ::on_pushButton_Gchange_clicked()
{
    //修改群名
    QString id=ui->label_GIDOUT->text();
    QString name=ui->lineEdit_GnameOUT->text();
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(id,"###","###");
    pgroup->readGroupFile();
    int flag=pgroup->changeGroupName(pql->returnPQQ()->returnID(),name);
    if(flag==1)
    {
        QMessageBox::warning(this,tr("修改失败"),tr("无群主或管理员权限！"),QMessageBox::Ok);
        ui->lineEdit_GnameOUT->setText(pgroup->returnGroupName());
        delete pgroup;
        return;
    }
    if(flag==0)
    {
        QMessageBox::information(this,"修改提示","修改成功！");
        pgroup->writeGroupFile();
        delete pgroup;
        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
        return;
    }
}

void QQInfoUI_KDZ::on_pushButton_Fsend_clicked()
{
    //好友发消息
    QString id=ui->label_FIDOUT->text();
    if(pqc==nullptr)
    {
        pqc=new QQChatUI_KDZ();
        pqc->toPage1();
        pqc->initChat1(id);
        pqc->show();

        pqi->close();
        delete pqi;
        pqi=nullptr;
    }
    else
    {
        if(pqc->currentI()==2)
        {
            pqc->DGServer();
        }
        pqc->toPage1();
        pqc->initChat1(id);

        pqi->close();
        delete pqi;
        pqi=nullptr;
    }
}

void QQInfoUI_KDZ::on_pushButton_Gsend_clicked()
{
    //群聊发消息
    QString id=ui->label_GIDOUT->text();
    if(pqc==nullptr)
    {
        pqc=new QQChatUI_KDZ();
        pqc->toPage2();
        pqc->initChat2(id);
        pqc->CGServer(id);
        pqc->show();

        pqi->close();
        delete pqi;
        pqi=nullptr;
    }
    else
    {
        if(pqc->currentI()==2)
        {
            pqc->DGServer();
        }
        pqc->toPage2();
        pqc->initChat2(id);
        pqc->CGServer(id);

        pqi->close();
        delete pqi;
        pqi=nullptr;
    }
}

void QQInfoUI_KDZ::on_pushButton_Gdelete_clicked()
{
    //退出/解散群聊
    QMessageBox::StandardButton btn=QMessageBox::question(this, "提示", "确认退出?", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QString creater=ui->label_GcreaterOUT->text();
        QString gid=ui->label_GIDOUT->text();
        QString mid=pql->returnPQQ()->returnID();
        if(mid!=creater)//退出
        {
            QQGroup_KDZ* pgroup=new QQGroup_KDZ(gid,"###","###");
            pgroup->readGroupFile();
            pgroup->deleteAdmin(creater,mid);
            pgroup->deleteMember(creater,mid);
            pgroup->writeGroupFile();
            delete pgroup;

            pql->returnPQQ()->returnList()->readListFile(mid);
            pql->returnPQQ()->returnList()->deleteGroupList(gid);
            pql->returnPQQ()->returnList()->writeListFile(mid);

            pqi->close();
            delete pqi;
            pqi=nullptr;

            pql->initInfo(mid);
            pql->initList();
            return;
        }
        else//解散
        {
            QQGroup_KDZ* pgroup=new QQGroup_KDZ(gid,"###","###");
            pgroup->readGroupFile();

            List_KDZ* plist=new List_KDZ();
            plist->readListFile(creater);
            plist->deleteGroupList(gid);
            plist->writeListFile(creater);
            delete plist;
            plist=nullptr;

            vector<QString> ans1=pgroup->returnGroupAdmin();
            for(int i=0;i<(int)ans1.size();i++)
            {
                plist=new List_KDZ();
                plist->readListFile(ans1[i]);
                plist->deleteGroupList(gid);
                plist->writeListFile(ans1[i]);
                delete plist;
                plist=nullptr;
            }

            vector<QString> ans2=pgroup->returnGroupMember();
            for(int i=0;i<(int)ans2.size();i++)
            {
                plist=new List_KDZ();
                plist->readListFile(ans2[i]);
                plist->deleteGroupList(gid);
                plist->writeListFile(ans2[i]);
                delete plist;
                plist=nullptr;
            }

            pgroup->removeGroupFile(creater);
            delete pgroup;

            pqi->close();
            delete pqi;
            pqi=nullptr;

            pql->initInfo(mid);
            pql->initList();
        }
    }
}
