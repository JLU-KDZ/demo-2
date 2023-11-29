#include "account.h"
#include "group.h"
#include "getip_kdz.h"
#include "smtp.h"
#include "widget.h"
#include "database.h"
#include "client_kdz.h"
#include "wcui_kdz.h"
#include "wbui_kdz.h"
#include "wclist_kdz.h"
#include "wblist_kdz.h"
#include "qqui_kdz.h"
#include "qqlistui_kdz.h"
#include "qqinfoui_kdz.h"
#include "qqchatui_kdz.h"
#include "qqexpui_kdz.h"

#include <QApplication>

Widget* pw=nullptr;
QQUI_KDZ* pq=nullptr;
WCUI_KDZ* pwc=nullptr;
WBUI_KDZ* pwb=nullptr;
WCList_KDZ* pwcl=nullptr;
WBList_KDZ* pwbl=nullptr;
QQListUI_KDZ* pql=nullptr;
QQInfoUI_KDZ* pqi=nullptr;
QQChatUI_KDZ* pqc=nullptr;
QQExpUI_KDZ* pqe=nullptr;
DBQQ_KDZ* dbqq=new DBQQ_KDZ();
DBWC_KDZ* dbwc=new DBWC_KDZ();
DBWB_KDZ* dbwb=new DBWB_KDZ();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQGroup_KDZ::createGroupFile();
    WeChatGroup_KDZ::createGroupFile();
    List_KDZ::createListFile();

    dbqq->init();
    dbwc->init();
    dbwb->init();

    pw=new Widget();
    pw->show();
    return a.exec();
}
