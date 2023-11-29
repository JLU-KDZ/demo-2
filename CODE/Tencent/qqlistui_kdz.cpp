#include "qqlistui_kdz.h"
#include "ui_qqlistui_kdz.h"
#include "qqui_kdz.h"
#include "qqinfoui_kdz.h"
#include "qqchatui_kdz.h"
#include "qqexpui_kdz.h"
#include "widget.h"
#include "group.h"
#include "account.h"
#include "database.h"

#include <QStandardItemModel>
#include <QModelIndex>
#include <QList>
#include <vector>
#include <QMenu>
#include <QPoint>
#include <QFile>
#include <QDir>

extern Widget* pw;
extern QQUI_KDZ* pq;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;
extern DBQQ_KDZ* dbqq;

QQListUI_KDZ::QQListUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQListUI_KDZ)
{
    ui->setupUi(this);
}

QQ_KDZ *QQListUI_KDZ::returnPQQ()
{
    return qq;
}

void QQListUI_KDZ::initList()
{
    //设置列表参数
    QStandardItemModel* model=new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("ID")<<QStringLiteral("NAME"));
    ui->treeView_QQlist->setModel(model);
    ui->treeView_QQlist->expandAll();
    ui->treeView_QQlist->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView_QQlist->header()->setDefaultSectionSize(300);
    ui->treeView_QQlist->header()->setStretchLastSection(true);

    //设置列表根节点
    root=model->invisibleRootItem();

    //设置子节点“好友”
    itemF=new QStandardItem(QString("好友"));
    root->appendRow(itemF);

    vector<QString>friendlist=qq->returnList()->returnFriendList();
    for(int i=0;i<(int)friendlist.size();i++)
    {
        QList<QStandardItem*> itemFListNode;
        QStandardItem* itemFListID;
        QStandardItem* itemFListName;

        vector<QString> tmp=dbqq->info(friendlist[i]);
        itemFListID=new QStandardItem(friendlist[i]);
        itemFListName=new QStandardItem(tmp[3]);
        itemFListNode<<itemFListID<<itemFListName;
        itemF->appendRow(itemFListNode);
    }

    //设置子节点“群聊”
    itemG=new QStandardItem(QString("群聊"));
    root->appendRow(itemG);

    vector<QString>grouplist=qq->returnList()->returnGroupList();
    for(int i=0;i<(int)grouplist.size();i++)
    {
        QList<QStandardItem*> itemGListNode;
        QStandardItem* itemGListID;
        QStandardItem* itemGListName;

        QQGroup_KDZ* pgroup=new QQGroup_KDZ(grouplist[i],"###","###");
        pgroup->readGroupFile();
        itemGListID=new QStandardItem(grouplist[i]);
        itemGListName=new QStandardItem(pgroup->returnGroupName());
        itemGListNode<<itemGListID<<itemGListName;
        itemG->appendRow(itemGListNode);
        delete pgroup;
    }

    //设置子节点“好友申请”
    itemNF=new QStandardItem(QString("好友申请"));
    root->appendRow(itemNF);

    vector<QString>newfriendlist=qq->returnList()->returnNewFriendList();
    for(int i=0;i<(int)newfriendlist.size();i++)
    {
        QList<QStandardItem*> itemNFListNode;
        QStandardItem* itemNFListID;
        QStandardItem* itemNFListName;

        vector<QString> tmp=dbqq->info(newfriendlist[i]);
        itemNFListID=new QStandardItem(newfriendlist[i]);
        itemNFListName=new QStandardItem(tmp[3]);
        itemNFListNode<<itemNFListID<<itemNFListName;
        itemNF->appendRow(itemNFListNode);
    }

    //设置子节点“群聊申请”
    itemNG=new QStandardItem(QString("群聊申请"));
    root->appendRow(itemNG);

    vector<QString>newgrouplist=qq->returnList()->returnNewGroupList();
    for(int i=0;i<(int)newgrouplist.size();i++)
    {
        QList<QStandardItem*> itemNGListNode;
        QStandardItem* itemNGListID;
        QStandardItem* itemNGListName;

        QQGroup_KDZ* pgroup=new QQGroup_KDZ(newgrouplist[i],"###","###");
        pgroup->readGroupFile();
        itemNGListID=new QStandardItem(newgrouplist[i]);
        itemNGListName=new QStandardItem(pgroup->returnGroupName());
        itemNGListNode<<itemNGListID<<itemNGListName;
        itemNG->appendRow(itemNGListNode);
        delete pgroup;
    }

    //连接信号和槽
    ui->treeView_QQlist->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_QQlist,&QTreeView::customContextMenuRequested,this,&QQListUI_KDZ::slotMenuPopup);
}

void QQListUI_KDZ::initInfo(QString id)
{
    vector<QString>inf=dbqq->info(id);
    QString password=inf[1];
    QString email=inf[2];
    QString name=inf[3];
    int sex;
    if(inf[4]==QString("男"))
    {
        sex=1;
    }
    else if(inf[4]==QString("女"))
    {
        sex=0;
    }
    else
    {
        sex=2;
    }
    QString area=inf[5];
    QString birthday=inf[6];
    QString signature=inf[7];
    QString applydate=inf[8];
    int status;
    if(inf[9]==QString("登录"))
    {
        status=1;
    }
    else
    {
        status=0;
    }
    QString hostname=inf[10];
    QString linkedwechat=inf[11];
    QString sharedWeibo=inf[12];

    qq=new QQ_KDZ(id,email,password,name,sex,area,birthday,signature,applydate,linkedwechat,sharedWeibo,status,hostname);

    List_KDZ::initListFile(pql->returnPQQ()->returnID());
    qq->returnList()->readListFile(pql->returnPQQ()->returnID());

    refresh();
}

void QQListUI_KDZ::refresh()
{
    ui->label_ID->setText(qq->returnID());
    ui->label_name->setText(qq->returnName());
    ui->label_signature->setText(qq->returnSignature());
}

void QQListUI_KDZ::closeEvent(QCloseEvent *e)
{
    dbqq->exit(qq->returnID());
    delete qq;
    delete pql;
    pql=nullptr;
    e->accept();
}

void QQListUI_KDZ::slotMenuPopup(const QPoint &pos)
{
    QModelIndex curIndex=ui->treeView_QQlist->currentIndex();
    QModelIndex parentIndex=curIndex.parent();
    QModelIndex curIndexID=curIndex.sibling(curIndex.row(),0);
    QModelIndex parentIndexID=parentIndex.sibling(parentIndex.row(),0);
    QString strc=curIndexID.data().toString();
    QString strp=parentIndexID.data().toString();
    QModelIndex Index=ui->treeView_QQlist->indexAt(pos);
    if(Index.isValid()&&strc!="好友"&&strc!="群聊"&&strc!="好友申请"&&strc!="群聊申请")
    {
        if(strp=="好友"||strp=="群聊")
        {
            menu=new QMenu(ui->treeView_QQlist);
            menu->addAction(QString("资料"),this,SLOT(slotAction1()));
            menu->addSeparator();
            menu->addAction(QString("聊天"),this,SLOT(slotAction2()));
            menu->exec(QCursor::pos());
        }
        if(strp=="好友申请"||strp=="群聊申请")
        {
            menu=new QMenu(ui->treeView_QQlist);
            menu->addAction(QString("同意"),this,SLOT(slotAction3()));
            menu->addSeparator();
            menu->addAction(QString("拒绝"),this,SLOT(slotAction4()));
            menu->exec(QCursor::pos());
        }
    }
}

void QQListUI_KDZ::slotAction1()//资料
{
    QModelIndex curIndex=ui->treeView_QQlist->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString str=curIndex.parent().data().toString();
    if(indexID.isValid()&&str=="好友")
    {
        if(pqi==nullptr)
        {
            pqi=new QQInfoUI_KDZ();
            pqi->toPage2();
            pqi->friendInfo(indexID.data().toString());
            pqi->show();
        }
        else
        {
            pqi->toPage2();
            pqi->friendInfo(indexID.data().toString());
        }
    }
    if(indexID.isValid()&&str=="群聊")
    {
        if(pqi==nullptr)
        {
            pqi=new QQInfoUI_KDZ();
            pqi->toPage3();
            pqi->groupInfo(indexID.data().toString());
            pqi->show();
        }
        else
        {
            pqi->toPage3();
            pqi->groupInfo(indexID.data().toString());
        }
    }
}

void QQListUI_KDZ::slotAction2()//聊天
{
    QModelIndex curIndex=ui->treeView_QQlist->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString str=curIndex.parent().data().toString();
    if(indexID.isValid()&&str=="好友")
    {
        if(pqc==nullptr)
        {
            pqc=new QQChatUI_KDZ();
            pqc->toPage1();
            pqc->initChat1(indexID.data().toString());
            pqc->show();
        }
        else
        {
            if(pqc->currentI()==2)//已修改page_2索引为2
            {
                pqc->DGServer();
            }
            pqc->toPage1();
            pqc->initChat1(indexID.data().toString());
        }
    }
    if(indexID.isValid()&&str=="群聊")
    {
        if(pqc==nullptr)
        {
            pqc=new QQChatUI_KDZ();
            pqc->toPage2();
            pqc->initChat2(indexID.data().toString());
            pqc->CGServer(indexID.data().toString());
            pqc->show();
        }
        else
        {
            if(pqc->currentI()==2)
            {
                pqc->DGServer();
            }
            pqc->toPage2();
            pqc->initChat2(indexID.data().toString());
            pqc->CGServer(indexID.data().toString());
        }
    }
}

void QQListUI_KDZ::slotAction3()//同意
{
    QModelIndex curIndex=ui->treeView_QQlist->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString str=curIndex.parent().data().toString();
    if(indexID.isValid()&&str=="好友申请")
    {
        //修改好友列表
        QString ID1=pql->returnPQQ()->returnID();
        QString ID2=indexID.data().toString();
        List_KDZ* list1=new List_KDZ();
        list1->readListFile(ID1);
        List_KDZ* list2=new List_KDZ();
        list2->readListFile(ID2);
        if(list1->queryNewFriendList(ID2)!=-1)
        {
            list1->deleteNewFriendList(ID2);
        }
        if(list2->queryNewFriendList(ID1)!=-1)
        {
            list2->deleteNewFriendList(ID1);
        }

        list1->addFriendList(ID2);
        list2->addFriendList(ID1);
        list1->writeListFile(ID1);
        list2->writeListFile(ID2);

        delete list1;
        delete list2;

        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
    }
    if(indexID.isValid()&&str=="群聊申请")
    {
        //修改群列表
        QQGroup_KDZ* pgroup=new QQGroup_KDZ(indexID.data().toString(),"###","###");
        pgroup->readGroupFile();
        pgroup->inviteMember(pql->returnPQQ()->returnID());
        pgroup->writeGroupFile();
        delete pgroup;

        qq->returnList()->readListFile(pql->returnPQQ()->returnID());
        qq->returnList()->deleteNewGroupList(indexID.data().toString());
        qq->returnList()->writeListFile(pql->returnPQQ()->returnID());

        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
    }
    return;
}

void QQListUI_KDZ::slotAction4()//拒绝
{
    QModelIndex curIndex=ui->treeView_QQlist->currentIndex();
    QModelIndex indexID=curIndex.sibling(curIndex.row(),0);
    QString str=curIndex.parent().data().toString();
    if(indexID.isValid()&&str=="好友申请")
    {
        //修改好友列表
        QString ID1=pql->returnPQQ()->returnID();
        QString ID2=indexID.data().toString();
        List_KDZ* list1=new List_KDZ();
        list1->readListFile(ID1);
        List_KDZ* list2=new List_KDZ();
        list2->readListFile(ID2);
        if(list1->queryNewFriendList(ID2)!=-1)
        {
            list1->deleteNewFriendList(ID2);
        }
        if(list2->queryNewFriendList(ID1)!=-1)
        {
            list2->deleteNewFriendList(ID1);
        }

        list1->writeListFile(ID1);
        list2->writeListFile(ID2);
        delete list1;
        delete list2;

        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
    }
    if(indexID.isValid()&&str=="群聊申请")
    {
        //修改群列表
        qq->returnList()->readListFile(pql->returnPQQ()->returnID());
        qq->returnList()->deleteNewGroupList(indexID.data().toString());
        qq->returnList()->writeListFile(pql->returnPQQ()->returnID());

        pql->initInfo(pql->returnPQQ()->returnID());
        pql->initList();
    }
    return;
}

QQListUI_KDZ::~QQListUI_KDZ()
{
    delete ui;
}

void QQListUI_KDZ::on_pushButton_myinfo_clicked()
{
    if(pqi==nullptr)
    {
        pqi=new QQInfoUI_KDZ();
        pqi->toPage1();
        pqi->myInfo(pql->returnPQQ()->returnID());
        pqi->show();
    }
    else
    {
        pqi->toPage1();
        pqi->myInfo(pql->returnPQQ()->returnID());
    }
}

void QQListUI_KDZ::on_pushButton_set_clicked()
{
    if(pqe==nullptr)
    {
        pqe=new QQExpUI_KDZ();
        pqe->toPage2();
        pqe->show();
    }
    else
    {
        pqe->toPage2();
    }
}

void QQListUI_KDZ::on_pushButton_newgroup_clicked()
{
    if(pqe==nullptr)
    {
        pqe=new QQExpUI_KDZ();
        pqe->toPage3();
        pqe->show();
    }
    else
    {
        pqe->toPage3();
    }
}

void QQListUI_KDZ::on_pushButton_addF_clicked()
{
    if(pqe==nullptr)
    {
        pqe=new QQExpUI_KDZ();
        pqe->toPage1();
        pqe->initBox();
        pqe->show();
    }
    else
    {
        pqe->toPage1();
        pqe->initBox();
    }
}

void QQListUI_KDZ::on_pushButton_space_clicked()
{
    if(pqe==nullptr)
    {
        pqe=new QQExpUI_KDZ();
        pqe->toPage4();
        pqe->show();
    }
    else
    {
        pqe->toPage4();
    }
}
