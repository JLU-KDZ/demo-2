#ifndef QQLISTUI_KDZ_H
#define QQLISTUI_KDZ_H

#include "account.h"

#include <QWidget>
#include <QMenu>
#include <QCloseEvent>
#include <QStandardItem>

namespace Ui {
class QQListUI_KDZ;
}

class QQListUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit QQListUI_KDZ(QWidget *parent = 0);
    QQ_KDZ* returnPQQ();//返回qq指针
    void initList();//初始化列表
    void initInfo(QString id);//初始化信息
    void refresh();//刷新内容
    void closeEvent(QCloseEvent* e);//重写关闭事件
    ~QQListUI_KDZ();

    QStandardItem* root;//根节点指针
    QStandardItem* itemF;//子节点指针-好友
    QStandardItem* itemG;//子节点指针-群聊
    QStandardItem* itemNF;//子节点指针-好友申请
    QStandardItem* itemNG;//子节点指针-群聊申请

private:
    Ui::QQListUI_KDZ *ui;
    QMenu* menu;//菜单指针
    QQ_KDZ* qq;//qq指针

private slots:
    void slotMenuPopup(const QPoint &pos);
    void slotAction1();
    void slotAction2();
    void slotAction3();
    void slotAction4();
    void on_pushButton_myinfo_clicked();
    void on_pushButton_set_clicked();
    void on_pushButton_newgroup_clicked();
    void on_pushButton_space_clicked();
    void on_pushButton_addF_clicked();
};

#endif // QQLISTUI_KDZ_H
