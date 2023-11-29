#include "widget.h"
#include "database.h"
#include "qqui_kdz.h"
#include "ui_qqui_kdz.h"
#include "qqinfoui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqexpui_kdz.h"
#include "qqchatui_kdz.h"
#include "smtp.h"

#include <time.h>
#include <QList>
#include <QListView>
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QHostInfo>

extern Widget* pw;
extern QQUI_KDZ* pq;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;
extern DBQQ_KDZ* dbqq;
extern DBWC_KDZ* dbwc;
extern DBWB_KDZ* dbwb;

QQUI_KDZ::QQUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQUI_KDZ)
{
    ui->setupUi(this);
    codeNew=-1;
    codeFind=-1;
}

void QQUI_KDZ::initPage()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

int QQUI_KDZ::getCodeNew()
{
    return codeNew;
}

int QQUI_KDZ::getCodeFind()
{
    return codeFind;
}

QString QQUI_KDZ::localHost()
{
    QString localeName=QHostInfo::localHostName();
    return localeName;
}

QQUI_KDZ::~QQUI_KDZ()
{
    delete ui;
}

void QQUI_KDZ::on_pushButton_R_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void QQUI_KDZ::on_pushButton_F_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void QQUI_KDZ::on_pushButton_Q_clicked()
{
    ui->comboBox_WC->clear();

    vector<QString> ans=dbwc->qLogin(localHost());
    QList<QString> wcList;
    for(int i=0;i<(int)ans.size();i++)
    {
        QString linked=dbwc->selectLQQ(ans[i]);
        if(linked.length()!=0)
        {
            wcList.append(linked);
        }
    }

    ui->comboBox_WC->addItems(wcList);

    QStandardItemModel* model=qobject_cast<QStandardItemModel*>(ui->comboBox_WC->model());
    for(int i=0;i<model->rowCount();i++)
    {
        QStandardItem* item=model->item(i);
        item->setSizeHint({0,45});
    }
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void QQUI_KDZ::on_pushButton_RQQ1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQUI_KDZ::on_pushButton_RQQ3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQUI_KDZ::on_pushButton_RQQ2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void QQUI_KDZ::on_pushButton_RW_clicked()
{
    pq->close();
    delete pq;
    pq=nullptr;
    pw=new Widget();
    pw->show();
}

void QQUI_KDZ::on_pushButton_L_clicked()
{
    QString _ID=ui->lineEdit_QQID->text();
    QString _PW=ui->lineEdit_QQpw->text();
    if(_ID.length()==0||_PW.length()==0)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码为空！"),QMessageBox::Ok);
        ui->lineEdit_QQID->clear();
        ui->lineEdit_QQpw->clear();
        ui->lineEdit_QQID->setFocus();
        return;
    }
    int ans=dbqq->login(_ID,_PW,pq->localHost());
    if(ans==0)
    {
        QMessageBox::information(this,"登录提示","登录成功");
        pq->close();
        delete pq;
        pq=nullptr;
        pql=new QQListUI_KDZ();
        pql->initInfo(_ID);
        pql->initList();
        pql->show();
        return;
    }
    if(ans==1)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码错误！"),QMessageBox::Ok);
        ui->lineEdit_QQID->clear();
        ui->lineEdit_QQpw->clear();
        ui->lineEdit_QQID->setFocus();
        return;
    }
    if(ans==2)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_QQID->clear();
        ui->lineEdit_QQpw->clear();
        ui->lineEdit_QQID->setFocus();
        return;
    }
    if(ans==3)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("该账号已登录！"),QMessageBox::Ok);
        ui->lineEdit_QQID->clear();
        ui->lineEdit_QQpw->clear();
        ui->lineEdit_QQID->setFocus();
        return;
    }
}

void QQUI_KDZ::on_pushButton_getcodenew_clicked()
{
    QString mail=ui->lineEdit_QQemailnew->text();
    if(mail.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱为空！"),QMessageBox::Ok);
        ui->lineEdit_QQemailnew->setFocus();
        return;
    }

    if(!mail.contains("@qq.com"))
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱格式错误！"),QMessageBox::Ok);
        ui->lineEdit_QQemailnew->clear();
        ui->lineEdit_QQemailnew->setFocus();
        return;
    }

    int r[6];
    srand(time(0));
    for (int i=0; i<6; i++)
    {
        r[i] = Smtp_KDZ::getRand(0,9);
    }
    codeNew=r[0]*10*10*10*10*10+r[1]*10*10*10*10+r[2]*10*10*10+r[3]*10*10+r[4]*10+r[5];
    QString num0=QString::number(codeNew);
    while(num0.size()<6)
    {
        num0.insert(0,'0');
    }
    QString num="您好！\n您的验证码为:"+num0+"，用于QQ账号注册。\n请勿泄露和转发，如非本人操作，请忽略此消息。";
    QByteArray byte=num.toUtf8();
    Smtp_KDZ smtp("3029104906@qq.com","vbwqhdjudobidgfg");

    QMessageBox::information(this,"验证码提示","验证码正在发送...");

    if(smtp.SendData(mail.toUtf8(),"验证码",byte)==0)
    {
        QMessageBox::information(this,"验证码提示","验证码发送成功！");
    }
    else
    {
        QMessageBox::warning(this,tr("验证码提示"),tr("验证码发送失败！"),QMessageBox::Ok);
    }
    return;
}

void QQUI_KDZ::on_pushButton_register_clicked()
{
    QString QQID=ui->lineEdit_QQIDnew->text();
    QString name=ui->lineEdit_QQnamenew->text();
    QString passWord=ui->lineEdit_QQpwnew->text();
    QString passWord2=ui->lineEdit_QQpwagainnew->text();
    QString mail=ui->lineEdit_QQemailnew->text();
    QString icode=ui->lineEdit_QQcodenew->text();

    if(QQID.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("账号为空！"),QMessageBox::Ok);
        ui->lineEdit_QQIDnew->setFocus();
        return;
    }

    if(name.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("昵称为空！"),QMessageBox::Ok);
        ui->lineEdit_QQnamenew->setFocus();
        return;
    }

    if(passWord.length()==0||passWord2.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("密码为空！"),QMessageBox::Ok);
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQpwnew->setFocus();
        return;
    }

    if(QString::compare(passWord,passWord2)!=0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("两次输入密码不一致！"),QMessageBox::Ok);
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQpwnew->setFocus();
        return;
    }

    if(icode.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("验证码为空！"),QMessageBox::Ok);
        ui->lineEdit_QQcodenew->setFocus();
        return;
    }

    if(icode.toLongLong()!=codeNew)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("验证码错误！"),QMessageBox::Ok);
        ui->lineEdit_QQcodenew->clear();
        ui->lineEdit_QQcodenew->setFocus();
        return;
    }

    codeNew=-1;

    int flag=dbqq->logon(QQID,passWord,mail,name);
    switch (flag) {
    case 0:
    {
        QMessageBox::information(this,"注册提示","注册成功！");
        ui->lineEdit_QQIDnew->clear();
        ui->lineEdit_QQnamenew->clear();
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQemailnew->clear();
        ui->lineEdit_QQcodenew->clear();
        ui->lineEdit_QQIDnew->setFocus();
        return;
    }
    case 1:
    {
        QMessageBox::warning(this,tr("注册提示"),tr("该账号已存在！"),QMessageBox::Ok);
        ui->lineEdit_QQIDnew->clear();
        ui->lineEdit_QQnamenew->clear();
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQemailnew->clear();
        ui->lineEdit_QQcodenew->clear();
        ui->lineEdit_QQIDnew->setFocus();
        return;
    }
    default:
    {
        QMessageBox::warning(this,tr("注册提示"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_QQIDnew->clear();
        ui->lineEdit_QQnamenew->clear();
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQemailnew->clear();
        ui->lineEdit_QQcodenew->clear();
        ui->lineEdit_QQIDnew->setFocus();
        return;
    }
    }
}

void QQUI_KDZ::on_pushButton_QQgetcodefind_clicked()
{
    QString mail=ui->lineEdit_QQemailfind->text();
    if(mail.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱为空！"),QMessageBox::Ok);
        ui->lineEdit_QQemailfind->setFocus();
        return;
    }

    if(!mail.contains("@qq.com"))
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱格式错误！"),QMessageBox::Ok);
        ui->lineEdit_QQemailfind->clear();
        ui->lineEdit_QQemailfind->setFocus();
        return;
    }

    int r[6];
    srand(time(0));
    for (int i=0; i<6; i++)
    {
        r[i] = Smtp_KDZ::getRand(0,9);
    }
    codeFind=r[0]*10*10*10*10*10+r[1]*10*10*10*10+r[2]*10*10*10+r[3]*10*10+r[4]*10+r[5];
    QString num0=QString::number(codeFind);
    while(num0.size()<6)
    {
        num0.insert(0,'0');
    }
    QString num="您好！\n您的验证码为:"+num0+"，用于QQ账号找回密码。\n请勿泄露和转发，如非本人操作，请忽略此消息。";
    QByteArray byte=num.toUtf8();
    Smtp_KDZ smtp("3029104906@qq.com","vbwqhdjudobidgfg");

    QMessageBox::information(this,"验证码提示","验证码正在发送...");

    if(smtp.SendData(mail.toUtf8(),"验证码",byte)==0)
    {
        QMessageBox::information(this,"验证码提示","验证码发送成功！");
    }
    else
    {
        QMessageBox::warning(this,tr("验证码提示"),tr("验证码发送失败！"),QMessageBox::Ok);
    }
    return;
}

void QQUI_KDZ::on_pushButton_find_clicked()
{
    QString QQID=ui->lineEdit_QQIDfind->text();
    QString passWord=ui->lineEdit_QQpwfind->text();
    QString passWord2=ui->lineEdit_QQpwagainfind->text();
    QString mail=ui->lineEdit_QQemailfind->text();
    QString icode=ui->lineEdit_QQcodefind->text();

    if(QQID.length()==0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("账号为空！"),QMessageBox::Ok);
        ui->lineEdit_QQIDfind->setFocus();
        return;
    }

    if(passWord.length()==0||passWord2.length()==0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("密码为空！"),QMessageBox::Ok);
        ui->lineEdit_QQpwfind->clear();
        ui->lineEdit_QQpwagainfind->clear();
        ui->lineEdit_QQpwfind->setFocus();
        return;
    }

    if(QString::compare(passWord,passWord2)!=0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("两次输入密码不一致！"),QMessageBox::Ok);
        ui->lineEdit_QQpwfind->clear();
        ui->lineEdit_QQpwagainfind->clear();
        ui->lineEdit_QQpwfind->setFocus();
        return;
    }

    if(icode.length()==0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("验证码为空！"),QMessageBox::Ok);
        ui->lineEdit_QQcodefind->setFocus();
        return;
    }

    if(icode.toLongLong()!=codeFind)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("验证码错误！"),QMessageBox::Ok);
        ui->lineEdit_QQcodefind->clear();
        ui->lineEdit_QQcodefind->setFocus();
        return;
    }

    codeFind=-1;

    int flag=dbqq->find(QQID,passWord,mail);
    switch (flag) {
    case 0:
    {
        QMessageBox::information(this,"找回密码提示","找回密码成功！");
        ui->lineEdit_QQIDfind->clear();
        ui->lineEdit_QQpwfind->clear();
        ui->lineEdit_QQpwagainfind->clear();
        ui->lineEdit_QQemailfind->clear();
        ui->lineEdit_QQcodefind->clear();
        ui->lineEdit_QQIDfind->setFocus();
        return;
    }
    case 1:
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("该账号不存在！"),QMessageBox::Ok);
        ui->lineEdit_QQIDfind->clear();
        ui->lineEdit_QQpwfind->clear();
        ui->lineEdit_QQpwagainfind->clear();
        ui->lineEdit_QQemailfind->clear();
        ui->lineEdit_QQcodefind->clear();
        ui->lineEdit_QQIDfind->setFocus();
        return;
    }
    case 2:
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("该邮箱错误！"),QMessageBox::Ok);
        ui->lineEdit_QQIDfind->clear();
        ui->lineEdit_QQpwfind->clear();
        ui->lineEdit_QQpwagainfind->clear();
        ui->lineEdit_QQemailfind->clear();
        ui->lineEdit_QQcodefind->clear();
        ui->lineEdit_QQIDfind->setFocus();
        return;
    }
    default:
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_QQIDnew->clear();
        ui->lineEdit_QQnamenew->clear();
        ui->lineEdit_QQpwnew->clear();
        ui->lineEdit_QQpwagainnew->clear();
        ui->lineEdit_QQemailnew->clear();
        ui->lineEdit_QQcodenew->clear();
        ui->lineEdit_QQIDnew->setFocus();
        return;
    }
    }
}

void QQUI_KDZ::on_pushButton_QWC_clicked()
{
    QString qqid=ui->comboBox_WC->currentText();
    if(qqid .length()==0)
    {
        QMessageBox::warning(this,tr("快捷登录提示"),tr("微信绑定账号为空！"),QMessageBox::Ok);
        this->on_pushButton_Q_clicked();
        return;
    }

    int flag=dbqq->QLogin(qqid,pq->localHost());
    if(flag==0)
    {
        QMessageBox::information(this,"快捷登录提示","登录成功！");
        pq->close();
        delete pq;
        pq=nullptr;
        pql=new QQListUI_KDZ();
        pql->initInfo(qqid);
        pql->initList();
        pql->show();
        return;
    }
    if(flag==1)
    {
        QMessageBox::warning(this,tr("快捷登录提示"),tr("QQ账号不存在！"),QMessageBox::Ok);
        this->on_pushButton_Q_clicked();
        return;
    }
    if(flag==2)
    {
        QMessageBox::warning(this,tr("快捷登录提示"),tr("数据库运行失败！"),QMessageBox::Ok);
        this->on_pushButton_Q_clicked();
        return;
    }
    if(flag==3)
    {
        QMessageBox::warning(this,tr("快捷登录提示"),tr("该账号已登录！"),QMessageBox::Ok);
        this->on_pushButton_Q_clicked();
        return;
    }
}
