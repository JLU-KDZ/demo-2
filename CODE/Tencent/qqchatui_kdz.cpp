#include "qqinfoui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqui_kdz.h"
#include "widget.h"
#include "qqchatui_kdz.h"
#include "qqexpui_kdz.h"
#include "ui_qqchatui_kdz.h"
#include "database.h"
#include "group.h"

#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QMessageBox>
#include <QList>
#include <QMenu>
#include <QPoint>

extern Widget* pw;
extern QQUI_KDZ* pq;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;
extern DBQQ_KDZ* dbqq;
extern DBWC_KDZ* dbwc;

QQChatUI_KDZ::QQChatUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQChatUI_KDZ)
{
    pgclient=nullptr;

    ui->setupUi(this);
}

void QQChatUI_KDZ::initChat1(QString id)
{
    vector<QString> ans=dbqq->info(id);
    ui->label_fchat->setText(ans[3]);
}

void QQChatUI_KDZ::initChat2(QString id)
{
    groupID=id;
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(id,"###","###");
    pgroup->readGroupFile();
    ui->label_gchat->setText(pgroup->returnGroupName());

    QStandardItemModel* model=new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("ID")<<QStringLiteral("NAME"));
    ui->treeView_QQchat->setModel(model);
    ui->treeView_QQchat->expandAll();
    ui->treeView_QQchat->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView_QQchat->header()->setDefaultSectionSize(250);
    ui->treeView_QQchat->header()->setStretchLastSection(true);
    QStandardItem* root=model->invisibleRootItem();

    QString createrID=pgroup->returnGroupCreater();
    vector<QString> ans1=dbqq->info(createrID);
    QString createrName=ans1[3];

    QStandardItem* itemC=new QStandardItem(QString("群主"));
    root->appendRow(itemC);
    QList<QStandardItem*> itemCListNode;
    QStandardItem* itemCListID;
    QStandardItem* itemCListName;
    itemCListID=new QStandardItem(createrID);
    itemCListName=new QStandardItem(createrName);
    itemCListNode<<itemCListID<<itemCListName;
    itemC->appendRow(itemCListNode);

    QStandardItem* itemA=new QStandardItem(QString("管理员"));
    root->appendRow(itemA);

    vector<QString> ans2=pgroup->returnGroupAdmin();
    for(int i=0;i<(int)ans2.size();i++)
    {
        QList<QStandardItem*> itemAListNode;
        QStandardItem* itemAListID;
        QStandardItem* itemAListName;
        itemAListID=new QStandardItem(ans2[i]);

        vector<QString> tmp=dbqq->info(ans2[i]);

        itemAListName=new QStandardItem(tmp[3]);
        itemAListNode<<itemAListID<<itemAListName;
        itemA->appendRow(itemAListNode);
    }


    QStandardItem* itemM=new QStandardItem(QString("成员"));
    root->appendRow(itemM);

    vector<QString> ans3=pgroup->returnGroupMember();
    for(int i=0;i<(int)ans3.size();i++)
    {
        QList<QStandardItem*> itemMListNode;
        QStandardItem* itemMListID;
        QStandardItem* itemMListName;
        itemMListID=new QStandardItem(ans3[i]);

        vector<QString> tmp=dbqq->info(ans3[i]);

        itemMListName=new QStandardItem(tmp[3]);
        itemMListNode<<itemMListID<<itemMListName;
        itemM->appendRow(itemMListNode);
    }

    QStandardItem* itemN=new QStandardItem(QString("成员申请"));
    root->appendRow(itemN);

    vector<QString> ans4=pgroup->returnNewMember();
    for(int i=0;i<(int)ans4.size();i++)
    {
        QList<QStandardItem*> itemNListNode;
        QStandardItem* itemNListID;
        QStandardItem* itemNListName;
        itemNListID=new QStandardItem(ans4[i]);

        vector<QString> tmp=dbqq->info(ans4[i]);

        itemNListName=new QStandardItem(tmp[3]);
        itemNListNode<<itemNListID<<itemNListName;
        itemN->appendRow(itemNListNode);
    }

    delete pgroup;

    ui->treeView_QQchat->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_QQchat,&QTreeView::customContextMenuRequested,this,&QQChatUI_KDZ::slotMenuPopup);
}

void QQChatUI_KDZ::toPage1()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQChatUI_KDZ::toPage2()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void QQChatUI_KDZ::toPage3()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

int QQChatUI_KDZ::currentI()
{
    return ui->stackedWidget->currentIndex();
}

void QQChatUI_KDZ::getHistoryMes(QString id)
{
    QFile file(QCoreApplication::applicationDirPath()+"/QQGroup/"+id+"/chat.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           ui->textEdit_GsendOUT->append(in.readLine());
        }
        file.close();
    }
    ui->textEdit_GsendOUT->append("                                                       --以上为离线消息--");
}

void QQChatUI_KDZ::closeEvent(QCloseEvent *e)
{
    DGServer();
    delete pqc;
    pqc=nullptr;
    e->accept();
}

QString QQChatUI_KDZ::returnGroupID()
{
    return groupID;
}

void QQChatUI_KDZ::CGServer(QString id)
{
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(id,"###","###");
    int flag=pgroup->searchFlag();

    if(flag==1)
    {
        ui->label_server->setText(QString("服务器未启动"));

        ui->textEdit_GsendOUT->clear();
        getHistoryMes(id);
    }
    else
    {
        if(pgclient!=nullptr)
        {
            delete pgclient;
            pgclient=nullptr;
        }
        pgclient=new Client_KDZ();
        connect(pgclient,SIGNAL(sendM(QString)),this,SLOT(recvM(QString)));
        pgclient->init(pql->returnPQQ()->returnName());
        pgclient->startC();
        ui->label_server->setText(QString("服务器已启动"));

        ui->textEdit_GsendOUT->clear();
        getHistoryMes(id);
    }
}

void QQChatUI_KDZ::DGServer()
{
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
    int flag=pgroup->searchFlag();
    if(flag==1)
    {
        delete pgroup;
        return;
    }
    else
    {
        if(ui->label_server->text()=="服务器已启动")
        {
            pgclient->closeC();
            delete pgroup;
            ui->label_server->setText(QString("服务器未启动"));
            return;
        }
    }
}

QQChatUI_KDZ::~QQChatUI_KDZ()
{
    delete ui;
}

void QQChatUI_KDZ::slotMenuPopup(const QPoint &pos)
{

    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex parentIndex=curIndex.parent();
    QModelIndex curIndexID=curIndex.sibling(curIndex.row(),0);
    QModelIndex parentIndexID=parentIndex.sibling(parentIndex.row(),0);
    QString strc=curIndexID.data().toString();
    QString strp=parentIndexID.data().toString();

    QModelIndex Index=ui->treeView_QQchat->indexAt(pos);


    if(Index.isValid()&&strc!="群主"&&strc!="管理员"&&strc!="成员"&&strc!="成员申请")
    {
        QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
        pgroup->readGroupFile();

        if(strp=="管理员"&&pql->returnPQQ()->returnID()==pgroup->returnGroupCreater())
        {
            menu=new QMenu(ui->treeView_QQchat);
            menu->addAction(QString("取消管理员"),this,SLOT(slotAction1()));
            menu->addSeparator();
            menu->addAction(QString("移除成员"),this,SLOT(slotAction2()));
            menu->exec(QCursor::pos());
        }
        if(strp=="成员")
        {
            if(pgroup->queryAdmin(pql->returnPQQ()->returnID())!=-1)
            {
                menu=new QMenu(ui->treeView_QQchat);
                menu->addAction(QString("移除成员"),this,SLOT(slotAction4()));
                menu->exec(QCursor::pos());
            }
            if(pql->returnPQQ()->returnID()==pgroup->returnGroupCreater())
            {
                menu=new QMenu(ui->treeView_QQchat);
                menu->addAction(QString("设置管理员"),this,SLOT(slotAction3()));
                menu->addSeparator();
                menu->addAction(QString("移除成员"),this,SLOT(slotAction4()));
                menu->exec(QCursor::pos());
            }
        }
        if(strp=="成员申请"&&(pql->returnPQQ()->returnID()==pgroup->returnGroupCreater()||pgroup->queryAdmin(pql->returnPQQ()->returnID())!=-1))
        {
            menu=new QMenu(ui->treeView_QQchat);
            menu->addAction(QString("同意"),this,SLOT(slotAction5()));
            menu->addSeparator();
            menu->addAction(QString("拒绝"),this,SLOT(slotAction6()));
            menu->exec(QCursor::pos());
        }
        delete pgroup;
    }
}

void QQChatUI_KDZ::slotAction1()//取消管理员
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();
       pgroup->deleteAdmin(pql->returnPQQ()->returnID(),id);
       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;
    }
}

void QQChatUI_KDZ::slotAction2()//移除成员
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();
       pgroup->deleteAdmin(pql->returnPQQ()->returnID(),id);
       pgroup->deleteMember(pql->returnPQQ()->returnID(),id);
       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;

       List_KDZ* plist=new List_KDZ();
       plist->readListFile(id);
       plist->deleteGroupList(groupID);
       plist->writeListFile(id);
       delete plist;
    }
}

void QQChatUI_KDZ::slotAction3()//设置管理员
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();
       pgroup->addAdmin(pql->returnPQQ()->returnID(),id);
       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;
    }
}

void QQChatUI_KDZ::slotAction4()//移除成员
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();
       pgroup->deleteMember(pql->returnPQQ()->returnID(),id);
       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;

       List_KDZ* plist=new List_KDZ();
       plist->readListFile(id);
       plist->deleteGroupList(groupID);
       plist->writeListFile(id);
       delete plist;
    }

}

void QQChatUI_KDZ::slotAction5()//同意加入
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();
       pgroup->accInvite(pql->returnPQQ()->returnID(),id);
       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;

       List_KDZ* plist=new List_KDZ();
       plist->readListFile(id);
       plist->addGroupList(groupID);
       plist->writeListFile(id);
       delete plist;
    }
}

void QQChatUI_KDZ::slotAction6()//拒绝加入
{
    QModelIndex curIndex=ui->treeView_QQchat->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString id=indexID.data().toString();

    if(indexID.isValid())
    {
       QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
       pgroup->readGroupFile();

       pgroup->accInvite(pql->returnPQQ()->returnID(),id);
       pgroup->deleteMember(pql->returnPQQ()->returnID(),id);

       pgroup->writeGroupFile();
       pqc->initChat2(groupID);
       delete pgroup;
    }
}

void QQChatUI_KDZ::on_pushButton_RGroup_clicked()
{
    pqc->toPage2();
    pqc->initChat2(groupID);

    pqc->CGServer(groupID);
}

void QQChatUI_KDZ::on_pushButton_inviteID_clicked()
{
    QString newID=ui->lineEdit_inviteID->text();
    if(newID.length()==0)
    {
        QMessageBox::warning(this,tr("邀请失败"),tr("账号为空！"),QMessageBox::Ok);
        return;
    }
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
    pgroup->readGroupFile();

    if(newID==pgroup->returnGroupCreater()||pgroup->queryAdmin(newID)!=-1||pgroup->queryMember(newID)!=-1||pgroup->queryNew(newID)!=-1)
    {
        QMessageBox::warning(this,tr("邀请失败"),tr("该账号已在列表！"),QMessageBox::Ok);
        ui->lineEdit_inviteID->clear();
        ui->lineEdit_inviteID->setFocus();
        delete pgroup;
        return;

    }

    int flag=pgroup->sendInvite(newID);
    if(flag==1)
    {
        QMessageBox::warning(this,tr("邀请失败"),tr("该账号不存在！"),QMessageBox::Ok);
        ui->lineEdit_inviteID->clear();
        ui->lineEdit_inviteID->setFocus();
        delete pgroup;
        return;
    }
    if(flag==2)
    {
        QMessageBox::warning(this,tr("邀请失败"),tr("该账号已被邀请！"),QMessageBox::Ok);
        ui->lineEdit_inviteID->clear();
        ui->lineEdit_inviteID->setFocus();
        delete pgroup;
        return;
    }
    if(flag==0)
    {
        QMessageBox::information(this,"邀请提示","邀请成功！");
        ui->lineEdit_inviteID->clear();
        ui->lineEdit_inviteID->setFocus();
        pgroup->writeGroupFile();
        delete pgroup;
        return;
    }
}

void QQChatUI_KDZ::on_pushButton_gsend_clicked()
{
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(groupID,"###","###");
    int flag=pgroup->searchFlag();
    if(flag==0&&ui->label_server->text()=="服务器已启动")
    {
        QString mes=ui->textEdit_GsendIN->toPlainText();
        pgclient->send(mes);
        ui->textEdit_GsendIN->clear();
        delete pgroup;
    }
    else
    {
        QMessageBox::warning(this,tr("发送失败"),tr("未连接服务器！"),QMessageBox::Ok);
        ui->textEdit_GsendIN->clear();
        delete pgroup;
    }
}

void QQChatUI_KDZ::recvM(QString m)
{
    ui->textEdit_GsendOUT->append(m);
}

void QQChatUI_KDZ::on_pushButton_gset_clicked()
{
    pqc->toPage3();
    pqc->DGServer();
}

void QQChatUI_KDZ::on_pushButton_subG_clicked()
{
    QString gid=ui->lineEdit_subGID->text();
    QString gname=ui->lineEdit_subGNAME->text();
    QString creater=pql->returnPQQ()->returnID();

    //判断群号是否为空
    if(gid.length()==0)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群号为空！"),QMessageBox::Ok);
        ui->lineEdit_subGID->clear();
        ui->lineEdit_subGNAME->clear();
        ui->lineEdit_subGID->setFocus();
        return;
    }

    //判断群名是否为空
    if(gname.length()==0)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群名为空！"),QMessageBox::Ok);
        ui->lineEdit_subGID->clear();
        ui->lineEdit_subGNAME->clear();
        ui->lineEdit_subGID->setFocus();
        return;
    }

    //QQ群创建并初始化
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(gid,gname,creater);
    if(pgroup->initGroupFile()==1)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群号已存在！"),QMessageBox::Ok);
        ui->lineEdit_subGID->clear();
        ui->lineEdit_subGNAME->clear();
        ui->lineEdit_subGID->setFocus();
        delete pgroup;
        return;
    }

    //写入列表文件
    pql->returnPQQ()->returnList()->readListFile(pql->returnPQQ()->returnID());
    pql->returnPQQ()->returnList()->addGroupList(gid);
    pql->returnPQQ()->returnList()->writeListFile(pql->returnPQQ()->returnID());

    //写入列表
    pql->initInfo(pql->returnPQQ()->returnID());
    pql->initList();

    //创建成功
    QMessageBox::information(this,"创建提示","创建成功！");
    ui->lineEdit_subGID->clear();
    ui->lineEdit_subGNAME->clear();
    ui->lineEdit_subGID->setFocus();
    delete pgroup;
    return;
}
