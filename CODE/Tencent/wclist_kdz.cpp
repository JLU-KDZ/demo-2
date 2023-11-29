#include "wclist_kdz.h"
#include "widget.h"
#include "database.h"
#include "wcui_kdz.h"
#include "ui_wclist_kdz.h"

extern Widget* pw;
extern DBWC_KDZ* dbwc;
extern WCUI_KDZ* pwc;
extern WCList_KDZ* pwcl;

WCList_KDZ::WCList_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WCList_KDZ)
{
    ui->setupUi(this);
}

void WCList_KDZ::setBackG()
{
    ui->pushButton_wcl1->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl1_.png)}");
    ui->pushButton_wcl2->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl2.png)}");

    ui->pushButton_wcl3->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl3.png)}");
    ui->pushButton_wcl4->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl4.png)}");
    ui->pushButton_wcl5->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl5.png)}");
    ui->pushButton_wcl6->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl6.png)}");
    ui->pushButton_wcl7->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl7.png)}");
    ui->pushButton_wcl8->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl8.png)}");
    ui->pushButton_wcl9->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl9.png)}");
    ui->pushButton_wcl10->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl10.png)}");
    ui->pushButton_wcl11->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl11.png)}");
}

void WCList_KDZ::toPage1()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void WCList_KDZ::toPage2()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void WCList_KDZ::toPage3()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void WCList_KDZ::closeEvent(QCloseEvent *e)
{
    dbwc->exit(WCID);
    delete pwcl;
    pwcl=nullptr;
    e->accept();
}

void WCList_KDZ::setWCID(QString id)
{
    WCID=id;
}

QString WCList_KDZ::returnWCID()
{
    return WCID;
}

WCList_KDZ::~WCList_KDZ()
{
    delete ui;
}

void WCList_KDZ::on_pushButton_wcl1_clicked()
{
    ui->pushButton_wcl1->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl1_.png)}");
    ui->pushButton_wcl2->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl2.png)}");
    ui->pushButton_wcl5->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl5.png)}");
    toPage1();
}

void WCList_KDZ::on_pushButton_wcl2_clicked()
{
    ui->pushButton_wcl1->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl1.png)}");
    ui->pushButton_wcl2->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl2_.png)}");
    ui->pushButton_wcl5->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl5.png)}");
    toPage2();
}

void WCList_KDZ::on_pushButton_wcl5_clicked()
{
    ui->pushButton_wcl1->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl1.png)}");
    ui->pushButton_wcl2->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl2.png)}");
    ui->pushButton_wcl5->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/wcl5_.png)}");
    toPage3();
}
