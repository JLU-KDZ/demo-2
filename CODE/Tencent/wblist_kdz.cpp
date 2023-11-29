#include "wblist_kdz.h"
#include "ui_wblist_kdz.h"
#include "wbui_kdz.h"
#include "widget.h"

#include <QDesktopServices>
#include <Qurl>

extern Widget* pw;
extern WBUI_KDZ* pwb;
extern WBList_KDZ* pwbl;

WBList_KDZ::WBList_KDZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WBList_KDZ)
{
    ui->setupUi(this);
}

WBList_KDZ::~WBList_KDZ()
{
    delete ui;
}

void WBList_KDZ::on_pushButton_search_clicked()
{
    QUrl regUrl(QLatin1String("https://www.weibo.com"));
    QDesktopServices::openUrl(regUrl);
}
