#ifndef QQINFOUI_KDZ_H
#define QQINFOUI_KDZ_H

#include "account.h"

#include <QWidget>
#include<QCloseEvent>

namespace Ui {
class QQInfoUI_KDZ;
}

class QQInfoUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit QQInfoUI_KDZ(QWidget *parent = 0);
    void toPage1();
    void toPage2();
    void toPage3();
    void myInfo(QString id);//初始化个人资料
    void friendInfo(QString id);//初始化好友资料
    void groupInfo(QString id);//初始化群资料
    void closeEvent(QCloseEvent* e);
    ~QQInfoUI_KDZ();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_Fdelete_clicked();

    void on_pushButton_Gchange_clicked();

    void on_pushButton_Fsend_clicked();

    void on_pushButton_Gsend_clicked();

    void on_pushButton_Gdelete_clicked();

private:
    Ui::QQInfoUI_KDZ *ui;
};

#endif // QQINFOUI_KDZ_H
