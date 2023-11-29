#ifndef WCLIST_KDZ_H
#define WCLIST_KDZ_H

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class WCList_KDZ;
}

class WCList_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit WCList_KDZ(QWidget *parent = 0);
    void setBackG();//设置背景
    void toPage1();
    void toPage2();
    void toPage3();
    void closeEvent(QCloseEvent *e);
    void setWCID(QString id);//设置微信id
    QString returnWCID();//返回微信id
    ~WCList_KDZ();

private slots:
    void on_pushButton_wcl1_clicked();

    void on_pushButton_wcl2_clicked();

    void on_pushButton_wcl5_clicked();

private:
    Ui::WCList_KDZ *ui;
    QString WCID;
};

#endif // WCLIST_KDZ_H
