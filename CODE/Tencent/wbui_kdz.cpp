#include "wbui_kdz.h"
#include "ui_wbui_kdz.h"
#include "wblist_kdz.h"
#include "widget.h"
#include "database.h"

#include <QMessageBox>

extern Widget* pw;
extern WBUI_KDZ* pwb;
extern WBList_KDZ* pwbl;
extern DBQQ_KDZ* dbqq;

WBUI_KDZ::WBUI_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WBUI_KDZ)
{
    ui->setupUi(this);
}

WBUI_KDZ::~WBUI_KDZ()
{
    delete ui;
}

void WBUI_KDZ::on_pushButton_WBRW_clicked()
{
    pwb->close();
    delete pwb;
    pwb=nullptr;
    pw=new Widget();
    pw->show();
}

void WBUI_KDZ::on_pushButton_Qlogin_clicked()
{
    QString _ID=ui->lineEdit_WBID->text();
    QString _PW=ui->lineEdit_WBpw->text();
    if(_ID.length()==0||_PW.length()==0)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码为空！"),QMessageBox::Ok);
        ui->lineEdit_WBID->clear();
        ui->lineEdit_WBpw->clear();
        ui->lineEdit_WBID->setFocus();
        return;
    }

    int ans=dbqq->wbLogin(_ID,_PW);
    if(ans==0)
    {
        QMessageBox::information(this,"登录提示","登录成功");
        pwb->close();
        delete pwb;
        pwb=nullptr;
        pwbl=new WBList_KDZ();
        pwbl->show();
        return;
    }
    if(ans==1)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("账号或密码错误！"),QMessageBox::Ok);
        ui->lineEdit_WBID->clear();
        ui->lineEdit_WBpw->clear();
        ui->lineEdit_WBID->setFocus();
        return;
    }
    if(ans==2)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("数据库运行失败！"),QMessageBox::Ok);
        ui->lineEdit_WBID->clear();
        ui->lineEdit_WBpw->clear();
        ui->lineEdit_WBID->setFocus();
        return;
    }
}
