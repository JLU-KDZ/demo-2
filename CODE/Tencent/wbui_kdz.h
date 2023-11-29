#ifndef WBUI_KDZ_H
#define WBUI_KDZ_H

#include <QWidget>

namespace Ui {
class WBUI_KDZ;
}

class WBUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit WBUI_KDZ(QWidget *parent = 0);
    ~WBUI_KDZ();

private slots:
    void on_pushButton_WBRW_clicked();

    void on_pushButton_Qlogin_clicked();

private:
    Ui::WBUI_KDZ *ui;
};

#endif // WBUI_KDZ_H
