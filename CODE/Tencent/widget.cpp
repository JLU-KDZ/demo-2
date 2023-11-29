#include "widget.h"
#include "ui_widget.h"
#include "qqui_kdz.h"
#include "wcui_kdz.h"
#include "wbui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqinfoui_kdz.h"
#include "qqchatui_kdz.h"
#include "qqexpui_kdz.h"

extern Widget* pw;
extern QQUI_KDZ* pq;
extern WCUI_KDZ* pwc;
extern WBUI_KDZ* pwb;
extern QQListUI_KDZ* pql;
extern QQInfoUI_KDZ* pqi;
extern QQChatUI_KDZ* pqc;
extern QQExpUI_KDZ* pqe;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_QQ_clicked()
{
    this->close();
    delete pw;
    pw=nullptr;
    pq=new QQUI_KDZ();
    pq->initPage();
    pq->show();
}

void Widget::on_pushButton_WC_clicked()
{
    this->close();
    delete pw;
    pw=nullptr;
    pwc=new WCUI_KDZ();
    pwc->initPage();
    pwc->show();
}

void Widget::on_pushButton_WB_clicked()
{
    this->close();
    delete pw;
    pw=nullptr;
    pwb=new WBUI_KDZ();
    pwb->show();
}
