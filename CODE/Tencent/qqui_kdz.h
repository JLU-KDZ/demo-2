#ifndef QQUI_KDZ_H
#define QQUI_KDZ_H

#include <QWidget>

namespace Ui {
class QQUI_KDZ;
}

class QQUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit QQUI_KDZ(QWidget *parent = 0);
    void initPage();//初始化
    int getCodeNew();//获取申请账号验证码
    int getCodeFind();//获取找回账号验证码
    QString localHost();//获取本地主机名
    ~QQUI_KDZ();

private slots:
    void on_pushButton_R_clicked();

    void on_pushButton_F_clicked();

    void on_pushButton_Q_clicked();

    void on_pushButton_RQQ1_clicked();

    void on_pushButton_RQQ3_clicked();

    void on_pushButton_RQQ2_clicked();

    void on_pushButton_RW_clicked();

    void on_pushButton_L_clicked();

    void on_pushButton_getcodenew_clicked();

    void on_pushButton_register_clicked();

    void on_pushButton_QQgetcodefind_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_QWC_clicked();

private:
    long long codeNew;//保存注册账号验证码
    long long codeFind;//保存找回账号验证码

    Ui::QQUI_KDZ *ui;
};

#endif // QQUI_KDZ_H
