#ifndef WCUI_KDZ_H
#define WCUI_KDZ_H

#include <QWidget>

namespace Ui {
class WCUI_KDZ;
}

class WCUI_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit WCUI_KDZ(QWidget *parent = 0);
    void initPage();
    QString localHost();
    ~WCUI_KDZ();

private slots:
    void on_pushButton_WCRW_clicked();

    void on_pushButton_WCR_clicked();

    void on_pushButton_RWC_clicked();

    void on_pushButton_WCL_clicked();

    void on_pushButton_WCregister_clicked();

private:
    Ui::WCUI_KDZ *ui;
};

#endif // WCUI_KDZ_H
