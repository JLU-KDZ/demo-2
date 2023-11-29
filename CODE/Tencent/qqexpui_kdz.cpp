#include "qqexpui_kdz.h"
#include "ui_qqexpui_kdz.h"
#include "widget.h"
#include "group.h"
#include "qqui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqinfoui_kdz.h"
#include "qqchatui_kdz.h"
#include "database.h"

#include <QMessageBox>

extern Widget* pw;
extern QQUI_KDZ* pq;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;
extern DBQQ_KDZ* dbqq;
extern DBWC_KDZ* dbwc;

QQExpUI_KDZ::QQExpUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQExpUI_KDZ)
{
    ui->setupUi(this);
}

void QQExpUI_KDZ::toPage1()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQExpUI_KDZ::toPage2()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void QQExpUI_KDZ::toPage3()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void QQExpUI_KDZ::toPage4()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void QQExpUI_KDZ::initBox()
{
    //加载所有账号
    QList<QString> List;
    vector<QString> ans=dbqq->allID();

    List_KDZ* plist=new List_KDZ();
    plist->readListFile(pql->returnPQQ()->returnID());

    for(int i=0;i<(int)ans.size();i++)
    {
        if(ans[i]!=pql->returnPQQ()->returnID()&&plist->queryNewFriendList(ans[i])==-1&&plist->queryFriendList(ans[i])==-1)
        {
            List.append(ans[i]);
        }
    }
    delete plist;

    //设置列表格式
    ui->comboBox_suggest->addItems(List);

    QStandardItemModel* model=qobject_cast<QStandardItemModel*>(ui->comboBox_suggest->model());
    for(int i=0;i<model->rowCount();i++)
    {
        QStandardItem* item=model->item(i);
        item->setSizeHint({0,45});
    }
}

void QQExpUI_KDZ::closeEvent(QCloseEvent *e)
{
    e->accept();
    delete pqe;
    pqe=nullptr;
}

QQExpUI_KDZ::~QQExpUI_KDZ()
{
    delete ui;
}

void QQExpUI_KDZ::on_pushButton_exit_clicked()
{
    //账号退出
    dbqq->exit(pql->returnPQQ()->returnID());

    pqe->close();
    delete pqe;
    pqe=nullptr;

    if(pqc!=nullptr)
    {
        if(pqc->currentI()==2)
        {
            pqc->DGServer();
        }
        pqc->close();
        delete pqc;
        pqc=nullptr;
    }

    if(pqi!=nullptr)
    {
        pqi->close();
        delete pqi;
        pqi=nullptr;
    }

    pql->close();
    delete pql;
    pql=nullptr;

    pq=new QQUI_KDZ();
    pq->initPage();
    pq->show();
}

void QQExpUI_KDZ::on_pushButton_linkedWC_clicked()
{
    //绑定微信
    QString wcid=ui->lineEdit_linkedWCID->text();
    QString wcpw=ui->lineEdit_linkedWCPW->text();
    if(wcid.length()==0)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("微信账号为空！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }
    if(wcpw.length()==0)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("微信密码为空！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCPW->setFocus();
        return;
    }

    int flag1=dbqq->uniqueWC(pql->returnPQQ()->returnID(),wcid);
    if(flag1==1)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("已绑定该微信账号！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }
    if(flag1==2)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("该微信账号已绑定其他QQ账号！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }
    if(flag1==3)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }

    int flag2=dbwc->check(wcid,wcpw);
    if(flag2==1)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("微信账号或密码错误！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }
    if(flag2==2)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }

    //写入数据库
    int flag3=dbqq->linkWC(pql->returnPQQ()->returnID(),wcid);
    int flag4=dbwc->linkQQ(wcid,pql->returnPQQ()->returnID());
    if(flag3==1||flag4==1)
    {
        QMessageBox::warning(this,tr("绑定失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_linkedWCID->clear();
        ui->lineEdit_linkedWCPW->clear();
        ui->lineEdit_linkedWCID->setFocus();
        return;
    }
    QMessageBox::information(this,"绑定提示","绑定成功！");
    ui->lineEdit_linkedWCID->clear();
    ui->lineEdit_linkedWCPW->clear();
    ui->lineEdit_linkedWCID->setFocus();

    pql->returnPQQ()->changeLinkedWeChat(wcid);
    return;
}

void QQExpUI_KDZ::on_pushButton_newG_clicked()
{
    QString gid=ui->lineEdit_GID->text();
    QString gname=ui->lineEdit_Gname->text();
    QString creater=pql->returnPQQ()->returnID();

    //判断群号是否为空
    if(gid.length()==0)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群号为空！"),QMessageBox::Ok);
        ui->lineEdit_GID->clear();
        ui->lineEdit_Gname->clear();
        ui->lineEdit_GID->setFocus();
        return;
    }

    //判断群名是否为空
    if(gname.length()==0)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群名为空！"),QMessageBox::Ok);
        ui->lineEdit_GID->clear();
        ui->lineEdit_Gname->clear();
        ui->lineEdit_Gname->setFocus();
        return;
    }

    //QQ群创建并初始化
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(gid,gname,creater);
    if(pgroup->initGroupFile()==1)
    {
        QMessageBox::warning(this,tr("创建失败"),tr("群号已存在！"),QMessageBox::Ok);
        ui->lineEdit_GID->clear();
        ui->lineEdit_Gname->clear();
        ui->lineEdit_GID->setFocus();
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
    ui->lineEdit_GID->clear();
    ui->lineEdit_Gname->clear();
    ui->lineEdit_GID->setFocus();
    delete pgroup;
    return;
}

void QQExpUI_KDZ::on_pushButton_add_clicked()
{
    QString addgid=ui->lineEdit_addGID->text();

    //判断群号是否为空
    if(addgid.length()==0)
    {
        QMessageBox::warning(this,tr("申请失败"),tr("群号为空！"),QMessageBox::Ok);
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        return;
    }

    //查询群号
    QString path = QCoreApplication::applicationDirPath()+"/QQGroup/"+addgid+"";
    if(!QDir(path).exists())
    {
        QMessageBox::warning(this,tr("申请失败"),tr("群号不存在！"),QMessageBox::Ok);
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        return;
    }

    pql->returnPQQ()->returnList()->readListFile(pql->returnPQQ()->returnID());
    if(pql->returnPQQ()->returnList()->queryNewGroupList(addgid)!=-1)
    {
        QMessageBox::warning(this,tr("申请失败"),tr("您被邀请！"),QMessageBox::Ok);
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        return;
    }

    //发出申请
    QQGroup_KDZ* pgroup=new QQGroup_KDZ(addgid,"###","###");
    pgroup->readGroupFile();
    int flag=pgroup->inviteMember(pql->returnPQQ()->returnID());
    if(flag==1)
    {
        QMessageBox::warning(this,tr("申请失败"),tr("您已加入该群！"),QMessageBox::Ok);
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        delete pgroup;
        return;
    }
    else if(flag==2)
    {
        QMessageBox::warning(this,tr("申请失败"),tr("您已申请该群！"),QMessageBox::Ok);
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        delete pgroup;
        return;
    }
    else
    {
        //写入群文件
        pgroup->writeGroupFile();

        QMessageBox::information(this,"申请提示","申请成功！");
        ui->lineEdit_addGID->clear();
        ui->lineEdit_addGID->setFocus();
        delete pgroup;
        return;
    }
}

void QQExpUI_KDZ::on_pushButton_confirm1_clicked()
{
    QString fid=ui->lineEdit_queryF->text();

    //判断好友账号是否为空
    if(fid.length()==0)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号为空！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //判断账号是否合法
    if(fid==pql->returnPQQ()->returnID())
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号为自己！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //查询是否在好友列表
    List_KDZ* _plist=new List_KDZ();
    _plist->readListFile(pql->returnPQQ()->returnID());
    if(_plist->queryNewFriendList(fid)!=-1||_plist->queryFriendList(fid)!=-1)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("该账号已在好友列表！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        delete _plist;
        return;
    }
    delete _plist;

    //查询好友账号
    int flag=dbqq->query(fid);
    if(flag==2)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }
    else if(flag==1)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号不存在！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //写入好友的列表文件
    List_KDZ* plist=new List_KDZ();
    plist->readListFile(fid);
    if(plist->addNewFriendList(pql->returnPQQ()->returnID())!=0)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("已申请该账号！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        delete plist;
        return;
    }

    plist->writeListFile(fid);
    QMessageBox::information(this,"查询提示","申请成功！");
    ui->lineEdit_queryF->clear();
    ui->lineEdit_queryF->setFocus();
    delete plist;
    return;
}

void QQExpUI_KDZ::on_pushButton__confirm2_clicked()
{
    QString fid=ui->comboBox_suggest->currentText();
    //判断好友账号是否为空
    if(fid.length()==0)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号为空！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //判断账号是否合法
    if(fid==pql->returnPQQ()->returnID())
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号为自己！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //查询好友账号
    int flag=dbqq->query(fid);
    if(flag==2)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }
    else if(flag==1)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("账号不存在！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        return;
    }

    //写入好友的列表文件
    List_KDZ* plist=new List_KDZ();
    plist->readListFile(fid);
    if(plist->addNewFriendList(pql->returnPQQ()->returnID())!=0)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("已申请该账号！"),QMessageBox::Ok);
        ui->lineEdit_queryF->clear();
        ui->lineEdit_queryF->setFocus();
        delete plist;
        return;
    }

    plist->writeListFile(fid);
    QMessageBox::information(this,"查询提示","申请成功！");
    ui->lineEdit_queryF->clear();
    ui->lineEdit_queryF->setFocus();
    delete plist;
    return;
}

void QQExpUI_KDZ::on_pushButton_refresh_clicked()
{
    pqe->close();
    delete pqe;
    pqe=nullptr;
    pql->initInfo(pql->returnPQQ()->returnID());
    pql->initList();
    return;
}
