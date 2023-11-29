#ifndef QQEXPUI_KDZ_H
#define QQEXPUI_KDZ_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class QQExpUI_KDZ;
}

class QQExpUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit QQExpUI_KDZ(QWidget *parent = 0);
    void toPage1();
    void toPage2();
    void toPage3();
    void toPage4();
    void initBox();//初始化下拉列表
    void closeEvent(QCloseEvent* e);
    ~QQExpUI_KDZ();

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_linkedWC_clicked();

    void on_pushButton_newG_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_confirm1_clicked();

    void on_pushButton__confirm2_clicked();

    void on_pushButton_refresh_clicked();

private:
    Ui::QQExpUI_KDZ *ui;
};

#endif // QQEXPUI_KDZ_H
