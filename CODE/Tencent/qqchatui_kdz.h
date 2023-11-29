#ifndef QQCHATUI_KDZ_H
#define QQCHATUI_KDZ_H

#include "client_kdz.h"

#include <QWidget>
#include <QMenu>

namespace Ui {
class QQChatUI_KDZ;
}

class QQChatUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit QQChatUI_KDZ(QWidget *parent = 0);
    void initChat1(QString id);
    void initChat2(QString id);
    void toPage1();
    void toPage2();
    void toPage3();
    int currentI();
    void getHistoryMes(QString id);
    void closeEvent(QCloseEvent* e);
    QString returnGroupID();
    void CGServer(QString id);//客户端连接
    void DGServer();//客户端断开
    ~QQChatUI_KDZ();

private:
    Ui::QQChatUI_KDZ *ui;
    QString groupID;
    QMenu* menu;
    Client_KDZ* pgclient;

private slots:
    void slotMenuPopup(const QPoint &pos);
    void slotAction1();
    void slotAction2();
    void slotAction3();
    void slotAction4();
    void slotAction5();
    void slotAction6();
    void on_pushButton_RGroup_clicked();
    void on_pushButton_inviteID_clicked();
    void on_pushButton_gsend_clicked();
    void recvM(QString m);
    void on_pushButton_gset_clicked();
    void on_pushButton_subG_clicked();
};

#endif // QQCHATUI_KDZ_H
