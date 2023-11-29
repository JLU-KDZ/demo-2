#ifndef GETIP_KDZ_H
#define GETIP_KDZ_H

#include<Windows.h>
#include<ipmib.h>
#include<winerror.h>
#include<iphlpapi.h>
#include<string>
#include<QString>

using namespace std;

class GetIP_KDZ
{
public:
    GetIP_KDZ();
    static void FreeIpForwardTable(PMIB_IPFORWARDTABLE pIpRouteTab);
    static PMIB_IPFORWARDTABLE GetIpForwardTable(BOOL bOrder);
    static string GetSystemIpAddress();

};

#endif // GETIP_KDZ_H
