#include "wcui_kdz.h"
#include "ui_wcui_kdz.h"
#include "widget.h"
#include "database.h"
#include "wclist_kdz.h"

#include <QHostAddress>
#include <QMessageBox>
#include <QHostInfo>

extern Widget* pw;
extern DBWC_KDZ* dbwc;
extern WCUI_KDZ* pwc;
extern WCList_KDZ* pwcl;
WCUI_KDZ::WCUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WCUI_KDZ)
{
    ui->setupUi(this);
}

void WCUI_KDZ::initPage()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

QString WCUI_KDZ::localHost()
{
    QString localeName=QHostInfo::localHostName();
    return localeName;
}

WCUI_KDZ::~WCUI_KDZ()
{
    delete ui;
}

void WCUI_KDZ::on_pushButton_WCRW_clicked()
{
    pwc->close();
    delete pwc;
    pwc=nullptr;
    pw=new Widget();
    pw->show();
}

void WCUI_KDZ::on_pushButton_WCR_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void WCUI_KDZ::on_pushButton_RWC_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void WCUI_KDZ::on_pushButton_WCL_clicked()
{
    QString _ID=ui->lineEdit_WCID->text();
    QString _PW=ui->lineEdit_WCpw->text();
    if(_ID.length()==0||_PW.length()==0)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码为空！"),QMessageBox::Ok);
        ui->lineEdit_WCID->clear();
        ui->lineEdit_WCpw->clear();
        ui->lineEdit_WCID->setFocus();
        return;
    }
    int ans=dbwc->login(_ID,_PW,pwc->localHost());
    if(ans==0)
    {
        QMessageBox::information(this,"登录提示","登录成功");
        pwc->close();
        delete pwc;
        pwc=nullptr;
        pwcl=new WCList_KDZ();
        pwcl->setWCID(_ID);
        pwcl->setBackG();
        pwcl->toPage1();
        pwcl->show();
    }
    if(ans==1)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码错误！"),QMessageBox::Ok);
        ui->lineEdit_WCID->clear();
        ui->lineEdit_WCpw->clear();
        ui->lineEdit_WCID->setFocus();
        return;
    }
    if(ans==2)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_WCID->clear();
        ui->lineEdit_WCpw->clear();
        ui->lineEdit_WCID->setFocus();
        return;
    }
    if(ans==3)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("该账号已登录！"),QMessageBox::Ok);
        ui->lineEdit_WCID->clear();
        ui->lineEdit_WCpw->clear();
        ui->lineEdit_WCID->setFocus();
        return;
    }
}

void WCUI_KDZ::on_pushButton_WCregister_clicked()
{
    QString WCID=ui->lineEdit_WCIDnew->text();
    QString name=ui->lineEdit_WCnamenew->text();
    QString passWord=ui->lineEdit_WCpwnew->text();
    QString passWord2=ui->lineEdit_WCpwagainnew->text();
    QString mail=ui->lineEdit_WCemailnew->text();

    if(WCID.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("账号为空！"),QMessageBox::Ok);
        ui->lineEdit_WCIDnew->setFocus();
        return;
    }

    if(name.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("昵称为空！"),QMessageBox::Ok);
        ui->lineEdit_WCnamenew->setFocus();
        return;
    }

    if(passWord.length()==0||passWord2.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("密码为空！"),QMessageBox::Ok);
        ui->lineEdit_WCpwnew->clear();
        ui->lineEdit_WCpwagainnew->clear();
        ui->lineEdit_WCpwnew->setFocus();
        return;
    }

    if(QString::compare(passWord,passWord2)!=0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("两次输入密码不一致！"),QMessageBox::Ok);
        ui->lineEdit_WCpwnew->clear();
        ui->lineEdit_WCpwagainnew->clear();
        ui->lineEdit_WCpwnew->setFocus();
        return;
    }

    if(mail.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱为空！"),QMessageBox::Ok);
        ui->lineEdit_WCemailnew->setFocus();
        return;
    }

    if(!mail.contains("@qq.com"))
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱不合法！"),QMessageBox::Ok);
        ui->lineEdit_WCemailnew->clear();
        ui->lineEdit_WCemailnew->setFocus();
        return;
    }

    int flag=dbwc->logon(WCID,passWord,mail,name);
    switch (flag) {
    case 0:
    {
        QMessageBox::information(this,"注册提示","注册成功！");
        ui->lineEdit_WCIDnew->clear();
        ui->lineEdit_WCnamenew->clear();
        ui->lineEdit_WCpwnew->clear();
        ui->lineEdit_WCpwagainnew->clear();
        ui->lineEdit_WCemailnew->clear();
        ui->lineEdit_WCIDnew->setFocus();
        return;
    }
    case 1:
    {
        QMessageBox::warning(this,tr("注册提示"),tr("该账号已存在！"),QMessageBox::Ok);
        ui->lineEdit_WCIDnew->clear();
        ui->lineEdit_WCnamenew->clear();
        ui->lineEdit_WCpwnew->clear();
        ui->lineEdit_WCpwagainnew->clear();
        ui->lineEdit_WCemailnew->clear();
        ui->lineEdit_WCIDnew->setFocus();
        return;
    }
    default:
    {
        QMessageBox::warning(this,tr("注册提示"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_WCIDnew->clear();
        ui->lineEdit_WCnamenew->clear();
        ui->lineEdit_WCpwnew->clear();
        ui->lineEdit_WCpwagainnew->clear();
        ui->lineEdit_WCemailnew->clear();
        ui->lineEdit_WCIDnew->setFocus();
        return;
    }
    }
}
