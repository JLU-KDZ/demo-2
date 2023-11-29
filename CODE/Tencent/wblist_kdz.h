#ifndef WBLIST_KDZ_H
#define WBLIST_KDZ_H

#include <QWidget>

namespace Ui {
class WBList_KDZ;
}

class WBList_KDZ : public QWidget
{
    Q_OBJECT

public:
    explicit WBList_KDZ(QWidget *parent = 0);
    ~WBList_KDZ();

private slots:
    void on_pushButton_search_clicked();

private:
    Ui::WBList_KDZ *ui;
};

#endif // WBLIST_KDZ_H
